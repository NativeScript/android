package com.tns;

import android.content.Context;
import android.net.LocalServerSocket;
import android.net.LocalSocket;

import java.io.Closeable;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.security.DigestInputStream;
import java.security.MessageDigest;
import java.io.OutputStream;
import java.util.Arrays;

public class NativeScriptSyncServiceSocketImpl {
    private static String DEVICE_APP_DIR;

    private final Runtime runtime;
    private static Logger logger;
    private final Context context;

    private LocalServerSocketThread localServerThread;
    private Thread localServerJavaThread;

    public NativeScriptSyncServiceSocketImpl(Runtime runtime, Logger logger, Context context) {
        this.runtime = runtime;
        NativeScriptSyncServiceSocketImpl.logger = logger;
        this.context = context;
        DEVICE_APP_DIR = this.context.getFilesDir().getAbsolutePath() + "/app";
    }

    private class LocalServerSocketThread implements Runnable {

        private volatile boolean running;
        private final String name;

        private LiveSyncWorker livesyncWorker;
        private LocalServerSocket deviceSystemSocket;

        public LocalServerSocketThread(String name) {
            this.name = name;
            this.running = false;
        }

        public void stop() {
            this.running = false;
            try {
                deviceSystemSocket.close();
            } catch (IOException e) {
                if (com.tns.Runtime.isDebuggable()) {
                    e.printStackTrace();
                }
            }
        }

        public void run() {
            running = true;
            try {
                deviceSystemSocket = new LocalServerSocket(this.name);
                while (running) {
                    LocalSocket systemSocket = deviceSystemSocket.accept();
                    livesyncWorker = new LiveSyncWorker(systemSocket);
                    Thread liveSyncThread = setUpLivesyncThread();
                    liveSyncThread.start();
                }
            } catch (IOException e) {
                if (com.tns.Runtime.isDebuggable()) {
                    e.printStackTrace();
                }
            }
            catch (java.security.NoSuchAlgorithmException e) {
                if (com.tns.Runtime.isDebuggable()) {
                    e.printStackTrace();
                }
            }
        }

        private Thread setUpLivesyncThread() {
            Thread livesyncThread = new Thread(livesyncWorker);
            livesyncThread.setUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
                @Override
                public void uncaughtException(Thread t, Throwable e) {
                    logger.write(String.format("%s(%s): %s", t.getName(), t.getId(), e.toString()));
                }
            });
            livesyncThread.setName("Livesync Thread");
            return livesyncThread;
        }

        @Override
        protected void finalize() throws Throwable {
            deviceSystemSocket.close();
        }
    }

    public void startServer() {
        localServerThread = new LocalServerSocketThread(context.getPackageName() + "-livesync");
        localServerJavaThread = new Thread(localServerThread);
        localServerJavaThread.setName("Livesync Server Thread");
        localServerJavaThread.start();
    }

    private class LiveSyncWorker implements Runnable {
        public static final int OPERATION_BYTE_SIZE = 1;
        public static final int HASH_BYTE_SIZE = 16;
        public static final int LENGTH_BYTE_SIZE = 1;
        public static final int OPERATION_ID_BYTE_SIZE = 32;
        public static final int DELETE_FILE_OPERATION = 7;
        public static final int CREATE_FILE_OPERATION = 8;
        public static final int DO_SYNC_OPERATION = 9;
        public static final int ERROR_REPORT_CODE = 1;
        public static final int OPERATION_END_NO_REFRESH_REPORT_CODE = 3;
        public static final int OPERATION_END_REPORT_CODE = 2;
        public static final int REPORT_CODE_SIZE = 1;
        public static final int DO_REFRESH_LENGTH = 1;
        public static final int DO_REFRESH_VALUE = 1;
        public static final String FILE_NAME = "fileName";
        public static final String FILE_NAME_LENGTH = FILE_NAME + "Length";
        public static final String OPERATION = "operation";
        public static final String FILE_CONTENT = "fileContent";
        public static final String FILE_CONTENT_LENGTH = FILE_CONTENT + "Length";
        public static final int DEFAULT_OPERATION = -1;
        private static final String PROTOCOL_VERSION = "0.2.0";
        private byte[] handshakeMessage;
        private final DigestInputStream input;
        private Closeable livesyncSocket;
        private OutputStream output;

        public LiveSyncWorker(LocalSocket systemSocket) throws IOException, java.security.NoSuchAlgorithmException {
            this.livesyncSocket = systemSocket;
            MessageDigest md = MessageDigest.getInstance("MD5");
            input = new DigestInputStream(systemSocket.getInputStream(), md);
            output = systemSocket.getOutputStream();
            handshakeMessage = getHandshakeMessage();
        }

        public void run() {
            try {
                output.write(handshakeMessage);
                output.flush();

            } catch (IOException e) {
                logger.write(String.format("Error while LiveSyncing: Client socket might be closed!", e.toString()));
                if (com.tns.Runtime.isDebuggable()) {
                    e.printStackTrace();
                }
            }
            try {
                do {
                    int operation = getOperation();

                    if (operation == DELETE_FILE_OPERATION) {

                        String fileName = getFileName();
                        validateData();
                        deleteRecursive(new File(DEVICE_APP_DIR, fileName));

                    } else if (operation == CREATE_FILE_OPERATION) {

                        String fileName = getFileName();
                        int contentLength = getFileContentLength(fileName);
                        validateData();
                        byte[] content = getFileContent(fileName, contentLength);
                        validateData();
                        createOrOverrideFile(fileName, content);

                    } else if (operation == DO_SYNC_OPERATION) {
                        byte[] operationUid = readNextBytes(OPERATION_ID_BYTE_SIZE);
                        byte doRefresh = readNextBytes(DO_REFRESH_LENGTH)[0];
                        int doRefreshInt = (int)doRefresh;
                        int operationReportCode;

                        validateData();
                        if(runtime != null && doRefreshInt == DO_REFRESH_VALUE) {
                            runtime.runScript(new File(NativeScriptSyncServiceSocketImpl.this.context.getFilesDir(), "internal/livesync.js"), false);
                            operationReportCode = OPERATION_END_REPORT_CODE;
                        } else {
                            operationReportCode = OPERATION_END_NO_REFRESH_REPORT_CODE;
                        }

                        output.write(getReportMessageBytes(operationReportCode, operationUid));
                        output.flush();

                    } else if (operation == DEFAULT_OPERATION) {
                        logger.write("LiveSync: input stream is empty!");
                        break;
                    } else {
                        throw new IllegalArgumentException(String.format("\nLiveSync: Operation not recognised. Received operation is %s.", operation));
                    }

                } while (true);
            } catch (Exception e) {
                String message = String.format("Error while LiveSyncing: %s", e.toString());
                this.closeWithError(message);
            } catch (Throwable e) {
                String message = String.format("%s(%s): Error while LiveSyncing.\nOriginal Exception: %s", Thread.currentThread().getName(), Thread.currentThread().getId(), e.toString());
                this.closeWithError(message);
            }

        }

        private byte[] getErrorMessageBytes(String message) {
            return this.getReportMessageBytes(ERROR_REPORT_CODE, message.getBytes());
        }

        private byte[] getReportMessageBytes(int reportType, byte[] messageBytes) {
            byte[] reportBytes = new byte[]{(byte)reportType};
            byte[] combined = new byte[messageBytes.length + REPORT_CODE_SIZE];

            System.arraycopy(reportBytes,0,combined, 0, REPORT_CODE_SIZE);
            System.arraycopy(messageBytes,0,combined, REPORT_CODE_SIZE, messageBytes.length);

            return combined;
        }

        private byte[] getHandshakeMessage() {
            byte[] protocolVersionBytes = PROTOCOL_VERSION.getBytes();
            byte[] versionLength = new byte[]{(byte)protocolVersionBytes.length};
            byte[] packageNameBytes = context.getPackageName().getBytes();
            byte[] combined = new byte[protocolVersionBytes.length + packageNameBytes.length + versionLength.length];

            System.arraycopy(versionLength,0,combined, 0, versionLength.length);
            System.arraycopy(protocolVersionBytes,0,combined, versionLength.length, protocolVersionBytes.length);
            System.arraycopy(packageNameBytes,0,combined,protocolVersionBytes.length + versionLength.length,packageNameBytes.length);

            return combined;
        }

        private void validateData() throws IOException {
            MessageDigest messageDigest = input.getMessageDigest();
            byte[] digest = messageDigest.digest();
            input.on(false);
            byte[] inputMD5 = readNextBytes(HASH_BYTE_SIZE);
            input.on(true);


            if(!Arrays.equals(digest, inputMD5)){
                throw new IllegalStateException(String.format("\nLiveSync: Validation of data failed.\nComputed hash: %s\nOriginal hash: %s ", Arrays.toString(digest), Arrays.toString(inputMD5)));
            }
        }

        /*
        * Tries to read operation input stream
        * If the stream is empty, method returns -1
        * */
        private int getOperation() {
            Integer operation;
            byte[] operationBuff = null;
            try {
                operationBuff = readNextBytes(OPERATION_BYTE_SIZE);
                if (operationBuff == null) {
                    return DEFAULT_OPERATION;
                }
                operation = Integer.parseInt(new String(operationBuff));

            } catch (Exception e) {
                if(operationBuff == null){
                    operationBuff = new byte[]{};
                }
                throw new IllegalStateException(String.format("\nLiveSync: failed to parse %s. Bytes read: $s %s\nOriginal Exception: %s", OPERATION, Arrays.toString(operationBuff), e.toString()));
            }
            return operation;
        }

        private String getFileName() {
            byte[] fileNameBuffer;
            int fileNameLength = -1;

            try {
                fileNameLength = getLength();
            } catch (Exception e) {
                throw new IllegalStateException(String.format("\nLiveSync: failed to parse %s. \nOriginal Exception: %s", FILE_NAME_LENGTH, e.toString()));
            }

            if(fileNameLength <= 0) {
                throw new IllegalStateException(String.format("\nLiveSync: File name length must be positive number or zero. Provided length: %s.", fileNameLength));
            }

            try {
                fileNameBuffer = readNextBytes(fileNameLength);
            } catch (Exception e) {
                throw new IllegalStateException(String.format("\nLiveSync: failed to parse %s.\nOriginal Exception: %s", FILE_NAME, e.toString()));
            }

            if (fileNameBuffer == null) {
                throw new IllegalStateException(String.format("\nLiveSync: Missing %s bytes.", FILE_NAME));
            }

            String fileName = new String(fileNameBuffer);
            if (fileName.trim().length() < fileNameLength) {
                logger.write(String.format("WARNING: %s parsed length is less than %s. We read less information than you specified!", FILE_NAME, FILE_NAME_LENGTH));
            }

            return fileName.trim();
        }

        private int getFileContentLength(String fileName) throws IllegalStateException {
            int contentLength;

            try {
                contentLength = getLength();
            } catch (Exception e) {
                throw new IllegalStateException(String.format("\nLiveSync: failed to read %s for %s.\nOriginal Exception: %s", FILE_CONTENT_LENGTH, fileName, e.toString()));
            }

            if(contentLength < 0){
                throw new IllegalStateException(String.format("\nLiveSync: Content length must be positive number or zero. Provided content length: %s.", contentLength));
            }

            return contentLength;
        }

        private byte[] getFileContent(String fileName, int contentLength) throws IllegalStateException {
            byte[] contentBuff = null;

            try {
                if(contentLength > 0) {
                    contentBuff = readNextBytes(contentLength);
                } else if(contentLength == 0){
                    contentBuff = new byte[]{};
                }
            } catch (Exception e) {
                throw new IllegalStateException(String.format("\nLiveSync: failed to parse content of file: %s.\nOriginal Exception: %s", fileName, e.toString()));
            }

            if (contentLength != 0 && contentBuff == null) {
                throw new IllegalStateException(String.format("\nLiveSync: Missing %s bytes for file: %s. Did you send %s bytes?", FILE_CONTENT, fileName, contentLength));
            }

            return contentBuff;
        }

        private int getLength() {
            byte[] lengthBuffer;
            int lengthInt;

            try {
                byte lengthSize = readNextBytes(LENGTH_BYTE_SIZE)[0];
                //Cast signed byte to unsigned int
                int lengthSizeInt = ((int) lengthSize) & 0xFF;
                lengthBuffer = readNextBytes(lengthSizeInt);

                if (lengthBuffer == null) {
                    throw new IllegalStateException(String.format("\nLiveSync: Missing size length bytes."));
                }
            } catch (Exception e) {
                throw new IllegalStateException(String.format("\nLiveSync: Failed to read size length. \nOriginal Exception: %s", e.toString()));
            }

            try {
                lengthInt = Integer.valueOf(new String(lengthBuffer));
            } catch (Exception e) {
                throw new IllegalStateException(String.format("\nLiveSync: Failed to parse size length. \nOriginal Exception: %s", e.toString()));
            }

            return lengthInt;
        }

        private void createOrOverrideFile(String fileName, byte[] content) throws IOException {
            File fileToCreate = prepareFile(fileName);
            try {

                fileToCreate.getParentFile().mkdirs();
                FileOutputStream fos = new FileOutputStream(fileToCreate.getCanonicalPath());
                if(runtime != null) {
                    runtime.lock();
                }
                fos.write(content);
                fos.close();

            } catch (Exception e) {
                throw new IOException(String.format("\nLiveSync: failed to write file: %s\nOriginal Exception: %s", fileName, e.toString()));
            } finally {
                if(runtime != null) {
                    runtime.unlock();
                }
            }
        }

        void deleteRecursive(File fileOrDirectory) {
            if (fileOrDirectory.isDirectory())
                for (File child : fileOrDirectory.listFiles()) {
                    deleteRecursive(child);
                }

            fileOrDirectory.delete();
        }

        private File prepareFile(String fileName) {
            File fileToCreate = new File(DEVICE_APP_DIR, fileName);
            if (fileToCreate.exists()) {
                fileToCreate.delete();
            }
            return fileToCreate;
        }

        /*
        * Reads next bites from input stream. Bytes read depend on passed parameter.
        * */
        private byte[] readNextBytes(int size) throws IOException {
            byte[] buffer = new byte[size];
            int bytesRead = 0;
            int bufferWriteOffset = bytesRead;
            try {
                do {

                    bytesRead = this.input.read(buffer, bufferWriteOffset, size);
                    if (bytesRead == -1) {
                        if (bufferWriteOffset == 0) {
                            return null;
                        }
                        break;
                    }
                    size -= bytesRead;
                    bufferWriteOffset += bytesRead;
                } while (size > 0);
            } catch (IOException e) {
                String message = e.getMessage();
                if (message != null && message.equals("Try again")) {
                    throw new IllegalStateException("Error while LiveSyncing: Read operation timed out.");
                } else {
                    throw e;
                }
            }

            return buffer;
        }

        private void closeWithError(String message) {
            try {
                output.write(getErrorMessageBytes(message));
                output.flush();
                logger.write(message);
                this.livesyncSocket.close();
            } catch (IOException e) {
                if (com.tns.Runtime.isDebuggable()) {
                    e.printStackTrace();
                }
            }
        }

        @Override
        protected void finalize() throws Throwable {
            this.livesyncSocket.close();
        }
    }
}

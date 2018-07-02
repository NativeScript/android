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
import java.nio.ByteBuffer;

public class NativeScriptSyncServiceSocketIml {
    private static String DEVICE_APP_DIR;

    private final Runtime runtime;
    private static Logger logger;
    private final Context context;

    private LocalServerSocketThread localServerThread;
    private Thread localServerJavaThread;

    public NativeScriptSyncServiceSocketIml(Runtime runtime, Logger logger, Context context) {
        this.runtime = runtime;
        this.logger = logger;
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
                e.printStackTrace();
            }
        }

        public void run() {
            running = true;
            try {
                deviceSystemSocket = new LocalServerSocket(this.name);
                while (running) {
                    LocalSocket systemSocket = deviceSystemSocket.accept();
                    livesyncWorker = new LiveSyncWorker(systemSocket);
                    Thread livesyncThread = setUpLivesyncThread();
                    livesyncThread.start();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            catch (java.security.NoSuchAlgorithmException e) {
                e.printStackTrace();
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
        localServerJavaThread.start();
    }

    private class LiveSyncWorker implements Runnable {
        public static final int OPERATION_BYTE_SIZE = 1;
        public static final int HASH_BYTE_SIZE = 16;
        public static final int FILE_NAME_LENGTH_BYTE_SIZE = 1;
        public static final int CONTENT_LENGTH_BYTE_SIZE = 1;
        public static final int OPERATION_ID_BYTE_SIZE = 32;
        public static final int DELETE_FILE_OPERATION = 7;
        public static final int CREATE_FILE_OPERATION = 8;
        public static final int DO_SYNC_OPERATION = 9;
        public static final String FILE_NAME = "fileName";
        public static final String FILE_NAME_LENGTH = FILE_NAME + "Length";
        public static final String OPERATION = "operation";
        public static final String FILE_CONTENT = "fileContent";
        public static final String FILE_CONTENT_LENGTH = FILE_CONTENT + "Length";
        public static final int DEFAULT_OPERATION = -1;
        public final String LIVESYNC_ERROR_SUGGESTION = String.format("\nMake sure you are following this protocol when transferring files." +
                "\nTransfer protocol: \n\tdelete: (%s)(%s)(%s)" +
                "\n\tcreate: (%s)(%s)(%s)(%s)(%s)" +
                "\n\t%s: exactly %s btye (%s - delete, %s - create)" +
                "\n\t%s: exactly %s bytes" +
                "\n\t%s: relative to app folder" +
                "\n\t%s: exactly %s bytes" +
                "\n\t%s: byte buffer" +
                "\n\tExample delete: 700003./a" +
                "\n\tExample create: 800007./a.txt0000000011fileContent",
                OPERATION, FILE_NAME_LENGTH, FILE_NAME,
                OPERATION, FILE_NAME_LENGTH, FILE_NAME, FILE_CONTENT_LENGTH, FILE_CONTENT,
                OPERATION, OPERATION_BYTE_SIZE, DELETE_FILE_OPERATION, CREATE_FILE_OPERATION,
                FILE_NAME_LENGTH, FILE_NAME_LENGTH_BYTE_SIZE,
                FILE_NAME,
                FILE_CONTENT_LENGTH, CONTENT_LENGTH_BYTE_SIZE,
                FILE_CONTENT);
        private static final String PROTOCOL_VERSION = "0.1.0";
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

            } catch (IOException e) {
                logger.write(String.format("Error while LiveSyncing: Client socket might be closed!", e.toString()));
                e.printStackTrace();
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
                        byte[] contentLength = getFileContentLength(fileName);
                        validateData();
                        byte[] content = getFileContent(fileName, contentLength);
                        validateData();
                        createOrOverrideFile(fileName, content);

                    } else if (operation == DO_SYNC_OPERATION) {
                        byte[] operationUid = readNextBytes(OPERATION_ID_BYTE_SIZE);

                        validateData();
                        if(runtime != null) {
                            runtime.runScript(new File(NativeScriptSyncServiceSocketIml.this.context.getFilesDir(), "internal/livesync.js"));
                        }

                        output.write(operationUid);

                    } else if (operation == DEFAULT_OPERATION) {
                        logger.write("LiveSync: input stream is empty!");
                        break;
                    } else {
                        throw new IllegalArgumentException(String.format("\nLiveSync: Operation not recognised. %s", LIVESYNC_ERROR_SUGGESTION));
                    }

                } while (true);

            } catch (Exception e) {
                logger.write(String.format("Error while LiveSyncing: %s", e.toString()));
                e.printStackTrace();
                flushInputStream();
            } catch (Throwable e) {
                logger.write(String.format("%s(%s): Error while LiveSyncing.\nOriginal Exception: %s", Thread.currentThread().getName(), Thread.currentThread().getId(), e.toString()));
                try {
                    this.livesyncSocket.close();
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
            }

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
                throw new IllegalArgumentException(String.format("\nLiveSync: Operation not recognised. %s", LIVESYNC_ERROR_SUGGESTION));
            }
        }

        private void flushInputStream() {
            try {
                this.input.skip(Long.MAX_VALUE);
            } catch (IOException e) {
            }
        }

        /*
        * Tries to read operation input stream
        * If the stream is empty, method returns -1
        * */
        private int getOperation() {
            Integer operation = DEFAULT_OPERATION;
            try {

                byte[] operationBuff = readNextBytes(OPERATION_BYTE_SIZE);
                if (operationBuff == null) {
                    return DEFAULT_OPERATION;
                }
                operation = Integer.parseInt(new String(operationBuff));

            } catch (Exception e) {
                throw new IllegalStateException(String.format("\nLiveSync: failed to parse %s. %s\nOriginal Exception: %s", OPERATION, LIVESYNC_ERROR_SUGGESTION, e.toString()));
            }
            return operation;
        }

        private String getFileName() {
            byte[] fileNameBuffer;
            byte fileNameLengthSize;
            int fileNameLenth = -1;
            byte[] fileNameLengthBuffer;

            try {

                fileNameLengthSize = readNextBytes(FILE_NAME_LENGTH_BYTE_SIZE)[0];
                fileNameLengthBuffer = readNextBytes(fileNameLengthSize);

            } catch (Exception e) {
                throw new IllegalStateException(String.format("\nLiveSync: failed to parse %s. %s\nOriginal Exception: %s", FILE_NAME_LENGTH, LIVESYNC_ERROR_SUGGESTION, e.toString()));
            }

            if (fileNameLengthBuffer == null) {
                throw new IllegalStateException(String.format("\nLiveSync: Missing %s bytes. %s", FILE_NAME_LENGTH, LIVESYNC_ERROR_SUGGESTION));
            }

            try {
                fileNameLenth = Integer.valueOf(new String(fileNameLengthBuffer));
                fileNameBuffer = readNextBytes(fileNameLenth);

            } catch (NumberFormatException e) {
                throw new IllegalStateException(String.format("\nLiveSync: failed to parse %s. %s\nOriginal Exception: %s", FILE_NAME, LIVESYNC_ERROR_SUGGESTION, e.toString()));
            } catch (Exception e) {
                throw new IllegalStateException(String.format("\nLiveSync: failed to parse %s. %s\nOriginal Exception: %s", FILE_NAME, LIVESYNC_ERROR_SUGGESTION, e.toString()));
            }

            if (fileNameBuffer == null) {
                throw new IllegalStateException(String.format("\nLiveSync: Missing %s bytes. %s", FILE_NAME, LIVESYNC_ERROR_SUGGESTION));
            }

            String fileName = new String(fileNameBuffer);
            if (fileName.trim().length() < fileNameLenth) {
                logger.write(String.format("WARNING: %s parsed length is less than %s. We read less information than you specified!", FILE_NAME, FILE_NAME_LENGTH));
            }

            return fileName.trim();
        }

        private byte[] getFileContentLength(String fileName)throws IllegalStateException {
            byte[] contentLength;
            try {
                byte contentLengthSize = readNextBytes(CONTENT_LENGTH_BYTE_SIZE)[0];
                contentLength = readNextBytes(contentLengthSize);
            } catch (Exception e) {
                throw new IllegalStateException(String.format("\nLiveSync: failed to parse %s %s. %s\nOriginal Exception: %s", fileName, FILE_CONTENT_LENGTH, LIVESYNC_ERROR_SUGGESTION, e.toString()));
            }

            return contentLength;
        }

        private byte[] getFileContent(String fileName, byte[] contentLength) throws IllegalStateException {
            byte[] contentBuff = null;
            int contentLengthInt = -1;

            if (contentLength == null) {
                throw new IllegalStateException(String.format("\nLiveSync: Missing %s bytes. Did you send %s %s? %s", FILE_CONTENT_LENGTH, fileName, FILE_CONTENT_LENGTH, LIVESYNC_ERROR_SUGGESTION));
            }

            try {
                contentLengthInt = Integer.parseInt(new String(contentLength));

                if(contentLengthInt > 0) {
                    contentBuff = readNextBytes(contentLengthInt);
                } else if(contentLengthInt == 0){
                    contentBuff = new byte[]{};
                }

            } catch (NumberFormatException e) {
                throw new IllegalStateException(String.format("\nLiveSync: failed to parse %s %s. %s\nOriginal Exception: %s", fileName, contentLength, contentLengthInt, e.toString()));
            } catch (Exception e) {
                throw new IllegalStateException(String.format("\nLiveSync: failed to parse %s %s. %s\nOriginal Exception: %s", fileName, contentLength, LIVESYNC_ERROR_SUGGESTION, e.toString()));
            }

            if (contentLengthInt != 0 && contentBuff == null) {
                throw new IllegalStateException(String.format("\nLiveSync: Missing %s bytes. Did you send %s %s? %s", FILE_CONTENT, fileName, FILE_CONTENT, LIVESYNC_ERROR_SUGGESTION));
            }

            return contentBuff;
        }

        private void createOrOverrideFile(String fileName, byte[] content) throws IOException {
            File fileToCreate = prepareFile(fileName);
            try {

                fileToCreate.getParentFile().mkdirs();
                FileOutputStream fos = new FileOutputStream(fileToCreate.getCanonicalPath());
                fos.write(content);
                fos.close();

            } catch (Exception e) {
                throw new IOException(String.format("\nLiveSync: failed to write file: %s\nOriginal Exception: %s", fileName, e.toString()));
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

            return buffer;
        }

        @Override
        protected void finalize() throws Throwable {
            this.livesyncSocket.close();
        }
    }
}

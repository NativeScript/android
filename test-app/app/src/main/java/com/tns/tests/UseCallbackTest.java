package com.tns.tests;


public class UseCallbackTest {
    public UseCallbackTest(int data) {
        this.data = new Data(data);
    }

    public boolean enqueDataCallback(final long delay) {
        Runnable r = new Runnable() {
            @Override
            public void run() {
                dataCallback.onData(data, delay);
            }
        };

        Thread t = new Thread(r);
        t.start();

        return true;
    }

    public boolean enqueCleanCallback(final long delay) {
        Runnable r = new Runnable() {
            @Override
            public void run() {
                cleanCallback.onClean(delay);
            }
        };

        Thread t = new Thread(r);
        t.start();

        return true;
    }

    public void setDataCallback(DataCallback dataCallback) {
        this.dataCallback = dataCallback;
    }

    public void setCleanCallback(CleanCallback cleanCallback) {
        this.cleanCallback = cleanCallback;
    }


    public interface DataCallback {
        void onData(Data data, long delay);
    }

    public interface CleanCallback {
        void onClean(long delay);
    }

    public static final class Data {
        public Data(int data) {
            setData(data);
        }

        public int getData() {
            return data;
        }

        public void setData(int data) {
            this.data = data;
        }

        private int data;
    }

    private DataCallback dataCallback;
    private CleanCallback cleanCallback;
    private Data data;
}

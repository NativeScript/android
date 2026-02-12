package com.tns.tests;

import java.util.ArrayList;

public class ConcurrentAccessTest {
    
    public interface Callback {
        void invoke(ArrayList list1, ArrayList list2, ArrayList list3, ArrayList list4, ArrayList list5,
                   ArrayList list6, ArrayList list7, ArrayList list8, ArrayList list9, ArrayList list10);
    }
    
    public interface ErrorCallback {
        void onError(Throwable error);
    }
    
    /**
     * Calls the callback from a background thread multiple times.
     * @param callback The callback to invoke
     * @param times Number of times to call the callback (default 50)
     */
    public static void callFromBackgroundThread(final Callback callback, final int times) {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                for (int i = 0; i < times; i++) {
                    invokeCallbackWithArrayLists(callback, i);
                }
            }
        });
        thread.start();
    }
    
    /**
     * Calls the callback synchronously from the current thread.
     * @param callback The callback to invoke
     * @param times Number of times to call the callback (default 50)
     */
    public static void callSynchronously(Callback callback, int times) {
        for (int i = 0; i < times; i++) {
            invokeCallbackWithArrayLists(callback, i);
        }
    }
    
    /**
     * Helper method that creates 10 ArrayLists and invokes the callback with them.
     * Each ArrayList contains some data based on the iteration number.
     */
    private static void invokeCallbackWithArrayLists(Callback callback, int iteration) {
        ArrayList<Integer> list1 = new ArrayList<>();
        ArrayList<Integer> list2 = new ArrayList<>();
        ArrayList<Integer> list3 = new ArrayList<>();
        ArrayList<Integer> list4 = new ArrayList<>();
        ArrayList<Integer> list5 = new ArrayList<>();
        ArrayList<Integer> list6 = new ArrayList<>();
        ArrayList<Integer> list7 = new ArrayList<>();
        ArrayList<Integer> list8 = new ArrayList<>();
        ArrayList<Integer> list9 = new ArrayList<>();
        ArrayList<Integer> list10 = new ArrayList<>();
        
        // Add some data to each list
        for (int i = 0; i < 5; i++) {
            list1.add(iteration * 10 + i);
            list2.add(iteration * 10 + i + 1);
            list3.add(iteration * 10 + i + 2);
            list4.add(iteration * 10 + i + 3);
            list5.add(iteration * 10 + i + 4);
            list6.add(iteration * 10 + i + 5);
            list7.add(iteration * 10 + i + 6);
            list8.add(iteration * 10 + i + 7);
            list9.add(iteration * 10 + i + 8);
            list10.add(iteration * 10 + i + 9);
        }
        
        callback.invoke(list1, list2, list3, list4, list5, list6, list7, list8, list9, list10);
    }
}
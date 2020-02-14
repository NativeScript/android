describe("Test multithreaded javascript ", () => {
    it("Should execute callbacks on specified native thread", done => {
        const currentThreadId = java.lang.Thread.currentThread().getId();
        new java.lang.Thread(new java.lang.Runnable({
            run() {
                const threadId = java.lang.Thread.currentThread().getId();
                expect(threadId).not.toEqual(currentThreadId);

                const mainHandler = new android.os.Handler(android.os.Looper.getMainLooper());
                // done() must called on the main thread to ensure that jasmine tests
                // continue executing there
                mainHandler.post(new java.lang.Runnable({
                    run: done
                }));
            }
        })).start();
    });
});

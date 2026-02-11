// WARNING: IF THIS TEST FAILS IT COMPLETELY BREAKS ALL OTHER TESTS!

describe("Tests concurrent access to JNI", function () {
  // Customizable test parameters
  const BACKGROUND_THREADS = 5;
  const SYNC_CALLS = 2;
  const ITERATIONS_PER_CALL = 100;
  const TIMEOUT_MS = 3000;

  it("test_high_contention_concurrent_access_with_multiple_objects", (done) => {
    console.log('STARTING PROBLEMATIC TEST. THIS MIGHT CRASH OR CAUSE ISSUES IN OTHER TESTS IF IT FAILS. If this is close to the end of the log, check test_high_contention_concurrent_access_with_multiple_objects');
    let callbackInvocations = 0;

    const callback = new com.tns.tests.ConcurrentAccessTest.Callback({
      invoke: (
        list1,
        list2,
        list3,
        list4,
        list5,
        list6,
        list7,
        list8,
        list9,
        list10,
      ) => {
        callbackInvocations++;
        // Assert that accessing size() on any of the lists doesn't throw
        expect(() => list1.size()).not.toThrow();
        expect(() => list2.size()).not.toThrow();
        expect(() => list3.size()).not.toThrow();
        expect(() => list4.size()).not.toThrow();
        expect(() => list5.size()).not.toThrow();
        expect(() => list6.size()).not.toThrow();
        expect(() => list7.size()).not.toThrow();
        expect(() => list8.size()).not.toThrow();
        expect(() => list9.size()).not.toThrow();
        expect(() => list10.size()).not.toThrow();

        // Verify that the lists actually have content
        expect(list1.size()).toBe(5);
        expect(list2.size()).toBe(5);
        expect(list3.size()).toBe(5);
        expect(list4.size()).toBe(5);
        expect(list5.size()).toBe(5);
        expect(list6.size()).toBe(5);
        expect(list7.size()).toBe(5);
        expect(list8.size()).toBe(5);
        expect(list9.size()).toBe(5);
        expect(list10.size()).toBe(5);
      },
    });

    // Start multiple background threads
    for (let i = 0; i < BACKGROUND_THREADS; i++) {
      com.tns.tests.ConcurrentAccessTest.callFromBackgroundThread(
        callback,
        ITERATIONS_PER_CALL,
      );
    }

    // Call synchronously multiple times
    for (let i = 0; i < SYNC_CALLS; i++) {
      com.tns.tests.ConcurrentAccessTest.callSynchronously(
        callback,
        ITERATIONS_PER_CALL,
      );
    }

    // Wait for all threads to complete
    setTimeout(() => {
      const expectedInvocations =
        (BACKGROUND_THREADS + SYNC_CALLS) * ITERATIONS_PER_CALL;
      expect(callbackInvocations).toBe(expectedInvocations);
      done();
    }, TIMEOUT_MS);
  });
});

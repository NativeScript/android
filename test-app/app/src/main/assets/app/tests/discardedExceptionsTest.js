describe("Tests discarded exception ", function () {
    var originalOnDiscardedError = null;

    beforeEach(function() {
      originalOnDiscardedError = global.__onDiscardedError;
    });

    it("should report discarded exception", function () {
        var reportedException = null;

        global.__onDiscardedError = (error) => {
            reportedException = error;
        }

        var test = new com.tns.tests.DiscardedExceptionTest();
        test.reportSupressedException();

        expect(reportedException).not.toBe(null);
        expect(reportedException.nativeException).not.toBe(null);
        expect(reportedException.nativeException.getMessage()).toBe('Exception to suppress');
        expect(reportedException.message).toBe('Exception to suppress');
        expect(reportedException.stackTrace).toContain('com.tns.tests.DiscardedExceptionTest.reportSupressedException');
    });

    afterEach(function() {
      global.__onDiscardedError = originalOnDiscardedError;
    });
});
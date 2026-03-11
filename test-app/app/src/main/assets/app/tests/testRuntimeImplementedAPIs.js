describe("Runtime exposes", function () {
  it("__time a low overhead, high resolution, time in ms.", function () {
    // Try to get the times using Date.now and __time and compare the results, expect them to be somewhat "close".
    // Sometimes GC hits after Date.now is captured but before __time or the vice-versa and the test fails,
    // so we are giving it several attempts.
    for (var i = 0; i < 5; i++) {
      try {
        var dateTimeStart = Date.now();
        var timeStart = __time();
        var acc = 0;
        var s = android.os.SystemClock.elapsedRealtime();
        for (var i = 0; i < 1000; i++) {
          var c = android.os.SystemClock.elapsedRealtime();
          acc += c - s;
          s = c;
        }
        var dateTimeEnd = Date.now();
        var timeEnd = __time();
        var dateDelta = dateTimeEnd - dateTimeStart;
        var timeDelta = timeEnd - timeStart;
        var tolerance = Math.max(10, dateDelta * 0.5);
        expect(timeDelta > 0).toBe(true);
        expect(Math.abs(dateDelta - timeDelta) < tolerance).toBe(true);
        break;
      } catch (e) {
        if (i == 4) {
          throw e;
        }
      }
    }
  });
});

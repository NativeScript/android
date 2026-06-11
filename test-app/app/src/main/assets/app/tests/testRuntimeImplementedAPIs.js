describe("Runtime exposes", function () {
  it("__time a low overhead, high resolution, time in ms.", function () {
    // Compare the Date.now and __time deltas over the same interval and expect
    // them to be somewhat "close". A GC pause or scheduler preemption landing
    // between the capture of the two clocks skews a single measurement, so the
    // window is kept wide enough for the proportional tolerance to absorb
    // realistic pauses, and the measurement is retried a few times with the
    // expectation only recorded once (jasmine expectations don't throw, so a
    // try/catch based retry would record the failed attempts anyway).
    var attempts = 5;
    var dateDelta, timeDelta, tolerance;
    var ok = false;

    for (var attempt = 0; attempt < attempts && !ok; attempt++) {
      var dateTimeStart = Date.now();
      var timeStart = __time();

      while (Date.now() - dateTimeStart < 50) {
        // busy-wait to widen the measured interval
      }

      var dateTimeEnd = Date.now();
      var timeEnd = __time();

      dateDelta = dateTimeEnd - dateTimeStart;
      timeDelta = timeEnd - timeStart;
      tolerance = Math.max(10, dateDelta * 0.5);
      ok = timeDelta > 0 && Math.abs(dateDelta - timeDelta) < tolerance;
    }

    expect(ok).toBe(true, "__time delta " + timeDelta + "ms diverged from Date.now delta " + dateDelta + "ms (tolerance " + tolerance + "ms) on all " + attempts + " attempts");
  });
});

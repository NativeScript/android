describe("Runtime exposes", function () {
  it("__time a low overhead, high resolution, time in ms.", function() {
    var dateTimeStart = Date.now();
    var timeStart = __time();
    var acc = 0;
    var s = android.os.SystemClock.elapsedRealtime();
    for (var i = 0; i < 1000; i++) {
      var c = android.os.SystemClock.elapsedRealtime();
      acc += (c - s);
      s = c;
    }
    var dateTimeEnd = Date.now();
    var timeEnd = __time();
    var dateDelta = dateTimeEnd - dateTimeStart;
    var timeDelta = timeEnd - timeStart;
    expect(Math.abs(dateDelta - timeDelta) < dateDelta * 0.25).toBe(true);
  });
});
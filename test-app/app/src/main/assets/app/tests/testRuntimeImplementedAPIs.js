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

// The shared Performance suite (submodule) gates itself on the API being
// present and skips otherwise; this unguarded canary makes absence on THIS
// runtime a failure rather than a silent skip.
describe("Performance API canary", function () {
  it("implements the Performance API", function () {
    expect(typeof performance.mark).toBe("function");
    expect(typeof PerformanceObserver).toBe("function");
  });
});

// The shared StructuredClone suite skips itself where the API is missing, which
// would turn this runtime losing structuredClone into a green run. This spec is
// deliberately unguarded so that regression fails instead.
describe("structuredClone canary", function () {
  it("is implemented by this runtime", function () {
    expect(typeof structuredClone).toBe("function");
  });
});

// Same contract as above for the shared DOMException / CustomEvent suites:
// they self-gate, these unguarded specs turn absence into a failure.
describe("DOMException canary", function () {
  it("is implemented by this runtime", function () {
    expect(typeof DOMException).toBe("function");
    expect(new DOMException("x", "AbortError") instanceof Error).toBe(true);
  });

  it("is not reachable as a module from app code", function () {
    expect(function () { require("internal/dom-exception"); }).toThrow();
  });

  it("serializes through structuredClone on this runtime", function () {
    var clone = structuredClone(new DOMException("x", "AbortError"));
    expect(clone instanceof DOMException).toBe(true);
    expect(clone.name).toBe("AbortError");
  });

  it("clones the isolate's first DOMException even when a getter creates it mid-clone", function (done) {
    var worker = new Worker("./domExceptionFirstCloneWorker.js");
    worker.onmessage = function (event) {
      expect(event.data.isDomException).toBe(true);
      expect(event.data.name).toBe("AbortError");
      expect(event.data.message).toBe("first in this isolate");
      worker.terminate();
      done();
    };
    worker.onerror = function (event) {
      fail("worker error: " + event.message);
      worker.terminate();
      done();
      return true;
    };
  });

  // Once an isolate holds a DOMException the serializer claims host objects
  // itself, and V8 then stops detecting native wrappers on its own. These are
  // the shapes that would silently clone as {} if the claim missed them.
  it("still rejects native wrappers once a DOMException exists", function () {
    new DOMException("x", "AbortError");
    var wrappers = [new java.lang.Object(), new URL("https://example.com/")];
    for (var i = 0; i < wrappers.length; i++) {
      var error;
      try {
        structuredClone(wrappers[i]);
      } catch (e) {
        error = e;
      }
      expect(error instanceof DOMException).toBe(true);
      expect(error.name).toBe("DataCloneError");
    }
  });
});

describe("CustomEvent canary", function () {
  it("is implemented by this runtime", function () {
    expect(typeof CustomEvent).toBe("function");
    expect(new CustomEvent("x") instanceof Event).toBe(true);
  });
});

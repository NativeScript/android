describe("test PostFrameCallback", function () {
  const defaultWaitTime = 300;
  it("__postFrameCallback exists", () => {
    expect(global.__postFrameCallback).toBeDefined();
  });

  it("__removeFrameCallback exists", () => {
    expect(global.__removeFrameCallback).toBeDefined();
  });

  it("should throw when providing wrong arguments", () => {
    expect(() => global.__postFrameCallback(null)).toThrow();
    expect(() => global.__removeFrameCallback(null)).toThrow();
    expect(() => global.__postFrameCallback("")).toThrow();
    expect(() => global.__removeFrameCallback("")).toThrow();
    expect(() => global.__postFrameCallback()).toThrow();
    expect(() => global.__removeFrameCallback()).toThrow();
  });

  it("should call the callback once", (done) => {
    let callCount = 0;
    const callback = () => {
      callCount++;
    };
    global.__postFrameCallback(callback);
    setTimeout(() => {
      expect(callCount).toBe(1);
      done();
    }, defaultWaitTime);
  });

  it("should call the callback once even if scheduled multiple times", (done) => {
    let callCount = 0;
    const callback = () => {
      callCount++;
    };
    global.__postFrameCallback(callback);
    global.__postFrameCallback(callback);
    setTimeout(() => {
      expect(callCount).toBe(1);
      done();
    }, defaultWaitTime);
  });

  it("should not trigger the callback if it was canceled", (done) => {
    let callCount = 0;
    const callback = () => {
      callCount++;
    };
    global.__postFrameCallback(callback);
    global.__removeFrameCallback(callback);
    setTimeout(() => {
      expect(callCount).toBe(0);
      done();
    }, defaultWaitTime);
  });

  it("should trigger the callback if it was canceled then re-scheduled", (done) => {
    let callCount = 0;
    const callback = () => {
      callCount++;
    };
    global.__postFrameCallback(callback);
    global.__removeFrameCallback(callback);
    global.__postFrameCallback(callback);
    setTimeout(() => {
      expect(callCount).toBe(1);
      done();
    }, defaultWaitTime);
  });

  it("should trigger the callback if it was re-scheduled by itself", (done) => {
    let callCount = 0;
    const callback = () => {
      callCount++;
      if (callCount === 1) {
        global.__postFrameCallback(callback);
      }
    };
    global.__postFrameCallback(callback);
    setTimeout(() => {
      expect(callCount).toBe(2);
      done();
    }, defaultWaitTime);
  });

  it("should release the callback after being done", (done) => {
    let callCount = 0;
    let callback = () => {
      callCount++;
    };
    global.__postFrameCallback(callback);
    const weakCallback = new WeakRef(callback);
    callback = null;
    gc();
    setTimeout(() => {
      gc();
      expect(callCount).toBe(1);
      expect(!!weakCallback.deref()).toBe(false);
      done();
    }, defaultWaitTime);
  });

  it("should release the callback removal", (done) => {
    let callCount = 0;
    let callback = () => {
      callCount++;
    };
    global.__postFrameCallback(callback);
    global.__removeFrameCallback(callback);
    const weakCallback = new WeakRef(callback);
    callback = null;
    gc();
    setTimeout(() => {
      gc();
      expect(callCount).toBe(0);
      expect(!!weakCallback.deref()).toBe(false);
      done();
    }, defaultWaitTime);
  });

  it("should retain callback until called", (done) => {
    let callCount = 0;
    let callback = () => {
      callCount++;
      gc();
      expect(!!weakCallback.deref()).toBe(true);
    };
    global.__postFrameCallback(callback);
    global.__removeFrameCallback(callback);
    global.__postFrameCallback(callback);
    const weakCallback = new WeakRef(callback);
    callback = null;
    gc();
    setTimeout(() => {
      gc();
      expect(callCount).toBe(1);
      expect(!!weakCallback.deref()).toBe(false);
      done();
    }, defaultWaitTime);
  });
});

// The two implementations behind __postFrameCallback (NDK AChoreographer,
// android.view.Choreographer for API < 24) must be indistinguishable from JS.
// A modern device always selects the NDK one, so the Java bridge is only
// reachable through __setFrameCallbackImpl, which debug runtimes expose for
// exactly this.
describe("frame callback timestamps", function () {
  const defaultWaitTime = 300;
  const impls = ["native", "java"];

  afterEach(() => {
    if (typeof global.__setFrameCallbackImpl === "function") {
      global.__setFrameCallbackImpl("auto");
    }
  });

  function withImpl(impl) {
    if (typeof global.__setFrameCallbackImpl !== "function") {
      return impl === "native";
    }
    return global.__setFrameCallbackImpl(impl) === impl;
  }

  impls.forEach((impl) => {
    describe(impl + " implementation", function () {
      it("passes the raw frame time and a performance-timeline timestamp", (done) => {
        if (!withImpl(impl)) {
          pending("this runtime cannot select the " + impl + " implementation");
          return;
        }

        global.__postFrameCallback((frameTimeNanos, performanceMillis) => {
          const now = performance.now();

          expect(typeof frameTimeNanos).toBe("number");
          expect(typeof performanceMillis).toBe("number");

          // Uptime-scale nanoseconds, not epoch-scale milliseconds.
          expect(frameTimeNanos).toBeGreaterThan(1e9);
          expect(frameTimeNanos / 1e6).toBeLessThan(Date.now());

          // The frame is stamped just before the callback runs, so its
          // timeline position sits a frame or two behind the reading taken
          // inside it, never ahead of it.
          expect(performanceMillis).toBeGreaterThan(0);
          expect(performanceMillis).not.toBeGreaterThan(now);
          expect(now - performanceMillis).toBeLessThan(250);

          // Both arguments describe the same instant, so their difference is
          // the timeline's monotonic origin. System.nanoTime() is on that same
          // clock, so the pair (nanoTime, now) must yield the same origin --
          // this is what would break if either argument moved off the base.
          const originFromFrame = frameTimeNanos / 1e6 - performanceMillis;
          const originFromClock = java.lang.System.nanoTime() / 1e6 - performance.now();
          expect(Math.abs(originFromFrame - originFromClock)).toBeLessThan(5);
          done();
        });
      });

      it("advances across consecutive frames", (done) => {
        if (!withImpl(impl)) {
          pending("this runtime cannot select the " + impl + " implementation");
          return;
        }

        const frames = [];
        const callback = (frameTimeNanos, performanceMillis) => {
          frames.push({ nanos: frameTimeNanos, millis: performanceMillis });
          if (frames.length === 1) {
            global.__postFrameCallback(callback);
          }
        };
        global.__postFrameCallback(callback);

        setTimeout(() => {
          expect(frames.length).toBe(2);
          expect(frames[1].nanos).not.toBeLessThan(frames[0].nanos);
          expect(frames[1].millis).not.toBeLessThan(frames[0].millis);
          // The origin the two arguments imply is a constant of the isolate.
          const origin = (f) => f.nanos / 1e6 - f.millis;
          expect(Math.abs(origin(frames[1]) - origin(frames[0]))).toBeLessThan(1);
          done();
        }, defaultWaitTime);
      });
    });
  });
});

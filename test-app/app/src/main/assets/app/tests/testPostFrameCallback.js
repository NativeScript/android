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

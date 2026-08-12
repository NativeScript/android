// Performance specs local to this runtime; the cross-runtime coverage lives in
// the shared suite (app/shared/Performance).
describe("Performance measure argument coercion", function () {
  beforeEach(function () {
    performance.clearMarks();
    performance.clearMeasures();
  });

  // The startOrMeasureOptions parameter is a (DOMString or
  // PerformanceMeasureOptions) union, and WebIDL converts null for such a
  // union to an empty dictionary -- so null means "no options", never the mark
  // name "null".
  it("Should treat a null options argument as no argument at all", function () {
    performance.mark("null-start");

    const fromNull = performance.measure("from-null", null);
    const fromOmitted = performance.measure("from-omitted");

    expect(fromNull.startTime).toBe(0);
    expect(fromNull.startTime).toBe(fromOmitted.startTime);
    expect(fromNull.duration).toBeGreaterThan(0);
    expect(fromNull.detail).toBeNull();
  });

  it("Should pair a null options argument with an end mark", function () {
    performance.mark("the-end");
    const endTime = performance.getEntriesByName("the-end")[0].startTime;

    const measure = performance.measure("null-and-end", null, "the-end");

    expect(measure.startTime).toBe(0);
    expect(measure.duration).toBe(endTime);
  });

  // endMark is a plain optional DOMString, not a union and not nullable, so
  // null stringifies and names a mark that does not exist.
  it("Should reject a null end mark", function () {
    // Present so the only mark this can fail to resolve is the null one.
    performance.mark("the-start");

    let thrown = null;
    try {
      performance.measure("null-end", "the-start", null);
    } catch (e) {
      thrown = e;
    }
    expect(thrown).not.toBeNull();
    expect(thrown && thrown.name).toBe("SyntaxError");
  });
});

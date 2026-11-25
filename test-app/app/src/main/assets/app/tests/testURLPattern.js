
describe("URLPattern", function () {
  it("throws on invalid URLPattern", function () {
    var exceptionCaught = false;
    try {
      const pattern = new URLPattern(1);
    } catch (e) {
      exceptionCaught = true;
    }
    expect(exceptionCaught).toBe(true);
  });

  it("does not throw on valid URLPattern", function () {
    var exceptionCaught = false;
    try {
      const pattern = new URLPattern("https://example.com/books/:id");
    } catch (e) {
      exceptionCaught = true;
    }
    expect(exceptionCaught).toBe(false);
  });

  it("parses simple pattern", function () {
    const pattern = new URLPattern("https://example.com/books/:id");
    expect(pattern.protocol).toBe("https");
    expect(pattern.hostname).toBe("example.com");
    expect(pattern.pathname).toBe("/books/:id");
    expect(pattern.port).toBe("");
    expect(pattern.search).toBe("*");
    expect(pattern.hash).toBe("*");
    expect(pattern.username).toBe("*");
    expect(pattern.password).toBe("*");
    expect(pattern.hasRegExpGroups).toBe(false);
  });


  it("accepts non-ASCII group names", function () {
    const pattern = new URLPattern({ pathname: "/:caf\u00E9/:\u540D\u524D" });
    const result = pattern.exec({ pathname: "/a/b" });
    expect(result.pathname.groups["caf\u00E9"]).toBe("a");
    expect(result.pathname.groups["\u540D\u524D"]).toBe("b");
  });

  it("rejects a group name that does not start with an identifier start", function () {
    expect(() => new URLPattern({ pathname: "/:\u0663x" })).toThrowError(TypeError);
  });

  it("parses with undefined base", function () {
    const pattern = new URLPattern("https://google.com", undefined);
    expect(pattern.protocol).toBe("https");
    expect(pattern.hostname).toBe("google.com");
  });

  it("parses with null base", function () {
    const pattern = new URLPattern("https://google.com", null);
    expect(pattern.protocol).toBe("https");
    expect(pattern.hostname).toBe("google.com");
  });

});

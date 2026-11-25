describe("URL", function () {
    it("throws on invalid URL", function () {
      var exceptionCaught = false;
      try {
        const url = new URL("");
      } catch (e) {
        exceptionCaught = true;
      }
      expect(exceptionCaught).toBe(true);
    });
  
    it("does not throw on valid URL", function () {
      var exceptionCaught = false;
      try {
        const url = new URL("https://google.com");
      } catch (e) {
        exceptionCaught = true;
      }
      expect(exceptionCaught).toBe(false);
    });
  
    it("parses simple urls", function () {
      const url = new URL("https://google.com");
      expect(url.protocol).toBe("https:");
      expect(url.hostname).toBe("google.com");
      expect(url.pathname).toBe("/");
      expect(url.port).toBe("");
      expect(url.search).toBe("");
      expect(url.hash).toBe("");
      expect(url.username).toBe("");
      expect(url.password).toBe("");
      expect(url.origin).toBe("https://google.com");
      expect(url.searchParams.size).toBe(0);
    });
  
    it("parses with undefined base", function () {
      const url = new URL("https://google.com", undefined);
      expect(url.protocol).toBe("https:");
      expect(url.hostname).toBe("google.com");
    });
  
    it("parses with null base", function () {
      const url = new URL("https://google.com", null);
      expect(url.protocol).toBe("https:");
      expect(url.hostname).toBe("google.com");
    });
  
    it("parses query strings", function () {
      const url = new URL("https://google.com?q=hello");
      expect(url.search).toBe("?q=hello");
      expect(url.searchParams.get("q")).toBe("hello");
      expect(url.pathname).toBe("/");
    });
  
    it("parses query strings with pathname", function () {
      const url = new URL("https://google.com/some/path?q=hello");
      expect(url.search).toBe("?q=hello");
      expect(url.searchParams.get("q")).toBe("hello");
      expect(url.pathname).toBe("/some/path");
    });

    it("throws a TypeError without arguments", function () {
      expect(() => new URL()).toThrowError(TypeError);
    });

    it("resolves a relative URL against a string base", function () {
      const url = new URL("../c?d=1#e", "https://a.test/x/y/z");
      expect(url.href).toBe("https://a.test/x/c?d=1#e");
    });

    it("resolves a relative URL against a URL object base", function () {
      const url = new URL("page", new URL("https://a.test/dir/"));
      expect(url.href).toBe("https://a.test/dir/page");
    });

    it("throws a TypeError for an invalid base", function () {
      expect(() => new URL("page", "not a url")).toThrowError(TypeError);
    });

    it("throws a TypeError when the input does not parse against the base", function () {
      expect(() => new URL("http://[", "https://a.test/")).toThrowError(TypeError);
    });

    it("propagates an exception thrown by the base's toString", function () {
      const base = {
        toString() {
          throw new Error("base toString");
        },
      };
      expect(() => new URL("page", base)).toThrowError("base toString");
    });

    it("canParse agrees with the constructor", function () {
      expect(URL.canParse("https://a.test/")).toBe(true);
      expect(URL.canParse("not a url")).toBe(false);
      expect(URL.canParse("page", "https://a.test/dir/")).toBe(true);
      expect(URL.canParse("page", "not a url")).toBe(false);
      expect(URL.canParse("http://[", "https://a.test/")).toBe(false);
    });

    it("canParse stringifies its arguments and ignores an undefined base", function () {
      expect(URL.canParse(new URL("https://a.test/x"))).toBe(true);
      expect(URL.canParse("page", new URL("https://a.test/dir/"))).toBe(true);
      expect(URL.canParse("https://a.test/", undefined)).toBe(true);
    });

    it("converts internationalized hostnames to ASCII", function () {
      expect(new URL("https://B\u00FCcher.example/").hostname).toBe("xn--bcher-kva.example");
      expect(new URL("https://\u4F8B\u3048.\u30C6\u30B9\u30C8/").hostname).toBe("xn--r8jz45g.xn--zckzah");
      expect(new URL("https://XN--BCHER-KVA.example/").hostname).toBe("xn--bcher-kva.example");
      expect(() => new URL("https://a\u200Db.example/")).toThrowError(TypeError);
    });
  });
  
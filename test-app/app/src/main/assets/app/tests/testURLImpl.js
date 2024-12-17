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
  });
  
describe("Test URLSearchParams ", function () {
    const fooBar = "foo=1&bar=2";
    it("Test URLSearchParams keys", function(){
        const params = new URLSearchParams(fooBar);
        const keys = params.keys();
        expect(keys[0]).toBe("foo");
        expect(keys[1]).toBe("bar");
    });

    it("Test URLSearchParams values", function(){
        const params = new URLSearchParams(fooBar);
        const values = params.values();
        expect(values[0]).toBe("1");
        expect(values[1]).toBe("2");
    });


    it("Test URLSearchParams entries", function(){
        const params = new URLSearchParams(fooBar);
        const entries = params.entries();
        expect(entries[0][0]).toBe("foo");
        expect(entries[0][1]).toBe("1");

        expect(entries[1][0]).toBe("bar");
        expect(entries[1][1]).toBe("2");

    });


    it("Test URLSearchParams size", function(){
        const params = new URLSearchParams(fooBar);
        expect(params.size).toBe(2);
    });

    it("Test URLSearchParams append", function(){
        const params = new URLSearchParams(fooBar);
        params.append("first", "Osei");
        expect(params.get("first")).toBe("Osei");
    });


    it("Test URLSearchParams delete", function(){
        const params = new URLSearchParams(fooBar);
        params.append("first", "Osei");
        params.delete("first");
        expect(params.get("first")).toBe(undefined);
    });


    it("Test URLSearchParams has", function(){
        const params = new URLSearchParams(fooBar);
        expect(params.has("foo")).toBe(true);
    });

    it("Test URLSearchParams changes propagates to URL parent", function(){
        const toBe = 'https://github.com/triniwiz?first=Osei';
        const url = new URL('https://github.com/triniwiz');
        const params = url.searchParams;
        console.log(params);
        params.set('first', 'Osei');
        expect(url.toString()).toBe(toBe);
    });

    it("Test URLSearchParams forEach", function(){
        const params = new URLSearchParams(fooBar);
        const results = [];
        params.forEach((value, key, searchParams) => {
            results.push({ key, value });
            expect(searchParams).toBe(params);
        });
        expect(results.length).toBe(2);
        expect(results[0].key).toBe("foo");
        expect(results[0].value).toBe("1");
        expect(results[1].key).toBe("bar");
        expect(results[1].value).toBe("2");
    });

    it("Test URLSearchParams forEach with URL", function(){
        const url = new URL('https://example.com?si=abc123&name=test');
        const results = [];
        url.searchParams.forEach((value, key) => {
            results.push({ key, value });
        });
        expect(results.length).toBe(2);
        expect(results[0].key).toBe("si");
        expect(results[0].value).toBe("abc123");
        expect(results[1].key).toBe("name");
        expect(results[1].value).toBe("test");
    });

    it("Test URLSearchParams forEach with thisArg", function(){
        const params = new URLSearchParams(fooBar);
        const context = { results: [] };
        params.forEach(function(value, key) {
            this.results.push({ key, value });
        }, context);
        expect(context.results.length).toBe(2);
        expect(context.results[0].key).toBe("foo");
        expect(context.results[0].value).toBe("1");
    });

    it("Test URLSearchParams forEach with duplicate keys", function(){
        const params = new URLSearchParams("foo=1&foo=2&bar=3");
        const results = [];
        params.forEach((value, key) => {
            results.push({ key, value });
        });
        expect(results.length).toBe(3);
        expect(results[0].key).toBe("foo");
        expect(results[0].value).toBe("1");
        expect(results[1].key).toBe("foo");
        expect(results[1].value).toBe("2");
        expect(results[2].key).toBe("bar");
        expect(results[2].value).toBe("3");
    });

});

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

});

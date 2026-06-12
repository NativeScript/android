describe("Test URLSearchParams ", function () {
    const fooBar = "foo=1&bar=2";
    it("Test URLSearchParams keys", function(){
        // keys() returns a spec iterator, not an array — consume it via spread.
        const params = new URLSearchParams(fooBar);
        const keys = [...params.keys()];
        expect(keys[0]).toBe("foo");
        expect(keys[1]).toBe("bar");
    });

    it("Test URLSearchParams values", function(){
        const params = new URLSearchParams(fooBar);
        const values = [...params.values()];
        expect(values[0]).toBe("1");
        expect(values[1]).toBe("2");
    });


    it("Test URLSearchParams entries", function(){
        const params = new URLSearchParams(fooBar);
        const entries = [...params.entries()];
        expect(entries[0][0]).toBe("foo");
        expect(entries[0][1]).toBe("1");

        expect(entries[1][0]).toBe("bar");
        expect(entries[1][1]).toBe("2");

    });

    it("Test URLSearchParams keys/values/entries return spec iterators", function(){
        const params = new URLSearchParams(fooBar);
        // A spec iterator has a next() and is itself iterable.
        expect(typeof params.entries().next).toBe("function");
        expect(typeof params.keys().next).toBe("function");
        expect(typeof params.values().next).toBe("function");
        const it = params.entries();
        const first = it.next();
        expect(first.done).toBe(false);
        expect(first.value[0]).toBe("foo");
        expect(first.value[1]).toBe("1");
    });

    it("Test URLSearchParams entries preserves duplicate keys", function(){
        // Regression: the old get_keys()+get() path returned the first value for
        // every occurrence of a repeated key.
        const params = new URLSearchParams("a=1&a=2&b=3");
        const entries = [...params.entries()];
        expect(entries.length).toBe(3);
        expect(entries[0][1]).toBe("1");
        expect(entries[1][1]).toBe("2");
        expect(entries[2][1]).toBe("3");
        expect([...params.values()].join(",")).toBe("1,2,3");
    });

    it("Test URLSearchParams default iterator aliases entries", function(){
        // The default @@iterator IS the entries method (browser identity). This binding
        // installs members per-instance (not on the prototype), so assert on an instance
        // AND assert it is actually a function — not the vacuous undefined === undefined.
        const params = new URLSearchParams(fooBar);
        expect(typeof params[Symbol.iterator]).toBe("function");
        expect(params[Symbol.iterator]).toBe(params.entries);
    });

    it("Test URLSearchParams iterator carries the spec brand", function(){
        const params = new URLSearchParams(fooBar);
        expect(Object.prototype.toString.call(params.entries())).toBe("[object URLSearchParams Iterator]");
    });

    it("Test URLSearchParams iterator is live", function(){
        // Spec iterators reflect mutations made after they are created.
        const params = new URLSearchParams("a=1&b=2");
        const it = params.entries();
        expect(it.next().value[0]).toBe("a");   // consume "a"
        params.append("c", "3");                // mutate mid-iteration
        const rest = [];
        let r;
        while (!(r = it.next()).done) {
            rest.push(r.value[0]);
        }
        expect(rest.join(",")).toBe("b,c");     // sees the appended "c"
    });

    it("Test URLSearchParams closes the source iterator on a bad pair", function(){
        // On an abrupt completion (a too-long pair) the source iterator must be
        // closed, so a generator's finally runs and resource-backed iterables free.
        let closed = false;
        function* gen() {
            try {
                yield ["a", "1", "2"];   // 3-element pair → TypeError
            } finally {
                closed = true;
            }
        }
        expect(function(){ new URLSearchParams(gen()); }).toThrow();
        expect(closed).toBe(true);
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
        // Spec: get() returns null for a missing name (url.bs:4016).
        expect(params.get("first")).toBe(null);
    });

    it("Test URLSearchParams get returns null for a missing name", function(){
        // Spec get(name): "...otherwise null" (url.bs:4016).
        const params = new URLSearchParams("a=1");
        expect(params.get("missing")).toBe(null);
    });

    it("Test URLSearchParams delete with value removes only matching pairs", function(){
        // Spec delete(name, value): when value is given, remove only tuples
        // matching BOTH name and value (url.bs:4000). The value is a USVString,
        // so a non-string (the number 1) coerces to "1".
        const params = new URLSearchParams("a=1&a=2&a=1&b=1");
        params.delete("a", 1);
        expect(params.getAll("a").join(",")).toBe("2");
        expect(params.getAll("b").join(",")).toBe("1");
        // Single-arg delete still removes every pair with that name.
        params.delete("a");
        expect(params.has("a")).toBe(false);
    });


    it("Test URLSearchParams has", function(){
        const params = new URLSearchParams(fooBar);
        expect(params.has("foo")).toBe(true);
    });

    it("Test URLSearchParams has with value matches name and value", function(){
        // Spec has(name, value): true only for a tuple matching BOTH (url.bs:4028).
        // The value is a USVString, so non-strings (number, boolean) coerce.
        const params = new URLSearchParams("a=1&a=2&flag=true");
        expect(params.has("a", "1")).toBe(true);
        expect(params.has("a", 2)).toBe(true);          // number coerces to "2"
        expect(params.has("a", "3")).toBe(false);
        expect(params.has("flag", true)).toBe(true);    // boolean coerces to "true"
        expect(params.has("missing", "1")).toBe(false);
        // Single-arg has still matches by name only.
        expect(params.has("a")).toBe(true);
    });

    it("Test URLSearchParams has/delete throw when the value cannot be coerced", function(){
        // The value argument is a USVString; a Symbol (or a throwing toString)
        // cannot convert, so the call must throw rather than silently matching ""
        // (WebIDL USVString conversion, url.bs:4000 / 4028).
        const params = new URLSearchParams("a=1");
        expect(function(){ params.has("a", Symbol("x")); }).toThrow();
        expect(function(){ params.delete("a", Symbol("x")); }).toThrow();
    });

    it("Test URLSearchParams has/delete treat an explicit undefined value as omitted", function(){
        // Per WPT (urlsearchparams-has / -delete "respects undefined as second
        // arg"), an explicit `undefined` second argument is treated as omitted
        // (name-only), NOT as the string "undefined".
        const params = new URLSearchParams("a=b&a=d&c&e&");
        expect(params.has("a", "b")).toBe(true);
        expect(params.has("a", "c")).toBe(false);
        expect(params.has("a", undefined)).toBe(true);   // undefined -> name-only

        const del = new URLSearchParams();
        del.append("a", "b");
        del.append("a", "c");
        del.append("b", "c");
        del.append("b", "d");
        del.delete("b", "c");
        del.delete("a", undefined);                      // undefined -> delete by name
        expect(del.toString()).toBe("b=d");
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

    it("Test URLSearchParams from record object", function(){
        const params = new URLSearchParams({ foo: "1", bar: "2" });
        expect(params.get("foo")).toBe("1");
        expect(params.get("bar")).toBe("2");
        expect(params.size).toBe(2);
        // A plain object must expand to its entries, not collapse into a
        // single "[object Object]" key.
        expect(params.has("[object Object]")).toBe(false);
    });

    it("Test URLSearchParams from record serializes every pair in toString", function(){
        const params = new URLSearchParams({ one: "1", two: "2" });
        expect(params.toString()).toBe("one=1&two=2");
    });

    it("Test URLSearchParams from record coerces values to strings", function(){
        const params = new URLSearchParams({ a: 1, b: true });
        expect(params.get("a")).toBe("1");
        expect(params.get("b")).toBe("true");
    });

    it("Test URLSearchParams from record encodes special characters", function(){
        const params = new URLSearchParams({ q: "a b&c" });
        expect(params.get("q")).toBe("a b&c");
        expect(params.toString()).toBe("q=a+b%26c");
    });

    it("Test URLSearchParams from array of pairs", function(){
        const params = new URLSearchParams([["foo", "1"], ["bar", "2"], ["foo", "3"]]);
        expect(params.get("foo")).toBe("1");
        expect(params.getAll("foo").length).toBe(2);
        expect(params.get("bar")).toBe("2");
        expect(params.size).toBe(3);
    });

    it("Test URLSearchParams empty record and no-arg produce empty query", function(){
        expect(new URLSearchParams().toString()).toBe("");
        expect(new URLSearchParams({}).toString()).toBe("");
    });

    it("Test URLSearchParams from record throws when a value cannot be coerced to a string", function(){
        // Per spec the record/sequence init coerces every value to a USVString;
        // a value that cannot convert (a Symbol here) must throw rather than
        // silently dropping or emptying the entry.
        expect(function(){ new URLSearchParams({ bad: Symbol("x") }); }).toThrow();
    });

    // --- Iterable (sequence) init: any iterable of pairs, not only arrays. ---

    it("Test URLSearchParams from a Map", function(){
        const params = new URLSearchParams(new Map([["a", "1"], ["b", "2"]]));
        expect(params.toString()).toBe("a=1&b=2");
    });

    it("Test URLSearchParams from a Set of pairs", function(){
        const params = new URLSearchParams(new Set([["x", "1"], ["y", "2"]]));
        expect(params.toString()).toBe("x=1&y=2");
    });

    it("Test URLSearchParams copy-constructs from another URLSearchParams", function(){
        // A URLSearchParams is iterable, so per spec it resolves to the sequence
        // (copy) form — including duplicate keys, which proves the @@iterator walks
        // entries rather than collapsing them.
        const source = new URLSearchParams("a=1&a=2&b=3");
        const copy = new URLSearchParams(source);
        expect(copy.toString()).toBe("a=1&a=2&b=3");
        expect(copy.getAll("a").length).toBe(2);
    });

    it("Test URLSearchParams from a generator of pairs", function(){
        function* pairs() {
            yield ["a", "1"];
            yield ["b", "2"];
        }
        const params = new URLSearchParams(pairs());
        expect(params.toString()).toBe("a=1&b=2");
    });

    it("Test URLSearchParams from sequence with non-array inner pairs", function(){
        // Each pair need only be a 2-element iterable, not specifically an array.
        // A Set is iterable but not an Array, so it exercises the inner iterator path.
        const params = new URLSearchParams([new Set(["k", "v"])]);
        expect(params.get("k")).toBe("v");
    });

    it("Test URLSearchParams sequence init throws on a too-long pair", function(){
        expect(function(){ new URLSearchParams([["a", "1", "2"]]); }).toThrow();
    });

    it("Test URLSearchParams sequence init throws on a too-short pair", function(){
        expect(function(){ new URLSearchParams([["a"]]); }).toThrow();
    });

    it("Test URLSearchParams sequence init throws on a non-iterable element", function(){
        expect(function(){ new URLSearchParams([null]); }).toThrow();
        expect(function(){ new URLSearchParams([1]); }).toThrow();
    });

    it("Test URLSearchParams sequence init throws on a primitive string element", function(){
        // WebIDL converts each element to sequence<USVString>, whose first step throws
        // when the element is not an Object. A 2-code-point string must NOT be accepted
        // as the pair ("a","b").
        expect(function(){ new URLSearchParams(["ab"]); }).toThrow();
    });

    it("Test URLSearchParams sequence init accepts a String-object element", function(){
        // A String *object* IS an Object and is iterable, so it is a valid 2-char pair.
        const params = new URLSearchParams([new String("ab")]);
        expect(params.get("a")).toBe("b");
    });

    it("Test URLSearchParams throws when @@iterator is present but not callable", function(){
        // Per WebIDL GetMethod, a non-callable @@iterator is a TypeError, not a
        // silent fall-back to the record form.
        expect(function(){ new URLSearchParams({ [Symbol.iterator]: 5 }); }).toThrow();
    });

    // --- The type itself is iterable<USVString, USVString>. ---

    it("Test URLSearchParams is spread-iterable via Symbol.iterator", function(){
        const params = new URLSearchParams("a=1&b=2");
        const entries = [...params];
        expect(entries.length).toBe(2);
        expect(entries[0][0]).toBe("a");
        expect(entries[0][1]).toBe("1");
        expect(entries[1][0]).toBe("b");
        expect(entries[1][1]).toBe("2");
    });

    it("Test URLSearchParams works in a for..of loop", function(){
        const params = new URLSearchParams("a=1&a=2");
        const seen = [];
        for (const [key, value] of params) {
            seen.push(key + "=" + value);
        }
        expect(seen.length).toBe(2);
        expect(seen[0]).toBe("a=1");
        expect(seen[1]).toBe("a=2");
    });

    // --- Primitive init: coerced to USVString, then parsed. ---

    it("Test URLSearchParams from a number", function(){
        expect(new URLSearchParams(123).toString()).toBe("123=");
    });

    it("Test URLSearchParams from a boolean", function(){
        expect(new URLSearchParams(true).toString()).toBe("true=");
    });

    it("Test URLSearchParams from a bigint", function(){
        expect(new URLSearchParams(10n).toString()).toBe("10=");
    });

    it("Test URLSearchParams strips a single leading question mark", function(){
        expect(new URLSearchParams("?a=1").get("a")).toBe("1");
    });

    it("Test URLSearchParams throws when init is a Symbol", function(){
        expect(function(){ new URLSearchParams(Symbol("x")); }).toThrow();
    });

    it("Test URLSearchParams from null or undefined is empty", function(){
        expect(new URLSearchParams(null).toString()).toBe("");
        expect(new URLSearchParams(undefined).toString()).toBe("");
    });

});

describe("Test URLSearchParamsImpl ", function () {


it("Test URLSearchParamsImpl keys", function(){
const params = new URLSearchParamsImpl("foo=1&bar=2");
const keys = params.keys();
expect(keys[0]).toBe("foo");
expect(keys[1]).toBe("bar");
});

it("Test URLSearchParamsImpl values", function(){
const params = new URLSearchParamsImpl("foo=1&bar=2");
const values = params.values();
expect(values[0]).toBe("1");
expect(values[1]).toBe("2");
});


it("Test URLSearchParamsImpl entries", function(){
const params = new URLSearchParamsImpl("foo=1&bar=2");
const entries = params.entries();
expect(entries[0][0]).toBe("foo");
expect(entries[0][1]).toBe("1");

expect(entries[1][0]).toBe("bar");
expect(entries[1][1]).toBe("2");

});


it("Test URLSearchParamsImpl size", function(){
const params = new URLSearchParamsImpl("foo=1&bar=2");
expect(params.size).toBe(2);
});

it("Test URLSearchParamsImpl append", function(){
const params = new URLSearchParamsImpl("foo=1&bar=2");
params.append("first", "Osei");
expect(params.get("first")).toBe("Osei");
});


it("Test URLSearchParamsImpl delete", function(){
const params = new URLSearchParamsImpl("foo=1&bar=2");
params.append("first", "Osei");
params.delete("first");
expect(params.get("first")).toBe(undefined);
});


it("Test URLSearchParamsImpl has", function(){
const params = new URLSearchParamsImpl("foo=1&bar=2");
expect(params.has("foo")).toBe(true);
});

});
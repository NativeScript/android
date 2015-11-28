describe("Tests JSON", function () {
	
	it("should load valid JSON file", function () {
		var data = require("./data/data1.json");
		expect(data.myValue).toBe(123);
	});
	
	it("shouldn't load invalid JSON file 1", function () {
		var loaded = true;
		try {
			var data = require("./data/badJSON1.json");
		} catch (e) {
			loaded = false;
		}
		expect(loaded).toBe(false);
	});
	
	it("shouldn't load invalid JSON file 2", function () {
		var loaded = true;
		try {
			var data = require("./data/badJSON2.json");
		} catch (e) {
			loaded = false;
		}
		expect(loaded).toBe(false);
	});

	it("shouldn't load non-existent JSON file", function () {
		var loaded = true;
		try {
			var data = require("./data/missing-file.json");
		} catch (e) {
			loaded = false;
		}
		expect(loaded).toBe(false);
	});
});
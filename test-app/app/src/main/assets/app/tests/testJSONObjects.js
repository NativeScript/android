describe("Test JSONObject conversions", () => {
    it("org.json.JSONObject.from method is defined", () => {
        expect(typeof org.json.JSONObject.from).toBe("function");
    });

    it("JSONObject.from takes at least one argument", () => {
        expect(() => org.json.JSONObject.from()).toThrowError();
    });

    it("JSONObject.from with boolean", () => {
        let param = true;
        let actual = org.json.JSONObject.from(param);
        expect(actual).toBe(param);
    });

    it("JSONObject.from with string", () => {
        let param = "some param";
        let actual = org.json.JSONObject.from(param);
        expect(actual).toBe(param);
    });

    it("JSONObject.from with number", () => {
        let param = 123;
        let actual = org.json.JSONObject.from(param);
        expect(actual).toBe(param);
    });

    it("JSONObject.from with date must serialize it to JSON using ISO8601", () => {
        let timestamp = 1570696661136;
        let param = new Date(timestamp);
        let actual = org.json.JSONObject.from(param);
        expect(actual).toBe("2019-10-10T08:37:41.136Z");
    });

    it("JSONObject.from with object", () => {
        let param = {
            prop1: "prop1 value",
            prop2: 123,
            prop3: {
                prop4: "prop 4 value"
            }
        };
        let actual = org.json.JSONObject.from(param);
        expect(actual instanceof org.json.JSONObject).toBe(true);
        let actualStr = com.tns.tests.JSONObjectMethods.testWithObject(actual);
        expect(actualStr).toBe(`{"prop1":"prop1 value","prop2":123,"prop3":{"prop4":"prop 4 value"}}`);
    });

    it("JSONObject.from with array", () => {
        let param = [{
            prop1: "item 1, prop1 value",
            prop2: 123
        }, {
            prop1: "item 2, prop1 value",
            prop2: 456
        }];
        let actual = org.json.JSONObject.from(param);
        expect(actual instanceof org.json.JSONArray).toBe(true);
        let actualStr = com.tns.tests.JSONObjectMethods.testWithArray(actual);
        expect(actualStr).toBe(`[{"prop1":"item 1, prop1 value","prop2":123},{"prop1":"item 2, prop1 value","prop2":456}]`);
    });
});
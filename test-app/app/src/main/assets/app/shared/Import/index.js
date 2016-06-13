describe("TNS import", function () {
    afterEach(TNSClearOutput);

    it("JSON files", function () {
        require("./ImportJSON");
        expect(TNSGetOutput()).toBe("testValue");
    });

    it("CommonJS", function () {
        require("./ImportCommonJS");
        expect(TNSGetOutput()).toBe("42");
    });
});

describe("Test JSONObject conversions", () => {
    it("console.log with number param should not crash", () => {
        console.log(123);
    });

    it("console.log with string param should not crash", () => {
        console.log("123");
    });

    it("console.log with object param should not crash", () => {
        console.log({ num: 123 });
    });

    it("console.log with function param should not crash", () => {
        console.log(function() {});
    });

    it("console.log with arrow function param should not crash", () => {
        console.log(() => {});
    });

    it("console.log with primitive array param should not crash", () => {
        console.log([1, 2, 3]);
    });

    it("console.log with object array param should not crash", () => {
        console.log([{
            num: 123
        }]);
    });
});
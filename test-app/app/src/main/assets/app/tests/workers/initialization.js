describe("Test Worker object/Thread initialization", function() {
    it("Should throw exception when no parameter is passed", function() {
        var exceptionThrown = false;

        try {
            new Worker();
        } catch(e) {
            exceptionThrown = true;
        }

        expect(exceptionThrown).toBe(true);
    });

    it("Should throw exception when script does not exist", function() {
        var exceptionThrown = false;

        try {
            new Worker("./idonot-exist.js");
        } catch(e) {
            exceptionThrown = true;
        }

        expect(exceptionThrown).toBe(true);
    });

    it("Should throw exception when parameter is not a proper string", function() {
        var exceptionThrown = false;

        try {
            new Worker({ filename: "./myFile.js" });
        } catch(e) {
            exceptionThrown = true;
        }

        expect(exceptionThrown).toBe(true);

        exceptionThrown = false;

        try {
            new Worker((function() {
                function a() {

                }
            }()));
        } catch(e) {
            exceptionThrown = true;
        }

        expect(exceptionThrown).toBe(true);
    });

    it("Should throw exception when not invoked as constructor", function() {
        var exceptionThrown = false;

        try {
            Worker();
        } catch(e) {
            exceptionThrown = true;
        }

        expect(exceptionThrown).toBe(true);

        exceptionThrown = false;

        try {
            Worker("./myFile.js");
        } catch(e) {
            exceptionThrown = true;
        }

        expect(exceptionThrown).toBe(true);
    });
});
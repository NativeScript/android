describe("Tests Java interfaces are implemented correctly", function () {

	it("should implement all methods when an interface extends other interfaces", function () {
        var callF = false;
        var callG = false;

        var i3 = new com.tns.tests.ImplementInterfaceTest.I3({
            f: function() { callF = true; },
            g: function() { callG = true; }
        })

        com.tns.tests.ImplementInterfaceTest.triggerF(i3);
        com.tns.tests.ImplementInterfaceTest.triggerG(i3);

		expect(callF).toBe(true);
		expect(callG).toBe(true);
	});

	it("should call overridden interface method", function () {
        var callClose = false;

        var c = new java.io.Closeable({
            close: function() { callClose = true; }
        })

        com.tns.tests.ImplementInterfaceTest.triggerClose(c);

		expect(callClose).toBe(true);
	});
});
describe("Tests native modules)", function () {

	it("should load native module", function () {
		var x = 12;
		var y = 34;

		var arch;
		var sysArch = java.lang.System.getProperty("os.arch"); // ro.product.cpu.abi
		var lcArch = sysArch.toLowerCase();
		if (lcArch.indexOf("arm") > -1) {
			arch = "arm";
		} else if (lcArch.indexOf("aarch64") > -1) {
		    arch = (com.tns.Runtime.getPointerSize() == 4) ? "arm" : "arm64";
		} else if (lcArch.indexOf("i686") > -1) {
			arch = "x86";
		} else {
			throw new Error("Unsupported architecture=" + sysArch);
		}

		var calc = require("../modules/libCalc-" + arch + ".so");

		var sum = calc.add(x, y);

		expect(sum).toBe(46);
	});
});
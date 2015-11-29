describe("Tests native modules)", function () {
	
	it("should load native module", function () {
		var x = 12;
		var y = 34;
		
		var arch;
		var sysArch = java.lang.System.getProperty("os.arch");
		var lcArch = sysArch.toLowerCase();
		if (lcArch.indexOf("arm") > -1) {
			arch = "arm";
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
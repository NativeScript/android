describe("Tests instancing java arrays", function () {
	
	it("should throw an exception when call Array.create with wrong number of arguments", function () {
		var exceptionCaught = false;
		try {
			var arr = Array.create();
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);
		
		exceptionCaught = false;
		try {
			var arr = Array.create(1);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);

		exceptionCaught = false;
		try {
			var arr = Array.create("int", 2, 3);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);

		exceptionCaught = false;
		try {
			var arr = Array.create(java.lang.Object, 2, 3);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);
	});
	
	it("should throw an exception when call Array.create with wrong type of arguments", function () {
		var exceptionCaught = false;
		try {
			var arr = Array.create({}, 123);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);
		
		exceptionCaught = false;
		try {
			var arr = Array.create("", -1);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);
		
		exceptionCaught = false;
		try {
			var arr = Array.create("int", 1.5);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);

		exceptionCaught = false;
		try {
			var arr = Array.create(function(){}, -1);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);

		exceptionCaught = false;
		try {
			var arr = Array.create(function(){}, 1.5);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);
	});

	it("should instantiate primitive char array", function () {
		var len = 123;
		var arr = Array.create("char", len);
		expect(arr.length).toBe(len);
		expect(arr.getClass().getName()).toBe("[C");
	});
	
	it("should instantiate primitive boolean array", function () {
		var len = 123;
		var arr = Array.create("boolean", len);
		expect(arr.length).toBe(len);
		expect(arr.getClass().getName()).toBe("[Z");
	});


	it("should instantiate primitive byte array", function () {
		var len = 123;
		var arr = Array.create("byte", len);
		expect(arr.length).toBe(len);
		expect(arr.getClass().getName()).toBe("[B");
	});

	it("should instantiate primitive short array", function () {
		var len = 123;
		var arr = Array.create("short", len);
		expect(arr.length).toBe(len);
		expect(arr.getClass().getName()).toBe("[S");
	});

	it("should instantiate primitive int array", function () {
		var len = 123;
		var arr = Array.create("int", len);
		expect(arr.length).toBe(len);
		expect(arr.getClass().getName()).toBe("[I");
	});

	it("should instantiate primitive long array", function () {
		var len = 123;
		var arr = Array.create("long", len);
		expect(arr.length).toBe(len);
		expect(arr.getClass().getName()).toBe("[J");
	});

	it("should instantiate primitive float array", function () {
		var len = 123;
		var arr = Array.create("float", len);
		expect(arr.length).toBe(len);
		expect(arr.getClass().getName()).toBe("[F");
	});

	it("should instantiate primitive double array", function () {
		var len = 123;
		var arr = Array.create("double", len);
		expect(arr.length).toBe(len);
		expect(arr.getClass().getName()).toBe("[D");
	});
	
	it("should instantiate Object array", function () {
		var len = 123;
		var arr = Array.create(java.lang.Object, len);
		expect(arr.length).toBe(len);
		expect(arr.getClass().getName()).toBe("[Ljava.lang.Object;");
	});

	it("should instantiate jagged java.lang.String array", function () {
		var count = 15;
		var typename = "Ljava.lang.String;";
		var arrayClassName = Array(count+1).join("[") + typename;
		var len = 123;
		var arr = Array.create(arrayClassName, len);
		expect(arr.length).toBe(len);
		var expectedArrayClassName = Array(count+2).join("[") + typename;
		expect(arr.getClass().getName()).toBe(expectedArrayClassName);
	});
});
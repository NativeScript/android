describe("Tests final fields set", function () {
		
	it("When trying to set a final field throw exception", function () {
		
		var exceptionCaught = false;
		try
		{
			com.tns.tests.Button1.STATIC_IMAGE_ID = "NEW STATIC IMAGE ID VALUE";	
		}
		catch (e)
		{
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
	});
	
	it("When setting a field with null it should return null object", function () {
		
		var dc = new com.tns.tests.DummyClass();
		
		dc.nameField = null;
		var s = dc.nameField;
		
		expect(s).toBe(null);
	});
});
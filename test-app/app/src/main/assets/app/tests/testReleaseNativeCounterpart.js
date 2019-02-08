describe("Test native counterpart release", function () {

	var myCustomEquality = function(first, second) {
		return first == second;
	};

	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});

	it("Calling a method on a released object should throw exception", function () {

		var errorMessage = "";

		try{
		    var object1 = new java.lang.Object();

		    global.__releaseNativeCounterpart(object1);

		    object1.toString();
		} catch(e){
		    errorMessage = e.message;
		}

		expect(errorMessage).toBe("Failed calling toString on a java/lang/Object instance. The JavaScript instance no longer has available Java instance counterpart.");
	});

	it("Calling the indexer operator on a released native array should throw an exception", function(){
	    var errorMessage = "";

	    try{
	        var arr = new java.lang.reflect.Array.newInstance(java.lang.Object.class, 10);
            global.__releaseNativeCounterpart(arr);
            arr[1];
	    } catch(e){
	        errorMessage = e.message;
	    }

	    expect(errorMessage).toBe("Failed calling indexer operator on native array. The JavaScript instance no longer has available Java instance counterpart.");

	});

    it("Calling release on a non native object should throw exception", function () {

        var errorMessage = "";

        try{
            var object2 = {prop: "test"};
            global.__releaseNativeCounterpart(object2);
        } catch(e){
            errorMessage = e.message;
        }

        expect(errorMessage).toBe("Trying to release a non native object!");
    });


    it("Calling release on a non native primitive type should throw exception", function () {

        var errorMessage = "";

        try{
            global.__releaseNativeCounterpart(42);
        } catch(e){
            errorMessage = e.message;
        }

        expect(errorMessage).toBe("Argument is not an object!");
    });

    it("Calling the __releaseNativeCounterpart function with 0 arguments should throw exception", function(){
        var errorMessage = "";

        try{
            global.__releaseNativeCounterpart();
        } catch(e){
            errorMessage = e.message;
        }

        expect(errorMessage).toBe("Unexpected arguments count!");
    });

    it("Calling the __releaseNativeCounterpart function with more than 1 arguments should throw exception", function(){
        var errorMessage = "";

        try{
            global.__releaseNativeCounterpart({},{});
        } catch(e){
            errorMessage = e.message;
        }

        expect(errorMessage).toBe("Unexpected arguments count!");
    });


});
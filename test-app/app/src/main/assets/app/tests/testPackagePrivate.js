describe('Test Package Private', function () {
	it('TestPackagePrivateAccessForAppCompiledFiles', function () {
		var theFactory = new com.tns.tests.MyFactoryPatternPrivate();
		var theProblem = theFactory.getIt();
		var didThrow = false;
		try {
			theProblem.run();
		} catch (e) {
			// throws if the runtime doesn't have access to instance
			didThrow = true;
		}

		expect(didThrow).toEqual(false);
	});

	it('TestPackagePrivateAccessForAppCompiledFilesPrivate', function () {
    		var theFactory = new com.tns.tests.MyFactoryPatternPrivate();
    		var theProblem = theFactory.getIt();
    		var didThrow = false;
    		try {
    			theProblem.privateThing();
    		} catch (e) {
    			// throws if the runtime doesn't have access to instance
    			didThrow = true;
    		}

    		expect(didThrow).toEqual(true);
    	});

    it('TestPackagePrivateAccessForAppCompiledFilesPackagePrivate', function () {
        		var theFactory = new com.tns.tests.MyFactoryPatternPrivate();
        		var theProblem = theFactory.getIt();
        		var didThrow = false;
        		try {
        			theProblem.packagePrivateThing();
        		} catch (e) {
        			// throws if the runtime doesn't have access to instance
        			didThrow = true;
        		}

        		expect(didThrow).toEqual(false);
        	});
});

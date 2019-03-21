describe("Test extending just classes in JS", function () {
    var packageName = com.tns.tests.inheritance.classes;

	it("Extend non generic abstract Java class without implementing any interfaces in JS: Implement some of its abstract methods ", function () {
	    var expectedMessage = "ErrorMessage";
        var NonGenericSimpleClassBase = packageName.NonGenericAbstractClassBase.extend({
            workGeneric: function(arg){
                throw new Error(arg);
            }
        });

        var nonGenericSimpleClassBase = new NonGenericSimpleClassBase();
        try{
            nonGenericSimpleClassBase.workGeneric(expectedMessage);
        } catch (e){
            expect(e.message).toBe(expectedMessage);
        }

	});

    it("Extend non generic abstract Java class without implementing any interfaces in JS: Implement all supported abstract methods ", function () {
            var nonGenericSimpleClassBase = packageName.NonGenericAbstractClassBase.extend({

                                                                                           });
    });

    it("Extend non generic abstract Java class without implementing any interfaces in JS: Don’t implement any abstract methods ", function () {
              var nonGenericSimpleClassBase = packageName.NonGenericAbstractClassBase.extend({

                                                                                             });
    });


    it("Extend non generic Java class without implementing any interfaces in JS: Override some of its method ", function () {
           var nonGenericSimpleClassBase = packageName.NonGenericSimpleClassBase.extend({

                                                                                        });
    });

    it("Extend non generic Java class without implementing any interfaces in JS: Override all supported methods ", function () {
               var nonGenericSimpleClassBase = packageName.NonGenericSimpleClassBase.extend({

                                                                                            });
    });

    it("Extend non generic Java class without implementing any interfaces in JS: Don’t override any methods ", function () {
                 var nonGenericSimpleClassBase = packageName.NonGenericSimpleClassBase.extend({

                                                                                              });
    });


});
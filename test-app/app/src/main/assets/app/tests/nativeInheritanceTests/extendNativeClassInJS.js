describe("Test extending just classes in JS", function () {

//	it("Extend non generic abstract Java class without implementing any interfaces in JS: Implement some of its abstract methods ", function () {
//	    var expectedMessage = "ErrorMessage";
//        var NonGenericAbstractClassBase = com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.extend({
//            workAbstract: function(arg){
//                throw new Error(arg);
//            }
//        });
//
//        var nonGenericAbstractClassBase = new NonGenericAbstractClassBase();
//        try{
//            nonGenericAbstractClassBase.workAbstract(expectedMessage);
//        } catch (e){
//            expect(e.message).toBe(expectedMessage);
//        }
//
//	});

//    it("Extend non generic abstract Java class without implementing any interfaces in JS: Implement all supported abstract methods ", function () {
//            var expectedMessage = "ErrorMessage";
//            var NonGenericAbstractClassBase = com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.extend({
//                workAbstract: function() {
//                    console.log("korrrr----->dddd")
//                    throw new Error("ErrorMessage");
//                },
//                workAbstract: function(arg) {
//                    console.log("korrrr111----->" + arg)
//                    throw new Error(arg);
//                },
//                workAbstract: function(arg, numberValue) {
//                    console.log("ddddddd----->" + arg)
//                    var ErrorMessage = arg + numberValue.toString()
//                    throw new Error(ErrorMessage);
//                }
//            });
//
//            var nonGenericAbstractClassBase = new NonGenericAbstractClassBase();
//            try {
//                nonGenericAbstractClassBase.workAbstract();
//            } catch (e) {
//
//                expect(e.message).toBe(expectedMessage);
//            }
//
//            try {
//
//                nonGenericAbstractClassBase.workAbstract(expectedMessage);
//            } catch (e) {
//
//                expect(e.message).toBe(expectedMessage);
//            }
//            try {
//                nonGenericAbstractClassBase.workAbstract(expectedMessage, 4);
//            } catch (e) {
//                var expectedValue = expectedMessage + "4";
//                expect(e.message).toBe(expectedValue);
//            }
//    });

//    it("Extend non generic abstract Java class without implementing any interfaces in JS: Don’t implement any abstract methods ", function () {
//              var nonGenericSimpleClassBase = com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.extend({
//
//                                                                                             });
//    });
//
//
//    it("Extend non generic Java class without implementing any interfaces in JS: Override some of its method ", function () {
//           var nonGenericSimpleClassBase = com.tns.sgbtestclasses.inheritance.classes.NonGenericSimpleClassBase.extend({
//
//                                                                                        });
//    });
//
//    it("Extend non generic Java class without implementing any interfaces in JS: Override all supported methods ", function () {
//               var nonGenericSimpleClassBase = com.tns.sgbtestclasses.inheritance.classes.NonGenericSimpleClassBase.extend({
//
//                                                                                            });
//    });
//
//    it("Extend non generic Java class without implementing any interfaces in JS: Don’t override any methods ", function () {
//                 var nonGenericSimpleClassBase = com.tns.sgbtestclasses.inheritance.classes.NonGenericSimpleClassBase.extend({
//
//                                                                                              });
//    });


});
describe("Test extending just classes in JS", function() {

    it("Extend non generic abstract Java class without implementing any interfaces in JS: Implement some of its abstract methods ", function() {
        var expectedMessage = "ErrorMessage";
        var noMethodExpectedMessage = "No Method!"
        var NonGenericAbstractClassBase = com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.extend({
            workAbstract: function() {
                if (arguments.length == 2) {
                    var ErrorMessage = noMethodExpectedMessage
                    throw new Error(ErrorMessage);
                }
                if (arguments.length == 1) {
                    throw new Error(arguments[0]);
                }
                if (arguments.length == 0) {
                    var ErrorMessage = noMethodExpectedMessage
                    throw new Error(ErrorMessage);
                }
            }
        });

        var nonGenericAbstractClassBase = new NonGenericAbstractClassBase();
        var nonGenericAbstractClassBaseHelper = new com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBaseHelper()
        try {
            nonGenericAbstractClassBaseHelper.work(nonGenericAbstractClassBase, expectedMessage);
        } catch (e) {
            expect(e.message).toBe(expectedMessage);
        }
        try {
            nonGenericAbstractClassBaseHelper.work(nonGenericAbstractClassBase, expectedMessage, 4);
        } catch (e) {
            expect(e.message).toBe(noMethodExpectedMessage);
        }
        try {
            nonGenericAbstractClassBaseHelper.work(nonGenericAbstractClassBase);
        } catch (e) {
            expect(e.message).toBe(noMethodExpectedMessage);
        }
        try {
            nonGenericAbstractClassBaseHelper.getStringAbstract(nonGenericAbstractClassBase);
        } catch (e) {
            expect(e.message).toContain("Error: java.lang.AbstractMethodError: abstract method \"java.lang.String com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.getStringAbstract()\"");
        }
        try {
            nonGenericAbstractClassBaseHelper.getStringAbstract(nonGenericAbstractClassBase, expectedMessage);
        } catch (e) {
            expect(e.message).toContain("Error: java.lang.AbstractMethodError: abstract method \"java.lang.String com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.getStringAbstract(java.lang.String)\"");
        }
        try {
            nonGenericAbstractClassBaseHelper.getStringAbstract(nonGenericAbstractClassBase, expectedMessage, 4);
        } catch (e) {
            expect(e.message).toContain("Error: java.lang.AbstractMethodError: abstract method \"java.lang.String com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.getStringAbstract(java.lang.String, int)\"");
        }

    });

    it("Extend non generic abstract Java class without implementing any interfaces in JS: Implement all supported abstract methods ", function() {
        var expectedMessage = "ErrorMessage";
        var NonGenericAbstractClassBase = com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.extend({
            workAbstract: function() {
                if (arguments.length == 2) {
                    var ErrorMessage = arguments[0] + arguments[1].toString()
                    throw new Error(ErrorMessage);
                }
                if (arguments.length == 1) {
                    throw new Error(arguments[0]);
                }
                if (arguments.length == 0) {
                    throw new Error(expectedMessage);
                }
            },
            getStringAbstract: function() {
                if (arguments.length == 2) {
                    var ErrorMessage = arguments[0] + arguments[1].toString()
                    throw new Error(ErrorMessage);
                }
                if (arguments.length == 1) {
                    throw new Error(arguments[0]);
                }
                if (arguments.length == 0) {
                    throw new Error(expectedMessage);
                }
            }
        });

        var nonGenericAbstractClassBase = new NonGenericAbstractClassBase();
        var nonGenericAbstractClassBaseHelper = new com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBaseHelper()
        try {
            nonGenericAbstractClassBaseHelper.work(nonGenericAbstractClassBase);
        } catch (e) {

            expect(e.message).toBe(expectedMessage);
        }

        try {

            nonGenericAbstractClassBaseHelper.work(nonGenericAbstractClassBase, expectedMessage);
        } catch (e) {

            expect(e.message).toBe(expectedMessage);
        }
        try {
            nonGenericAbstractClassBaseHelper.work(nonGenericAbstractClassBase, expectedMessage, 4);
        } catch (e) {
            var expectedValue = expectedMessage + "4";
            expect(e.message).toBe(expectedValue);
        }

        try {
            nonGenericAbstractClassBaseHelper.getStringAbstract(nonGenericAbstractClassBase);
        } catch (e) {

            expect(e.message).toBe(expectedMessage);
        }

        try {

            nonGenericAbstractClassBaseHelper.getStringAbstract(nonGenericAbstractClassBase, expectedMessage);
        } catch (e) {

            expect(e.message).toBe(expectedMessage);
        }
        try {
            nonGenericAbstractClassBaseHelper.getStringAbstract(nonGenericAbstractClassBase, expectedMessage, 4);
        } catch (e) {
            var expectedValue = expectedMessage + "4";
            expect(e.message).toBe(expectedValue);
        }
    });

    it("Extend non generic abstract Java class without implementing any interfaces in JS: Don’t implement any abstract methods ", function() {
        var expectedMessage = "ErrorMessage";
        var NonGenericAbstractClassBase = com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.extend({});

        var nonGenericAbstractClassBase = new NonGenericAbstractClassBase();
        var nonGenericAbstractClassBaseHelper = new com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBaseHelper()
        try {
            nonGenericAbstractClassBaseHelper.work(nonGenericAbstractClassBase);
        } catch (e) {

            expect(e.message).toContain("Error: java.lang.AbstractMethodError: abstract method \"void com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.workAbstract()\"")
        }
        try {

            nonGenericAbstractClassBaseHelper.work(nonGenericAbstractClassBase, expectedMessage);
        } catch (e) {

            expect(e.message).toContain("Error: java.lang.AbstractMethodError: abstract method \"void com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.workAbstract(java.lang.String)\"");
        }
        try {
            nonGenericAbstractClassBaseHelper.work(nonGenericAbstractClassBase, expectedMessage, 4);
        } catch (e) {
            var expectedValue = expectedMessage + "4";
            expect(e.message).toContain("Error: java.lang.AbstractMethodError: abstract method \"void com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.workAbstract(java.lang.String, int)\"");
        }

        try {
            nonGenericAbstractClassBaseHelper.getStringAbstract(nonGenericAbstractClassBase);
        } catch (e) {
            expect(e.message).toContain("Error: java.lang.AbstractMethodError: abstract method \"java.lang.String com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.getStringAbstract()\"");
        }
        try {
            nonGenericAbstractClassBaseHelper.getStringAbstract(nonGenericAbstractClassBase, expectedMessage);
        } catch (e) {
            expect(e.message).toContain("Error: java.lang.AbstractMethodError: abstract method \"java.lang.String com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.getStringAbstract(java.lang.String)\"");
        }
        try {
            nonGenericAbstractClassBaseHelper.getStringAbstract(nonGenericAbstractClassBase, expectedMessage, 4);
        } catch (e) {
            expect(e.message).toContain("Error: java.lang.AbstractMethodError: abstract method \"java.lang.String com.tns.sgbtestclasses.inheritance.classes.NonGenericAbstractClassBase.getStringAbstract(java.lang.String, int)\"");
        }
    });


    it("Extend non generic Java class without implementing any interfaces in JS: Override some of its method ", function() {
        var nonGenericSimpleClassBase = com.tns.sgbtestclasses.inheritance.classes.NonGenericSimpleClassBase.extend({

        });
    });

    it("Extend non generic Java class without implementing any interfaces in JS: Override all supported methods ", function() {
        var nonGenericSimpleClassBase = com.tns.sgbtestclasses.inheritance.classes.NonGenericSimpleClassBase.extend({

        });
    });

    it("Extend non generic Java class without implementing any interfaces in JS: Don’t override any methods ", function() {
        var nonGenericSimpleClassBase = com.tns.sgbtestclasses.inheritance.classes.NonGenericSimpleClassBase.extend({

        });
    });


});
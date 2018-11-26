describe("Tests Java 8 static methods support", function () {

    it("Test static method in interface", function () {
        var producer = com.tns.tests.interfaces.staticmethods.StaticProducer;
        expect(producer.staticProduce()).toBe("static produce");
    })


})
describe("Tests Java 8 default methods support", function () {

    it("Test default method overrides in extending classes", function () {
        var firstGen = new com.tns.tests.interfaces.defaultmethods.impl.FirstGeneration();
        expect(firstGen.grow()).toBe("first generation grow");

        var secondGeneration = new com.tns.tests.interfaces.defaultmethods.impl.SecondGeneration();
        expect(secondGeneration.grow()).toBe("second generation grow");
    })

    it("Test default method overrides in interfaces chain", function () {
        var producer = new com.tns.tests.interfaces.defaultmethods.impl.CarProducerImpl();
        expect(producer.produce()).toBe("default produce in CarProducer");
    })


})
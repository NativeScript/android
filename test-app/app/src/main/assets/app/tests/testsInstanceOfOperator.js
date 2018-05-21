describe("instanceof operator tests", function () {
	it("should return false for numeric types", function () {
		const actual = 5 instanceof com.tns.tests.MyInterface1;
		expect(actual).toBe(false);
	});

	it("should return false for plain javascript classes", function () {
		function Polygon() {
		}
		Polygon.prototype.sayName = function() {
			return "polygon";
		};
		const actual = new Polygon() instanceof com.tns.tests.MyInterface1;
		expect(actual).toBe(false);
	});

	it("should return false for ES6 classes", function () {
		class Polygon {
			sayName() {
				return "polygon";
			}
		}
		const actual = new Polygon() instanceof com.tns.tests.MyInterface1;
		expect(actual).toBe(false);
	});

	it("should return false for null", function () {
		const actual = null instanceof com.tns.tests.MyInterface1;
		expect(actual).toBe(false);
	});

	it("should return false for undefined", function () {
		const actual = undefined instanceof com.tns.tests.MyInterface1;
		expect(actual).toBe(false);
	});

	it("should return false for instances that do not implement the interface", function () {
		const actual = new java.io.File("/") instanceof com.tns.tests.MyInterface1;
		expect(actual).toBe(false);
	});

	it("should return true for instances that inherit from a base type", function () {
		const derivedChild = new com.tns.tests.DerivedChild();
		const actual = derivedChild instanceof com.tns.tests.DerivedParent;
		expect(actual).toBe(true);
	});

	it("should return true for instances whose parent class directly implements an interface", function () {
		const derivedChild = new com.tns.tests.DerivedChild();
		const actual = derivedChild instanceof com.tns.tests.MyInterface2;
		expect(actual).toBe(true);
	});

	it("should return true for instances who have a parent class in the hierarchy implementing an interface", function () {
		const derivedChild = new com.tns.tests.DerivedChild();
		const actual = derivedChild instanceof com.tns.tests.MyInterface1;
		expect(actual).toBe(true);
	});
});
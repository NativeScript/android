describe("Tests Java iterators", function () {
	function className(javaObject) {
		try {
			return javaObject.getClass().getName();
		} catch (e) {
			return "<unknown: " + e + ">";
		}
	}

	function drain(iterator) {
		var context = "iterator class " + className(iterator);
		expect(typeof iterator.hasNext).withContext(context + " hasNext").toBe("function");
		expect(typeof iterator.next).withContext(context + " next").toBe("function");

		var items = [];
		while (iterator.hasNext()) {
			items.push(iterator.next());
		}
		return items;
	}

	function drainToStrings(iterator) {
		return drain(iterator).map(function (item) {
			return String(item);
		}).sort();
	}

	function fill(collection) {
		collection.add("a");
		collection.add("b");
		collection.add("c");
		return collection;
	}

	function fillMap(map) {
		map.put("a", "1");
		map.put("b", "2");
		map.put("c", "3");
		return map;
	}

	var collections = {
		"java.util.ArrayList": function () { return fill(new java.util.ArrayList()); },
		"java.util.LinkedList": function () { return fill(new java.util.LinkedList()); },
		"java.util.Vector": function () { return fill(new java.util.Vector()); },
		"java.util.ArrayDeque": function () { return fill(new java.util.ArrayDeque()); },
		"java.util.HashSet": function () { return fill(new java.util.HashSet()); },
		"java.util.LinkedHashSet": function () { return fill(new java.util.LinkedHashSet()); },
		"java.util.TreeSet": function () { return fill(new java.util.TreeSet()); },
		"java.util.concurrent.CopyOnWriteArrayList": function () { return fill(new java.util.concurrent.CopyOnWriteArrayList()); },
		"java.util.Collections.unmodifiableList": function () { return java.util.Collections.unmodifiableList(fill(new java.util.ArrayList())); },
		"java.util.Collections.synchronizedList": function () { return java.util.Collections.synchronizedList(fill(new java.util.ArrayList())); },
		"java.util.Arrays.asList": function () {
			var array = Array.create(java.lang.Object, 3);
			array[0] = "a";
			array[1] = "b";
			array[2] = "c";
			return java.util.Arrays.asList(array);
		}
	};

	Object.keys(collections).forEach(function (name) {
		it("iterates " + name + " with hasNext/next", function () {
			var collection = collections[name]();
			expect(drainToStrings(collection.iterator())).toEqual(["a", "b", "c"]);
		});
	});

	var maps = {
		"java.util.HashMap": function () { return fillMap(new java.util.HashMap()); },
		"java.util.LinkedHashMap": function () { return fillMap(new java.util.LinkedHashMap()); },
		"java.util.TreeMap": function () { return fillMap(new java.util.TreeMap()); },
		"java.util.Hashtable": function () { return fillMap(new java.util.Hashtable()); },
		"java.util.concurrent.ConcurrentHashMap": function () { return fillMap(new java.util.concurrent.ConcurrentHashMap()); },
		"java.util.Collections.unmodifiableMap": function () { return java.util.Collections.unmodifiableMap(fillMap(new java.util.HashMap())); },
		"android.util.ArrayMap": function () { return fillMap(new android.util.ArrayMap()); }
	};

	Object.keys(maps).forEach(function (name) {
		it("iterates the keySet of " + name, function () {
			expect(drainToStrings(maps[name]().keySet().iterator())).toEqual(["a", "b", "c"]);
		});

		it("iterates the values of " + name, function () {
			expect(drainToStrings(maps[name]().values().iterator())).toEqual(["1", "2", "3"]);
		});

		it("iterates the entrySet of " + name, function () {
			// android.util.ArrayMap reuses one entry object, so each entry is read before advancing.
			var iterator = maps[name]().entrySet().iterator();
			var entries = [];
			while (iterator.hasNext()) {
				var entry = iterator.next();
				entries.push(entry.getKey() + "=" + entry.getValue());
			}
			expect(entries.sort()).toEqual(["a=1", "b=2", "c=3"]);
		});
	});

	it("iterates a java.util.ListIterator in both directions", function () {
		var iterator = fill(new java.util.ArrayList()).listIterator();
		var forward = [];
		while (iterator.hasNext()) {
			forward.push(String(iterator.next()));
		}
		var backward = [];
		while (iterator.hasPrevious()) {
			backward.push(String(iterator.previous()));
		}
		expect(forward).toEqual(["a", "b", "c"]);
		expect(backward).toEqual(["c", "b", "a"]);
	});

	it("iterates java.util.List.of", function () {
		if (android.os.Build.VERSION.SDK_INT < 30) {
			pending("java.util.List.of needs API 30");
			return;
		}
		expect(drainToStrings(java.util.List.of("a", "b", "c").iterator())).toEqual(["a", "b", "c"]);
	});

	it("iterates the keys of an org.json.JSONObject", function () {
		var json = new org.json.JSONObject('{"a": 1, "b": 2, "c": 3}');
		expect(drainToStrings(json.keys())).toEqual(["a", "b", "c"]);
	});

	it("iterates the keys of an org.json.JSONObject built from a java.util.HashMap", function () {
		var json = new org.json.JSONObject(fillMap(new java.util.HashMap()));
		expect(json.length()).toBe(3);
		expect(drainToStrings(json.keys())).toEqual(["a", "b", "c"]);
	});

	it("iterates the keySet of an android.os.Bundle", function () {
		var bundle = new android.os.Bundle();
		bundle.putString("a", "1");
		bundle.putString("b", "2");
		bundle.putString("c", "3");
		expect(drainToStrings(bundle.keySet().iterator())).toEqual(["a", "b", "c"]);
	});

	it("enumerates a java.util.Vector with hasMoreElements/nextElement", function () {
		var enumeration = fill(new java.util.Vector()).elements();
		var items = [];
		while (enumeration.hasMoreElements()) {
			items.push(String(enumeration.nextElement()));
		}
		expect(items).toEqual(["a", "b", "c"]);
	});

	it("iterates a Kotlin list and range", function () {
		var array = Array.create(java.lang.Object, 3);
		array[0] = "a";
		array[1] = "b";
		array[2] = "c";
		expect(drainToStrings(kotlin.collections.CollectionsKt.mutableListOf(array).iterator())).toEqual(["a", "b", "c"]);
		expect(drainToStrings(new kotlin.ranges.IntRange(1, 3).iterator())).toEqual(["1", "2", "3"]);
	});

	var fixtures = {
		"a package-private iterator class": function () { return com.tns.tests.IteratorFixtures.packagePrivateIterator(); },
		"a private nested iterator class": function () { return com.tns.tests.IteratorFixtures.privateNestedIterator(); },
		"an anonymous iterator class": function () { return com.tns.tests.IteratorFixtures.anonymousIterator(); },
		"an iterator whose hasNext is inherited from a non-public base": function () { return com.tns.tests.IteratorFixtures.inheritedHasNextIterator(); },
		"the iterator of a lambda Iterable": function () { return com.tns.tests.IteratorFixtures.lambdaIterable().iterator(); }
	};

	Object.keys(fixtures).forEach(function (name) {
		it("iterates " + name, function () {
			expect(drainToStrings(fixtures[name]())).toEqual(["a", "b", "c"]);
		});
	});

	it("reads the elements of an array of a non-public class", function () {
		var array = com.tns.tests.IteratorFixtures.privateNestedIteratorArray();
		expect(array.length).toBe(2);
		expect(drainToStrings(array[0])).toEqual(["a", "b", "c"]);
		expect(drainToStrings(array[1])).toEqual(["a", "b", "c"]);
	});

	it("iterates through a Proxy wrapping a Java iterator", function () {
		var iterator = new Proxy(fillMap(new java.util.HashMap()).keySet().iterator(), {});
		var items = [];
		while (iterator.hasNext()) {
			items.push(String(iterator.next()));
		}
		expect(items.sort()).toEqual(["a", "b", "c"]);
	});
});

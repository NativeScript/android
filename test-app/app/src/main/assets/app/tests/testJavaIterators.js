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

// Non-public JDK classes are described at run time. Android 9 hides some of their
// members from reflection, so the methods of their public interfaces fill the gaps.
describe("Tests interface methods of runtime-resolved JDK types", function () {
	var sdk = android.os.Build.VERSION.SDK_INT;

	function concurrentMap() {
		var map = new java.util.concurrent.ConcurrentHashMap();
		map.put("a", "1");
		map.put("b", "2");
		map.put("c", "3");
		return map;
	}

	function collector(items) {
		return new java.util.function.Consumer({
			accept: function (item) {
				items.push(String(item));
			}
		});
	}

	function ownNames(object) {
		return Object.getOwnPropertyNames(object).sort();
	}

	function own(object, name) {
		return Object.prototype.hasOwnProperty.call(object, name);
	}

	it("removes through the keySet iterator of a ConcurrentHashMap", function () {
		var map = concurrentMap();
		var iterator = map.keySet().iterator();
		var removed = [];
		while (iterator.hasNext()) {
			removed.push(String(iterator.next()));
			iterator.remove();
		}
		expect(removed.sort()).toEqual(["a", "b", "c"]);
		expect(map.size()).toBe(0);
	});

	it("removes through the values iterator of a ConcurrentHashMap", function () {
		var map = concurrentMap();
		var iterator = map.values().iterator();
		while (iterator.hasNext()) {
			if (String(iterator.next()) === "2") {
				iterator.remove();
			}
		}
		expect(map.size()).toBe(2);
		expect(map.containsKey("b")).toBe(false);
	});

	it("advances an ArrayList spliterator with tryAdvance and forEachRemaining", function () {
		if (sdk < 24) {
			pending("spliterators need API 24");
			return;
		}
		var list = new java.util.ArrayList();
		list.add("a");
		list.add("b");
		list.add("c");
		var spliterator = list.spliterator();
		var items = [];
		var consumer = collector(items);
		expect(spliterator.tryAdvance(consumer)).toBe(true);
		spliterator.forEachRemaining(consumer);
		expect(items).toEqual(["a", "b", "c"]);
	});

	it("sizes and advances a HashMap keySet spliterator", function () {
		if (sdk < 24) {
			pending("spliterators need API 24");
			return;
		}
		var map = new java.util.HashMap();
		map.put("a", "1");
		map.put("b", "2");
		map.put("c", "3");
		var spliterator = map.keySet().spliterator();
		expect(spliterator.estimateSize()).toBe(3);
		var items = [];
		expect(spliterator.tryAdvance(collector(items))).toBe(true);
		expect(items.length).toBe(1);
	});

	it("drains a TreeMap sub-map key iterator with forEachRemaining", function () {
		if (sdk < 24) {
			pending("Iterator.forEachRemaining needs API 24");
			return;
		}
		var map = new java.util.TreeMap();
		map.put("a", "1");
		map.put("b", "2");
		map.put("c", "3");
		map.put("d", "4");
		var iterator = map.subMap("a", true, "c", true).keySet().iterator();
		expect(String(iterator.next())).toBe("a");
		var items = [];
		iterator.forEachRemaining(collector(items));
		expect(items).toEqual(["b", "c"]);
	});

	it("has the default forEachRemaining on the keySet iterator of a ConcurrentHashMap", function () {
		if (sdk < 24) {
			pending("Iterator.forEachRemaining needs API 24");
			return;
		}
		var items = [];
		concurrentMap().keySet().iterator().forEachRemaining(collector(items));
		expect(items.sort()).toEqual(["a", "b", "c"]);
	});

	it("has the default reversed on the private comparator behind String.CASE_INSENSITIVE_ORDER", function () {
		if (sdk < 24) {
			pending("Comparator.reversed needs API 24");
			return;
		}
		var order = java.lang.String.CASE_INSENSITIVE_ORDER;
		expect(order.getClass().getName()).toBe("java.lang.String$CaseInsensitiveComparator");
		expect(order.compare("a", "B")).toBeLessThan(0);
		expect(order.reversed().compare("a", "B")).toBeGreaterThan(0);
	});

	// With app code as the Java caller, Android 9 denies a JNI lookup of a hidden
	// member, so the first lookup of these methods has to go to the interface. Nothing
	// else in the suite uses a HashMap values spliterator, so the lookup happens here.
	it("calls the hidden methods of a HashMap values spliterator first from a callback that app Java code invokes", function () {
		if (sdk < 24) {
			pending("spliterators need API 24");
			return;
		}
		var size = -1;
		var advanced = false;
		var items = [];
		com.tns.tests.IteratorFixtures.run(new java.lang.Runnable({
			run: function () {
				var map = new java.util.HashMap();
				map.put("a", "1");
				map.put("b", "2");
				map.put("c", "3");
				var spliterator = map.values().spliterator();
				size = spliterator.estimateSize();
				advanced = spliterator.tryAdvance(collector(items));
			}
		}));
		expect(size).toBe(3);
		expect(advanced).toBe(true);
		expect(items.length).toBe(1);
	});

	it("has the interface methods of a ConcurrentHashMap iterator in a worker", function (done) {
		var worker = new Worker("./runtimeTypeInterfaceMethodsWorker.js");
		worker.onmessage = function (msg) {
			expect(msg.data).toEqual({ drained: ["a", "b", "c"], sizeAfter: 0 });
			worker.terminate();
			done();
		};
		worker.onerror = function (error) {
			fail("worker failed: " + error.message);
			worker.terminate();
			done();
		};
		worker.postMessage("go");
	}, 30000);

	it("keeps the methods the class chain declares on the prototype that declares them", function () {
		var hashMap = new java.util.HashMap();
		hashMap.put("a", "1");
		var hashMapKeys = Object.getPrototypeOf(hashMap.keySet().iterator());
		var hashIterator = Object.getPrototypeOf(hashMapKeys);
		expect(own(hashMapKeys, "next")).toBe(true);
		expect(own(hashMapKeys, "hasNext")).toBe(false);
		expect(own(hashMapKeys, "remove")).toBe(false);
		expect(own(hashIterator, "hasNext")).toBe(true);
		expect(own(hashIterator, "remove")).toBe(true);

		// Android 9 hides these from reflection, so there they come from Iterator.
		if (sdk !== 28) {
			var concurrentKeys = Object.getPrototypeOf(concurrentMap().keySet().iterator());
			var baseIterator = Object.getPrototypeOf(concurrentKeys);
			expect(own(concurrentKeys, "hasNext")).toBe(false);
			expect(own(concurrentKeys, "remove")).toBe(false);
			expect(own(baseIterator, "hasNext")).toBe(true);
			expect(own(baseIterator, "remove")).toBe(true);
		}
	});

	it("leaves the members of app-module iterator classes unchanged", function () {
		var fixtures = com.tns.tests.IteratorFixtures;
		// These two are in the build-time metadata, the others are resolved at run time.
		var described = ["<init>", "constructor", "forEachRemaining", "hasNext", "next", "remove"];
		expect(ownNames(Object.getPrototypeOf(fixtures.packagePrivateIterator()))).toEqual(described);
		expect(ownNames(Object.getPrototypeOf(fixtures.anonymousIterator()))).toEqual(described);
		expect(ownNames(Object.getPrototypeOf(fixtures.privateNestedIterator()))).toEqual(["constructor", "hasNext", "next"]);
		var keyCursor = Object.getPrototypeOf(fixtures.inheritedHasNextIterator());
		expect(ownNames(keyCursor)).toEqual(["constructor", "next"]);
		expect(ownNames(Object.getPrototypeOf(keyCursor))).toEqual(["constructor", "hasNext"]);
	});

	it("wraps a direct ByteBuffer without the members of its non-SDK interface", function () {
		var buffer = java.nio.ByteBuffer.allocateDirect(4);
		expect(buffer.capacity()).toBe(4);
		expect(buffer.isDirect()).toBe(true);
		// sun.nio.ch.DirectBuffer.address() is allowed for apps, while DirectByteBuffer's
		// own address() is blocked from API 29 on, so there it could only come from the
		// interface. Before API 29 the class's own method is visible.
		if (sdk >= 29) {
			expect(typeof buffer.address).toBe("undefined");
		}
		expect(typeof buffer.compact).toBe("function");
	});

	it("keeps overload resolution on a runtime-resolved sub-list", function () {
		var list = new java.util.ArrayList();
		list.add("a");
		list.add("b");
		list.add("c");
		list.add("d");
		var sub = list.subList(0, 3);
		expect(String(sub.remove(0))).toBe("a");
		expect(sub.remove("c")).toBe(true);
		expect(sub.size()).toBe(1);
		expect(list.size()).toBe(2);
	});
});

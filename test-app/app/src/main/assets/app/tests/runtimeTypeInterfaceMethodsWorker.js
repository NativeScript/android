// Drains a ConcurrentHashMap through its non-public key iterator inside a worker,
// whose isolate builds its own prototypes for the runtime-resolved class.
onmessage = function () {
	var map = new java.util.concurrent.ConcurrentHashMap();
	map.put("a", "1");
	map.put("b", "2");
	map.put("c", "3");
	var iterator = map.keySet().iterator();
	var drained = [];
	while (iterator.hasNext()) {
		drained.push(String(iterator.next()));
		iterator.remove();
	}
	postMessage({ drained: drained.sort(), sizeAfter: map.size() });
};

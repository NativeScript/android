// close() lets the running callback finish, so this one never returns unless
// terminate() interrupts it, or the spec raises the stop flag to clean up.
onmessage = function (event) {
	var shared = new Int32Array(event.data);
	close();
	while (Atomics.load(shared, 1) === 0) {
		Atomics.add(shared, 0, 1);
	}
};

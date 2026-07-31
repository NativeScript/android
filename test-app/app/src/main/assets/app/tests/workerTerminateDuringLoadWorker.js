// Spins at module scope so a terminate() from the parent lands while this
// module body is still executing, which is the window the test targets.
var deadline = Date.now() + 5000;
while (Date.now() < deadline) {
}

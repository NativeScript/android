// Ends itself on request, so the parent can observe the end-of-worker path
// that does not go through terminate().
onmessage = function () {
    postMessage("closing");
    close();
};

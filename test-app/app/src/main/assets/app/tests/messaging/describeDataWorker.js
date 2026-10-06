// Reports what `event.data` was before echoing it, so the parent can tell a
// value lost on the way in from one lost on the way back.
function describe(value) {
    return value === undefined ? "undefined" : value === null ? "null" : typeof value;
}

onmessage = function (event) {
    postMessage({ received: describe(event.data) });
    postMessage(event.data);
};

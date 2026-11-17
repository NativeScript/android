// Every runtime generates proxies with its own generator, on its own thread. Several
// workers and this runtime generate different classes at once; each class must come
// out intact, which Java proves by calling every overridden method through it.
describe("Concurrent runtime proxy generation", function () {
    var generateAndCallProxies = require("./proxyGenerationJob");
    var WORKERS = 3;
    var DEADLINE_MS = 60000;

    it("generates intact classes in several runtimes at once", function (done) {
        var workers = [];
        var ready = 0;
        var pending = WORKERS;
        var failures = [];
        var finished = false;

        function finish() {
            if (finished) {
                return;
            }
            finished = true;
            clearTimeout(timer);
            workers.forEach(function (worker) {
                worker.terminate();
            });
            expect(failures).toEqual([]);
            done();
        }

        var timer = setTimeout(function () {
            failures.push(pending + " worker(s) did not report within " + DEADLINE_MS + "ms");
            finish();
        }, DEADLINE_MS);

        function start() {
            workers.forEach(function (worker, index) {
                worker.postMessage("w" + index);
            });
            failures = failures.concat(generateAndCallProxies("main"));
        }

        for (var i = 0; i < WORKERS; i++) {
            var worker = new Worker("./proxyGenerationWorker.js");
            worker.onmessage = function (msg) {
                if (msg.data === "ready") {
                    if (++ready === WORKERS) {
                        start();
                    }
                    return;
                }
                failures = failures.concat(msg.data);
                if (--pending === 0) {
                    finish();
                }
            };
            worker.onerror = function (e) {
                failures.push("worker error: " + (e && e.message));
                if (--pending === 0) {
                    finish();
                }
            };
            workers.push(worker);
        }
    }, 90000);
});

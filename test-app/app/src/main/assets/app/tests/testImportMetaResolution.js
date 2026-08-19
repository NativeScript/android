// `import.meta` is populated by identifying the module in the loader registry,
// so each module in a graph must get its own — not the entry's, and not the
// importer's.
describe("import.meta resolution", function () {
    function loadGraph() {
        return import("~/esm/meta/parent.mjs");
    }

    function rejected(done) {
        return function (error) {
            expect("rejected: " + String((error && error.message) || error)).toBe("resolved");
            done();
        };
    }

    it("gives every module in a graph its own url and dirname", function (done) {
        loadGraph().then(function (graph) {
            var parent = graph.parentMeta;
            var child = graph.childMeta;

            expect(child).not.toBe(parent);
            // `dirname` is a filesystem path, `url` is a file: URL over it.
            expect(parent.url).toBe("file://" + parent.dirname + "/parent.mjs");
            expect(child.url).toBe("file://" + child.dirname + "/child.mjs");
            expect(child.dirname).toBe(parent.dirname + "/nested");
            done();
        }, rejected(done));
    });

    it("returns the identical import.meta object on a repeated import", function (done) {
        Promise.all([loadGraph(), loadGraph()]).then(function (results) {
            expect(results[1]).toBe(results[0]);
            expect(results[1].parentMeta).toBe(results[0].parentMeta);
            expect(results[1].childMeta).toBe(results[0].childMeta);
            done();
        }, rejected(done));
    });
});

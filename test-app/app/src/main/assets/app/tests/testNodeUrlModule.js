describe("node:url", function () {
    function capture(fn) {
        try {
            fn();
        } catch (e) {
            return e;
        }
        return null;
    }

    it("resolves to one frozen object through both require and import", function (done) {
        var required = require("node:url");

        expect(Object.isFrozen(required)).toBe(true);
        expect(Object.keys(required).sort()).toEqual(["fileURLToPath", "pathToFileURL"]);
        // The shim converts between paths and file URLs; it is not a place to
        // reach the URL intrinsic from.
        expect(required.URL).toBeUndefined();
        expect(require("node:url")).toBe(required);

        Promise.all([import("node:url"), import("node:url")]).then(function (results) {
            expect(results[1]).toBe(results[0]);
            expect(results[0].default).toBe(required);
            expect(results[0].fileURLToPath).toBe(required.fileURLToPath);
            expect(results[0].pathToFileURL).toBe(required.pathToFileURL);
            done();
        }).catch(function (error) {
            expect("rejected: " + String((error && error.message) || error)).toBe("resolved");
            done();
        });
    });

    it("converts file URLs to paths the way Node does", function () {
        var fileURLToPath = require("node:url").fileURLToPath;

        expect(fileURLToPath("file:///foo/bar.txt")).toBe("/foo/bar.txt");
        expect(fileURLToPath(new URL("file:///foo/bar.txt"))).toBe("/foo/bar.txt");
        // The URL spec folds a "localhost" authority to no host at all.
        expect(fileURLToPath("file://localhost/foo/bar.txt")).toBe("/foo/bar.txt");
        // Query and fragment are URL syntax, never part of the path.
        expect(fileURLToPath("file:///foo/bar.txt?x=1#frag")).toBe("/foo/bar.txt");
        expect(fileURLToPath("file:///foo/a%20b.txt")).toBe("/foo/a b.txt");
        expect(fileURLToPath("file:///foo/100%25.txt")).toBe("/foo/100%.txt");
    });

    it("rejects file URLs it cannot honestly convert", function () {
        var fileURLToPath = require("node:url").fileURLToPath;

        var wrongScheme = capture(function () { fileURLToPath("http://example.com/x.js"); });
        expect(wrongScheme instanceof TypeError).toBe(true);
        expect(wrongScheme.message).toBe("The URL must be of scheme file");

        var remoteHost = capture(function () { fileURLToPath("file://otherhost/foo.txt"); });
        expect(remoteHost instanceof TypeError).toBe(true);
        expect(remoteHost.message).toBe('File URL host must be "localhost" or empty');

        // %2F would decode into a separator and change the path's shape.
        var encodedSlash = capture(function () { fileURLToPath("file:///foo%2Fbar.txt"); });
        expect(encodedSlash instanceof TypeError).toBe(true);
        expect(encodedSlash.message).toBe("File URL path must not include encoded / characters");

        var expectedArgMessage =
            'The "path" argument must be of type string or an instance of URL.';
        var notAString = capture(function () { fileURLToPath(42); });
        expect(notAString instanceof TypeError).toBe(true);
        expect(notAString.message).toBe(expectedArgMessage);

        var notAUrl = capture(function () { fileURLToPath("not a url"); });
        expect(notAUrl instanceof TypeError).toBe(true);
        expect(notAUrl.message).toBe(expectedArgMessage);
    });

    it("converts paths to file URLs and round-trips them", function () {
        var nodeUrl = require("node:url");
        var url = nodeUrl.pathToFileURL("/foo/bar.txt");

        expect(url instanceof URL).toBe(true);
        expect(url.protocol).toBe("file:");
        expect(url.pathname).toBe("/foo/bar.txt");

        // The characters that would otherwise be read as URL syntax.
        var paths = ["/foo/bar.txt", "/foo/a b.txt", "/foo/100%.txt",
                     "/foo/q?x.txt", "/foo/h#x.txt", "/foo/dir/"];
        for (var i = 0; i < paths.length; i++) {
            expect(nodeUrl.fileURLToPath(nodeUrl.pathToFileURL(paths[i]))).toBe(paths[i]);
        }

        var notAString = capture(function () { nodeUrl.pathToFileURL(42); });
        expect(notAString instanceof TypeError).toBe(true);
        expect(notAString.message).toBe('The "path" argument must be of type string.');

        // No process working directory here, so a relative path has no answer.
        var relative = capture(function () { nodeUrl.pathToFileURL("foo/bar.txt"); });
        expect(relative instanceof TypeError).toBe(true);
        expect(relative.message).toBe('The "path" argument must be an absolute path.');
    });
});

describe("node:module", function () {
    it("exposes exactly createRequire, frozen", function () {
        var nodeModule = require("node:module");

        expect(Object.isFrozen(nodeModule)).toBe(true);
        expect(Object.keys(nodeModule)).toEqual(["createRequire"]);
        expect(typeof nodeModule.createRequire).toBe("function");
        expect(require("node:module")).toBe(nodeModule);
    });

    it("is a distinct module object from ns:module sharing one createRequire", function () {
        var nodeModule = require("node:module");
        var nsModule = require("ns:module");

        expect(nodeModule).not.toBe(nsModule);
        expect(nodeModule.createRequire).toBe(nsModule.createRequire);
    });

    it("omits createPumpingRequire, which has no Node counterpart", function () {
        expect(require("node:module").createPumpingRequire).toBeUndefined();
        expect(typeof require("ns:module").createPumpingRequire).toBe("function");
    });

    it("resolves to the same object through dynamic import", function (done) {
        var nodeModule = require("node:module");
        import("node:module").then(function (ns) {
            expect(ns.default).toBe(nodeModule);
            expect(ns.createRequire).toBe(nodeModule.createRequire);
            done();
        }).catch(function (error) {
            expect("rejected: " + String((error && error.message) || error)).toBe("resolved");
            done();
        });
    });
});

// node:path had an in-resolver polyfill once; it is not a registered builtin,
// so it must now fail exactly like any other unshimmed node: specifier.
describe("unregistered node: specifiers", function () {
    var NOT_FOUND = "No such built-in module: node:path";

    it("fails on require", function () {
        var error = null;
        try {
            require("node:path");
        } catch (e) {
            error = e;
        }
        expect(error instanceof Error).toBe(true);
        expect(error.message).toBe(NOT_FOUND);
    });

    it("fails on dynamic import", function (done) {
        import("node:path").then(function () {
            expect("resolved").toBe("rejected with " + NOT_FOUND);
            done();
        }, function (error) {
            expect(error instanceof Error).toBe(true);
            expect(error.message).toBe(NOT_FOUND);
            done();
        });
    });

    it("fails on a static import from a module", function (done) {
        import("~/esm/nodebuiltins/importsNodePath.mjs").then(function () {
            expect("resolved").toBe("rejected with " + NOT_FOUND);
            done();
        }, function (error) {
            // The instantiation failure wraps the resolver's message.
            expect(String((error && error.message) || error)).toContain(NOT_FOUND);
            done();
        });
    });
});

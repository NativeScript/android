// require() of an ES module follows Node's populateCJSExportsFromESM cascade:
// an explicit 'module.exports' export wins, a namespace without a default or
// with its own __esModule passes through, and everything else is wrapped in a
// facade that adds __esModule while keeping the target's live bindings.
describe("require(esm) exports interop", function () {
    it("wraps a default-exporting module in an __esModule facade", function () {
        var mod = require("~/esm/interop/default-live.mjs");
        expect(mod.__esModule).toBe(true);
        expect(mod.default).toBe(1);
    });

    it("keeps the facade's default binding live", function () {
        var mod = require("~/esm/interop/default-live.mjs");
        var before = mod.default;
        mod.bump();
        expect(mod.default).toBe(before + 1);
    });

    it("returns the 'module.exports' export verbatim", function () {
        var mod = require("~/esm/interop/module-exports.mjs");
        expect(typeof mod).toBe("function");
        expect(mod.marker).toBe("module.exports fixture");
        expect(mod(2, 3)).toBe(5);
    });

    it("passes the namespace through when the module declares __esModule", function () {
        var mod = require("~/esm/interop/own-esmodule.mjs");
        expect(mod.__esModule).toBe("mine");
        expect(mod.default.tag).toBe("own-esmodule");
    });

    it("passes the namespace through when there is no default export", function () {
        var mod = require("~/esm/interop/named-only.mjs");
        expect(mod.alpha).toBe("a");
        expect(mod.beta()).toBe("b");
        expect(mod.default).toBeUndefined();
        expect(mod.__esModule).toBeUndefined();
    });

    it("returns the same exports object for repeated requires", function () {
        var first = require("~/esm/interop/identity.mjs");
        var second = require("~/esm/interop/identity.mjs");
        expect(first).toBe(second);
        expect(first.__esModule).toBe(true);
    });

    it("re-exports the very same default the namespace holds", function (done) {
        var required = require("~/esm/interop/agreement.mjs");
        import("~/esm/interop/agreement.mjs").then(function (ns) {
            expect(required.default).toBe(ns.default);
            expect(required.named).toBe(ns.named);
            // The facade is a distinct namespace: only it carries __esModule.
            expect(required).not.toBe(ns);
            expect(ns.__esModule).toBeUndefined();
            done();
        }).catch(function (e) {
            expect("rejected: " + String((e && e.message) || e)).toBe("resolved");
            done();
        });
    });

    // import() keeps observing the raw namespace whichever side loaded first.
    it("still gives import() the raw namespace after a require()", function (done) {
        import("~/esm/interop/default-live.mjs").then(function (ns) {
            expect(ns.__esModule).toBeUndefined();
            expect(typeof ns.bump).toBe("function");
            expect(require("~/esm/interop/default-live.mjs").default).toBe(ns.default);
            done();
        }).catch(function (e) {
            expect("rejected: " + String((e && e.message) || e)).toBe("resolved");
            done();
        });
    });
});

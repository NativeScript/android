// Dev-loader boundary tests.
//
// Pins the mechanism-only native contract: the runtime exposes exactly the
// `__NS_DEV__` namespace (configureRuntime, invalidateModules,
// kickstartPrefetch, seedModuleBodies, getLoadedModuleUrls,
// setDevBootComplete, terminateAllWorkers, and the debug-only
// canonicalizeHttpUrlKey), and nothing else. HMR *policy* —
// `import.meta.hot`, hot-data/accept/dispose registries, dev-session state,
// boot orchestration — lives in the JS dev client (@nativescript/vite), not
// in the runtime.

describe("__NS_DEV__ dev-loader boundary", function () {
    it("exposes the __NS_DEV__ namespace with the core primitives", function () {
        const dev = globalThis.__NS_DEV__;
        expect(dev).toBeDefined();
        expect(typeof dev.configureRuntime).toBe("function");
        expect(typeof dev.invalidateModules).toBe("function");
        expect(typeof dev.kickstartPrefetch).toBe("function");
        expect(typeof dev.seedModuleBodies).toBe("function");
        expect(typeof dev.getLoadedModuleUrls).toBe("function");
        expect(typeof dev.setDevBootComplete).toBe("function");
        // Main isolate: worker termination is installed here (and ONLY here —
        // worker isolates must not receive it).
        expect(typeof dev.terminateAllWorkers).toBe("function");
    });

    it("seedModuleBodies rejects invalid input without seeding", function () {
        const dev = globalThis.__NS_DEV__;
        const noArg = dev.seedModuleBodies();
        expect(noArg.ok).toBe(false);
        expect(noArg.seeded).toBe(0);
        const badEntries = dev.seedModuleBodies([
            null,
            { body: "export {};" }, // no url
            { url: "not-a-url", body: "export {};" }, // non-http scheme
            { url: "http://127.0.0.1:5173/ns/m/src/app.css", body: "body{}" }, // non-JS shape
            { url: "http://127.0.0.1:5173/ns/m/src/main", body: "" }, // empty body
        ]);
        expect(badEntries.ok).toBe(false);
        expect(badEntries.seeded).toBe(0);
        expect(badEntries.bytes).toBe(0);
    });

    it("keeps the dev surface confined to __NS_DEV__ (no flat __ns* globals)", function () {
        // The contract is the single namespace object: no dev primitive is
        // reachable as a flat global, so tooling can feature-detect exactly
        // one thing and the global namespace stays unpolluted.
        [
            "__nsConfigureRuntime",
            "__nsConfigureDevRuntime",
            "__nsInvalidateModules",
            "__nsKickstartHmrPrefetch",
            "__nsGetLoadedModuleUrls",
            "__nsSetDevBootComplete",
            "__nsTerminateAllWorkers",
            "__nsCanonicalizeHttpUrlKey",
            "__nsStartDevSession",
            "__nsGetHotData",
            "__nsRegisterHotAccept",
            "__nsRegisterHotDispose",
        ].forEach(function (name) {
            expect(typeof globalThis[name]).toBe("undefined");
        });
    });

    it("does not attach import.meta.hot natively", async function () {
        const mod = await import("~/tests/esm/meta-no-hot.mjs");
        // Hot contexts are injected by the JS dev client via source rewrite;
        // a module loaded outside a dev session must see no hot object.
        expect(mod.hasHot).toBe(false);
        expect(mod.hotValue).toBeUndefined();
    });

    it("getLoadedModuleUrls returns an array of registry keys", function () {
        const urls = globalThis.__NS_DEV__.getLoadedModuleUrls();
        expect(Array.isArray(urls)).toBe(true);
        urls.forEach(function (u) {
            expect(typeof u).toBe("string");
        });
    });
});

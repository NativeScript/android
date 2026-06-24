describe("ES Modules", () => {
  it("loads .mjs files as ES modules", async () => {
    const moduleExports = await import("~/testSimpleESModule.mjs");
    expect(moduleExports).toBeTruthy();
    expect(moduleExports?.moduleType).toBe("ES Module");
  });

  it("supports import.meta functionality", async () => {
    const importMetaModule = await import("~/testImportMeta.mjs");
    expect(importMetaModule).toBeTruthy();
    expect(typeof importMetaModule.default).toBe("function");

    const metaResults = importMetaModule.default();
    expect(metaResults).toBeTruthy();
    expect(metaResults.hasImportMeta).toBe(true);
    expect(metaResults.hasUrl).toBe(true);
    expect(metaResults.hasDirname).toBe(true);
    expect(metaResults.url).toBeTruthy();
    expect(metaResults.dirname).toBeTruthy();
  });

  it("supports Worker enhancements", async () => {
    // TODO: make these tests actually be normal tests instead of just importing and checking existence
    const workerModule = await import("~/testWorkerFeatures.mjs");
    expect(workerModule).toBeTruthy();
    expect(typeof workerModule.testWorkerFeatures).toBe("function");

    const workerResults = workerModule.testWorkerFeatures();
    expect(workerResults).toBeTruthy();
    expect(workerResults.stringPathSupported).toBe(true);
    expect(workerResults.urlObjectSupported).toBe(true);
    expect(workerResults.tildePathSupported).toBe(true);
  });

  // These use the done-callback form: this Jasmine version only awaits a spec
  // when its function declares an argument (an async function returning a
  // promise is run synchronously and its result ignored).
  it("resolves a relative dynamic import from a subdirectory module", (done) => {
    import("~/esm-subdir/parent.mjs")
      .then((parent) => parent.loadSibling())
      .then(
        (value) => { expect(value).toBe("sibling-loaded"); done(); },
        (err) => { expect(err).toBeUndefined(); done(); }
      );
  });

  it("resolves a '../' relative dynamic import from a nested module", (done) => {
    import("~/esm-subdir/nested/child.mjs")
      .then((child) => child.loadParentSibling())
      .then(
        (value) => { expect(value).toBe("sibling-loaded"); done(); },
        (err) => { expect(err).toBeUndefined(); done(); }
      );
  });

  it("still resolves a relative dynamic import from an app-root module", (done) => {
    import("~/testRelativeDynamicImport.mjs")
      .then((root) => root.loadRootSibling())
      .then(
        (value) => { expect(value).toBe("ES Module"); done(); },
        (err) => { expect(err).toBeUndefined(); done(); }
      );
  });
});

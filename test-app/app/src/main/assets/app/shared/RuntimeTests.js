describe(module.id, function () {
    it('Runtime version', function () {
        expect(__runtimeVersion).toMatch(/^\d+\.\d+\.\d+.*$/);
    });
});

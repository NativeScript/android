describe('performance.now()', () => {
    it('returns increasing high-resolution time', () => {
        const t1 = performance.now();
        const t2 = performance.now();
        expect(typeof t1).toBe('number');
        expect(isNaN(t1)).toBe(false);
        expect(t2).not.toBeLessThan(t1); // non-decreasing
        // Should be relative (well below 1h after startup)
        expect(t1).toBeLessThan(60 * 60 * 1000);
    });

    it('advances over real time', (done) => {
        const t1 = performance.now();
        setTimeout(() => {
            const t2 = performance.now();
            // 8ms threshold accounts for timer clamping on some devices
            expect(t2 - t1).not.toBeLessThan(8);
            done();
        }, 10);
    });
});

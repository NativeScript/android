describe('queueMicrotask', () => {
    it('should be defined as a function', () => {
        expect(typeof queueMicrotask).toBe('function');
    });

    it('should throw TypeError when callback is not a function', () => {
        expect(() => queueMicrotask(null)).toThrow();
        expect(() => queueMicrotask(123)).toThrow();
        expect(() => queueMicrotask({})).toThrow();
    });

    it('runs after current stack but before setTimeout(0)', (done) => {
        const order = [];
        queueMicrotask(() => order.push('microtask'));
        setTimeout(() => {
            order.push('timeout');
            expect(order).toEqual(['microtask', 'timeout']);
            done();
        }, 0);
        // at this point, nothing should have run yet
        expect(order.length).toBe(0);
    });

    it('preserves ordering with Promise microtasks', (done) => {
        const order = [];
        queueMicrotask(() => order.push('qm1'));
        Promise.resolve().then(() => order.push('p'));
        queueMicrotask(() => order.push('qm2'));

        setTimeout(() => {
            expect(order).toEqual(['qm1', 'p', 'qm2']);
            done();
        }, 0);
    });
});

describe('native timer', () => {
    
    /** @type {global.setTimeout} */
    let setTimeout = global.__ns__setTimeout; 
    /** @type {global.setInterval} */
    let setInterval = global.__ns__setInterval; /** @type global.setTimeout */
    /** @type {global.clearTimeout} */
    let clearTimeout = global.__ns__clearTimeout;
    /** @type {global.clearInterval} */
    let clearInterval = global.__ns__clearInterval;

    it('exists', () => {
        expect(setTimeout).toBeDefined();
        expect(setInterval).toBeDefined();
        expect(clearTimeout).toBeDefined();
        expect(clearInterval).toBeDefined();
    });

    it('triggers timeout', (done) => {
        const now = Date.now();
        setTimeout(() => {
            expect(Date.now() - now).not.toBeLessThan(100);
            done();
        }, 100);
    });

    it('triggers timeout', (done) => {
        const now = Date.now();
        setTimeout(() => {
            expect(Date.now() - now).not.toBeLessThan(100);
            done();
        }, 100);
    });

    it('triggers interval', (done) => {
        let calls = 0;
        const itv = setInterval(() => {
            calls++;
        }, 100);
        setTimeout(() => {
            clearInterval(itv);
            expect(calls).toBe(10);
            done();
        }, 1000);
    });

    it('cancels timeout', (done) => {
        let triggered = false;
        const now = Date.now();
        const timeout = setTimeout(() => {
            triggered = true;
        }, 100);
        clearTimeout(timeout);
        setTimeout(() => {
            expect(triggered).toBe(false);
            done();
        }, 200);
    });

    it('cancels interval', (done) => {
        let triggered = false;
        const now = Date.now();
        const timeout = setInterval(() => {
            triggered = true;
        }, 100);
        clearInterval(timeout);
        setTimeout(() => {
            expect(triggered).toBe(false);
            done();
        }, 200);
    });

    it('cancels interval inside function', (done) => {
        let calls = 0;
        const itv = setInterval(() => {
            calls++;
            clearInterval(itv);
        }, 10);
        setTimeout(() => {
            expect(calls).toBe(1);
            done();
        }, 100);
    });

    it('preserves order', (done) => {
        let calls = 0;
        setTimeout(() => {
            expect(calls).toBe(0);
            calls++;
        });
        setTimeout(() => {
            expect(calls).toBe(1);
            calls++;
            done();
        });
    });
    it('frees up resources after complete', (done) => {
        let timeout = 0;
        let interval = 0;
        let weakRef;
        {
            let obj = {
                value: 0
            };
            weakRef = new WeakRef(obj);
            timeout = setTimeout(() => {
                obj.value++;
            }, 100);
            interval = setInterval(() => {
                obj.value++;
            }, 50);
        }
        setTimeout(() => {
            // use !! here because if you pass weakRef.get() it creates a strong reference (side effect of expect)
            expect(!!weakRef.get()).toBe(true);
            clearInterval(interval);
            clearTimeout(timeout);
            gc();
            expect(!!weakRef.get()).toBe(false);
            done();
        }, 200);
    })
});
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
    // these specs schedule from a java-posted runnable so they run outside any
    // timer callback: jasmine chains specs through timer callbacks, and when
    // the runtime is built with NS_TIMERS_NESTING_CLAMP the nesting clamp
    // (>=5 levels -> 4ms minimum) would otherwise make setTimeout(0)
    // legitimately lose to a postDelayed(0)
    it('preserves order with java handler posts', (done) => {
        const order = [];
        const handler = new android.os.Handler(android.os.Looper.myLooper());
        handler.post(new java.lang.Runnable({
            run: () => {
                setTimeout(() => order.push(1));
                handler.postDelayed(new java.lang.Runnable({ run: () => order.push(2) }), 0);
                setTimeout(() => order.push(3));
                setTimeout(() => {
                    expect(order.join(',')).toBe('1,2,3');
                    done();
                }, 100);
            }
        }));
    });

    it('interleaves many timers with a java handler post', (done) => {
        const order = [];
        const handler = new android.os.Handler(android.os.Looper.myLooper());
        handler.post(new java.lang.Runnable({
            run: () => {
                for (let i = 0; i < 50; i++) {
                    setTimeout(() => order.push('t'));
                }
                handler.postDelayed(new java.lang.Runnable({ run: () => order.push('j') }), 0);
                for (let i = 0; i < 50; i++) {
                    setTimeout(() => order.push('t'));
                }
                setTimeout(() => {
                    // the java runnable must land exactly between the two timer batches
                    expect(order.indexOf('j')).toBe(50);
                    expect(order.length).toBe(101);
                    done();
                }, 100);
            }
        }));
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
            // use another timeout as native weakrefs can't be gced until we leave the isolate after being used once
            setTimeout(() => {
                gc();
                expect(!!weakRef.get()).toBe(false);
                done();
            })
        }, 200);
    })
});
// Settles only if v8 foreground tasks are pumped while the module evaluation
// promise is pending: the notify wakeup arrives as a platform task.
const i32 = new Int32Array(new SharedArrayBuffer(4));
const wait = Atomics.waitAsync(i32, 0, 0);
Atomics.notify(i32, 0);

export const value = await wait.value;

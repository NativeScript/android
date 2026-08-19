// Exclusive to the microtask-context refusal spec: that refusal happens before
// evaluation, so this fixture must never be evaluated by any other spec or the
// guard would have nothing left to refuse.
await Promise.resolve();

export const value = "ok";

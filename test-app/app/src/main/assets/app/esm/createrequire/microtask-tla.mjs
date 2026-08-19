// Top-level await that settles from microtasks alone, so a pumping require can
// finish it while a strict require must still refuse the async graph.
await Promise.resolve();

export const value = "ok";

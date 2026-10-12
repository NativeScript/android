// Never finishes evaluating, so messages posted to this worker stay queued on
// the wrapper: the queue is only enabled once the entry has settled.
await new Promise(() => {});

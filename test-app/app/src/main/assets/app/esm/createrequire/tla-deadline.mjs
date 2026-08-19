// Exclusive to the deadlineSeconds spec: same never-settling park, so the
// require always reaches its deadline and throws.
export const value = await new Promise(function () {});

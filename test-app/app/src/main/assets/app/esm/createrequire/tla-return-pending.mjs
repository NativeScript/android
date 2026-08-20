// Exclusive to the onTimeout:"return-pending" spec. Parks on a promise nothing
// ever settles, so the outcome cannot depend on how v8 schedules a wakeup task
// — the deadline is always what ends the wait.
export const value = await new Promise(function () {});

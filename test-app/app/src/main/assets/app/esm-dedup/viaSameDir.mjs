// Reaches counter.mjs as a same-directory sibling: "./counter.mjs".
import { state, increment } from "./counter.mjs";

increment();

export const seenState = state;

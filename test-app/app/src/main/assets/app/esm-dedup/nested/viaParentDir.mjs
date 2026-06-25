// Reaches the same counter.mjs one directory up: "../counter.mjs".
import { state, increment } from "../counter.mjs";

increment();

export const seenState = state;

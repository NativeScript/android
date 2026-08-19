// Two scopes match this referrer; the more specific one must win.
import { name as scoped } from "ns-scoped-leaf";

export const leaf = scoped;

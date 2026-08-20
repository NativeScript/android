export const testValue = "http-esm-loaded";
export const timestamp = Date.now();

// `import.meta.hot` exists only when a dev server injects a hot context into the
// served source; a disk module must therefore see nothing here.
const hotContext = (typeof import.meta !== "undefined" && import.meta) ? import.meta.hot : undefined;

export function getHotContext() {
    return hotContext;
}

export function callInvalidateSafe() {
    if (!hotContext || typeof hotContext.invalidate !== "function") {
        return false;
    }
    hotContext.invalidate();
    return true;
}

export default function testFunction() {
    return "HTTP ESM loader working at " + new Date().toISOString();
}

export { testFunction as namedExport };

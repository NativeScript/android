// Control: a relative dynamic import from an app-root module, where the
// referrer's directory is the application root. Must keep resolving.
export async function loadRootSibling() {
    const mod = await import("./testSimpleESModule.mjs");
    return mod.moduleType;
}

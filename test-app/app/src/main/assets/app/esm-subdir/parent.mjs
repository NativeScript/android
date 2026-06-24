// Performs a relative dynamic import of a sibling in the same subdirectory.
// The specifier "./sibling.mjs" must resolve against this module's directory,
// not the application root.
export async function loadSibling() {
    const sibling = await import("./sibling.mjs");
    return sibling.value;
}

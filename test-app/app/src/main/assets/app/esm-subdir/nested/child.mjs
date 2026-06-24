// Performs a "../" relative dynamic import reaching up one directory.
export async function loadParentSibling() {
    const sibling = await import("../sibling.mjs");
    return sibling.value;
}

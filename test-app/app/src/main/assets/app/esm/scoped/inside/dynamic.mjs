// Dynamic import must consult the same scope as a static import from this
// file: the host names the referrer by script origin, and that origin must
// canonicalize to the registry key the scope prefixes match against.
export function load() {
    return import("ns-scoped-leaf");
}

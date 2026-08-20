// Default export through a `let` binding, so the facade's re-export has to
// stay live: a copied value would freeze `default` at 1.
let current = 1;

export { current as default };

export function bump() {
    current += 1;
}

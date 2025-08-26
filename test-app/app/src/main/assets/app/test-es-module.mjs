// Test ES Module
export const message = "Hello from ES Module!";
export function greet(name) {
    return `Hello, ${name}!`;
}

export const moduleType = "ES Module";
export const version = "1.0.0";

// Export object with multiple properties
export const utilities = {
    add: (a, b) => a + b,
    multiply: (a, b) => a * b,
    format: (str) => `[${str}]`
};

// Default export
const defaultExport = {
    type: "ESModule",
    version: "1.0.0",
    features: ["exports", "imports", "default-export"],
    status: "working"
};

export default defaultExport;

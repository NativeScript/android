// Lint setup for the runtime's builtin JavaScript
// (test-app/runtime/src/main/cpp/js). Each file is compiled by BuiltinLoader
// as a FUNCTION BODY with the fixed parameters `exports`, `module`, `binding`
// and `primordials` (see that directory's README.md), which are declared as
// globals here. no-undef is the typo net for binding-bag destructures and
// native-global usage alike; no-restricted-properties keeps the captured
// intrinsics from being read off the live globals again.
import globals from 'globals';

// Statics that primordials.js captures, mapped to their replacement. Instance
// methods (Array.prototype.slice and friends) cannot be matched by
// no-restricted-properties on the receiver, so uncurried use of those stays a
// review rule.
const capturedStatics = [
  ['Array', 'isArray', 'ArrayIsArray'],
  ['JSON', 'stringify', 'JSONStringify'],
  ['Object', 'create', 'ObjectCreate'],
  ['Object', 'defineProperty', 'ObjectDefineProperty'],
  ['Object', 'keys', 'ObjectKeys'],
];

// Captured constructors. A destructure from `primordials` shadows the global,
// so these only fire on the unguarded reference.
const restrictedGlobals = ['Date', 'Map', 'Proxy', 'String', 'TypeError'].map((name) => ({
  name,
  message: `Destructure ${name} from primordials — builtins must not read intrinsics off globals user code can replace.`,
}));

const restrictedProperties = capturedStatics.map(([object, property, primordial]) => ({
  object,
  property,
  message: `Use the ${primordial} primordial instead of ${object}.${property} — builtins must not read intrinsics off globals user code can replace.`,
}));

export default [
  {
    files: ['test-app/runtime/src/main/cpp/js/**/*.js'],
    languageOptions: {
      ecmaVersion: 2022,
      sourceType: 'script',
      globals: {
        ...globals.es2021,
        exports: 'readonly',
        module: 'readonly',
        binding: 'readonly',
        primordials: 'readonly',
        global: 'readonly',
        console: 'readonly',
        URL: 'readonly',
        URLSearchParams: 'readonly',
        Blob: 'readonly',
        File: 'readonly',
        WebAssembly: 'readonly',
        // Java package roots resolved through the metadata interceptor at
        // runtime:
        java: 'readonly',
        org: 'readonly',
      },
    },
    rules: {
      'no-undef': 'error',
      'no-unused-vars': ['error', { args: 'none', caughtErrors: 'none' }],
      'no-restricted-properties': ['error', ...restrictedProperties],
      'no-restricted-globals': ['error', ...restrictedGlobals],
    },
  },
  {
    // The file that does the capturing.
    files: ['test-app/runtime/src/main/cpp/js/primordials.js'],
    rules: {
      'no-restricted-properties': 'off',
      'no-restricted-globals': 'off',
    },
  },
];

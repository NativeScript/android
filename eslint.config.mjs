// Lint setup for the runtime's builtin JavaScript
// (test-app/runtime/src/main/cpp/js). Each file is compiled by BuiltinLoader
// as a FUNCTION BODY with the fixed parameters `exports`, `module` and
// `binding` (see that directory's README.md), which are declared as globals
// here. no-undef is the typo net for binding-bag destructures and
// native-global usage alike.
import globals from 'globals';

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
    },
  },
];

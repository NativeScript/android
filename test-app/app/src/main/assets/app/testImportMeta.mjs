// ES Module test for import.meta functionality
console.log('=== Testing import.meta functionality ===');

// Test import.meta.url
console.log('import.meta.url:', import.meta.url);
console.log('Type of import.meta.url:', typeof import.meta.url);

// Test import.meta.dirname
console.log('import.meta.dirname:', import.meta.dirname);
console.log('Type of import.meta.dirname:', typeof import.meta.dirname);

// Validate expected values
export function testImportMeta() {
    const results = {
        url: import.meta.url,
        dirname: import.meta.dirname,
        urlType: typeof import.meta.url,
        dirnameType: typeof import.meta.dirname,
        urlIsString: typeof import.meta.url === 'string',
        dirnameIsString: typeof import.meta.dirname === 'string',
        urlStartsWithFile: import.meta.url && import.meta.url.startsWith('file://'),
        dirnameExists: import.meta.dirname && import.meta.dirname.length > 0,
        // Properties expected by the test
        hasImportMeta: typeof import.meta !== 'undefined',
        hasUrl: typeof import.meta.url === 'string' && import.meta.url.length > 0,
        hasDirname: typeof import.meta.dirname === 'string' && import.meta.dirname.length > 0
    };
    
    console.log('=== Import.meta Test Results ===');
    console.log('URL:', results.url);
    console.log('Dirname:', results.dirname);
    console.log('URL Type:', results.urlType);
    console.log('Dirname Type:', results.dirnameType);
    console.log('URL is string:', results.urlIsString);
    console.log('Dirname is string:', results.dirnameIsString);
    console.log('URL starts with file://:', results.urlStartsWithFile);
    console.log('Dirname exists:', results.dirnameExists);
    
    return results;
}

// Test basic export functionality
export const testValue = 'import.meta works!';
export default testImportMeta;

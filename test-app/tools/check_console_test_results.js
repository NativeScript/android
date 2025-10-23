#!/usr/bin/env node

const { execSync } = require('child_process');

console.log('\n=== Checking Console Test Results ===\n');

function checkTestResults() {
    try {
        console.log('Getting logcat output for test results...');
        
        // Get only recent logcat entries and filter for JS entries to reduce output size
        // Also include "{N} Runtime Tests" tag for Jasmine output
        // Use a larger window to capture all failure details
        const logcatOutput = execSync('adb -e logcat -d -s JS -s "{N} Runtime Tests" | tail -500', { 
            encoding: 'utf8',
            maxBuffer: 4 * 1024 * 1024 // 4MB buffer limit for comprehensive logs
        });
        
        console.log('\n=== Analyzing Test Results ===\n');
        
        // Track different types of test results
        const testResults = {
            esModules: { passed: false, failed: false },
            jasmine: { specs: 0, failures: 0 },
            manual: { tests: [], failures: [] },
            general: { passes: 0, failures: 0 }
        };
        
        // Look for ES Module test results
        testResults.esModules.passed = logcatOutput.includes('🎉 ALL ES MODULE TESTS PASSED!');
        testResults.esModules.failed = logcatOutput.includes('💥 SOME ES MODULE TESTS FAILED!');
        
        // Look for Jasmine test results
        const jasmineSuccessMatch = logcatOutput.match(/SUCCESS:\s*(\d+)\s+specs?,\s*(\d+)\s+failures?/);
        const jasmineFailureMatch = logcatOutput.match(/FAILURE:\s*(\d+)\s+specs?,\s*(\d+)\s+failures?/);
        
        if (jasmineSuccessMatch) {
            testResults.jasmine.specs = parseInt(jasmineSuccessMatch[1]);
            testResults.jasmine.failures = parseInt(jasmineSuccessMatch[2]);
        } else if (jasmineFailureMatch) {
            testResults.jasmine.specs = parseInt(jasmineFailureMatch[1]);
            testResults.jasmine.failures = parseInt(jasmineFailureMatch[2]);
        } else {
            // Try alternative pattern: "X of Y passed (Z skipped)"
            const altJasmineMatch = logcatOutput.match(/(\d+)\s+of\s+(\d+)\s+passed\s*\((\d+)\s+skipped\)/);
            if (altJasmineMatch) {
                const passed = parseInt(altJasmineMatch[1]);
                const total = parseInt(altJasmineMatch[2]);
                const skipped = parseInt(altJasmineMatch[3]);
                testResults.jasmine.specs = total;
                testResults.jasmine.failures = total - passed - skipped;
            }
        }
        
        // Look for manual test patterns (TEST: prefix)
        const testLines = logcatOutput.split('\n');
        testLines.forEach(line => {
            if (line.includes('CONSOLE LOG') || line.includes('JS') || line.includes('{N} Runtime Tests')) {
                // Handle both JS console logs and Jasmine runtime test logs
                const logMatch = line.match(/(?:CONSOLE LOG|JS|{N} Runtime Tests):\s*(.+)/);
                if (logMatch) {
                    const logContent = logMatch[1];
                    
                    // Count manual tests (those that start with "TEST:")
                    if (logContent.startsWith('TEST:')) {
                        testResults.manual.tests.push(logContent);
                    }
                    
                    // Count general pass/fail indicators
                    if (logContent.includes('✅ PASS') || logContent.includes('PASS:')) {
                        testResults.general.passes++;
                    }
                    if (logContent.includes('❌ FAIL') || logContent.includes('FAIL:')) {
                        testResults.general.failures++;
                        testResults.manual.failures.push(logContent);
                    }
                }
            }
        });
        
        // Report results
        console.log('📊 Test Results Summary:');
        console.log('='.repeat(50));
        
        // ES Module tests
        if (testResults.esModules.passed) {
            console.log('✅ ES Module Tests: PASSED');
        } else if (testResults.esModules.failed) {
            console.log('❌ ES Module Tests: FAILED');
        } else {
            console.log('⚠️  ES Module Tests: No clear results found');
        }
        
        // Jasmine tests
        if (testResults.jasmine.specs > 0) {
            if (testResults.jasmine.failures === 0) {
                console.log(`✅ Jasmine Tests: ${testResults.jasmine.specs} specs, 0 failures`);
            } else {
                console.log(`❌ Jasmine Tests: ${testResults.jasmine.specs} specs, ${testResults.jasmine.failures} failures`);
            }
        } else {
            console.log('ℹ️  Jasmine Tests: No Jasmine output detected (may use different execution path)');
        }
        
        // Manual tests
        if (testResults.manual.tests.length > 0) {
            console.log(`📝 Manual Tests: ${testResults.manual.tests.length} tests executed`);
            if (testResults.manual.failures.length > 0) {
                console.log(`❌ Manual Test Failures: ${testResults.manual.failures.length}`);
            }
        }
        
        // General pass/fail counts
        if (testResults.general.passes > 0 || testResults.general.failures > 0) {
            console.log(`📈 General Results: ${testResults.general.passes} passes, ${testResults.general.failures} failures`);
        }
        
        console.log('='.repeat(50));
        
        // Show recent test output for debugging
        const recentTestLines = testLines
            .filter(line => {
                const logMatch = line.match(/(?:CONSOLE LOG|JS|{N} Runtime Tests):\s*(.+)/);
                if (!logMatch) return false;
                
                const logContent = logMatch[1];
                
                // Skip unhelpful messages
                if (logContent === 'Passed' || logContent === 'Failed' || logContent === 'Skipped') {
                    return false;
                }
                
                return (
                    logContent.includes('✅') || 
                    logContent.includes('❌') ||
                    logContent.includes('TEST:') ||
                    logContent.includes('PASS') ||
                    logContent.includes('FAIL') ||
                    logContent.includes('specs') ||
                    logContent.includes('failures') ||
                    logContent.includes('SUCCESS:') ||
                    logContent.includes('FAILURE:') ||
                    logContent.includes('ES MODULE') ||
                    logContent.includes('FAILED TEST:') ||
                    logContent.includes('Suite:') ||
                    logContent.includes('File:') ||
                    logContent.includes('Error:')
                );
            })
            .slice(-5); // Show only last 5 relevant test lines to avoid duplicates
        
        // Remove consecutive duplicate lines
        const uniqueTestLines = [];
        let lastLine = '';
        recentTestLines.forEach(line => {
            const logMatch = line.match(/(?:CONSOLE LOG|JS|{N} Runtime Tests):\s*(.+)/);
            if (logMatch) {
                const currentContent = logMatch[1];
                if (currentContent !== lastLine) {
                    uniqueTestLines.push(line);
                    lastLine = currentContent;
                }
            }
        });
        
        if (uniqueTestLines.length > 0) {
            console.log('\n📋 Recent Test Output:');
            uniqueTestLines.forEach(line => {
                const logMatch = line.match(/(?:CONSOLE LOG|JS|{N} Runtime Tests):\s*(.+)/);
                if (logMatch) {
                    console.log(`  ${logMatch[1]}`);
                }
            });
        }
        
        // Determine overall result
        const hasFailures = testResults.esModules.failed || 
                          testResults.jasmine.failures > 0 || 
                          testResults.manual.failures.length > 0 ||
                          testResults.general.failures > 0;
        
        const hasSuccesses = testResults.esModules.passed || 
                           testResults.jasmine.specs > 0 || 
                           testResults.manual.tests.length > 0 ||
                           testResults.general.passes > 0;
        
        console.log('\n' + '='.repeat(50));
        
        if (hasFailures) {
            console.error('💥 OVERALL RESULT: TESTS FAILED');
            console.log('\nFailure Details:');
            if (testResults.esModules.failed) {
                console.log('  - ES Module tests failed');
            }
            if (testResults.jasmine.failures > 0) {
                console.log(`  - ${testResults.jasmine.failures} Jasmine test failures`);
            }
            if (testResults.manual.failures.length > 0) {
                console.log(`  - ${testResults.manual.failures.length} manual test failures`);
                testResults.manual.failures.slice(0, 5).forEach(failure => {
                    console.log(`    • ${failure}`);
                });
                if (testResults.manual.failures.length > 5) {
                    console.log(`    ... and ${testResults.manual.failures.length - 5} more`);
                }
            }
            
            // Show detailed failure information from logs
            console.log('\n📋 Detailed Failure Information:');
            
            // Debug: Let's see what's actually in the logcat output
            console.log('\n🔍 DEBUG: All recent logcat lines containing "fail" or "error":');
            const debugLines = logcatOutput.split('\n').filter(line => 
                line.toLowerCase().includes('fail') || 
                line.toLowerCase().includes('error') ||
                line.toLowerCase().includes('expected') ||
                line.toLowerCase().includes('debug:')
            ).slice(-20); // Show more lines to catch failure details
            
            debugLines.forEach((line, index) => {
                console.log(`  ${index + 1}: ${line}`);
            });
            
            const failureLines = logcatOutput.split('\n').filter(line => 
                (line.includes('CONSOLE LOG') || line.includes('JS') || line.includes('{N} Runtime Tests')) &&
                (line.toLowerCase().includes('failed test:') ||
                 line.toLowerCase().includes('suite:') ||
                 line.toLowerCase().includes('file:') ||
                 line.toLowerCase().includes('error:') ||
                 line.includes('JASMINE FAILURE:') ||
                 line.includes('JASMINE SUITE:') ||
                 line.includes('JASMINE FILE:') ||
                 line.includes('JASMINE ERROR:') ||
                 line.includes('JASMINE STACK:') ||
                 line.includes('Expected') ||
                 line.includes('Actual') ||
                 line.includes('at '))
            ).slice(-15); // Last 15 failure-related lines for more context
            
            if (failureLines.length > 0) {
                console.log('\n📋 Formatted Failure Information:');
                failureLines.forEach(line => {
                    const logMatch = line.match(/(?:CONSOLE LOG|JS|{N} Runtime Tests):\s*(.+)/);
                    if (logMatch) {
                        console.log(`  ${logMatch[1]}`);
                    }
                });
            } else {
                console.log('  No detailed failure information found in formatted logs');
            }
            
            process.exit(1);
        } else if (hasSuccesses) {
            console.log('🎉 OVERALL RESULT: ALL DETECTED TESTS PASSED');
            console.log('Note: Some tests may use different execution paths or output methods');
        } else {
            console.log('⚠️  OVERALL RESULT: NO TEST RESULTS DETECTED');
            console.log('This might indicate tests did not run or complete properly.');
            process.exit(1);
        }
        
        console.log('\n=== Test verification completed successfully ===');
        
    } catch (error) {
        console.error(`Error checking test results: ${error.message}`);
        process.exit(1);
    }
}

// Run the check
checkTestResults();

//
// Security configuration 
// {
//   "security": {
//     "allowRemoteModules": true,          // Enable remote module loading in production
//     "remoteModuleAllowlist": [           // Optional: restrict to specific URL prefixes
//       "https://cdn.example.com/modules/",
//       "https://esm.sh/"
//     ]
//   }
// }
//
// Behavior:
// - Debug mode: Remote modules always allowed
// - Production mode: Requires security.allowRemoteModules = true
// - With allowlist: Only URLs matching a prefix in remoteModuleAllowlist are allowed

describe("Remote Module Security", function() {
    
    describe("Debug Mode Behavior", function() {
        
        it("should allow HTTP module imports in debug mode", function(done) {
            // This test uses a known unreachable IP to trigger the HTTP loading path
            // In debug mode, the security check passes and we get a network error
            // (not a security error)
            import("http://192.0.2.1:5173/test-module.js").then(function(module) {
                // If we somehow succeed, that's fine too
                expect(module).toBeDefined();
                done();
            }).catch(function(error) {
                // Should fail with a network/timeout error, NOT a security error
                var message = error.message || String(error);
                // In debug mode, we should NOT see security-related error messages
                expect(message).not.toContain("not allowed in production");
                expect(message).not.toContain("remoteModuleAllowlist");
                done();
            });
        });
        
        it("should allow HTTPS module imports in debug mode", function(done) {
            // Test HTTPS URL - should be allowed in debug mode
            import("https://192.0.2.1:5173/test-module.js").then(function(module) {
                expect(module).toBeDefined();
                done();
            }).catch(function(error) {
                var message = error.message || String(error);
                // Should NOT be a security error in debug mode
                expect(message).not.toContain("not allowed in production");
                expect(message).not.toContain("remoteModuleAllowlist");
                done();
            });
        });
    });
    
    describe("Security Configuration", function() {
        
        it("should have security configuration in package.json", function() {
            var context = com.tns.Runtime.getCurrentRuntime().getContext();
            var assetManager = context.getAssets();
            
            try {
                var inputStream = assetManager.open("app/package.json");
                var reader = new java.io.BufferedReader(new java.io.InputStreamReader(inputStream));
                var sb = new java.lang.StringBuilder();
                var line;
                
                while ((line = reader.readLine()) !== null) {
                    sb.append(line);
                }
                reader.close();
                
                var jsonString = sb.toString();
                var config = JSON.parse(jsonString);
                
                // Verify security config structure
                expect(config.security).toBeDefined();
                expect(typeof config.security.allowRemoteModules).toBe("boolean");
                expect(Array.isArray(config.security.remoteModuleAllowlist)).toBe(true);
            } catch (e) {
                fail("Failed to read package.json: " + e.message);
            }
        });
        
        it("should parse security allowRemoteModules from package.json", function() {
            var allowed = com.tns.Runtime.getSecurityAllowRemoteModules();
            expect(typeof allowed).toBe("boolean");
            expect(allowed).toBe(true); // Matches our test package.json config
        });
        
        it("should parse security remoteModuleAllowlist from package.json", function() {
            var allowlist = com.tns.Runtime.getSecurityRemoteModuleAllowlist();
            expect(allowlist).not.toBeNull();
            expect(Array.isArray(allowlist)).toBe(true);
            expect(allowlist.length).toBeGreaterThan(0);
            
            // Verify our test allowlist entries are present
            var hasEsmSh = false;
            var hasCdn = false;
            for (var i = 0; i < allowlist.length; i++) {
                if (allowlist[i].indexOf("esm.sh") !== -1) hasEsmSh = true;
                if (allowlist[i].indexOf("cdn.example.com") !== -1) hasCdn = true;
            }
            expect(hasEsmSh).toBe(true);
            expect(hasCdn).toBe(true);
        });
    });
    
    describe("URL Allowlist Matching", function() {
        
        it("should match URLs in the allowlist (esm.sh)", function() {
            // esm.sh is in our test allowlist
            var isAllowed = com.tns.Runtime.isRemoteUrlAllowed("https://esm.sh/lodash");
            expect(isAllowed).toBe(true);
        });
        
        it("should match URLs in the allowlist (cdn.example.com)", function() {
            // cdn.example.com is in our test allowlist
            var isAllowed = com.tns.Runtime.isRemoteUrlAllowed("https://cdn.example.com/modules/utils.js");
            expect(isAllowed).toBe(true);
        });
        
        it("should allow non-allowlisted URLs in debug mode", function() {
            // In debug mode, all URLs should be allowed even if not in allowlist
            var isAllowed = com.tns.Runtime.isRemoteUrlAllowed("https://unknown-domain.com/evil.js");
            // In debug mode, this returns true because debug bypasses allowlist
            expect(isAllowed).toBe(true);
        });
    });
    
    describe("Static Import HTTP Loading", function() {
        
        it("should attempt to load HTTP module in debug mode", function(done) {
            // Use a valid but unreachable URL to test the HTTP loading path
            // In debug mode, security check passes, then network fails
            import("http://10.255.255.1:5173/nonexistent-module.js").then(function(module) {
                // Unexpected success - but OK if it happens
                expect(module).toBeDefined();
                done();
            }).catch(function(error) {
                // Should be network error, not security error
                var message = error.message || String(error);
                expect(message).not.toContain("not allowed in production");
                expect(message).not.toContain("security");
                // Network errors contain phrases like "fetch", "network", "connect", etc
                done();
            });
        });
    });
    
    describe("Dynamic Import HTTP Loading", function() {
        // Test dynamic imports (ImportModuleDynamicallyCallback path)
        
        it("should attempt to load HTTPS module dynamically in debug mode", function(done) {
            var url = "https://10.255.255.1:5173/dynamic-module.js";
            
            import(url).then(function(module) {
                expect(module).toBeDefined();
                done();
            }).catch(function(error) {
                var message = error.message || String(error);
                // In debug mode, should NOT be a security error
                expect(message).not.toContain("not allowed in production");
                expect(message).not.toContain("remoteModuleAllowlist");
                done();
            });
        });
    });
});

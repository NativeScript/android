// DevFlags.h
#pragma once

#include <string>

namespace tns {

// Fast cached flag: whether to log script loading diagnostics.
// First call queries Java once; subsequent calls are atomic loads only.
bool IsScriptLoadingLogEnabled();

// HTTP module loader flags
//
// Returns true when speculative HTTP module prefetching (the dep-graph BFS
// kicked off after each successful HttpFetchText) should be enabled. Default
// OFF so cold-boot behaviour is unchanged for users who have not opted in.
// Controlled by package.json: "httpModulePrefetch": true|false
bool IsHttpModulePrefetchEnabled();

// Returns true when one log line should be emitted per HTTP fetch URL.
// Default OFF because the volume is high (one line per fetch, hundreds per
// cold boot, hundreds per HMR refresh). Opt in via package.json:
// "httpFetchUrlLog": true|false
bool IsHttpFetchUrlLogEnabled();

// Security config

// "security.allowRemoteModules" from nativescript.config
bool IsRemoteModulesAllowed();

// "security.remoteModuleAllowlist" array from nativescript.config
// If no allowlist is configured but allowRemoteModules is true, all URLs are allowed.
bool IsRemoteUrlAllowed(const std::string& url);

// Mirrors com.tns.Runtime.isDebuggable() (config.isDebuggable), cached once via
// InitializeSecurityConfig(). Use this to gate dev/HMR-only native surfaces so
// they never execute in a plain release build. Returns false (fail-safe) until
// the security config has been initialized or if the JNI lookup fails.
bool IsDebuggable();

// Init security configuration
void InitializeSecurityConfig();

}

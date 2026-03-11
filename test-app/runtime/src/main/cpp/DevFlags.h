// DevFlags.h
#pragma once

#include <string>

namespace tns {

// Fast cached flag: whether to log script loading diagnostics.
// First call queries Java once; subsequent calls are atomic loads only.
bool IsScriptLoadingLogEnabled();

// Security config

// "security.allowRemoteModules" from nativescript.config
bool IsRemoteModulesAllowed();

// "security.remoteModuleAllowlist" array from nativescript.config
// If no allowlist is configured but allowRemoteModules is true, all URLs are allowed.
bool IsRemoteUrlAllowed(const std::string& url);

// Init security configuration
void InitializeSecurityConfig();

}

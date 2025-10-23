// DevFlags.h
#pragma once

namespace tns {

// Fast cached flag: whether to log script loading diagnostics.
// First call queries Java once; subsequent calls are atomic loads only.
bool IsScriptLoadingLogEnabled();

}

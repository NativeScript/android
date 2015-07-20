/*
 * Require.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: gatanasov
 */
#include "Require.h"
#include "File.h"
#include "V8GlobalHelpers.h"
#include "NativeScriptAssert.h"

#include <sstream>

using namespace v8;
using namespace std;

namespace tns
{
	Local<String> Require::LoadModule(const string& path)
	{
		string content = File::ReadText(path);

		auto separatorIndex = path.find_last_of("/");
		string dirName = path.substr(0, separatorIndex);

		// TODO: Use statically allocated buffer for better performance
		string result(MODULE_PART_1);
		result.reserve(content.length() + 1024);
		result += dirName;
		result += MODULE_PART_2;
		result += path;
		result += MODULE_PART_3;
		result += content;
		result += MODULE_PART_4;

		return ConvertToV8String(result);
	}

	const char* Require::MODULE_PART_1 = "(function(){\n var module = {}; module.exports = arguments[0]; var exports = module.exports; var __dirname = \"";
	const char* Require::MODULE_PART_2 = "\"; var __filename = \"";
	const char* Require::MODULE_PART_3 = "\"; function require(moduleName){ return __global.require(moduleName, __dirname); } module.filename = __filename; this.__extends = __global.__extends; \n";
	const char* Require::MODULE_PART_4 = "\n return module.exports; \n})";
}

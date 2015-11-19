#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>

class Constants
{
public:
	const static int PRIMITIVE_TYPE_OFFSET = 1;

	const static char CLASS_NAME_LOCATION_SEPARATOR = '_';

	static std::string APP_ROOT_FOLDER_PATH;
	static std::string V8_STARTUP_FLAGS;
	static bool V8_CACHE_COMPILED_CODE;
	static bool V8_HEAP_SNAPSHOT;

private:
	Constants() {}
};



#endif /* CONSTANTS_H_ */

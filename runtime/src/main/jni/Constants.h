#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>

class Constants {
    public:
        const static char CLASS_NAME_LOCATION_SEPARATOR = '_';

        static std::string APP_ROOT_FOLDER_PATH;
        static std::string V8_STARTUP_FLAGS;
        static std::string V8_HEAP_SNAPSHOT_SCRIPT;
        static std::string V8_HEAP_SNAPSHOT_BLOB;
        static bool V8_CACHE_COMPILED_CODE;

    private:
        Constants() {
        }
};

#endif /* CONSTANTS_H_ */

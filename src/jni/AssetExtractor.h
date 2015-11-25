#ifndef ASSETEXTRACTOR_
#define ASSETEXTRACTOR_

#include "JEnv.h"

namespace tns {
	class AssetExtractor {
		public:
			static void ExtractAssets(JNIEnv *env, jobject obj, jstring apk, jstring outputDir, jboolean _forceOverwrite);

		private:
			static std::string jstringToString(JNIEnv *env, jstring value);
			static void mkdir_rec(const char *dir);
	};
}
#endif /* ASSETEXTRACTOR_ */

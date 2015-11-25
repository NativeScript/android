#include "jni.h"
#include "zip.h"
#include "ArgConverter.h"
#include "NativeScriptAssert.h"
#include <assert.h>
#include <libgen.h>
#include <time.h>
#include <utime.h>
#include <sys/stat.h>
#include "AssetExtractor.h"

using namespace tns;

std::string jstringToString(JNIEnv *env, jstring value);
void mkdir_rec(const char *dir);

void AssetExtractor::ExtractAssets(JNIEnv *env, jobject obj, jstring apk, jstring outputDir, jboolean _forceOverwrite)
{
	auto forceOverwrite = JNI_TRUE == _forceOverwrite;
	auto strApk = jstringToString(env, apk);
	auto baseDir = jstringToString(env, outputDir);
	int err = 0;
	auto z = zip_open(strApk.c_str(), 0, &err);
	assert(z != nullptr);
	zip_int64_t num = zip_get_num_entries(z, 0);
	struct zip_stat sb;
	struct zip_file *zf;
	char buf[65536];
	auto pathcopy = new char[1024];
	for (zip_int64_t i = 0; i < num; i++)
	{
		zip_stat_index(z, i, ZIP_STAT_MTIME, &sb);
		if (strstr(sb.name, "assets/") == sb.name)
		{
			auto name = sb.name + 7; // strlen("assets/") == 7

			std::string assetFullname(baseDir);
			assetFullname.append(name);

			struct stat attrib;
			auto shouldOverwrite = true;
			int ret = stat(assetFullname.c_str(), &attrib);
			if (ret == 0 /* file exists */)
			{
				auto diff = difftime(sb.mtime, attrib.st_mtime);
				shouldOverwrite = diff > 0;
			}

			if (shouldOverwrite || forceOverwrite)
			{
				//DEBUG_WRITE("write asset %s", assetFullname.c_str());
				strcpy(pathcopy, name);
				auto path = dirname(pathcopy);
				std::string dirFullname(baseDir);
				dirFullname.append(path);
				mkdir_rec(dirFullname.c_str());

				zf = zip_fopen_index(z, i, 0);
				assert(zf != nullptr);

				auto fd = fopen(assetFullname.c_str(), "w");

				zip_int64_t sum = 0;
				while (sum != sb.size)
				{
					zip_int64_t len = zip_fread(zf, buf, sizeof(buf));
					assert(len > 0);

					fwrite(buf, 1, len, fd);
					sum += len;
				}
				fclose(fd);
				utimbuf t;
				t.modtime = sb.mtime;
				ret = utime(assetFullname.c_str(), &t);
				zip_fclose(zf);
			}
		}
	}
	delete[] pathcopy;
	zip_close(z);
}

void AssetExtractor::mkdir_rec(const char *dir)
{
	char opath[256];
	snprintf(opath, sizeof(opath), "%s", dir);
	size_t len = strlen(opath);

	if(opath[len - 1] == '/')
		opath[len - 1] = 0;

	for (char *p = opath + 1; *p; p++)
	{
		if (*p == '/')
		{
			*p = 0;
			mkdir(opath, S_IRWXU);
			*p = '/';
		}
	}

	mkdir(opath, S_IRWXU);
}

std::string AssetExtractor::jstringToString(JNIEnv *env, jstring value)
{
	if (value == nullptr) {
		return std::string();
	}

	jboolean f = false;
	const char* chars = env->GetStringUTFChars(value, &f);
	std::string s(chars);
	env->ReleaseStringUTFChars(value, chars);

	return s;
}

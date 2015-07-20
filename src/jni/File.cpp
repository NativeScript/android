/*
 * File.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: gatanasov
 */

#include "File.h"
#include <sstream>

using namespace std;

namespace tns
{
	string File::ReadText(const string& filePath)
	{
		int len;
		bool isNew;
		const char *content = ReadText(filePath, len, isNew);

		string s(content, len);

		if(isNew)
		{
			delete[] content;
		}

		return s;
	}

	const char* File::ReadText(const string& filePath, int& charLength, bool& isNew)
	{
		FILE *file = fopen(filePath.c_str(), "rb");
		fseek(file, 0, SEEK_END);

		charLength = ftell(file);
		isNew = charLength > BUFFER_SIZE;

		rewind(file);

		if(isNew)
		{
			char* newBuffer = new char[charLength];
			fread(newBuffer, 1, charLength, file);
			fclose(file);

			return newBuffer;
		}

		fread(Buffer, 1, charLength, file);
		fclose(file);

		return Buffer;
	}

	char* File::Buffer = new char[BUFFER_SIZE];
}

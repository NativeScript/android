/*
 * File.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: gatanasov
 */

#include "File.h"
#include <sstream>
#include <fstream>

using namespace std;

namespace tns
{
	bool File::Exists(const string& path)
	{
		std::ifstream infile(path.c_str());
		return infile.good();
	}
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

	void* File::ReadBinary(const string& filePath, int& length)
	{
		length = 0;
		if(!File::Exists(filePath))
		{
			return nullptr;
		}

		auto file = fopen(filePath.c_str(), READ_BINARY);
		if(!file)
		{
			return nullptr;
		}

		fseek(file, 0, SEEK_END);
		length = ftell(file);
		rewind(file);

		uint8_t* data = new uint8_t[length];
		fread(data, sizeof(uint8_t), length, file);
		fclose(file);

		return data;
	}

	bool File::WriteBinary(const string& filePath, const void* data, int length)
	{
		auto file = fopen(filePath.c_str(), WRITE_BINARY);
		if(!file)
		{
			return false;
		}

		auto writtenBytes = fwrite(data, sizeof(uint8_t), length, file);
		fclose(file);

		return writtenBytes == length;
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

	const char* File::WRITE_BINARY = "wb";
	const char* File::READ_BINARY = "rb";
}

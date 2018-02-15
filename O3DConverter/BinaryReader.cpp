/**
	Copyright 2017 Jiyan Akgül

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http ://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/
#include "stdafx.h"
#include "BinaryReader.h"
#include <climits>
#include <winsock.h>

BinaryReader::BinaryReader(const char* filename)
{
	f = fopen(filename,"rb");
	fseek(f, 0,SEEK_SET);
	close = true;
}
BinaryReader::BinaryReader(FILE* file, bool CloseAfterUse)
{
	f = file;
	close = CloseAfterUse;
}
char BinaryReader::ReadByte() {
	char buffer = 0;
	fread(&buffer, sizeof(char), 1, f);
	return buffer;
}
char* BinaryReader::ReadBytes(int len) {
	char* buffer = new char[len];
	fread(buffer, len, 1, f);
	return buffer;
}

int BinaryReader::ReadInt32() {
	int* tmp = (int*)ReadBytes(4);
	int val = *tmp;
	delete[] tmp;
	return val;
}


float BinaryReader::ReadSingle() {
	float* tmp = (float*)ReadBytes(sizeof(float));
	float val = *tmp;
	delete[] tmp;
	return val;
}

unsigned int BinaryReader::ReadUInt32() {
	unsigned int* tmp = (unsigned int*)ReadBytes(4);
	unsigned int val = *tmp;
	delete[] tmp;
	return val;
}

unsigned short BinaryReader::ReadUInt16() {
	unsigned short* tmp = (unsigned short*)ReadBytes(2);
	unsigned short val = *tmp;
	delete[] tmp;
	return val;
}

bool BinaryReader::eof() 
{
	return feof(f);
}

BinaryReader::~BinaryReader()
{
	if(close && f!=NULL)fclose(f);
}

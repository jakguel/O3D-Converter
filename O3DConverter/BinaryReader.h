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
#pragma once
#include <fstream>
#include <iostream>
#include "stdafx.h"
#include "RawD3D.h"
class BinaryReader
{
public:
	BinaryReader(const char * filename);
	BinaryReader(FILE*, bool CloseAfterUse = false);
	~BinaryReader();
public :
	template<class T> T* Read(int count = 1) {
		return (T*)ReadBytes(sizeof(T)*count);
	}

	int ReadInt32();
	float ReadSingle();

	unsigned int ReadUInt32();
	unsigned short ReadUInt16();

	char ReadByte();
	char* ReadBytes(int len);

	bool eof();
private:
	bool close;
	FILE* f;

};


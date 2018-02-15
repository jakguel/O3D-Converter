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
#include "assimp\scene.h"
#include "BinaryReader.h"
#include "O3D.h"
#include <vector>

class O3DImporter 
{
public:
	O3DImporter(std::string FileName, std::string TextureFolder = "Textures", std::string OutPutFolder = ".");
	~O3DImporter();
	aiScene* scene;
	O3D* Model;
	std::string TextureFolder;
	std::string OutPutFolder;
	std::vector<aiTexture*>* _textures;
private : 
	void AddMaterial(RawGMObject* obj, aiMesh* mesh, std::string MaterialNamingPrefix);
	void AddMeshData(RawGMObject* obj, aiMesh* mesh);
	std::string AddTexture(char* TextureFileName);
	void AddAnimations();
	void AddTextures();
};


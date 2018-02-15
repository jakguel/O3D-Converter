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
#include "O3DImporter.h"
#include "assimp\mesh.h"
#include "O3D.h"
#include <functional>
#include <soil.h>
#include <lodepng.h>




O3DImporter::O3DImporter(std::string FileName, std::string TextureFolder, std::string OutPutFolder)
{
	auto reader = new BinaryReader(FileName.c_str());

	// Set Texture Folder
	this->TextureFolder = TextureFolder;
	this->OutPutFolder = OutPutFolder;
	_textures = new std::vector<aiTexture*>();

	// Parse O3D Model
	Model = new O3D(reader);

	scene = new aiScene();
	// count meshes
	int total_meshes = 0;
	int total_material = 0;

	for (int group = 0; group < Model->nMaxGroup; group++)
	{
		total_meshes += Model->m_Group[group].m_nMaxObject;
		total_material += Model->m_Group[group].m_pObject->m_nMaxMtrlBlk;
	}
	// Material Array
	if (total_material == 0) {
		// Default Material if no material found
		scene->mMaterials = new aiMaterial*[1];

	}
	else {
		// Init material arrays
		scene->mMaterials = new aiMaterial*[total_material + 1];
	}
	// Add Default Material
	scene->mNumMaterials++;
	scene->mMaterials[0] = new aiMaterial();
	scene->mMaterials[0]->AddProperty(new aiString(AI_DEFAULT_MATERIAL_NAME), AI_MATKEY_NAME);
	// Meshes Array
	scene->mMeshes = new aiMesh*[total_meshes];

	// Init Root Node
	auto fn = std::string(Model->FileName);
	fn = fn.substr(0, fn.find_last_of('.'));
	scene->mRootNode = new aiNode(fn);
	scene->mRootNode->mChildren = new aiNode*[Model->nMaxGroup];
	scene->mRootNode->mNumChildren = Model->nMaxGroup;
	scene->mRootNode->mMeshes = NULL;

	// Add Nodes and meshes to scene
	for (int group = 0; group < Model->nMaxGroup; group++)
	{
		// Init new assimp Node as child node of the root Node in the current assimp scene
		aiNode* groupNode = scene->mRootNode->mChildren[group] = new aiNode("group" + std::to_string(group));
		// Generate array for m_nMaxObject Child Nodes
		groupNode->mChildren = new aiNode*[Model->m_Group[group].m_nMaxObject];
		groupNode->mNumChildren = Model->m_Group[group].m_nMaxObject;
		// Disable Meshes for Group Node
		groupNode->mMeshes = NULL;
		// Iterate through each Object of the Model (in the current group) 
		for (int obj_id = 0; obj_id < Model->m_Group[group].m_nMaxObject; obj_id++)
		{
			// Fetch current Object from Model
			RawGMObject *obj = &Model->m_Group[group].m_pObject[obj_id];

			aiNode* node = new aiNode("object" + std::to_string(obj_id));

			// Create new Mesh Object
			aiMesh* mesh = new aiMesh();
			// Set Name to obj id
			mesh->mName.Set("mesh");

			// Set primitives to Triangle 
			mesh->mPrimitiveTypes |= aiPrimitiveType_TRIANGLE;

			// Disable vertex colors
			mesh->mColors[0] = NULL;

			// Load Mesh Data from Model of current object and pass it to the assimp Mesh
			AddMeshData(obj, mesh);

			// Add Model Bones of current object to assimp Mesh
			if (obj->m_UseBone != NULL && Model->m_pBaseBone!=NULL)
			{
				mesh->mBones = new aiBone*[obj->m_nMaxUseBone];
				for (int bone = 0; bone < obj->m_nMaxUseBone; bone++) {
					// init Bone
					mesh->mBones[bone] = new aiBone();
					mesh->mBones[bone]->mName = "bone" + std::to_string(obj->m_UseBone[bone]);
					// Converter to aiMatrix
					auto b = Model->m_pBaseBone[obj->m_UseBone[bone]];
					mesh->mBones[bone]->mOffsetMatrix = aiMatrix4x4(
						b._11, b._12, b._13, b._14,
						b._21, b._22, b._23, b._24,
						b._31, b._32, b._33, b._34,
						b._41, b._42, b._43, b._44
					);
				}
			}

			// Generate string prefix for loaded material names
			std::string MaterialNamingPrefix = /*std::string(Model->FileName) + */"mat_" + std::to_string(group) + "_" + std::to_string(obj_id);
			// Generate Material Information and pass it to the assimp Mesh
			AddMaterial(obj, mesh, MaterialNamingPrefix);

			// Add Mesh to scene and increase aiscene->nNumMeshes
			scene->mMeshes[scene->mNumMeshes++] = mesh;
			// Link mesh with node
			node->mNumChildren = 0;
			node->mChildren = NULL;
			node->mMeshes = new unsigned int[1]{ 0 };
			node->mNumMeshes = 1;
			// Link node with parent group Noe
			groupNode->mChildren[obj_id] = node;
		}

	}

	// Load Animations
	AddAnimations();
	//AddTextures();
	printf("Imported %s successfully\n", Model->FileName);
	delete reader;
}
void O3DImporter::AddAnimations() {
}

void O3DImporter::AddTextures() {
	scene->mNumTextures = _textures->size();
	scene->mTextures = _textures->data();

}

void O3DImporter::AddMeshData(RawGMObject* obj, aiMesh* mesh) {
	// Dont use Normals (because tangents are missing)
	//mesh->mNormals = new aiVector3D[obj->m_nMaxVB];
	mesh->mNormals = NULL;
	// Vertices Array
	mesh->mNumVertices = obj->m_nMaxVB;
	mesh->mVertices = new aiVector3D[obj->m_nMaxVB];
	// Texture Coords Array
	mesh->mNumUVComponents[0] = 2;
	mesh->mTextureCoords[0] = new aiVector3D[mesh->mNumVertices];
	for (int i = 0; i < obj->m_nMaxVB; i++)
	{
		if (obj->m_Type == GMT_NORMAL) {
			NORMALVERTEX vb = ((NORMALVERTEX*)obj->m_pVB)[i];
			//mesh->mNormals[i] = aiVector3D(vb.normal.x, vb.normal.y, vb.normal.z);
			mesh->mVertices[i] = aiVector3D(vb.position.x, vb.position.y, vb.position.z);
			mesh->mTextureCoords[0][i] = aiVector3D(vb.tu, 1-vb.tv, 0);
		}
		else {
			SKINVERTEX vb = ((SKINVERTEX*)obj->m_pVB)[i];
			//mesh->mNormals[i] = aiVector3D(vb.normal.x, vb.normal.y, vb.normal.z);
			mesh->mVertices[i] = aiVector3D(vb.position.x, vb.position.y, vb.position.z);
			mesh->mTextureCoords[0][i] = aiVector3D(vb.tu, 1-vb.tv, 0);
		}
	}
	/*if (obj->m_Type == GMT_SKIN) {
		mesh->mVertices = new aiVector3D[obj->m_nMaxVertexList];
		mesh->mNumVertices = obj->m_nMaxVertexList;
		for (int i = 0; i < obj->m_nMaxVertexList; i++)
			mesh->mVertices[i] = aiVector3D(obj->m_pVertexList[i].x, obj->m_pVertexList[i].y, obj->m_pVertexList[i].z);
	}*/

	// Add Faces to Mesh
	mesh->mFaces = new aiFace[obj->m_nMaxFaceList];
	mesh->mNumFaces = obj->m_nMaxFaceList;
	for (int i = 0; i < obj->m_nMaxFaceList; i++) {

		mesh->mFaces[i].mIndices = new unsigned int[3]{ obj->m_pIB[(i * 3)], obj->m_pIB[(i * 3) + 1], obj->m_pIB[(i * 3) + 2] };
		mesh->mFaces[i].mNumIndices = 3;
	}
}

void O3DImporter::AddMaterial(RawGMObject* obj, aiMesh* mesh, std::string MaterialNamingPrefix) {
	// Add Materials to Scene
	if (obj->m_nMaxMaterial > 0) {
		// get next material id and increase counter
		int mid = scene->mNumMaterials++;
		// Init new Material in Scene Material array
		scene->mMaterials[mid] = new aiMaterial();
		// Set Material ID to assimp mesh
		mesh->mMaterialIndex = mid;
		// Set Material Name (must be unique)
		scene->mMaterials[mid]->AddProperty(new aiString(MaterialNamingPrefix), AI_MATKEY_NAME);

		// Copy Material settings only from first Material found in Object to current assimp material
		auto Ambient = obj->m_MaterialAry[0].m_Material->Ambient;
		auto Diffuse = obj->m_MaterialAry[0].m_Material->Diffuse;
		auto Emissive = obj->m_MaterialAry[0].m_Material->Emissive;
		auto Power = obj->m_MaterialAry[0].m_Material->Power;
		auto Specular = obj->m_MaterialAry[0].m_Material->Specular;
		scene->mMaterials[mid]->AddProperty(new aiColor3D[1]{ aiColor3D(Ambient.r, Ambient.g, Ambient.b) }, 1, AI_MATKEY_COLOR_AMBIENT);
		scene->mMaterials[mid]->AddProperty(new aiColor3D[1]{ aiColor3D(Diffuse.r, Diffuse.g, Diffuse.b) }, 1, AI_MATKEY_COLOR_DIFFUSE);
		scene->mMaterials[mid]->AddProperty(new aiColor3D[1]{ aiColor3D(Emissive.r, Emissive.g, Emissive.b) }, 1, AI_MATKEY_COLOR_EMISSIVE);
		scene->mMaterials[mid]->AddProperty(new float[1]{ Power }, 1, AI_MATKEY_SHININESS);

		// Iterate through all materials and only add textures to current assimp material
		for (int cur_mat = 0; cur_mat < obj->m_nMaxMaterial; cur_mat++) {
			// Loads the Texture into assimp Scene and returns Texture ID as String for assimp Material
			std::string TextureName = AddTexture(obj->m_MaterialAry[cur_mat].strBitMapFileName);
			if (TextureName.empty()) {
				printf("[Warning]Texture not found : %s\n", obj->m_MaterialAry[cur_mat].strBitMapFileName);
				continue;
			}
			// Add diffuse Texture to assimp Material
			scene->mMaterials[mid]->AddProperty(new aiString(TextureName), AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0));
			//scene->mMaterials[mid]->AddProperty(new aiString(TextureName), AI_MATKEY_TEXTURE_OPACITY(0));

		}
	}
};

// Finds Texture of given File Name from Texture Folder
// Converts Texture File from DDS to PNG
// Stores Texture in current assimp Scene 
// Returns string reference to Texture for assimp Materials
std::string O3DImporter::AddTexture(char* TextureFileName) {

	// Load DDS Texture
	int width, height, channels;
	unsigned char *ht_map = SOIL_load_image
	(
		(TextureFolder + "\\" + std::string(TextureFileName)).c_str(),
		&width, &height, &channels,
		SOIL_LOAD_RGBA
	);
	// break if error occured
	if (ht_map == NULL || channels == 0)return "";

	// Convert to PNG compatible format
	//std::vector<std::uint8_t> PngBuffer(width * height * 4);
	for (int i = 0; i < width*height; i++) {
		auto r = ht_map[i * 4];
		auto g = ht_map[i * 4 + 1];
		auto b = ht_map[i * 4 + 2];
		auto a = ht_map[i * 4 + 3];
		if (r == 255 && g == 0 && a == 255)a = 255;

		/*PngBuffer[i * 4 + 0] = b;
		PngBuffer[i * 4 + 0] = g;
		PngBuffer[i * 4 + 0] = r;
		PngBuffer[i * 4 + 0] = a;*/
	}
	

	// Preape Name
	auto name = std::string(TextureFileName);
	name = name.substr(0, name.find_last_of(".")) + ".png";
	try {
		// Save as PNG
		std::vector<std::uint8_t> ImageBuffer;
		lodepng::encode(ImageBuffer, ht_map, width, height,LCT_RGBA);
		lodepng::save_file(ImageBuffer, OutPutFolder + "\\" + name);
		// Save as TGA
		//SOIL_save_image(name.c_str(), SOIL_SAVE_TYPE_TGA, width, height, 4, ht_map);
	
	}
	catch(std::exception e){
		printf("Texture parsing exception Exception for %s", name);
		name = "";
	}

	// Free memory of DDS File
	SOIL_free_image_data(ht_map);
	return name;

	/*
	// init assimp Texture and Texel Buffer
	auto texture = new aiTexture();
	texture->mWidth = width;
	texture->mHeight = height;
	texture->pcData = new aiTexel[width*height];
	// Parse from RGBA to ARGB888 Texels
	for (int i = 0; i < width*height; i ++) {
	texture->pcData[i].r = ht_map[i * 4];
	texture->pcData[i].g = ht_map[i * 4 + 1];
	texture->pcData[i].b = ht_map[i * 4 + 2];
	texture->pcData[i].a = ht_map[i * 4 + 3];
	}
	// Store in texture vector
	_textures->push_back(texture);

	// Free Memory of Image File
	SOIL_free_image_data(ht_map);

	// See : http://www.assimp.org/lib_html/structai_texture.html
	// Quote : Embedded textures are referenced from materials using strings like "*0", "*1", etc. as the texture paths (a single asterisk character followed by the zero-based index of the texture in the aiScene::mTextures array).
	return "*" + std::to_string(_textures->size()-1);
	*/
}

O3DImporter::~O3DImporter()
{
	// Delete all elements in memory
	// Materials
	/*for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		delete scene->mMaterials[i];
	delete scene->mMaterials;
	// Meshes
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		delete scene->mMeshes[i];
	delete scene->mMeshes;
	// Nodes
	std::function<void(aiNode*)> recursiveDelete;
	recursiveDelete  = [&recursiveDelete](aiNode* node)->void{
		for (unsigned int i = 0; i < node->mNumChildren; i++)
			recursiveDelete(node->mChildren[i]);
		delete node;
	};*/
	delete Model;
//	delete scene;
	//recursiveDelete(scene->mRootNode);*
}

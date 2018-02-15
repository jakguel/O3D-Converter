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
#include <iostream>
#include <fstream>
#include "binaryReader.h"
#include "RawD3D.h"



enum GMTYPE
{
	GMT_ERROR = -1,
	GMT_NORMAL,
	GMT_SKIN,
	GMT_BONE
};

class RawGMObject
{
public:
	TM_ANIMATION* m_pFrame;
	GMTYPE m_Type;
	bool m_bLight;
	int m_nMaxUseBone;
	int* m_UseBone;
	int m_nID;
	int m_nParentIdx;
	GMTYPE m_ParentType;
	RawGMObject* m_pParent;
	D3DXMATRIX* m_mLocalTM;

	D3DXVECTOR3* m_vBBMi;
	D3DXVECTOR3* m_vBBMax;
	int m_bOpacity;
	int m_bBump;
	int m_bRigid;
	int m_nMaxVertexList;
	int m_nMaxVB;
	int m_nMaxFaceList;
	int m_nMaxIB;
	D3DXVECTOR3* m_pVertexList;
	void* m_pVB;
	unsigned short* m_pIB;
	unsigned short* m_pIIB;
	int* m_pPhysiqueVertex;
	int m_bMaterial;
	int m_nMaxMaterial;
	MATERIAL* m_MaterialAry;
	int m_nMaxMtrlBlk;
	MATERIAL_BLOCK* m_pMtrlBlk;

	int LoadFromBinary(BinaryReader* binaryReader);
};


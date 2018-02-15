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
#include "RawGMObject.h"

int RawGMObject::LoadFromBinary(BinaryReader* binaryReader)
{
	m_vBBMi = binaryReader->Read<D3DXVECTOR3>();
	m_vBBMax = binaryReader->Read<D3DXVECTOR3>();
	m_bOpacity = binaryReader->ReadInt32();
	m_bBump = binaryReader->ReadInt32();
	m_bRigid = binaryReader->ReadInt32();

	//reserved
	char* reserved = binaryReader->ReadBytes(28);

	m_nMaxVertexList = binaryReader->ReadInt32();
	m_nMaxVB = binaryReader->ReadInt32();
	m_nMaxFaceList = binaryReader->ReadInt32();
	m_nMaxIB = binaryReader->ReadInt32();

	// Vertices
	m_pVertexList = binaryReader->Read<D3DXVECTOR3>(m_nMaxVertexList);
	// Normals
	if (m_Type == GMT_SKIN)
		m_pVB = binaryReader->Read<SKINVERTEX>(m_nMaxVB);
	else
		m_pVB = binaryReader->Read<NORMALVERTEX>(m_nMaxVB);

	m_pIB = new unsigned short[m_nMaxIB];
	for (int i = 0; i < m_nMaxIB; i++)
		m_pIB[i] = binaryReader->ReadUInt16();

	m_pIIB = new unsigned short[m_nMaxVB];
	for (int i = 0; i < m_nMaxVB; i++)
		m_pIIB[i] = binaryReader->ReadUInt16();
	// Physique
	int dwTemp = binaryReader->ReadInt32();
	if (dwTemp > 0)
	{
		m_pPhysiqueVertex = new int[m_nMaxVertexList];
		for (int i = 0; i < m_nMaxVertexList; i++)
			m_pPhysiqueVertex[i] = binaryReader->ReadInt32();
	}
	// Material
	m_bMaterial = binaryReader->ReadInt32();
	if (m_bMaterial > 0)
	{
		m_nMaxMaterial = binaryReader->ReadInt32();
		if (m_nMaxMaterial == 0) m_nMaxMaterial = 1;
		m_MaterialAry = new MATERIAL[m_nMaxMaterial];
		for (int i = 0; i < m_nMaxMaterial; i++)
		{
			m_MaterialAry[i].m_Material = binaryReader->Read<D3DMATERIAL9>();
			int nLen = binaryReader->ReadInt32();
			m_MaterialAry[i].strBitMapFileName = binaryReader->ReadBytes(nLen);
		}

	}
	m_nMaxMtrlBlk = binaryReader->ReadInt32();
	if (m_nMaxMtrlBlk > 0)
		m_pMtrlBlk = binaryReader->Read<MATERIAL_BLOCK>(m_nMaxMtrlBlk);

	return m_nMaxFaceList;
}
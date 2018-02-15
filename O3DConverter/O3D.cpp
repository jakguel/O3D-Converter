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
#include "O3D.h"

O3D::O3D(BinaryReader* binaryReader)
{
	// FileName
	int fileNameLen = binaryReader->ReadByte();
	if (fileNameLen > 0) {
		char* buffer = binaryReader->ReadBytes(fileNameLen);
		FileName = new char[fileNameLen + 1];
		FileName[fileNameLen] = '\0';
		for (int i = 0; i < fileNameLen; i++)
			FileName[i] = buffer[i] ^ 0xcd;
		delete buffer;
	}
	// Version
	nVer = binaryReader->ReadInt32();
	m_nID = binaryReader->ReadInt32();
	m_vForce1 = binaryReader->Read<D3DXVECTOR3>();
	m_vForce2 = binaryReader->Read<D3DXVECTOR3>();
	if (nVer >= 22)
	{
		m_vForce3 = binaryReader->Read<D3DXVECTOR3>();
		m_vForce4 = binaryReader->Read<D3DXVECTOR3>();
	}
	// Texture ?
	m_fScrlU = binaryReader->ReadSingle();
	m_fScrlV = binaryReader->ReadSingle();

	// reserved
	binaryReader->ReadBytes(16);

	m_vBBMin = binaryReader->Read<D3DXVECTOR3>();
	m_vBBMax = binaryReader->Read<D3DXVECTOR3>();
	m_fPerSlerp = binaryReader->ReadSingle();
	m_nMaxFrame = binaryReader->ReadInt32();

	m_nMaxEvent = binaryReader->ReadInt32();
	if (m_nMaxEvent > 0)
	{
		m_vEvent = binaryReader->Read<D3DXVECTOR3>(m_nMaxEvent);
	}

	// Collision Object
	int nTemp = binaryReader->ReadInt32();
	if (nTemp > 0)
	{
		m_CollObject = new RawGMObject();
		m_CollObject->m_Type = GMT_NORMAL;
		m_nMaxFace += m_CollObject->LoadFromBinary(binaryReader);

	}

	m_bLOD = binaryReader->ReadInt32();

	// Bones
	m_nMaxBone = binaryReader->ReadInt32();
	if (m_nMaxBone > 0)
	{
		m_pBaseBone = binaryReader->Read<D3DXMATRIX>(m_nMaxBone);
		m_pBaseBoneInv = binaryReader->Read<D3DXMATRIX>(m_nMaxBone);
		if (m_nMaxFrame > 0)
			m_pMotion = new RawMotion(binaryReader, m_nMaxBone, m_nMaxFrame);
		m_bSendVS = binaryReader->ReadInt32();
	}

	// Groups
	nMaxGroup = (m_bLOD > 0) ? MAX_GROUP : 1;
	m_Group = new LOD_GROUP[nMaxGroup];
	int nPoolSize = binaryReader->ReadInt32();
	for (int group = 0; group < nMaxGroup; group++)
	{
		LOD_GROUP *pGroup = &m_Group[group];
		pGroup->m_nMaxObject = binaryReader->ReadInt32();
		pGroup->m_pObject = new RawGMObject[pGroup->m_nMaxObject];
		// Objects 
		for (int j = 0; j < pGroup->m_nMaxObject; j++)
		{
			RawGMObject* pObject = &pGroup->m_pObject[j];
			// Type
			int nType = binaryReader->ReadInt32();
			pObject->m_Type = (GMTYPE)(nType & 0xffff);
			if ((nType & 0x80000000) > 0)
				pObject->m_bLight = true;
			// Bones
			pObject->m_nMaxUseBone = binaryReader->ReadInt32();
			if (pObject->m_nMaxUseBone > 0)
			{
				pObject->m_UseBone = new int[pObject->m_nMaxUseBone];
				for (int x = 0; x < pObject->m_nMaxUseBone; x++)
				{
					pObject->m_UseBone[x] = binaryReader->ReadInt32();
				}
			}
			else pObject->m_UseBone = NULL;

			// ID/ParentID
			pObject->m_nID = binaryReader->ReadInt32();
			pObject->m_nParentIdx = binaryReader->ReadInt32();
			if (pObject->m_nParentIdx != -1)
			{
				pObject->m_ParentType = (GMTYPE)binaryReader->ReadInt32();
				switch (pObject->m_ParentType)
				{
				case GMT_BONE:
					pObject->m_pParent = (RawGMObject*)0; break;
				case GMT_NORMAL:
					pObject->m_pParent = &pGroup->m_pObject[pObject->m_nParentIdx]; break;
				}
			}
			pObject->m_mLocalTM = binaryReader->Read<D3DXMATRIX>();
			m_nMaxFace += pObject->LoadFromBinary(binaryReader);
			// LoadTMAnim
			if (pObject->m_Type == GMT_NORMAL && m_nMaxFrame > 0)
			{
				int bFrame = binaryReader->ReadInt32();
				if (bFrame > 0)
					pObject->m_pFrame = binaryReader->Read<TM_ANIMATION>(m_nMaxFrame);
			}

		}

	}

	if (nVer >= 21 && !binaryReader->eof())
	{
		nAttr = binaryReader->ReadInt32();
		if (nAttr > 0 && nAttr == m_nMaxFrame)
			m_pAttr = binaryReader->Read<MOTION_ATTR>(m_nMaxFrame);
	}
	return;
}

O3D::~O3D()
{
	for (int group = 0; group < nMaxGroup; group++)
	{
		LOD_GROUP *pGroup = &m_Group[group];
		// Objects 
		for (int j = 0; j < pGroup->m_nMaxObject; j++)
		{
			RawGMObject* pObject = &pGroup->m_pObject[j];
			delete[] pObject->m_pIB;
			delete[] pObject->m_pIIB;
			if (pObject->m_nMaxMtrlBlk>0)delete[] pObject->m_pMtrlBlk;
			delete[] pObject->m_pVB;
			delete[] pObject->m_pVertexList;
		}
		//delete[] pGroup;
	}
}

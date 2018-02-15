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
#include <Windows.h>
#include "binaryReader.h"
#include "RawD3D.h"



typedef struct _MOTION_ATTR
{
public:

	unsigned int m_dwAttr;
	//	char	m_szFileName[32];
	int m_nSndID;
	float m_fFrame;

}MOTION_ATTR;

typedef struct _BONE
{
public:

	_BONE* m_pParent;
	int m_nParentIdx;
	char* m_szName;
	D3DXMATRIX* m_mTM;
	D3DXMATRIX* m_mInverseTM;
	D3DXMATRIX* m_mLocalTM;
}BONE;

typedef struct _BONE_FRAME
{
public:

	TM_ANIMATION* m_pFrame;
	D3DXMATRIX* m_mLocalTM;
}BONE_FRAME;

class RawMotion
{
public:
	BONE* m_pBoneInfo;
	BONE_FRAME* m_pBoneFrame;
	RawMotion(BinaryReader* binaryReader, int nNumBone, int nNumFrame)
	{

		m_pBoneInfo = new BONE[nNumBone];
		// Read bones
		for (int i = 0; i < nNumBone; i++)
		{
			int len = binaryReader->ReadInt32();
			m_pBoneInfo[i].m_szName = binaryReader->ReadBytes(len);
			m_pBoneInfo[i].m_mInverseTM = binaryReader->Read<D3DXMATRIX>();
			m_pBoneInfo[i].m_mLocalTM = binaryReader->Read<D3DXMATRIX>();
			m_pBoneInfo[i].m_nParentIdx = binaryReader->ReadInt32();
		}
		// Link bones
		for (int i = 0; i < nNumBone; i++)
			if (m_pBoneInfo[i].m_nParentIdx != -1)
				m_pBoneInfo[i].m_pParent = &m_pBoneInfo[m_pBoneInfo[i].m_nParentIdx];
		// Animation
		int nNumSize = binaryReader->ReadInt32();
		m_pBoneFrame = new BONE_FRAME[nNumBone];
		for (int i = 0; i < nNumBone; i++)
		{
			int nFrame = binaryReader->ReadInt32();
			if (nFrame == 1)
				m_pBoneFrame[i].m_pFrame = binaryReader->Read<TM_ANIMATION>(nNumFrame);
			else
				m_pBoneFrame[i].m_mLocalTM = binaryReader->Read<D3DXMATRIX>();
		}

	}
};


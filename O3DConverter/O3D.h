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
#include "RawGMObject.h"
#include "RawMotion.h"
#include "stdafx.h"

struct LOD_GROUP
{
	int m_nMaxObject;
	RawGMObject* m_pObject;
	D3DXMATRIX _mUpdate;
};

class O3D
{
public:
	const int MAX_GROUP = 3;
	char* FileName;

	int nVer;
	/// <summary>
	/// Serial ID
	/// </summary>
	int m_nID;
	D3DXVECTOR3* m_vForce1;
	D3DXVECTOR3* m_vForce2;
	D3DXVECTOR3* m_vForce3;
	D3DXVECTOR3* m_vForce4;

	float m_fScrlU;
	float m_fScrlV;

	D3DXVECTOR3* m_vBBMin;
	D3DXVECTOR3* m_vBBMax;

	float m_fPerSlerp;

	/// <summary>
	/// Animation Frames
	/// </summary>
	int m_nMaxFrame;

	int m_nMaxEvent;
	D3DXVECTOR3* m_vEvent;

	RawGMObject* m_CollObject;

	int m_bLOD;
	/// <summary>
	/// Bones Count
	/// </summary>
	int m_nMaxBone;
	/// <summary>
	/// Bones
	/// </summary>
	D3DXMATRIX* m_pBaseBone;
	/// <summary>
	/// Bones Inverted side
	/// </summary>
	D3DXMATRIX* m_pBaseBoneInv;

	RawMotion* m_pMotion;

	int m_bSendVS;

	int nMaxGroup;
	LOD_GROUP* m_Group;

	int nAttr;
	MOTION_ATTR* m_pAttr;
	int m_nMaxFace;

	O3D(BinaryReader* binaryReader);
	~O3D();
};


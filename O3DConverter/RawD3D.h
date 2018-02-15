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
#include "stdafx.h"
#define MAX_BONES 28

/*
* DirectX 9 Headers 
*/

typedef struct  _D3DXVECTOR3
{
public:
	float x, y, z;
}D3DXVECTOR3;


typedef struct _D3DXMATRIX
{
public:
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
}D3DXMATRIX;

typedef struct _D3DXQUATERNION
{
public:
	float x;
	float y;
	float z;
	float w;

}D3DXQUATERNION;
typedef struct _TM_ANIMATION
{
	//	D3DXMATRIX			m_mAniTM;
	D3DXQUATERNION m_qRot;
	D3DXVECTOR3 m_vPos;
}TM_ANIMATION;
typedef struct _D3DCOLORVALUE
{
public:
	float r;
	float g;
	float b;
	float a;

}D3DCOLORVALUE;

struct D3DMATERIAL9
{
public:
	D3DCOLORVALUE Diffuse;
	D3DCOLORVALUE Ambient;
	D3DCOLORVALUE Specular;
	D3DCOLORVALUE Emissive;
	float Power;

};

typedef struct _MATERIAL_BLOCK
{
	int m_nStartVertex;
	int m_nPrimitiveCount;
	int m_nTextureID;
	//	int		m_n2Side;				// 2 side render block
	//	int		m_nReflect;
	//	int		m_nOpacity;
	unsigned int m_dwEffect;
	int m_nAmount;      // ������
	int m_nMaxUseBone;
	int m_UseBone[MAX_BONES];
}MATERIAL_BLOCK;

struct SKINVERTEX
{
	D3DXVECTOR3 position;	// The 3D position for the vertex
	float		w1, w2;		// vertex weight
	unsigned int	matIdx;
	D3DXVECTOR3 normal;		// The surface normal for the vertex
	float       tu, tv;		// The texture coordinates
};
typedef struct _NORMALVERTEX
{
public:
	D3DXVECTOR3 position;   // The 3D position for the vertex
	D3DXVECTOR3 normal;     // The surface normal for the vertex
	float tu, tv;       // The texture coordinates
}NORMALVERTEX;

typedef struct _MATERIAL
{
public:
	D3DMATERIAL9 * m_Material;
	char* strBitMapFileName;
	int m_nUseCnt;
} MATERIAL;
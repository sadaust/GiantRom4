/*=======================================
=ResourceDefines.h						=
=by Tyler van Gastel					=
=latest update 11/19/2015				=
=list of defines that the engine uses	=
=========================================*/

#pragma once

#include <fmod.hpp>
#pragma comment(lib,"Fmodex_vc.lib")

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <string>

#define soundAsset FMOD::Sound*

#define Texture IDirect3DTexture9*
#define TexInfo D3DXIMAGE_INFO

enum soundType {
	music,
	soundEffect,
	ambientSound,
	voice,

	numSTypes,
	sError
};

enum renderType {
	screenSprite,
	worldSprite,
	bbSprite,
	cubePrim,
	prim,
	text,
	wText,
	bbText,
	xmodel,
	line,
	numRenTypes,
	renError
};

enum resourceType {
	image,
	primitive,
	cube,
	xModel,
	audio,
	stream,
	error
};

struct resourceStruct {
	std::string name;
	void* resource;
	resourceType resType;
};

struct frect {
	float top;
	float bottom;
	float left;
	float right;
};

struct textStruct {
	std::string text;
	DWORD flags;
	//text rect in percentage of screen range of 0.0 to 1.0
	frect rect;
	D3DCOLOR color;
};

struct vector {
	float x;
	float y;
	float z;
	bool operator!=(vector &other) {
		if(x != other.x) {
			return true;
		} else if (y != other.y) {
			return true;
		} else if (z != other.z) {
			return true;
		}
		return false;
	}
	bool operator==(vector &other) {
		if (x != other.x) {
			return false;
		}
		else if (y != other.y) {
			return false;
		}
		else if (z != other.z) {
			return false;
		}
		return true;
	}
	vector() {
		x = y = z = 0;
	}
	vector(float fX,float fY,float fZ) {
		x = fX;
		y = fY;
		z = fZ;
	}
};

struct renInfo {
	renderType type;
	//pointer to the render asset
	void* asset;
	//transformation matrix not used in text, or line
	D3DXMATRIX matrix;
};

struct camera {
	D3DXVECTOR3 cam_pos;
	D3DXVECTOR3 cam_look_pos;
	D3DXVECTOR3 cam_up_vec;
	float fov;
	float drawDist;
};

//asset used for textures and sprite sheets
struct imageAsset {
	Texture objTex;
	TexInfo texInfo;
	D3DCOLOR mask;
	inline bool operator==(imageAsset other) {
		return this->objTex == other.objTex;
	}
};

struct cubeAsset {
	IDirect3DVertexBuffer9* obj;
	IDirect3DIndexBuffer9* objInd;
	IDirect3DVertexDeclaration9* objDec;

	float bottom;
	float top;
	float left;
	float right;
	float front;
	float back;

	inline bool operator==(cubeAsset other) {
		return this->obj == other.obj;
	}
};

struct cubeStruct {
	cubeAsset* cubeObj;
	imageAsset* texture;
	D3DMATERIAL9 mat;
};

struct modelAsset {
	ID3DXMesh* mesh;
	ID3DXBuffer* adj;
	ID3DXBuffer* matPoint;
	D3DXMATERIAL* mats;
	IDirect3DVertexDeclaration9* dec;
	DWORD numMats;
	imageAsset** textures; 

	inline bool operator==(modelAsset other) {
		return this->mesh == other.mesh;
	}
};

struct spriteStruct {
	imageAsset* image;
	//rectangle of the texture that will be rendered
	RECT rec;
	//this is the local space point the sprite will rotate on when transformed, upper left is 0,0,0
	D3DXVECTOR3 center;
	//color to tint the sprite with
	D3DCOLOR color;
};

struct musicStruct {
	soundAsset asset;
	soundType type;
	inline bool operator==(musicStruct other) {
		return this->asset == other.asset;
	}
};

struct soundStruct {
	soundAsset asset;
	soundType type;
	float minDist;
	float maxDist;
	inline bool operator==(soundStruct other) {
		return this->asset == other.asset;
	}
};

struct Vertex {
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
};

struct lineStruct {
	D3DXVECTOR2* vec;
	DWORD vecCount;
	D3DCOLOR color;
	//width in pixels
	int width;
};
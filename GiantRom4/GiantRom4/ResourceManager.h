/*===================================
=ResourceManager.h/.cpp				=
=by Tyler van Gastel				=
=latest update 11/19/2015			=
=manages loading and loaded assets	=
=====================================*/

#pragma once
#include "ResourceDefines.h"

#include <vector>
#include <list>

#define defaultMinSound 100
#define defaultMaxSound 5000

class ResourceManager {
private:
	std::vector<resourceStruct> resVec;
	std::list<imageAsset> imageList;
	std::list<cubeAsset> cubeList;
	std::list<modelAsset> modelList;
	std::list<soundStruct> soundList;
	std::list<musicStruct> musicList;
public:
	ResourceManager();
	~ResourceManager();
	resourceStruct* getResource(LPCSTR fileName, resourceType resType);
	resourceStruct* getResource(LPCSTR fileName, D3DXCOLOR mask);
	resourceStruct* createCube(LPCSTR name, float top, float bottom, float front, float back, float left, float right);
	void reload();
	void reload(LPCSTR assetName);
	void reload(resourceType type);
	void release();
	void release(LPCSTR assetName);
};
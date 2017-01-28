#include "Graphics.h"
#include <fstream>

Graphics::Graphics() {
	m_hWnd = 0;
	m_hInts = 0;

	m_pD3DObject = 0;
	m_pD3DDevice = 0;

	m_pD3DFont = 0;
	m_pD3DSprite = 0;
	m_pD3DLine = 0;

	_width = 0;
	_height = 0;

	_fontH = 30;
	_fontW = 0;

	_vSync = false;
	_fullScreen = false;
	_lost = true;

	ren.setSize(ren_startSize);
	screenRen.setSize(ren_startSize);

	for(int i = 0; i < MAX_LIGHTS; ++i) {
		_lights[i].Range = 0;
	}

	clearColor =  D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
}

Graphics::~Graphics() {
	shutdown();
}

void Graphics::init(HWND hWnd,HINSTANCE hInst, bool bWindowed) {
	ren.clear();
	m_hWnd = hWnd;
	m_hInts = hInst;
	//create D3D Object
	m_pD3DObject = Direct3DCreate9(DIRECT3D_VERSION);
	_fullScreen = !bWindowed;
	//find height and width of window
	RECT rect;
	GetWindowRect(hWnd, &rect);
	if(_width == 0) {
		_width = rect.right - rect.left;
	}
	if(_height == 0) {
		_height = rect.bottom - rect.top;
	}

	_aspectRatio = (float)_width/(float)_height;

	// Set D3D Device presentation parameters before creating the device
	ZeroMemory(&D3Dpp, sizeof(D3Dpp));  // NULL the structure's memory

	D3Dpp.hDeviceWindow					= hWnd;										// Handle to the focus window
	D3Dpp.Windowed						= bWindowed;								// Windowed or Full-screen
	D3Dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;								// Format of depth/stencil buffer
	D3Dpp.EnableAutoDepthStencil		= TRUE;										// Enables Z-Buffer
	D3Dpp.BackBufferCount				= 1;										// Change if need of > 1 is required at a later date
	D3Dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;							// Back-buffer format
	D3Dpp.BackBufferHeight				= _height;									// resolution
	D3Dpp.BackBufferWidth				= _width;									// (Same as above)
	D3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;					// Discard back-buffer
	D3Dpp.PresentationInterval			= _vSync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE; // Present back-buffer immediately, unless V-Sync is on								
	D3Dpp.Flags							= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;		// This flag should improve performance, if not set to NULL.
	D3Dpp.FullScreen_RefreshRateInHz	= bWindowed ? 0 : D3DPRESENT_RATE_DEFAULT;	// Full-screen refresh rate
	D3Dpp.MultiSampleQuality			= 0;										// MSAA quality
	D3Dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;						// MSAA type

	// Check device capabilities
	DWORD deviceBehaviorFlags = 0;
	m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps);

	// Determine vertex processing mode
	if(m_D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING) {
		// Hardware vertex processing supported? (Video Card)
		deviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	
	}
	else {
		// If not, use software (CPU)
		deviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	}

	// If hardware vertex processing is on, check pure device support
	if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) {
		deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	
	}

	// Create the D3D Device with the present parameters and device flags above
	if(m_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,		// which adapter to use, set to primary
		D3DDEVTYPE_HAL,			// device type to use, set to hardware rasterization
		hWnd,					// handle to the focus window
		deviceBehaviorFlags,	// behavior flags
		&D3Dpp,					// presentation parameters
		&m_pD3DDevice)==D3D_OK) {			// returned device pointer
			// Create a Font Object
#ifdef FONTPATH
			AddFontResource(FONTPATH);
#endif 

			//AddFontResource("PRISTINA.TTF");
			D3DXCreateFont(m_pD3DDevice,_fontH,_fontW,FW_BOLD,0,false,DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, FONT,&m_pD3DFont);

			// Create Sprite Object and Textures
			D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

			//create line object
			D3DXCreateLine(m_pD3DDevice, &m_pD3DLine);

			m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
			m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(60, 60, 60));
			m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
			m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			_lost = false;

			m_pD3DLine->SetAntialias(true);
	} else {
		_lost = true;
	}
}

void Graphics::addRender(renInfo& info) {
	switch(info.type) {
	case cubePrim:
	case xmodel:
	case worldSprite:
	case bbSprite:
	case wText:
	case bbText:
		ren.add(info);
		break;
	case screenSprite:
	case line:
	case text:
		screenRen.add(info);
		break;
	}
}

void Graphics::render() {
	int renSize;
	D3DXMATRIX transMat;
	renInfo* tempInfo;
	spriteStruct* sprStruct;
	modelAsset* modStruct;
	cubeStruct* tempCube;
	textStruct* tempText;
	lineStruct* tempLine;
	RECT tempRect;
	if(_lost)
		return;
	renSize = ren.getSize();
	if(D3D_OK==(m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clearColor, 1.0f, 0))) {
		if(SUCCEEDED(m_pD3DDevice->BeginScene())) {
			//trying an alternitive of what I did in elimbination,
			//the idea is that not changing the type of thing being rendered will be more efficent and make up for the repeted loops, might change after testing
			D3DXMatrixIdentity(&transMat);
			m_pD3DSprite->SetTransform(&transMat);
			//set camera trans
			D3DXMatrixLookAtLH(&transMat,&cam.cam_pos,&cam.cam_look_pos,&cam.cam_up_vec);
			m_pD3DDevice->SetTransform(D3DTS_VIEW,&transMat);
			D3DXMatrixPerspectiveFovLH(&transMat,D3DXToRadian(cam.fov),_aspectRatio,1.0f,cam.drawDist);
			m_pD3DDevice->SetTransform(D3DTS_PROJECTION,&transMat);

			//world space
			for(int i = 0; i < renSize; ++i) {
				D3DXMatrixIdentity(&transMat);
				//reset trans
				m_pD3DDevice->SetTransform(D3DTS_WORLD,&transMat);
				m_pD3DSprite->SetTransform(&transMat);
				tempInfo = ren.get(i);
				//primitive
				if(tempInfo->type == cubePrim) {
					tempCube = (cubeStruct*)tempInfo->asset;
					m_pD3DDevice->SetStreamSource(0,tempCube->cubeObj->obj, 0, sizeof(Vertex));
					m_pD3DDevice->SetIndices(tempCube->cubeObj->objInd);
					m_pD3DDevice->SetVertexDeclaration(tempCube->cubeObj->objDec);
					//set texture
					m_pD3DDevice->SetTexture(0,tempCube->texture->objTex);
					m_pD3DDevice->SetMaterial(&tempCube->mat);
					//set trans
					m_pD3DDevice->SetTransform(D3DTS_WORLD, &tempInfo->matrix);
					//render
					m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,24,0,12);
				}
				//xModels
				if(tempInfo->type == xmodel) {
					modStruct = (modelAsset*)tempInfo->asset;
					m_pD3DDevice->SetTransform(D3DTS_WORLD,&tempInfo->matrix);
					for(int i = 0;i < modStruct->numMats;++i) {
						//set texture
						m_pD3DDevice->SetTexture(0,modStruct->textures[i]->objTex);
						m_pD3DDevice->SetMaterial(&modStruct->mats[i].MatD3D);
						//draw model
						modStruct->mesh->DrawSubset(i);
					}
				}

				//world sprite
				//check asset type
				if(tempInfo->type == worldSprite) {
					m_pD3DSprite->Begin(D3DXSPRITE_OBJECTSPACE);
					sprStruct = (spriteStruct*)tempInfo->asset;
					m_pD3DDevice->SetTransform(D3DTS_WORLD,&tempInfo->matrix);
					//m_pD3DSprite->SetTransform(&tempInfo->matrix);
					m_pD3DSprite->Draw(sprStruct->image->objTex,&sprStruct->rec,&sprStruct->center,0,sprStruct->color);
					m_pD3DSprite->End();
				}
				//billboarded world sprite
				//check asset type
				if(tempInfo->type == bbSprite) {
					m_pD3DDevice->GetTransform(D3DTS_VIEW,&transMat);
					sprStruct = (spriteStruct*)tempInfo->asset;
					m_pD3DSprite->SetWorldViewLH(&tempInfo->matrix,&transMat);
					m_pD3DSprite->Begin(D3DXSPRITE_BILLBOARD);
					m_pD3DSprite->Draw(sprStruct->image->objTex,&sprStruct->rec,&sprStruct->center,0,sprStruct->color);
					m_pD3DSprite->End();
				}

				//world space text
				if(tempInfo->type == wText) {
					m_pD3DSprite->Begin(D3DXSPRITE_OBJECTSPACE);
					tempText = (textStruct*)tempInfo->asset;
					m_pD3DDevice->SetTransform(D3DTS_WORLD,&tempInfo->matrix);
					tempRect.bottom = tempText->rect.bottom;
					tempRect.top = tempText->rect.top;
					tempRect.left = tempText->rect.left;
					tempRect.right = tempText->rect.right;
					m_pD3DFont->DrawTextA(m_pD3DSprite,tempText->text.c_str(),-1,&tempRect,tempText->flags,tempText->color);
					m_pD3DSprite->End();
				}

				//billboarded text
				if(tempInfo->type == bbText) {
					m_pD3DDevice->GetTransform(D3DTS_VIEW,&transMat);
					tempText = (textStruct*)tempInfo->asset;
					m_pD3DSprite->SetWorldViewLH(&tempInfo->matrix,&transMat);
					m_pD3DSprite->Begin(D3DXSPRITE_BILLBOARD);
					tempRect.bottom = tempText->rect.bottom;
					tempRect.top = tempText->rect.top;
					tempRect.left = tempText->rect.left;
					tempRect.right = tempText->rect.right;
					m_pD3DFont->DrawTextA(m_pD3DSprite,tempText->text.c_str(),-1,&tempRect,tempText->flags,tempText->color);
					m_pD3DSprite->End();
				}
				//m_pD3DSprite->End();
			}
			renSize = screenRen.getSize();

			//screen space
			for(int i = 0; i < renSize; ++i) {

				D3DXMatrixIdentity(&transMat);
				m_pD3DSprite->SetTransform(&transMat);
				//reset trans
				m_pD3DDevice->SetTransform(D3DTS_WORLD,&transMat);
				tempInfo = screenRen.get(i);
				//check asset type
				//screen space sprite
				if(tempInfo->type == screenSprite) {
					m_pD3DSprite->Begin(NULL);
					sprStruct = (spriteStruct*)tempInfo->asset;
					m_pD3DSprite->SetTransform(&tempInfo->matrix);
					m_pD3DSprite->Draw(sprStruct->image->objTex,&sprStruct->rec,&sprStruct->center,0,sprStruct->color);
					m_pD3DSprite->End();
				}
				//line
				if(tempInfo->type == line) {
					tempLine = (lineStruct*)tempInfo->asset;
					m_pD3DLine->SetWidth(tempLine->width);

					m_pD3DLine->Begin();
					m_pD3DLine->Draw(tempLine->vec,tempLine->vecCount,tempLine->color);
					m_pD3DLine->End();
				}
				//text
				if(tempInfo->type == text) {
					m_pD3DSprite->Begin(NULL);
					m_pD3DSprite->SetTransform(&tempInfo->matrix);
					tempText = (textStruct*)tempInfo->asset;
					tempRect.bottom = tempText->rect.bottom * _height;
					tempRect.top = tempText->rect.top * _height;
					tempRect.left = tempText->rect.left * _width;
					tempRect.right = tempText->rect.right * _width;
					m_pD3DFont->DrawTextA(m_pD3DSprite,tempText->text.c_str(),-1,&tempRect,tempText->flags,tempText->color);
					m_pD3DSprite->End();
				}
			}

			m_pD3DDevice->EndScene();
		}
		if(!SUCCEEDED(m_pD3DDevice->Present(NULL,NULL,NULL,NULL)))
			_lost = true;
	} else {
		_lost = true;
	}
	ren.clear();
	screenRen.clear();
}

//call if device is lost or you update the options
bool Graphics::restart(HWND hWnd,HINSTANCE hInst) {
	/*shutdown();
	init(hWnd,hInst,!_fullScreen);
	if(m_pD3DDevice) {
	for(int i = 0; i < MAX_LIGHTS; ++i) {
	if(_lights[i].Range != 0) {
	m_pD3DDevice->SetLight(i,&_lights[i]);
	m_pD3DDevice->LightEnable(i,true);
	}
	}
	return true;
	}
	return false;*/

	HRESULT result = m_pD3DDevice->TestCooperativeLevel();
	if(result == D3DERR_DEVICENOTRESET) {
		m_pD3DLine->OnLostDevice();
		m_pD3DSprite->OnLostDevice();
		//m_pD3DFont->OnLostDevice();
		if(m_pD3DDevice->Reset(&D3Dpp) == D3D_OK) {
			m_pD3DLine->OnResetDevice();
			m_pD3DSprite->OnResetDevice();
			//m_pD3DFont->OnResetDevice();
			m_pD3DLine->SetAntialias(true);

			m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
			m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(60, 60, 60));
			m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
			m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			_lost = false;
			return true;
		}
	} else if(result == D3DERR_DRIVERINTERNALERROR) {
		MessageBox(0, "A driver error has occured", 0, MB_OK);
		PostQuitMessage(0);
	}
	return false;
}

void Graphics::shutdown() {
	//release all objects in revers order that they were created
	//check to see if it exists befor calling release
	if(m_pD3DFont != 0) {
		m_pD3DFont->Release();
		m_pD3DFont = 0;
	}

	if(m_pD3DSprite != 0) {
		m_pD3DSprite->Release();
		m_pD3DSprite = 0;
	}

	if(m_pD3DLine != 0) {
		m_pD3DLine->Release();
		m_pD3DLine = 0;
#ifdef FONTPATH
		RemoveFontResource(FONTPATH);
#endif
	}

	if(m_pD3DDevice != 0) {
		m_pD3DDevice->Release();
		m_pD3DDevice = 0;
	}

	if(m_pD3DObject != 0) {
		m_pD3DObject->Release();
		m_pD3DObject = 0;
	}
}

bool Graphics::isLost() {
	return _lost;
}

void Graphics::setResolution(int height, int width) {
	_height = height;
	_width = width;
}

void Graphics::setTextSize(int height,int width) {
	_fontH = height;
	_fontW = width;
}

void Graphics::loadImage(LPCSTR FileName,UINT Width,UINT Height,UINT MipLevels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,DWORD Filter,DWORD MipFilter,D3DCOLOR ColorKey,D3DXIMAGE_INFO *SrcInfo,PALETTEENTRY *pPalette,LPDIRECT3DTEXTURE9 *Tex) {
	D3DXCreateTextureFromFileEx(m_pD3DDevice,FileName,Width,Height,MipLevels,Usage,Format,Pool,Filter,MipFilter,ColorKey,SrcInfo,pPalette,Tex);
}

void Graphics::loadImage(LPCSTR FileName,imageAsset* asset) {
	asset->mask = D3DCOLOR_XRGB(255,255,255);
	D3DXCreateTextureFromFileEx(m_pD3DDevice,FileName,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,asset->mask,&asset->texInfo,0,&asset->objTex);
}

void Graphics::loadImage(LPCSTR FileName, D3DXCOLOR mask, imageAsset* asset) {
	asset->mask = mask;

	//TODO REMOVE THIS!!
	//testing loading from memory
	std::ifstream file;
	int length = 0;
	char* buffer;
	file.open(FileName,std::ifstream::binary);
	//get file size
	file.seekg(0,file.end);
	length = file.tellg();
	file.seekg(0,file.beg);
	//create buffer
	buffer = new char[length];
	//read file
	file.read(buffer,length);
	//
	loadImage(buffer,(UINT)length,mask,asset);
	file.close();
	delete [] buffer;

	//D3DXCreateTextureFromFileEx(m_pD3DDevice,FileName,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,asset->mask,&asset->texInfo,0,&asset->objTex);
}

void Graphics::loadImage(void* &source, UINT size, UINT Width,UINT Height,UINT MipLevels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,DWORD Filter,DWORD MipFilter,D3DCOLOR ColorKey,D3DXIMAGE_INFO *SrcInfo,PALETTEENTRY *pPalette,LPDIRECT3DTEXTURE9 *Tex) {
	D3DXCreateTextureFromFileInMemoryEx(m_pD3DDevice, source, size, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, SrcInfo, pPalette, Tex); 
}

void Graphics::loadImage(LPCVOID source, UINT size, D3DXCOLOR mask, imageAsset* asset) {
	asset->mask = mask;
	D3DXCreateTextureFromFileInMemoryEx(m_pD3DDevice, source, size, 0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,asset->mask,&asset->texInfo,0,&asset->objTex);
}

void Graphics::setLight(int num,D3DLIGHT9& light,bool active) {
	if(num >= 0 && num < MAX_LIGHTS) {
		_lights[num] = light;
		if(m_pD3DDevice) {
			m_pD3DDevice->SetLight(num,&_lights[num]);
			m_pD3DDevice->LightEnable(num,active);
		}
	}
}

void Graphics::setClearColor(D3DXCOLOR color) {
	clearColor = color;
}

D3DLIGHT9* Graphics::getLight(int num) {
	if(num >= 0 && num < MAX_LIGHTS)
		return &_lights[num];
	return 0;
}

void Graphics::loadXModel(LPCSTR fileName, modelAsset* asset) {
	D3DXLoadMeshFromX(fileName, // file to load
		D3DXMESH_MANAGED, // flags for memory management
		m_pD3DDevice, // DXD3D device
		&asset->adj, // adjacency buffer for mesh
		&asset->matPoint, // materials in mesh
		0, // effects (such as lighting)
		// similar to those in a .fx file
		&asset->numMats, // count of materials in file
		&asset->mesh); // reference to the mesh object
	asset->textures = new imageAsset*[asset->numMats];
	asset->mats = (D3DXMATERIAL*)asset->matPoint->GetBufferPointer();
}

void Graphics::loadXModel(void* &source, UINT size, modelAsset* asset) {
	D3DXLoadMeshFromXInMemory( source, size, 
		D3DXMESH_MANAGED, // flags for memory management
		m_pD3DDevice, // DXD3D device
		&asset->adj, // adjacency buffer for mesh
		&asset->matPoint, // materials in mesh
		0, // effects (such as lighting)
		// similar to those in a .fx file
		&asset->numMats, // count of materials in file
		&asset->mesh); // reference to the mesh object
	asset->textures = new imageAsset*[asset->numMats];
	asset->mats = (D3DXMATERIAL*)asset->matPoint->GetBufferPointer();
}

void Graphics::createCube(LPCSTR FileName, cubeAsset* asset) {

}

void Graphics::createCube(cubeAsset* asset,float top, float bottom, float front, float back, float left, float right) {
	asset->bottom = bottom;
	asset->top = top;
	asset->left = left;
	asset->right = right;
	asset->back = back;
	asset->front = front;
	Vertex m_cubeVerts[24];
	WORD m_cubeIndices[36];
	// Load vertex info, listed per cube face quads
	// back
	m_cubeVerts[0].position = D3DXVECTOR3(left, bottom, back); 
	m_cubeVerts[1].position = D3DXVECTOR3(left, top, back); 
	m_cubeVerts[2].position = D3DXVECTOR3(right, top, back); 
	m_cubeVerts[3].position = D3DXVECTOR3(right, bottom, back); 
	D3DXVec3Normalize(&m_cubeVerts[0].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[1].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[2].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[3].normal, &D3DXVECTOR3(0.0f, 0.0f, -1.0f)); 
	m_cubeVerts[1].uv = D3DXVECTOR2(0.25f, 0.0f); 
	m_cubeVerts[2].uv = D3DXVECTOR2(0.5f, 0.0f); 
	m_cubeVerts[3].uv = D3DXVECTOR2(0.5f, 0.875f); 
	m_cubeVerts[0].uv = D3DXVECTOR2(0.25f, 0.875f);

	// front 
	m_cubeVerts[4].position = D3DXVECTOR3(left, bottom, front); 
	m_cubeVerts[5].position = D3DXVECTOR3(right, bottom, front); 
	m_cubeVerts[6].position = D3DXVECTOR3(right, top, front); 
	m_cubeVerts[7].position = D3DXVECTOR3(left, top, front); 
	D3DXVec3Normalize(&m_cubeVerts[4].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[5].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[6].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[7].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f)); 
	m_cubeVerts[7].uv = D3DXVECTOR2(0.25f, 0.0f); 
	m_cubeVerts[4].uv = D3DXVECTOR2(0.25f, 0.875f); 
	m_cubeVerts[5].uv = D3DXVECTOR2(0.0f, 0.875f); 
	m_cubeVerts[6].uv = D3DXVECTOR2(0.0f, 0.0f);

	// top
	m_cubeVerts[8].position = D3DXVECTOR3(left, top, back); 
	m_cubeVerts[9].position = D3DXVECTOR3(left, top, front); 
	m_cubeVerts[10].position = D3DXVECTOR3(right, top, front); 
	m_cubeVerts[11].position = D3DXVECTOR3(right, top, back); 
	D3DXVec3Normalize(&m_cubeVerts[8].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[9].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[10].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[11].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); 
	m_cubeVerts[8].uv = D3DXVECTOR2(0.0f, 1.0f); 
	m_cubeVerts[9].uv = D3DXVECTOR2(0.0f, 0.875f); 
	m_cubeVerts[10].uv = D3DXVECTOR2(0.25f, 0.875f); 
	m_cubeVerts[11].uv = D3DXVECTOR2(0.25f, 1.0f);

	// bottom
	m_cubeVerts[12].position = D3DXVECTOR3(left, bottom, back); 
	m_cubeVerts[13].position = D3DXVECTOR3(right, bottom, back); 
	m_cubeVerts[14].position = D3DXVECTOR3(right, bottom,  front); 
	m_cubeVerts[15].position = D3DXVECTOR3(left, bottom, front); 
	D3DXVec3Normalize(&m_cubeVerts[12].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[13].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[14].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[15].normal, &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	m_cubeVerts[14].uv = D3DXVECTOR2(0.5f, 1.0f); 
	m_cubeVerts[15].uv = D3DXVECTOR2(0.25f, 1.0f); 
	m_cubeVerts[12].uv = D3DXVECTOR2(0.25f, 0.875f); 
	m_cubeVerts[13].uv = D3DXVECTOR2(0.5f, 0.875f);

	// Left 
	m_cubeVerts[16].position = D3DXVECTOR3(left, bottom, front); 
	m_cubeVerts[17].position = D3DXVECTOR3(left, top, front); 
	m_cubeVerts[18].position = D3DXVECTOR3(left, top, back); 
	m_cubeVerts[19].position = D3DXVECTOR3(left, bottom, back); 
	D3DXVec3Normalize(&m_cubeVerts[16].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[17].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[18].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[19].normal, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f)); 
	m_cubeVerts[19].uv = D3DXVECTOR2(0.75f, 0.875f); 
	m_cubeVerts[16].uv = D3DXVECTOR2(0.5f, 0.875f); 
	m_cubeVerts[17].uv = D3DXVECTOR2(0.5f, 0.0f); 
	m_cubeVerts[18].uv = D3DXVECTOR2(0.75f, 0.0f);

	// Right 
	m_cubeVerts[20].position = D3DXVECTOR3(right, bottom, back); 
	m_cubeVerts[21].position = D3DXVECTOR3(right, top, back); 
	m_cubeVerts[22].position = D3DXVECTOR3(right, top, front); 
	m_cubeVerts[23].position = D3DXVECTOR3(right, bottom, front); 
	D3DXVec3Normalize(&m_cubeVerts[20].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[21].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[22].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)); 
	D3DXVec3Normalize(&m_cubeVerts[23].normal, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)); 
	m_cubeVerts[21].uv = D3DXVECTOR2(0.75f, 0.0f); 
	m_cubeVerts[22].uv = D3DXVECTOR2(1.0f, 0.0f); 
	m_cubeVerts[23].uv = D3DXVECTOR2(1.0f, 0.875f); 
	m_cubeVerts[20].uv = D3DXVECTOR2(0.75f, 0.875f);

	// Load index info, refers into index into verts array to compose triangles 
	// Note: A clockwise winding order of verts will show the front face.

	// Front 
	m_cubeIndices[0] = 0; m_cubeIndices[1] = 1; m_cubeIndices[2] = 2; // Triangle 0 
	m_cubeIndices[3] = 0; m_cubeIndices[4] = 2; m_cubeIndices[5] = 3; // Triangle 1

	// Back 
	m_cubeIndices[6] = 4; m_cubeIndices[7] = 5; m_cubeIndices[8] = 6; // Triangle 2 
	m_cubeIndices[9] = 4; m_cubeIndices[10] = 6; m_cubeIndices[11] = 7; // Triangle 3

	// Top 
	m_cubeIndices[12] = 8; m_cubeIndices[13] = 9; m_cubeIndices[14] = 10; // Triangle 4 
	m_cubeIndices[15] = 8; m_cubeIndices[16] = 10; m_cubeIndices[17] = 11; // Triangle 5

	// Bottom 
	m_cubeIndices[18] = 12; m_cubeIndices[19] = 13; m_cubeIndices[20] = 14; // Triangle 6 
	m_cubeIndices[21] = 12; m_cubeIndices[22] = 14; m_cubeIndices[23] = 15; // Triangle 7

	// Left 
	m_cubeIndices[24] = 16; m_cubeIndices[25] = 17; m_cubeIndices[26] = 18; // Triangle 8 
	m_cubeIndices[27] = 16; m_cubeIndices[28] = 18; m_cubeIndices[29] = 19; // Triangle 9

	// Right 
	m_cubeIndices[30] = 20; m_cubeIndices[31] = 21; m_cubeIndices[32] = 22; // Triangle 10 
	m_cubeIndices[33] = 20; m_cubeIndices[34] = 22; m_cubeIndices[35] = 23; // Triangle 11
	//create decloration
	D3DVERTEXELEMENT9 decl[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	m_pD3DDevice->CreateVertexDeclaration(decl, &asset->objDec);
	//create memory
	m_pD3DDevice->CreateVertexBuffer(24*sizeof(Vertex),D3DUSAGE_WRITEONLY,0,D3DPOOL_MANAGED,&asset->obj,0);
	m_pD3DDevice->CreateIndexBuffer(36*sizeof(WORD),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&asset->objInd,0);
	//copy info to memory
	VOID* pVertices; VOID* pInd;
	asset->obj->Lock(0,0,&pVertices,0);
	memcpy(pVertices,m_cubeVerts,24*sizeof(Vertex));
	asset->obj->Unlock();

	asset->objInd->Lock(0,0,&pInd,0);
	memcpy(pInd,m_cubeIndices,36*sizeof(WORD));
	asset->objInd->Unlock();
}

void Graphics::updateCam(const camera* a_cam) {
	cam = *a_cam;
}
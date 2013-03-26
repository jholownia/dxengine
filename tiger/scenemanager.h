/*
================================
 scenemanager.h
 
 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <Windows.h>

#include "d3dmanager.h"
#include "camera.h"
#include "object3d.h"
#include "shadermanager.h"
#include "light.h"
#include "text.h"
#include "objectlist.h"
#include "frustum.h"
#include "terrain.h"
#include "quadtree.h"
#include "config.h"
#include "rendertexture.h"
#include "debugwindow.h"


/*
================================
 
 SceneManager
 
 Main class for rendering the scene.
 Contains pointers to all the objects in the scene.

================================
*/
class SceneManager : Uncopyable
{
public:
	SceneManager(void);
	SceneManager(SceneManager const&);
	~SceneManager(void);

	bool init(int, int, HWND);
	void shutdown();
	bool frame(int, int, float, int, int, D3DXVECTOR3, float, float, float, float, float, float, float, bool);
	bool render();

private:
	D3DManager* d3d_;
	Camera* camera_;
	Object3D* tiger_;
	Object3D* wingr_;
	Object3D* wingl_;
	Object3D* skyBox_;
	Object3D* ball_;
	Object3D* floor_;
	Object3D* water_;
	Terrain* terrain_;
	ShaderManager* shaderManager_;
	Light* light_;
	Light* skyboxLight_;
	Light* terrainLight_;
	Text* text_;
	ObjectList* ballsList_;
	Frustum* frustum_;
	Quadtree* quadtree_;
	RenderTexture* renderTexture_;
	RenderTexture* refractionTexture_;
	RenderTexture* reflectionTexture_;
	DebugWindow* debugWindow_;
	
	float waterHeight_;
	float waterTranslation;
	
	void renderToTexture();
	void renderRefractionToTexture();
	void renderReflectionToTexture();
	void renderScene();
};


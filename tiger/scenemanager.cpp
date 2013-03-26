/*
================================
 scenemanager.cpp

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "scenemanager.h"

/*
================
 SceneManager::SceneManager
================
*/
SceneManager::SceneManager(void) :
	d3d_           (NULL),
	camera_        (NULL),
	tiger_         (NULL),
	wingr_         (NULL),
	wingl_         (NULL),
	skyBox_        (NULL),
	ball_          (NULL),
	floor_         (NULL),
	water_         (NULL),
	terrain_       (NULL),
	shaderManager_ (NULL),
	light_         (NULL),
	skyboxLight_   (NULL),
	terrainLight_  (NULL),
	text_          (NULL),
	ballsList_     (NULL),
	frustum_       (NULL),
	quadtree_      (NULL),
	renderTexture_ (NULL),
	reflectionTexture_ (NULL),
	refractionTexture_ (NULL),
	debugWindow_   (NULL)
{

}

/*
================
 SceneManager::~SceneManager
================
*/
SceneManager::~SceneManager(void)
{

}

/*
================
 SceneManager::init

 All the objects are initialized here.
================
*/
bool SceneManager::init( int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	d3d_ = new D3DManager;

	if (!d3d_)
	{
		return false;
	}

	result = d3d_->init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	if (!result)
	{
		MessageBox(hwnd, L"Error initializing Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create camera
	camera_ = new Camera;
	camera_->setPosition(0.0f, 0.0f, -40.0f);
	camera_->render();
	D3DXMATRIX baseViewMatrix;
	camera_->getViewMatrix(baseViewMatrix);

	// Create text
	text_ = new Text;
	result = text_->init(d3d_->getDevice(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize text object.", L"Error", MB_OK);
		return false;
	}
	
	// Create Tiger
	tiger_ = new Object3D;

	result = tiger_->init(d3d_->getDevice(), "data/tiger5.obj", L"data/bengal_tiger.jpg");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Object3D (tiger).", L"Error", MB_OK);
		return false;
	}

	// Create right wing
	wingr_ = new Object3D;
	result = wingr_->init(d3d_->getDevice(), "data/wingl.obj", L"data/wing.jpg");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Object3D (right wing).", L"Error", MB_OK);
		return false;
	}

	// Create left wing
	wingl_ = new Object3D;
	result = wingl_->init(d3d_->getDevice(), "data/wingr.obj", L"data/wing.jpg" );
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Object3D (left wing).", L"Error", MB_OK);
		return false;
	}

	// Create SkyBox
	skyBox_ = new Object3D;
	result = skyBox_->init(d3d_->getDevice(), "data/skybox14.obj", L"data/skybox2.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Object3D (skybox).", L"Error", MB_OK);
		return false;
	}

	// Create ball
	ball_ = new Object3D;
	result = ball_->init(d3d_->getDevice(), "data/ball.obj", L"data/ball.jpg");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Object3D (ball).", L"Error", MB_OK);
		return false;
	}

	// Create floor
	floor_ = new Object3D;
	result = floor_->init(d3d_->getDevice(), "data/floor.txt", L"data/floor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Object3D (floor).", L"Error", MB_OK);
		return false;
	}

	// Create water
	water_ = new Object3D;
	result = water_->init(d3d_->getDevice(), "data/water.txt", L"data/water01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Object3D (water).", L"Error", MB_OK);
		return false;
	}
	
	// Create terrain	
	terrain_ = new Terrain;
	result = terrain_->init(d3d_->getDevice(), "data/terrain/heightmap01.bmp", L"data/terrain/dirt01.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Terrain.", L"Error", MB_OK);
		return false;
	}
			
	// Create shader manager
	shaderManager_ = new ShaderManager;
	
	result = shaderManager_->init(d3d_->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize shader manager.", L"Error", MB_OK);
		return false;
	}

	// Balls container
	ballsList_ = new ObjectList;

	// Light
	light_ = new Light;
	light_->setAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
	light_->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	light_->setDirection(0.65f, -0.15f, -0.75f);
	light_->setLookAt(0.0f, 0.0f, 0.0f);
	light_->generateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	// Skybox light
	skyboxLight_ = new Light;
	skyboxLight_->setAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	skyboxLight_->setDiffuseColor(0.0f, 0.0f, 0.0f, 0.0f);
	skyboxLight_->setLookAt(0.0f, 0.0f, 0.0f);
	skyboxLight_->setDirection(0.0f, 0.0f, 0.0f);

	// Terrain light
	terrainLight_ = new Light;
	terrainLight_->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	terrainLight_->setDiffuseColor(1.0f, 0.35f, 0.0f, 1.0f);
	terrainLight_->setLookAt(0.65f, -0.15f, -0.75f);
	terrainLight_->setDirection(0.65f, -0.15f, -0.75f);
	
	// Frustum
	frustum_ = new Frustum;	
	
	// Quadtree
	quadtree_ = new Quadtree;
	result = quadtree_->init(terrain_, d3d_->getDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the quad tree.", L"Error", MB_OK);
		return false;
	}

	// Render texture
	renderTexture_ = new RenderTexture;
	result = renderTexture_->init(d3d_->getDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Render Texture object.", L"Error", MB_OK);
		return false;
	}

	// Render texture
	reflectionTexture_ = new RenderTexture;
	result = reflectionTexture_->init(d3d_->getDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Render Texture object.", L"Error", MB_OK);
		return false;
	}

	// Render texture
	refractionTexture_ = new RenderTexture;
	result = refractionTexture_->init(d3d_->getDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Render Texture object.", L"Error", MB_OK);
		return false;
	}

	// Debug window
	debugWindow_ = new DebugWindow;
	result = debugWindow_->init(d3d_->getDevice(), screenWidth, screenHeight, 150, 150);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Debug Window.", L"Error", MB_OK);
		return false;
	}

	// Set water height and initialize translation
	waterHeight_ = 2.75f;
	waterTranslation = 0.0f;
	
	return true;
}

/*
================
 SceneManager::shutdown
================
*/
void SceneManager::shutdown()
{
	if (debugWindow_)
	{
		debugWindow_->shutdown();
		delete debugWindow_;
		debugWindow_ = NULL;
	}

	if (reflectionTexture_)
	{
		reflectionTexture_->shutdown();
		delete reflectionTexture_;
		reflectionTexture_ = NULL;
	}

	if (refractionTexture_)
	{
		refractionTexture_->shutdown();
		delete refractionTexture_;
		refractionTexture_ = NULL;
	}
	
	if (renderTexture_)
	{
		renderTexture_->shutdown();
		delete renderTexture_;
		renderTexture_ = NULL;
	}

	if (quadtree_)
	{
		quadtree_->shutdown();
		delete quadtree_;
		quadtree_ = NULL;
	}
		
	if (frustum_)
	{
		delete frustum_;
		frustum_ = NULL;
	}
		
	if (text_)
	{
		text_->shutdown();
		delete text_;
		text_ = NULL;
	}
	
	delete ballsList_;
	ballsList_ = NULL;

	if (light_)
	{
		delete light_;
		light_ = NULL;
	}
	
	if (shaderManager_)
	{
		shaderManager_->shutdown();
		delete shaderManager_;
		shaderManager_ = NULL;
	}

	if (terrain_)
	{
		terrain_->shutdown();
		delete terrain_;
		terrain_ = NULL;
	}

	if (water_)
	{
		water_->shutdown();
		delete water_;
		water_ = NULL;
	}

	if (floor_)
	{
		floor_->shutdown();
		delete floor_;
		floor_ = NULL;
	}

	if (ball_)
	{
		ball_->shutdown();
		delete ball_;
		ball_ = NULL;
	}

	if (skyBox_)
	{
		skyBox_->shutdown();
		delete skyBox_;
		skyBox_ = NULL;
	}

	if (wingl_)
	{
		wingl_->shutdown();
		delete wingl_;
		wingl_ = NULL;
	}

	if (wingr_)
	{
		wingr_->shutdown();
		delete wingr_;
		wingr_ = NULL;
	}

	if (tiger_)
	{
		tiger_->shutdown();
		delete tiger_;
		tiger_ = NULL;
	}
	
	delete camera_;
	camera_ = NULL;
	
	if (d3d_)
	{
		d3d_->shutdown();
		delete d3d_;
		d3d_ = NULL;
	}
}

/*
================
 SceneManager::frame

 This method is called every frame from App object
 and sets up all the variables before rendering.
================
*/
bool SceneManager::frame(int mouseX, int mouseY, float frameTime, int fps, int cpuUsage, D3DXVECTOR3 cameraRotation, float cameraX, float cameraY, float cameraZ, float tigerRotation, float tigerX, float tigerY, float tigerZ, bool spawnBall)
{	
	// text_->setMousePosition(mouseX, mouseY);

	text_->setFps(fps);
	text_->setCpu(cpuUsage);
	
	camera_->setPosition(cameraX, cameraY, cameraZ);	
	camera_->setRotation(cameraRotation.x, cameraRotation.y, cameraRotation.z);

	// Set skybox position to this of camera but lower
	skyBox_->setPosition(cameraX, 510, cameraZ);

	// tiger's legs are very weak so he won't be able to land
	
	if (quadtree_->getHeightAtPosition(tigerX, tigerZ, tigerY))
	{
		tigerY += 2.7f;
	}
			
	tiger_->setPosition(tigerX, tigerY, tigerZ);	
	tiger_->setRotation(0.0f, tigerRotation, 0.0f);
	
	// Spawn ball
	if (spawnBall)
	{
		ballsList_->addObject(tigerX, tigerY, tigerZ, tigerRotation);
	}
	
	// Update physics	
	ballsList_->frame(frameTime, quadtree_);

	// Update water position
	waterTranslation += 0.001f;
	if (waterTranslation > 1.0f)
	{
		waterTranslation -= 1.0f;
	}

	// Light position
	static float lightPositionX = -5.0f;
	lightPositionX += 0.05f;

	if (lightPositionX > 5.0f)
	{
		lightPositionX = -5.0f;
	}

	light_->setPosition(lightPositionX, 8.0f, -5.0f);
	
	return render();
}

/*
================
 SceneManager::render

 All rendering takes place here
================
*/
bool SceneManager::render()
{
	renderToTexture();

	renderRefractionToTexture();

	renderReflectionToTexture();

	renderScene();

	return true;
	
	/*
	// Render to texture first
	renderToTexture();

	// Clear buffers
	d3d_->beginScene(0.0f, 0.0f, 0.0f, 1.0f);
	
	// Render the scene to back buffer
	renderScene();
		
	
	// Render 2D window displaying the RenderTexture
	
	d3d_->turnZBufferOff();

	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;

	d3d_->getWorldMatrix(worldMatrix);
	camera_->getViewMatrix(viewMatrix);
	d3d_->getOrthoMatrix(orthoMatrix);

	bool result = debugWindow_->render(d3d_->getDevice(), 150, 50);
	if (!result)
	{
		return false;
	}

	shaderManager_->renderTransparentShader(d3d_->getDevice(), debugWindow_->getIndexCount(), worldMatrix, viewMatrix, orthoMatrix, renderTexture_->getShaderResourceView(), 1.0f);
	
	d3d_->turnZBufferOn();
	

	// Present scene to the screen
	d3d_->endScene();

	return true;
	*/
}

/*
================
 SceneManager::renderToTexture
================
*/
void SceneManager::renderToTexture()
{
	renderTexture_->setRenderTarget(d3d_->getDevice(), d3d_->getDepthStencilView());
	renderTexture_->clearRenderTarget(d3d_->getDevice(), d3d_->getDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);
	
	// renderScene();
	//------------------------------------------------------------------	
	camera_->renderReflection(-1.5f);
	camera_->render();
	
	// Get matrices
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;	
	D3DXMATRIX orthoMatrix;

	viewMatrix = camera_->getReflectionViewMatrix();	
	d3d_->getProjectionMatrix(projectionMatrix);
	d3d_->getOrthoMatrix(orthoMatrix);

	// Create frustum based on view and projection matrices
	frustum_->createFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// Move the tiger
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationY(&rotationMatrix, tiger_->getRotation().y);	
	D3DXMATRIX translationMatrix;
	D3DXMatrixTranslation(&translationMatrix, tiger_->getPosition().x, tiger_->getPosition().y, tiger_->getPosition().z);

	// Wing angle for wings animation
	float wingAngle = (float) sin((double) timeGetTime() / 100);

	// Render the tiger

	// This is for tiger moving up and down as he flaps his wings
	float tigerShift = wingAngle / 10;
	D3DXMATRIX shiftMatrix;
	D3DXMatrixTranslation(&shiftMatrix, 0.0f, tigerShift, 0.0f);

	D3DXMATRIX worldMatrix;
	d3d_->getWorldMatrix(worldMatrix);

	D3DXMATRIX scaleUp;	
	D3DXMatrixScaling(&scaleUp, 0.2f, 0.2f, 0.2f);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scaleUp);

	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translationMatrix);

	worldMatrix *= shiftMatrix;

	// D3DXMATRIX moveUp;	
	// D3DXMatrixTranslation(&moveUp, 0, 10, 0);
	// worldMatrix *= moveUp;
	
	// Store tiger matrix for the wings
	D3DXMATRIX tigerMatrix = worldMatrix;

	// Render
	tiger_->render(d3d_->getDevice());
	shaderManager_->renderLightShader(d3d_->getDevice(), tiger_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, tiger_->getTexture(), light_->getDirection(), light_->getAmbientColor(), light_->getDiffuseColor(), wingAngle, Shader::DEFAULT);

	// transparency test
	// shaderManager_->renderTransparentShader(d3d_->getDevice(), tiger_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, tiger_->getTexture(), 0.5f);

	// Render right wing
	d3d_->getWorldMatrix(worldMatrix);		
	D3DXMatrixTranslation(&translationMatrix, 4.5f, 2.0f, -1.5f);	
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translationMatrix);			

	// Place the wing relative to the tiger
	worldMatrix *= tigerMatrix;

	wingr_->render(d3d_->getDevice());
	shaderManager_->renderLightShader(d3d_->getDevice(), wingr_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, wingr_->getTexture(), light_->getDirection(), light_->getAmbientColor(), light_->getDiffuseColor(), wingAngle, Shader::RWING);

	// Render left wing
	d3d_->getWorldMatrix(worldMatrix);	
	D3DXMatrixTranslation(&translationMatrix, -7.0f, 2.0f, -1.5f);	
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translationMatrix);

	// Place the wing relative to the tiger
	worldMatrix *= tigerMatrix;	

	wingl_->render(d3d_->getDevice());	
	shaderManager_->renderLightShader(d3d_->getDevice(), wingl_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, wingl_->getTexture(), light_->getDirection(), light_->getAmbientColor(), light_->getDiffuseColor(), wingAngle, Shader::LWING);
	
	// Render skybox
	d3d_->getWorldMatrix(worldMatrix);

	// For some reason the skybox is upside down (possibly problem with the loader)
	// so we need to rotate it.
	D3DXMatrixRotationZ(&rotationMatrix, (float) D3DX_PI);
	worldMatrix *= rotationMatrix;

	// Move it down, so the camera is in the center
	D3DXMatrixTranslation(&translationMatrix, skyBox_->getPosition().x, skyBox_->getPosition().y, skyBox_->getPosition().z);
	worldMatrix *= translationMatrix;

	skyBox_->render(d3d_->getDevice());
	shaderManager_->renderLightShader(d3d_->getDevice(), skyBox_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, skyBox_->getTexture(), skyboxLight_->getDirection(), skyboxLight_->getAmbientColor(), skyboxLight_->getDiffuseColor(), wingAngle, Shader::DEFAULT);


	//-------------------------------------------------------------------
	d3d_->setBackBufferRenderTarget();
	
}

/*
================
 SceneManager::renderScene

 Renders the scene to current render target.
================
*/
void SceneManager::renderScene()
{
	// Render camera
	d3d_->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	camera_->render();

	// Get matrices
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;	
	D3DXMATRIX orthoMatrix;

	camera_->getViewMatrix(viewMatrix);	
	d3d_->getProjectionMatrix(projectionMatrix);
	d3d_->getOrthoMatrix(orthoMatrix);

	// Create frustum based on view and projection matrices
	frustum_->createFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// Move the tiger
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationY(&rotationMatrix, tiger_->getRotation().y);	
	D3DXMATRIX translationMatrix;
	D3DXMatrixTranslation(&translationMatrix, tiger_->getPosition().x, tiger_->getPosition().y, tiger_->getPosition().z);

	// Wing angle for wings animation
	float wingAngle = (float) sin((double) timeGetTime() / 100);

	// Render the tiger

	// This is for tiger moving up and down as he flaps his wings
	float tigerShift = wingAngle / 10.0f;
	D3DXMATRIX shiftMatrix;
	D3DXMatrixTranslation(&shiftMatrix, 0.0f, tigerShift, 0.0f);

	D3DXMATRIX worldMatrix;
	d3d_->getWorldMatrix(worldMatrix);

	D3DXMATRIX scaleUp;	
	D3DXMatrixScaling(&scaleUp, 0.2f, 0.2f, 0.2f);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scaleUp);

	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translationMatrix);

	worldMatrix *= shiftMatrix;

	// Store tiger matrix for the wings
	D3DXMATRIX tigerMatrix = worldMatrix;

	// Render
	tiger_->render(d3d_->getDevice());
	shaderManager_->renderLightShader(d3d_->getDevice(), tiger_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, tiger_->getTexture(), light_->getDirection(), light_->getAmbientColor(), light_->getDiffuseColor(), wingAngle, Shader::DEFAULT);

	// transparency test
	// shaderManager_->renderTransparentShader(d3d_->getDevice(), tiger_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, tiger_->getTexture(), 0.5f);

	// Render right wing
	d3d_->getWorldMatrix(worldMatrix);		
	D3DXMatrixTranslation(&translationMatrix, 4.5f, 2.0f, -1.5f);	
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translationMatrix);			

	// Place the wing relative to the tiger
	worldMatrix *= tigerMatrix;

	wingr_->render(d3d_->getDevice());
	shaderManager_->renderLightShader(d3d_->getDevice(), wingr_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, wingr_->getTexture(), light_->getDirection(), light_->getAmbientColor(), light_->getDiffuseColor(), wingAngle, Shader::RWING);

	// Render left wing
	d3d_->getWorldMatrix(worldMatrix);	
	D3DXMatrixTranslation(&translationMatrix, -7.0f, 2.0f, -1.5f);	
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translationMatrix);

	// Place the wing relative to the tiger
	worldMatrix *= tigerMatrix;	

	wingl_->render(d3d_->getDevice());	
	shaderManager_->renderLightShader(d3d_->getDevice(), wingl_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, wingl_->getTexture(), light_->getDirection(), light_->getAmbientColor(), light_->getDiffuseColor(), wingAngle, Shader::LWING);

	// Render skybox
	d3d_->getWorldMatrix(worldMatrix);

	// For some reason the skybox is upside down (possibly problem with the loader)
	// so we need to rotate it.
	D3DXMatrixRotationZ(&rotationMatrix, (float) D3DX_PI);
	worldMatrix *= rotationMatrix;

	// Move it down, so the camera is in the center
	D3DXMatrixTranslation(&translationMatrix, skyBox_->getPosition().x, skyBox_->getPosition().y, skyBox_->getPosition().z);
	worldMatrix *= translationMatrix;

	skyBox_->render(d3d_->getDevice());
	shaderManager_->renderLightShader(d3d_->getDevice(), skyBox_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, skyBox_->getTexture(), skyboxLight_->getDirection(), skyboxLight_->getAmbientColor(), skyboxLight_->getDiffuseColor(), wingAngle, Shader::DEFAULT);

	// Render terrain
	d3d_->getWorldMatrix(worldMatrix);

	// Note that this only set shader parameters, the actual rendering now happens in the quadtree
	shaderManager_->renderTerrainShader(d3d_->getDevice(), terrain_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, terrainLight_->getAmbientColor(), terrainLight_->getDiffuseColor(), terrainLight_->getDirection(), terrain_->getTexture());
	quadtree_->render(frustum_, d3d_->getDevice(), shaderManager_->getTerrainShader());
	// shaderManager_->renderTransparentShader(d3d_->getDevice(), terrain_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, terrain_->getTexture(), 1.0);

	
	// Show the number of rendered polygons (currently only for the terrain, but it shows that the
	// quadtree is in fact working.
	text_->setPolyCount(quadtree_->getDrawCount());

	// Render floor
	// d3d_->getWorldMatrix(worldMatrix);
	// D3DXMatrixTranslation(&worldMatrix, 0.0f, -1.5f, 0.0f);

	// D3DXMATRIX reflectionMatrix = camera_->getReflectionViewMatrix();

	// floor_->render(d3d_->getDevice());

	// shaderManager_->renderReflectionShader(d3d_->getDevice(), floor_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, floor_->getTexture(), renderTexture_->getShaderResourceView(), reflectionMatrix);
	
	// Render balls
	int renderedCount = 0;		
	float posX, posY, posZ, radius;

	for (int i = 0; i < ballsList_->getObjectCount(); ++i)
	{
		ballsList_->getData(i, posX, posY, posZ);
		radius = 1.0f;

		if (frustum_->checkSphere(posX, posY, posZ, radius))
		{
			// Move the object to its location
			D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

			// Put the object on the graphics pipeline
			ball_->render(d3d_->getDevice());

			// Render the object
			shaderManager_->renderLightShader(d3d_->getDevice(), ball_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, ball_->getTexture(), light_->getDirection(), light_->getAmbientColor(), light_->getDiffuseColor(), wingAngle, Shader::DEFAULT);

			// Get the original world matrix
			d3d_->getWorldMatrix(worldMatrix);

			++renderedCount;
		}
	}

	// Reset the world matrix.
	d3d_->getWorldMatrix(worldMatrix);
	
	// Get the camera reflection view matrix.
	D3DXMATRIX reflectionMatrix = camera_->getReflectionViewMatrix();

	// Translate to where the water model will be rendered.
	D3DXMatrixTranslation(&worldMatrix, 100.0f, waterHeight_, 100.0f); 

	// Put the water model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	water_->render(d3d_->getDevice());

	// Render the water model using the water shader.
	shaderManager_->renderWaterShader(d3d_->getDevice(), water_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		reflectionMatrix, reflectionTexture_->getShaderResourceView(), 
		refractionTexture_->getShaderResourceView(), water_->getTexture(), waterTranslation, 0.05f);


	// Turn off the z-buffer for 2D rendering
	d3d_->turnZBufferOff();

	// Render text strings
	d3d_->getWorldMatrix(worldMatrix);
	text_->render(d3d_->getDevice(), worldMatrix, orthoMatrix);

	// Turn z-buffer back on
	d3d_->turnZBufferOn();
	
	d3d_->endScene();
}

void SceneManager::renderRefractionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Setup a clipping plane based on the height of the water to clip everything above it.
	clipPlane = D3DXVECTOR4(0.0f, -1.0f, 0.0f, waterHeight_ + 0.1f);

	// Set the render target to be the refraction render to texture.
	refractionTexture_->setRenderTarget(d3d_->getDevice(), d3d_->getDepthStencilView());

	// Clear the refraction render to texture.
	refractionTexture_->clearRenderTarget(d3d_->getDevice(), d3d_->getDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera_->render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	d3d_->getWorldMatrix(worldMatrix);
	camera_->getViewMatrix(viewMatrix);
	d3d_->getProjectionMatrix(projectionMatrix);
		
	// Render terrain using refraction shader	
	shaderManager_->renderTerrainShader(d3d_->getDevice(), terrain_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, terrainLight_->getAmbientColor(), terrainLight_->getDiffuseColor(), terrainLight_->getDirection(), terrain_->getTexture());
	quadtree_->render(frustum_, d3d_->getDevice(), shaderManager_->getTerrainShader());

	// Move the tiger
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationY(&rotationMatrix, tiger_->getRotation().y);	
	D3DXMATRIX translationMatrix;
	D3DXMatrixTranslation(&translationMatrix, tiger_->getPosition().x, tiger_->getPosition().y, tiger_->getPosition().z);

	// Wing angle for wings animation
	float wingAngle = (float) sin((double) timeGetTime() / 100);

	// Render the tiger

	// This is for tiger moving up and down as he flaps his wings
	float tigerShift = wingAngle / 10;
	D3DXMATRIX shiftMatrix;
	D3DXMatrixTranslation(&shiftMatrix, 0.0f, tigerShift, 0.0f);
		
	d3d_->getWorldMatrix(worldMatrix);

	D3DXMATRIX scaleUp;	
	D3DXMatrixScaling(&scaleUp, 0.2f, 0.2f, 0.2f);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scaleUp);

	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translationMatrix);

	worldMatrix *= shiftMatrix;

	// Store tiger matrix for the wings
	D3DXMATRIX tigerMatrix = worldMatrix;

	// Render
	tiger_->render(d3d_->getDevice());
	shaderManager_->renderRefractionShader(d3d_->getDevice(), tiger_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, tiger_->getTexture(), light_->getDirection(), light_->getAmbientColor(), light_->getDiffuseColor(), clipPlane);
	// shaderManager_->renderLightShader(d3d_->getDevice(), tiger_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, tiger_->getTexture(), light_->getDirection(), light_->getAmbientColor(), light_->getDiffuseColor(), 0, Shader::DEFAULT);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	d3d_->setBackBufferRenderTarget();
}

void SceneManager::renderReflectionToTexture()
{
	D3DXMATRIX reflectionViewMatrix, worldMatrix, projectionMatrix;
	
	// Set the render target to be the reflection render to texture.
	reflectionTexture_->setRenderTarget(d3d_->getDevice(), d3d_->getDepthStencilView());

	// Clear the reflection render to texture.
	reflectionTexture_->clearRenderTarget(d3d_->getDevice(), d3d_->getDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);	
	
	// Use the camera to render the reflection and create a reflection view matrix.
	camera_->renderReflection(waterHeight_);

	// Get the camera reflection view matrix instead of the normal view matrix.
	reflectionViewMatrix = camera_->getReflectionViewMatrix();

	// Move the tiger
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationY(&rotationMatrix, tiger_->getRotation().y);	
	D3DXMATRIX translationMatrix;
	D3DXMatrixTranslation(&translationMatrix, tiger_->getPosition().x, tiger_->getPosition().y, tiger_->getPosition().z);

	// Wing angle for wings animation
	float wingAngle = (float) sin((double) timeGetTime() / 100);

	// Render the tiger

	// This is for tiger moving up and down as he flaps his wings
	float tigerShift = wingAngle / 10;
	D3DXMATRIX shiftMatrix;
	D3DXMatrixTranslation(&shiftMatrix, 0.0f, tigerShift, 0.0f);


	// Get the world and projection matrices from the d3d object.
	d3d_->getWorldMatrix(worldMatrix);
	d3d_->getProjectionMatrix(projectionMatrix);

	

	D3DXMATRIX scaleUp;	
	D3DXMatrixScaling(&scaleUp, 0.2f, 0.2f, 0.2f);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scaleUp);


	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translationMatrix);

	worldMatrix *= shiftMatrix;

	// Store tiger matrix for the wings
	D3DXMATRIX tigerMatrix = worldMatrix;

	// Put the wall model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	tiger_->render(d3d_->getDevice());

	// Render the wall model using the light shader and the reflection view matrix.
	shaderManager_->renderLightShader(d3d_->getDevice(), tiger_->getIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
		tiger_->getTexture(), light_->getDirection(), light_->getAmbientColor(), 
		light_->getDiffuseColor(), 1, Shader::DEFAULT);

	// Render right wing
	d3d_->getWorldMatrix(worldMatrix);		
	D3DXMatrixTranslation(&translationMatrix, 4.5f, 2.0f, -1.5f);	
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translationMatrix);			

	// Place the wing relative to the tiger
	worldMatrix *= tigerMatrix;

	wingr_->render(d3d_->getDevice());
	shaderManager_->renderLightShader(d3d_->getDevice(), wingr_->getIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix, wingr_->getTexture(), light_->getDirection(), light_->getAmbientColor(), light_->getDiffuseColor(), wingAngle, Shader::RWING);

	// Render left wing
	d3d_->getWorldMatrix(worldMatrix);	
	D3DXMatrixTranslation(&translationMatrix, -7.0f, 2.0f, -1.5f);	
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translationMatrix);

	// Place the wing relative to the tiger
	worldMatrix *= tigerMatrix;	

	wingl_->render(d3d_->getDevice());	
	shaderManager_->renderLightShader(d3d_->getDevice(), wingl_->getIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix, wingl_->getTexture(), light_->getDirection(), light_->getAmbientColor(), light_->getDiffuseColor(), wingAngle, Shader::LWING);

	// Render skybox
	d3d_->getWorldMatrix(worldMatrix);

	// For some reason the skybox is upside down (possibly problem with the loader)
	// so we need to rotate it.
	D3DXMatrixRotationZ(&rotationMatrix, (float) D3DX_PI);
	worldMatrix *= rotationMatrix;

	// Move it down, so the camera is in the center
	D3DXMatrixTranslation(&translationMatrix, skyBox_->getPosition().x, skyBox_->getPosition().y, skyBox_->getPosition().z);
	worldMatrix *= translationMatrix;

	skyBox_->render(d3d_->getDevice());
	shaderManager_->renderLightShader(d3d_->getDevice(), skyBox_->getIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix, skyBox_->getTexture(), skyboxLight_->getDirection(), skyboxLight_->getAmbientColor(), skyboxLight_->getDiffuseColor(), 0, Shader::DEFAULT);
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	d3d_->setBackBufferRenderTarget();
}

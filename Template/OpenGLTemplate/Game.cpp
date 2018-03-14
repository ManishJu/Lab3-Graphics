/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 
*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "CatmullRom.h"
#include "Cube.h"
#include "Tetrahedron.h"

// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pPlanarTerrain = NULL;
	m_pFtFont = NULL;
	m_pBarrelMesh = NULL;
	m_pHorseMesh = NULL;
	m_pSphere = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pPath = NULL;
	m_pCube = NULL;
	m_pTetrahedron = NULL;
	m_pTree1 = NULL;
	m_pApple = NULL;
	m_pRabbit = NULL;
	m_pFlowersSet1 = NULL;
	m_pFLowersSet2 =NULL;
	m_pRock = NULL;
	

	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
	m_currentDistance = 0.0f;
	m_cameraSpeed = 0.05f;
	m_pPlayerPos = 0.0f;

}

// Destructor
Game::~Game() 
{ 
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pPlanarTerrain;
	delete m_pFtFont;
	delete m_pBarrelMesh;
	delete m_pHorseMesh;
	delete m_pSphere;
	delete m_pAudio;
	delete m_pPath;
	delete m_pCube;
	delete m_pTetrahedron;
	delete m_pTree1;
	delete m_pApple;
	delete m_pRabbit;
	delete m_pFlowersSet1;
	delete m_pFLowersSet2;
	delete m_pRock;
	for (auto it = m_pPlants.begin(); it != m_pPlants.end(); ++it)	delete (*it);
	m_pPlants.clear();

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{

	for (int i = 0; i < 100; i++) {
		plantPos.push_back(glm::vec3(rand() % 150 - 45, 0, (rand() % 450) -450));
		
	}

	for (int i = 0; i < 100; i++) {
		plantPos2.push_back(glm::vec4(rand() % 150 - 45, 0, (rand() % 450) - 450,rand()% 8));
		
	}
	for (int i = 0; i < 100; i++) {
		plantPos2.push_back(glm::vec4((rand() % 85) + 125, 0, -180 - (rand() % 320), rand() % 8));

	}

	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	m_pPlayerPos = 0.0f;
	m_pCameraUpVector = &B;
	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
	m_pPlanarTerrain = new CPlane;
	m_pFtFont = new CFreeTypeFont;
	m_pBarrelMesh = new COpenAssetImportMesh;
	m_pHorseMesh = new COpenAssetImportMesh;
	m_pSphere = new CSphere;
	m_pAudio = new CAudio;
	m_pPath = new CCatmullRom;
	m_pCube = new CCube;
	m_pTetrahedron = new Tetrahedron;
	m_pTree1 = new COpenAssetImportMesh;
	m_pApple = new COpenAssetImportMesh;
	m_pRabbit = new COpenAssetImportMesh;
	m_pFlowersSet1 = new COpenAssetImportMesh;
	m_pFLowersSet2 = new COpenAssetImportMesh;
	m_pRock = new COpenAssetImportMesh;
	
	m_pPlants.reserve(10);
	for (int i = 0; i < 10; ++i) m_pPlants.emplace_back(new COpenAssetImportMesh);
	

	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height); 
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");

	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	// You can follow this pattern to load additional shaders

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(2500.0f);
	
	// Create the planar terrain
	m_pPlanarTerrain->Create("resources\\textures\\", "grassfloor01.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	m_pFtFont->LoadSystemFont("arial.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	m_pBarrelMesh->Load("resources\\models\\Barrel\\Barrel02.obj");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
	m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj");  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
	m_pTree1->Load("resources\\models\\Trees\\tree2.obj");
	m_pApple->Load("resources\\models\\Apple\\apple.obj");
	m_pRabbit->Load("resources\\models\\Rabbit\\rabbit.obj");
	m_pFlowersSet1->Load("resources\\models\\Flowers\\Flowers1\\flowers.obj");
	m_pRock->Load("resources\\models\\rock\\rock.obj");
	//loading the 9 plant models in the vector
	{
		m_pPlants[0]->Load("resources\\models\\Plants\\one.obj");
		m_pPlants[1]->Load("resources\\models\\Plants\\two.obj");
		m_pPlants[2]->Load("resources\\models\\Plants\\three.obj");
		m_pPlants[3]->Load("resources\\models\\Plants\\four.obj");
		m_pPlants[4]->Load("resources\\models\\Plants\\five.obj");
		m_pPlants[5]->Load("resources\\models\\Plants\\six.obj");
		m_pPlants[6]->Load("resources\\models\\Plants\\seven.obj");
		m_pPlants[7]->Load("resources\\models\\Plants\\eight.obj");
		m_pPlants[8]->Load("resources\\models\\Plants\\nine.obj");
		//m_pPlants[0]->Load("resources\\models\\rock\\Plants\\ten.obj");

	}

	// Create a sphere
	m_pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	glEnable(GL_CULL_FACE);

	// Initialise audio and play background music
	m_pAudio->Initialise();
	m_pAudio->LoadEventSound("Resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
	m_pAudio->LoadMusicStream("Resources\\Audio\\On Horse Day - Zelda Breath of the Wild Soundtrack.mp3");	// Royalty free music from http://www.nosoapradio.us/
	m_pAudio->PlayMusicStream();

	//m_pCamera->Set(glm::vec3(0,300,0), glm::vec3(0,0,0), glm::vec3(1,0,0));
	/* p0 = glm::vec3(-500, 10, -200);
	 p1 = glm::vec3(0, 10, -200);
	 p2 = glm::vec3(0, 10, 200);
	 p3 = glm::vec3(-500, 10, 200);*/

	 m_pPath->CreateCentreline();
	 m_pPath->CreateOffsetCurves();
	 m_pPath->CreateTrack();
	 m_pCube->Create();
	 m_pTetrahedron->Create();
	 m_pPath->CreateLeftSideFence();
	 m_pPath->CreateRightSideFence();
	 

}

// Render method runs repeatedly in a loop
void Game::Render()
{

	// Clear the buffers and enable depth testing (z-buffering)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10;
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);


	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)

	//if(turnOnThirdPersonMode)  modelViewMatrixStack.Rotate(glm::cross(m_pCamera->GetView(), m_pCamera->GetUpVector()), -30.0f);
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);
	//if (turnOnThirdPersonMode)  modelViewMatrixStack.Rotate(glm::cross(m_pCamera->GetView(), m_pCamera->GetUpVector()), -30.0f);



	// Set light and materials in main shader program
	glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*
	pMainProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La", glm::vec3(1.0f));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f));		// Specular colour of light
	pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property


	// Render the skybox and terrain with full ambient reflectance 
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("renderSkybox", true);
	// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
	glm::vec3 vEye = m_pCamera->GetPosition();
	modelViewMatrixStack.Translate(vEye);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pSkybox->Render(cubeMapTextureUnit);
	pMainProgram->SetUniform("renderSkybox", false);
	modelViewMatrixStack.Pop();

	// Render the planar terrain
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pPlanarTerrain->Render();
	modelViewMatrixStack.Pop();


	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	

	{
	// Render the horse 
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
	modelViewMatrixStack.Scale(2.5f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pHorseMesh->Render();
	modelViewMatrixStack.Pop();

	// Render the crate box (cube) 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(0.0f - 70.0f, 2.0f, 0.0f -10.0f));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
		modelViewMatrixStack.Scale(2.5f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCube->Render();
	modelViewMatrixStack.Pop();

	// Render the tetrahedron 
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3( 170.0f, 50.0f, - 320.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
	modelViewMatrixStack.Scale(50.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pTetrahedron->Render();
	modelViewMatrixStack.Pop();

	// Render the big  tree
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(0.0f - 50.0f, 2.0f, 0.0f - 10.0f));
	modelViewMatrixStack.Rotate(glm::vec3(1.0f, 0.0f, 0.0f), -90.0f);
	modelViewMatrixStack.Scale(100.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_pTree1->Render();
	modelViewMatrixStack.Pop();
	

	// Render the second big tree
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(+ 180.0f, 2.0f, - 40.0f));
	modelViewMatrixStack.Rotate(glm::vec3(1.0f, 0.0f, 0.0f), -90.0f);
	modelViewMatrixStack.Scale(100.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_pTree1->Render();
	modelViewMatrixStack.Pop();

	// Render the Rabbit
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(0.0f - 80.0f, 0.0f, 0.0f - 10.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
	modelViewMatrixStack.Scale(5.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_pRabbit->Render();
	modelViewMatrixStack.Pop();


	// Render the apple
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(0.0f - 80.0f, 2.0f, 0.0f - 10.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
	modelViewMatrixStack.Scale(40.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//m_pApple->Render();
	modelViewMatrixStack.Pop();


	// Render the barrel 
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(100.0f, 0.0f, 0.0f));
	modelViewMatrixStack.Scale(5.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pBarrelMesh->Render();
	modelViewMatrixStack.Pop();


	// Render the sphere
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(0.0f, 2.0f, 150.0f));
	modelViewMatrixStack.Scale(2.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//pMainProgram->SetUniform("bUseTexture", false);
	m_pSphere->Render();
	modelViewMatrixStack.Pop();


	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("bUseTexture", true); // turn off texturing
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix",
		m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	// Render your object here
	//m_pPath->RenderCentreline();
	//m_pPath->RenderOffsetCurves();
	m_pPath->RenderTrack();
	m_pPath->RenderLeftSideFence();
	m_pPath->RenderRightSideFence();
	modelViewMatrixStack.Pop();

}

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(0.0f, 1.0f, 90.0f));
	modelViewMatrixStack.Scale(15.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//pMainProgram->SetUniform("bUseTexture", false);
	m_pFlowersSet1->Render();
	modelViewMatrixStack.Pop();
	
	for (int i = 1; i < 100; i++) {

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(plantPos[i]);
		modelViewMatrixStack.Scale(15.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
		//pMainProgram->SetUniform("bUseTexture", false);
		m_pFlowersSet1->Render();
		modelViewMatrixStack.Pop();

	}

	for (int i = 0; i < 20; i++) {

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(plantPos2[i].x, plantPos2[i].y, plantPos2[i].z);
		modelViewMatrixStack.Scale(15.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
		//pMainProgram->SetUniform("bUseTexture", false);
		m_pPlants[plantPos2[i].w]->Render();
		modelViewMatrixStack.Pop();

	}

	for (int i = 110; i < 120; i++) {

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(plantPos2[i].x, plantPos2[i].y, plantPos2[i].z);
		modelViewMatrixStack.Scale(15.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
		//pMainProgram->SetUniform("bUseTexture", false);
		m_pPlants[plantPos2[i].w]->Render();
		modelViewMatrixStack.Pop();

	}
	


	DisplayFrameRate();

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());		

}

// Update method runs repeatedly with the Render method
void Game::Update() 
{
	
	
	static glm::vec3 p,pNext,dist,pos,thirdPView,bThirdView,x;


	m_currentDistance += m_dt*m_cameraSpeed*0.5f;
	m_pPath->Sample(m_currentDistance, p);
	m_pPath->Sample(m_currentDistance+0.1f, pNext);
	p.y += 1; pNext.y += 1;
	T = glm::normalize(pNext - p);
	N = glm::normalize(glm::cross(T, glm::vec3(0, 1, 0)));
	B = glm::normalize(glm::cross(N, T));

	// position change on track (left right or center)
	if (m_pPlayerPos == 0) pos = glm::vec3(0.0f, 0.0f, 0.0f);
	else if (m_pPlayerPos == 1) pos = N;
	else pos = -N;

	//
	if (turnOnThirdPersonMode) {
		p -= 5.0f*T;
		p.y += 4.0f;
		
		*m_pCameraUpVector = glm::rotate(*m_pCameraUpVector, m_cameraRotation, N);
		*m_pCameraViewDir = glm::rotate(*m_pCameraViewDir, m_cameraRotation, N);

	}
	//m_pCameraUpVector = &bThirdView;

	//case 1
	//m_pCamera->Set(p, p + 10.0f*T, m_pCamera->GetUpVector());
	//case 2
	//p += 1.0f*T; //moving forward
	p += 2.0f*pos; //position change on the left side of track
	//m_pCamera->Set(p, p + 1.0f*(*m_pCameraViewDir), *m_pCameraUpVector);

	

	//static CCatmullRom *path;
		
	/*static float t = 0.0f;
	t += 0.0005f * (float)m_dt;
	if (t > 1.0f) t = 0.0f;*/
	m_pCamera->Update(m_dt);
	//m_pCamera->Set( p, m_pCamera->GetView(), m_pCamera->GetUpVector());
	//m_pCamera->Set(p,p + 10.0f*T, m_pCamera->GetUpVector());
	//m_pPath->RenderPath();
	
	//m_pPath->RenderCentreline();
	//m_pAudio->Update();
}



void Game::DisplayFrameRate()
{

	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
    {
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
    }

	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(20, height - 20, 20, "FPS: %d", m_framesPerSecond);
		m_pFtFont->Render(20, height - 40, 20, "Player Position %f", m_pPlayerPos);

		m_pFtFont->Render(20, height - 60, 20, "View direction %f ,%f, %f", (*m_pCameraViewDir).x, (*m_pCameraViewDir).y, (*m_pCameraViewDir).z);
		m_pFtFont->Render(20, height - 80, 20, "View direction %f ,%f, %f", m_pCamera->GetView().x, m_pCamera->GetView().y, m_pCamera->GetView().z);

	}
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/
	
	
	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();
	

}


WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	
	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_appActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_appActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_appActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch(w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case '1':
			m_pAudio->PlayEventSound();
			break;
		case VK_F1:
			m_pAudio->PlayEventSound();
			break;
		case 'A':
			m_pPlayerPos = std::min(1.0f, std::max(m_pPlayerPos - 1.0f, -1.0f));
			break;
		case 'D':
			m_pPlayerPos = std::min(1.0f, std::max(m_pPlayerPos + 1.0f, -1.0f));
			break;
		case 'L':
			m_pCameraViewDir = &N;
			m_pCameraUpVector = &B;
			turnOnThirdPersonMode = false;
			break;
		case 'F':
			m_pCameraViewDir = &T;
			m_pCameraUpVector = &B;
			turnOnThirdPersonMode = false;

			break;
		case 'U':
			m_pCameraViewDir = &B;
			m_pCameraUpVector = &T;
			*m_pCameraUpVector = -*m_pCameraUpVector;
			turnOnThirdPersonMode = false;
			break;
		case 'T':
			m_pCameraViewDir = &T;
			m_pCameraUpVector = &B;
			turnOnThirdPersonMode = true;
			break;

		case 'W':
			m_cameraRotation -= m_dt*0.01f;
			break;
		case 'S':
			m_cameraRotation += m_dt*0.01f;
			break;


		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}

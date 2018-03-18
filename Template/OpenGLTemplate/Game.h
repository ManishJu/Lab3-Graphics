#pragma once

#include "Common.h"
#include "GameWindow.h"
#include <memory>

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class COpenAssetImportMesh;
class CAudio;
class CCatmullRom;
class CCube;
class Tetrahedron;
//class glutil::MatrixStack;

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox *m_pSkybox;
	CCamera *m_pCamera;
	vector <CShaderProgram *> *m_pShaderPrograms;
	CPlane *m_pPlanarTerrain;
	CFreeTypeFont *m_pFtFont;
	COpenAssetImportMesh *m_pBarrelMesh;
	COpenAssetImportMesh *m_pHorseMesh;
	CSphere *m_pSphere;
	CHighResolutionTimer *m_pHighResolutionTimer;
	CAudio *m_pAudio;
	glm::vec3 p0, p1, p2, p3;
	CCatmullRom *m_pPath;
	CCube *m_pCube;
	Tetrahedron *m_pTetrahedron;

	//My meshes:
	COpenAssetImportMesh *m_pTree1;
	COpenAssetImportMesh *m_pApple;
	COpenAssetImportMesh *m_pRabbit;
	COpenAssetImportMesh *m_pFlowersSet1;
	COpenAssetImportMesh *m_pFLowersSet2;
	COpenAssetImportMesh *m_pRock;
	std::vector<COpenAssetImportMesh*> m_pPlants;
	std::vector<COpenAssetImportMesh*> m_pCollectible;
	std::vector<COpenAssetImportMesh*> m_pObstacle;



	//glutil::MatrixStack modelViewMatrixStack;

	// Some other member variables
	bool turnOnThirdPersonMode = false;
	bool turnOnTopView = false;
	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;
	float m_currentDistance;
	float m_cameraSpeed;
	float m_cameraRotation = 0.0f;
	float m_pPlayerPosChecker;
	float m_tt;
	glm::vec3 T, B, N;
	glm::vec3 *m_pCameraViewDir = &T;
	glm::vec3 *m_pCameraUpVector = &B;
	glm::vec3 m_pPlayerPos;
	glm::mat4 m_pPlayerOrientation;

	std::vector<glm::vec3> plantPos;
	std::vector<glm::vec3> rockPos;
	std::vector<glm::vec4> plantPos2;
	std::vector<glm::vec3> collectiblePos;
	std::vector<glm::vec3> m_pObstaclesPos;


public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	static const int FPS = 60;
	void DisplayFrameRate();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;
	unsigned int m_pPointsCollected;
	unsigned int m_pLapsCompleted;
	long long int m_pCounter = 0;

};

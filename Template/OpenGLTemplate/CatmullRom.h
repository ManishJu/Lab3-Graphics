#pragma once
#include "Common.h"
#include "vertexBufferObject.h"
#include "vertexBufferObjectIndexed.h"
#include "Texture.h"


class CCatmullRom
{
public:
	CCatmullRom();
	~CCatmullRom();

	void CreateCentreline();
	void RenderCentreline();

	void CreateOffsetCurves();
	void RenderOffsetCurves();

	void CreateTrack();
	void RenderTrack();

	void CreateLeftSideFence();
	void RenderLeftSideFence();

	void CreateRightSideFence();
	void RenderRightSideFence();

	void CreateLineButterflyPath();
	void RenderLineButterflyPath();

	int CurrentLap(float d); // Return the currvent lap (starting from 0) based on distance along the control curve.

	bool Sample(float d, glm::vec3 &p, glm::vec3 &up = glm::vec3(0, 0, 0)); // Return a point on the centreline based on a certain distance along the control curve.
	bool SampleButterflyPath(float d, glm::vec3 &p, glm::vec3 &up = glm::vec3(0, 0, 0)); // Return a point on the centreline based on a certain distance along the control curve.
 
	const unsigned int num_samples_curve = 1000;
	const unsigned int num_samples_curve_butterfly =200;
	//GLuint m_vaoCentreline;
private:

	void SetControlPoints();
	void SetControlPointsButterflyPath();
	void ComputeLengthsAlongControlPoints();
	void ComputeLengthsAlongControlPointsButterflyPath();
	void UniformlySampleControlPoints(int numSamples);
	void UniformlySampleControlPointsButterflyPath(int numSamples);

	glm::vec3 Interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t);


	vector<float> m_distances;
	vector<float> m_distancesButterflyPath;
	CTexture m_texture;

	GLuint m_vaoCentreline;
	GLuint m_vaoLeftOffsetCurve;
	GLuint m_vaoRightOffsetCurve;
	GLuint m_vaoTrack;
	GLuint m_vaoSideFence;
	GLuint m_vaoSideFence2;

	GLuint m_vaoCentrelineButterflyPath;

	vector<glm::vec3> m_controlPoints;		// Control points, which are interpolated to produce the centreline points
	vector<glm::vec3> m_controlUpVectors;	// Control upvectors, which are interpolated to produce the centreline upvectors
	vector<glm::vec3> m_centrelinePoints;	// Centreline points
	vector<glm::vec3> m_centrelineUpVectors;// Centreline upvectors
	

	vector<glm::vec3> m_controlPointsButterflyPath; // Control points for the path of the butterflies
	vector<glm::vec3> m_controlUpVectorsButterflyPath;
	vector<glm::vec3> m_centrelinePointsButterflyPath;
	vector<glm::vec3> m_centrelineUpVectorsButterflyPath;

	vector<glm::vec3> m_leftOffsetPoints;	// Left offset curve points
	vector<glm::vec3> m_rightOffsetPoints;	// Right offset curve points
	vector<glm::vec3> m_leftUpOffsetPoints; // for the boundary fence
	vector<glm::vec3> m_rightUpOffsetPoints;// for the boundary fence

	CTexture m_pTex1;
	CTexture m_pTex2;
	CTexture m_pTex3;


	unsigned int m_vertexCount;				// Number of vertices in the track VBO
};

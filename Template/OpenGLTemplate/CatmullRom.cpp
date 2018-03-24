#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>



CCatmullRom::CCatmullRom()
{
    m_vertexCount = 0;
}

CCatmullRom::~CCatmullRom()
{}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

    glm::vec3 a = p1;
    glm::vec3 b = 0.5f * (-p0 + p2);
    glm::vec3 c = 0.5f * (2.0f*p0 - 5.0f*p1 + 4.0f*p2 - p3);
    glm::vec3 d = 0.5f * (-p0 + 3.0f*p1 - 3.0f*p2 + p3);

    return a + b*t + c*t2 + d*t3;

}


void CCatmullRom::SetControlPoints()
{
    // Set control points (m_controlPoints) here, or load from disk
	int y = 1;
    // Optionally, set upvectors (m_controlUpVectors, one for each control point as well)
    m_controlPoints.push_back(glm::vec3(100, y, 0));
    m_controlPoints.push_back(glm::vec3(71, y, 71));
    m_controlPoints.push_back(glm::vec3(0, y, 100));
    m_controlPoints.push_back(glm::vec3(-71, y, 71));
    m_controlPoints.push_back(glm::vec3(-100, y, 0));


	m_controlPoints.push_back(glm::vec3(-100, y, -50));
	m_controlPoints.push_back(glm::vec3(-100, y, -100));
	m_controlPoints.push_back(glm::vec3(-71, y, -129));
	/*m_controlPoints.push_back(glm::vec3(-30, y, -160));
	m_controlPoints.push_back(glm::vec3(70, y, -180));
	m_controlPoints.push_back(glm::vec3(120, y, -231));
	m_controlPoints.push_back(glm::vec3(90, y, -140));*/
	m_controlPoints.push_back(glm::vec3(-100, y, -321));
	m_controlPoints.push_back(glm::vec3(-71, y, -452));
	m_controlPoints.push_back(glm::vec3(0, y, -530));
	m_controlPoints.push_back(glm::vec3(200, y, -470));
	m_controlPoints.push_back(glm::vec3(300, y, -300));
	m_controlPoints.push_back(glm::vec3(100, y, 0));
	
/*
    m_controlPoints.push_back(glm::vec3(-71, 5, -71));
    m_controlPoints.push_back(glm::vec3(0, 5, -100));
    m_controlPoints.push_back(glm::vec3(71, 5, -71));*/
}

void CCatmullRom::SetControlPointsButterflyPath(){

    vector<glm::vec3> all = { 
        glm::vec3(0,10,100),
        glm::vec3(-10,10,90),
        glm::vec3(-30,10,75),
        glm::vec3(-10,10,60),
        glm::vec3(0,10,50),
        glm::vec3(25,10,60),
        glm::vec3(40,10,75),
        glm::vec3(25,10,90),

         glm::vec3(0,8,100),
        glm::vec3(-10,8,90),
        glm::vec3(-30,8,75),
        glm::vec3(-10,8,60),
        glm::vec3(0,8,50),
        glm::vec3(25,8,60),
        glm::vec3(40,8,75),
        glm::vec3(25,8,90),

        glm::vec3(0,6,100),
        glm::vec3(-10,6,90),
        glm::vec3(-30,6,75),
        glm::vec3(-10,6,60),
        glm::vec3(0,6,50),
        glm::vec3(25,6,60),
        glm::vec3(40,6,75),
        glm::vec3(25,6,90),

         glm::vec3(0,3,100),
        glm::vec3(-10,3,90),
        glm::vec3(-30,3,75),
        glm::vec3(-10,3,60),
        glm::vec3(0,3,50),
        glm::vec3(25,3,60),
        glm::vec3(40,3,75),
        glm::vec3(25,3,90),

         glm::vec3(0,1,100),
        glm::vec3(-10,1,90),
        glm::vec3(-30,1,75),
        glm::vec3(-10,1,60),
        glm::vec3(0,1,50),
        glm::vec3(25,1,60),
        glm::vec3(40,1,75),
        glm::vec3(25,1,90),

          glm::vec3(0,12,100),
        glm::vec3(-10,12,90),
        glm::vec3(-30,12,75),
        glm::vec3(-10,12,60),
        glm::vec3(0,12,50),
        glm::vec3(25,12,60),
        glm::vec3(40,12,75),
        glm::vec3(25,12,90),

          glm::vec3(0,14,100),
        glm::vec3(-10,14,90),
        glm::vec3(-30,14,75),
        glm::vec3(-10,14,60),
        glm::vec3(0,14,50),
        glm::vec3(25,14,60),
        glm::vec3(40,14,75),
        glm::vec3(25,14,90),
    };

    for (int i = 0; i < 8; i ++){
        m_controlPointsButterflyPath.push_back(all[rand()% 49]);
    }
}

// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
    int M = (int)m_controlPoints.size();

    float fAccumulatedLength = 0.0f;
    m_distances.push_back(fAccumulatedLength);
    for (int i = 1; i < M; i++) {
        fAccumulatedLength += glm::distance(m_controlPoints[i - 1], m_controlPoints[i]);
        m_distances.push_back(fAccumulatedLength);
    }

    // Get the distance from the last point to the first
    fAccumulatedLength += glm::distance(m_controlPoints[M - 1], m_controlPoints[0]);
    m_distances.push_back(fAccumulatedLength);
}

void CCatmullRom::ComputeLengthsAlongControlPointsButterflyPath(){

    int M = (int)m_controlPointsButterflyPath.size();

    float fAccumulatedLength = 0.0f;
    m_distancesButterflyPath.push_back(fAccumulatedLength);
    for (int i = 1; i < M; i++) {
        fAccumulatedLength += glm::distance(m_controlPointsButterflyPath[i - 1], m_controlPointsButterflyPath[i]);
        m_distancesButterflyPath.push_back(fAccumulatedLength);
    }

    // Get the distance from the last point to the first
    fAccumulatedLength += glm::distance(m_controlPointsButterflyPath[M - 1], m_controlPointsButterflyPath[0]);
    m_distancesButterflyPath.push_back(fAccumulatedLength);

}
// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Sample(float d, glm::vec3 &p, glm::vec3 &up)
{
    if (d < 0)
        return false;

    int M = (int)m_controlPoints.size();
    if (M == 0)
        return false;


    float fTotalLength = m_distances[m_distances.size() - 1];

    // The the current length along the control polygon; handle the case where we've looped around the track
    float fLength = d - (int)(d / fTotalLength) * fTotalLength;

    // Find the current segment
    int j = -1;
    for (int i = 0; i < (int)m_distances.size(); i++) {
        if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
            j = i; // found it!
            break;
        }
    }

    if (j == -1)
        return false;

    // Interpolate on current segment -- get t
    float fSegmentLength = m_distances[j + 1] - m_distances[j];
    float t = (fLength - m_distances[j]) / fSegmentLength;

    // Get the indices of the four points along the control polygon for the current segment
    int iPrev = ((j - 1) + M) % M;
    int iCur = j;
    int iNext = (j + 1) % M;
    int iNextNext = (j + 2) % M;

    // Interpolate to get the point (and upvector)
    p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
    if (m_controlUpVectors.size() == m_controlPoints.size())
        up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));


    return true;
}

bool CCatmullRom::SampleButterflyPath(float d, glm::vec3 &p, glm::vec3 &up){
     if (d < 0)
        return false;

    int M = (int)m_controlPointsButterflyPath.size();
    if (M == 0)
        return false;


    float fTotalLength = m_distancesButterflyPath[m_distancesButterflyPath.size() - 1];

    // The the current length along the control polygon; handle the case where we've looped around the track
    float fLength = d - (int)(d / fTotalLength) * fTotalLength;

    // Find the current segment
    int j = -1;
    for (int i = 0; i < (int)m_distancesButterflyPath.size(); i++) {
        if (fLength >= m_distancesButterflyPath[i] && fLength < m_distancesButterflyPath[i + 1]) {
            j = i; // found it!
            break;
        }
    }

    if (j == -1)
        return false;

    // Interpolate on current segment -- get t
    float fSegmentLength = m_distancesButterflyPath[j + 1] - m_distancesButterflyPath[j];
    float t = (fLength - m_distancesButterflyPath[j]) / fSegmentLength;

    // Get the indices of the four points along the control polygon for the current segment
    int iPrev = ((j - 1) + M) % M;
    int iCur = j;
    int iNext = (j + 1) % M;
    int iNextNext = (j + 2) % M;

    // Interpolate to get the point (and upvector)
    p = Interpolate(m_controlPointsButterflyPath[iPrev], m_controlPointsButterflyPath[iCur], m_controlPointsButterflyPath[iNext], m_controlPointsButterflyPath[iNextNext], t);
    if (m_controlUpVectorsButterflyPath.size() == m_controlPointsButterflyPath.size())
        up = glm::normalize(Interpolate(m_controlUpVectorsButterflyPath[iPrev], m_controlUpVectorsButterflyPath[iCur], m_controlUpVectorsButterflyPath[iNext], m_controlUpVectorsButterflyPath[iNextNext], t));


    return true;
}


// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples)
{
    glm::vec3 p, up;

    // Compute the lengths of each segment along the control polygon, and the total length
    ComputeLengthsAlongControlPoints();
    float fTotalLength = m_distances[m_distances.size() - 1];

    // The spacing will be based on the control polygon
    float fSpacing = fTotalLength / numSamples;

    // Call PointAt to sample the spline, to generate the points
    for (int i = 0; i < numSamples; i++) {
        Sample(i * fSpacing, p, up);
        m_centrelinePoints.push_back(p);
        if (m_controlUpVectors.size() > 0)
            m_centrelineUpVectors.push_back(up);

    }


    // Repeat once more for truly equidistant points
    m_controlPoints = m_centrelinePoints;
    m_controlUpVectors = m_centrelineUpVectors;
    m_centrelinePoints.clear();
    m_centrelineUpVectors.clear();
    m_distances.clear();
    ComputeLengthsAlongControlPoints();
    fTotalLength = m_distances[m_distances.size() - 1];
    fSpacing = fTotalLength / numSamples;
    for (int i = 0; i < numSamples; i++) {
        Sample(i * fSpacing, p, up);
        m_centrelinePoints.push_back(p);
        if (m_controlUpVectors.size() > 0)
            m_centrelineUpVectors.push_back(up);
    }


}

void CCatmullRom::UniformlySampleControlPointsButterflyPath(int numSamples)
{
    glm::vec3 p, up;

    // Compute the lengths of each segment along the control polygon, and the total length
    ComputeLengthsAlongControlPointsButterflyPath();
    float fTotalLength = m_distancesButterflyPath[m_distancesButterflyPath.size() - 1];

    // The spacing will be based on the control polygon
    float fSpacing = fTotalLength / numSamples;

    // Call PointAt to sample the spline, to generate the points
    for (int i = 0; i < numSamples; i++) {
        SampleButterflyPath(i * fSpacing, p, up);
        m_centrelinePointsButterflyPath.push_back(p);
        if (m_controlUpVectorsButterflyPath.size() > 0)
            m_centrelineUpVectorsButterflyPath.push_back(up);

    }


    // Repeat once more for truly equidistant points
    m_controlPointsButterflyPath = m_centrelinePointsButterflyPath;
    m_controlUpVectorsButterflyPath = m_centrelineUpVectorsButterflyPath;
    m_centrelinePointsButterflyPath.clear();
    m_centrelineUpVectorsButterflyPath.clear();
    m_distancesButterflyPath.clear();
    ComputeLengthsAlongControlPointsButterflyPath();
    fTotalLength = m_distancesButterflyPath[m_distancesButterflyPath.size() - 1];
    fSpacing = fTotalLength / numSamples;
    for (int i = 0; i < numSamples; i++) {
        SampleButterflyPath(i * fSpacing, p, up);
        m_centrelinePointsButterflyPath.push_back(p);
        if (m_controlUpVectorsButterflyPath.size() > 0)
            m_centrelineUpVectorsButterflyPath.push_back(up);
    }


}

void CCatmullRom::CreateLineButterflyPath(){

    SetControlPointsButterflyPath();
    UniformlySampleControlPointsButterflyPath(num_samples_curve_butterfly);

    // Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
    glGenVertexArrays(1, &m_vaoCentrelineButterflyPath);
    glBindVertexArray(m_vaoCentrelineButterflyPath);
    CVertexBufferObject vbo;
    vbo.Create();
    vbo.Bind();
    glm::vec2 texCoord(0.0f, 0.0f);
    glm::vec3 normal(0.0f, 1.0f, 0.0f);
    for (unsigned int i = 0; i < num_samples_curve_butterfly; i++)
    {
        vbo.AddData(&m_centrelinePointsButterflyPath[i], sizeof(glm::vec3));
        vbo.AddData(&texCoord, sizeof(glm::vec2));
        vbo.AddData(&normal, sizeof(glm::vec3));
    }
    vbo.UploadDataToGPU(GL_STATIC_DRAW);
    GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
        + sizeof(glm::vec2)));

}


void CCatmullRom::CreateCentreline()
{
    // Call Set Control Points
    SetControlPoints();

    // Call UniformlySampleControlPoints with the number of samples required
    UniformlySampleControlPoints(num_samples_curve);

    // Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
    glGenVertexArrays(1, &m_vaoCentreline);
    glBindVertexArray(m_vaoCentreline);
    CVertexBufferObject vbo;
    vbo.Create();
    vbo.Bind();
    glm::vec2 texCoord(0.0f, 0.0f);
    glm::vec3 normal(0.0f, 1.0f, 0.0f);
    for (unsigned int i = 0; i < num_samples_curve; i++)
    {
        vbo.AddData(&m_centrelinePoints[i], sizeof(glm::vec3));
        vbo.AddData(&texCoord, sizeof(glm::vec2));
        vbo.AddData(&normal, sizeof(glm::vec3));
    }
    vbo.UploadDataToGPU(GL_STATIC_DRAW);
    GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
        + sizeof(glm::vec2)));
}


void CCatmullRom::CreateOffsetCurves()
{       
    float w = 20.0f;
    for (unsigned int i = 0; i < num_samples_curve; i++)
    {
        glm::vec3 p = m_centrelinePoints[i];
        glm::vec3 pNext = m_centrelinePoints[(i+1) % num_samples_curve];
        glm::vec3 T = glm::normalize(pNext - p);
        glm::vec3 N = glm::normalize(glm::cross(T, glm::vec3(0.0f, 1.0f, 0.0f)));;
        glm::vec3 B = glm::normalize(glm::cross(N, T));;
       
        glm::vec3 l = p - (w / 2) *N;
        glm::vec3 r = p + (w / 2) *N;

		glm::vec3 lup = glm::vec3(l.x, l.y + 2.50f, l.z);
		glm::vec3 rup = glm::vec3(r.x, r.y + 2.50f, r.z);


        m_leftOffsetPoints.push_back(l);
        m_rightOffsetPoints.push_back(r);
		m_leftUpOffsetPoints.push_back(lup);
		m_rightUpOffsetPoints.push_back(rup);
    }

    // Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively

    // Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
    // Note it is possible to only use one VAO / VBO with all the points instead.
    glGenVertexArrays(1, &m_vaoLeftOffsetCurve);
    glBindVertexArray(m_vaoLeftOffsetCurve);
    CVertexBufferObject vbo;
    vbo.Create();
    vbo.Bind();
    glm::vec2 texCoord(0.0f, 0.0f);
    glm::vec3 normal(0.0f, 1.0f, 0.0f);
    for (unsigned int i = 0; i < num_samples_curve; i++)
    {
        vbo.AddData(&m_leftOffsetPoints[i], sizeof(glm::vec3));
        vbo.AddData(&texCoord, sizeof(glm::vec2));
        vbo.AddData(&normal, sizeof(glm::vec3));
    }
	vbo.AddData(&m_leftOffsetPoints[0], sizeof(glm::vec3));
	vbo.AddData(&texCoord, sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));

    vbo.UploadDataToGPU(GL_STATIC_DRAW);
    GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
        + sizeof(glm::vec2)));

    glGenVertexArrays(1, &m_vaoRightOffsetCurve);
    glBindVertexArray(m_vaoRightOffsetCurve);
    CVertexBufferObject vbo1;
    vbo1.Create();
    vbo1.Bind();
    for (unsigned int i = 0; i < num_samples_curve; i++)
    {
        vbo1.AddData(&m_rightOffsetPoints[i], sizeof(glm::vec3));
        vbo1.AddData(&texCoord, sizeof(glm::vec2));
        vbo1.AddData(&normal, sizeof(glm::vec3));
    }
	vbo1.AddData(&m_rightOffsetPoints[0], sizeof(glm::vec3));
	vbo1.AddData(&texCoord, sizeof(glm::vec2));
	vbo1.AddData(&normal, sizeof(glm::vec3));

    vbo1.UploadDataToGPU(GL_STATIC_DRAW);
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
        + sizeof(glm::vec2)));
    
}



void CCatmullRom::CreateTrack()
{

	//texture mapping;
	m_pTex1.Load("resources\\textures\\path.tif");
	m_pTex1.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_pTex1.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_pTex1.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_pTex1.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);


   
    // Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively
    // Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
    // Note it is possible to only use one VAO / VBO with all the points instead.
    glGenVertexArrays(1, &m_vaoTrack);
    glBindVertexArray(m_vaoTrack);
    CVertexBufferObject vbo;
    vbo.Create();
    vbo.Bind();
	glm::vec2 texCoord0(0.0f, 0.0f);
	glm::vec2 texCoord1(0.0f, 1.0f);
	glm::vec2 texCoord2(1.0f, 0.0f);
	glm::vec2 texCoord3(1.0f, 1.0f);

    glm::vec3 normal(0.0f, 1.0f, 0.0f);
    for (unsigned int i = 0; i < num_samples_curve;i++)
    {
        
		glm::vec2((i)/4.0f, 0.0f);
		vbo.AddData(& m_leftOffsetPoints[i], sizeof(glm::vec3));
        vbo.AddData(&glm::vec2( 0.0f, (i) / 4.0f ), sizeof(glm::vec2));
        vbo.AddData(&normal, sizeof(glm::vec3));

		
		vbo.AddData(&m_rightOffsetPoints[i], sizeof(glm::vec3));
        vbo.AddData(&glm::vec2(1.0f,(i) / 4.0f) , sizeof(glm::vec2));
        vbo.AddData(&normal, sizeof(glm::vec3));

    }

	glm::vec2((0) / 4.0f, 0.0f);
	vbo.AddData(&m_leftOffsetPoints[0], sizeof(glm::vec3));
	vbo.AddData(&glm::vec2(0.0f, (0) / 4.0f), sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));


	vbo.AddData(&m_rightOffsetPoints[0], sizeof(glm::vec3));
	vbo.AddData(&glm::vec2(1.0f, (0) / 4.0f), sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));

    vbo.UploadDataToGPU(GL_STATIC_DRAW);
    GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
    // Normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
        + sizeof(glm::vec2)));

}

void CCatmullRom::CreateLeftSideFence()
{

	//texture mapping;
	m_pTex2.Load("resources\\textures\\fence.png");
	m_pTex2.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_pTex2.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_pTex2.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_pTex2.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);



	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively
	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	// Note it is possible to only use one VAO / VBO with all the points instead.
	glGenVertexArrays(1, &m_vaoSideFence);
	glBindVertexArray(m_vaoSideFence);
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord0(0.0f, 0.0f);
	glm::vec2 texCoord1(0.0f, 1.0f);
	glm::vec2 texCoord2(1.0f, 0.0f);
	glm::vec2 texCoord3(1.0f, 1.0f);

	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < num_samples_curve; i++)
	{

		vbo.AddData(&m_leftUpOffsetPoints[i], sizeof(glm::vec3));
		vbo.AddData(&glm::vec2((i) / 4.0f,1.0f), sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));

		glm::vec2((i) / 4.0f, 0.0f);
		vbo.AddData(&m_leftOffsetPoints[i], sizeof(glm::vec3));
		vbo.AddData(&glm::vec2((i) / 4.0f,0.0f), sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));


		

	}


	vbo.AddData(&m_leftUpOffsetPoints[0], sizeof(glm::vec3));
	vbo.AddData(&glm::vec2(1.0f, (0) / 4.0f), sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));

	glm::vec2((0) / 4.0f, 0.0f);
	vbo.AddData(&m_leftOffsetPoints[0], sizeof(glm::vec3));
	vbo.AddData(&glm::vec2(0.0f, (0) / 4.0f), sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));



	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));

}

void CCatmullRom::CreateRightSideFence()
{

	//texture mapping;
	m_pTex3.Load("resources\\textures\\fence.png");
	m_pTex3.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_pTex3.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_pTex3.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_pTex3.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);



	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively
	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	// Note it is possible to only use one VAO / VBO with all the points instead.
	glGenVertexArrays(1, &m_vaoSideFence2);
	glBindVertexArray(m_vaoSideFence2);
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord0(0.0f, 0.0f);
	glm::vec2 texCoord1(0.0f, 1.0f);
	glm::vec2 texCoord2(1.0f, 0.0f);
	glm::vec2 texCoord3(1.0f, 1.0f);

	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < num_samples_curve; i++)
	{
		glm::vec2((i) / 4.0f, 0.0f);
		vbo.AddData(&m_rightOffsetPoints[i], sizeof(glm::vec3));
		vbo.AddData(&glm::vec2((i) / 4.0f, 0.0f), sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));

		vbo.AddData(&m_rightUpOffsetPoints[i], sizeof(glm::vec3));
		vbo.AddData(&glm::vec2((i) / 4.0f, 1.0f), sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));



	}

	glm::vec2((0) / 4.0f, 0.0f);
	vbo.AddData(&m_rightOffsetPoints[0], sizeof(glm::vec3));
	vbo.AddData(&glm::vec2(0.0f, (0) / 4.0f), sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));

	vbo.AddData(&m_rightUpOffsetPoints[0], sizeof(glm::vec3));
	vbo.AddData(&glm::vec2(1.0f, (0) / 4.0f), sizeof(glm::vec2));
	vbo.AddData(&normal, sizeof(glm::vec3));

	



	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));

}


void CCatmullRom::RenderCentreline()
{
    // Bind the VAO m_vaoCentreline and render it
    glBindVertexArray(m_vaoCentreline);
    glDrawArrays(GL_POINTS, 0, num_samples_curve);
    glPointSize(3.0f);
    glDrawArrays(GL_LINE_LOOP, 0, num_samples_curve);
    glLineWidth(10.0f);
}

void CCatmullRom::RenderLineButterflyPath()
{
    // Bind the VAO m_vaoCentreline and render it
    glBindVertexArray(m_vaoCentrelineButterflyPath);
    glDrawArrays(GL_POINTS, 0, num_samples_curve_butterfly);
    glPointSize(3.0f);
    glDrawArrays(GL_LINE_LOOP, 0, num_samples_curve_butterfly);
    glLineWidth(10.0f);
}

void CCatmullRom::RenderOffsetCurves()
{
    // Bind the VAO m_vaoLeftOffsetCurve and render it

    // Bind the VAO m_vaoRightOffsetCurve and render it

    glBindVertexArray(m_vaoLeftOffsetCurve); 
    glDrawArrays(GL_LINE_LOOP, 0, num_samples_curve+1);
    glBindVertexArray(m_vaoRightOffsetCurve);
    glDrawArrays(GL_LINE_STRIP,0,num_samples_curve+1);

}


void CCatmullRom::RenderTrack()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Bind the VAO m_vaoTrack and render it
    glBindVertexArray(m_vaoTrack);
	m_pTex1.Bind();
	//glBindTexture(GL_TEXTURE_2D,m_vaoTrack);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*num_samples_curve+2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void CCatmullRom::RenderLeftSideFence() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Bind the VAO m_vaoSideFence and render it
	glBindVertexArray(m_vaoSideFence);
	m_pTex2.Bind();
	//glBindTexture(GL_TEXTURE_2D,m_vaoSideFence);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * num_samples_curve + 2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void CCatmullRom::RenderRightSideFence() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Bind the VAO m_vaoSideFence and render it
	glBindVertexArray(m_vaoSideFence2);
	m_pTex3.Bind();
	//glBindTexture(GL_TEXTURE_2D,m_vaoSideFence);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * num_samples_curve + 2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int CCatmullRom::CurrentLap(float d)
{

    return (int)(d / m_distances.back());

}
#version 400 core

uniform bool perlinModeOn;


// Structure for matrices
uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix; 
	mat3 normalMatrix;
} matrices;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
//out vec3 vColour;	// Colour computed using reflectance model
out vec2 vTexCoord;	// Texture coordinate
out vec3 worldPosition;	// used for skybox
out vec3 vEyeNorm;
out vec4 vEyePosition;

float snoise(vec3 uv, float res)
{
	const vec3 s = vec3(1e0, 1e2, 1e3);
	uv *= res;
	vec3 uv0 = floor(mod(uv, res))*s;
	vec3 uv1 = floor(mod(uv + vec3(1.), res))*s;
	vec3 f = fract(uv); f = f*f*(3.0 - 2.0*f);
	vec4 v = vec4(uv0.x + uv0.y + uv0.z, uv1.x + uv0.y + uv0.z,uv0.x + uv1.y + uv0.z, uv1.x + uv1.y + uv0.z);
	vec4 r = fract(sin(v*1e-1)*1e3);
	float r0 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);
	r = fract(sin((v + uv1.z - uv0.z)*1e-1)*1e3);
	float r1 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);
	return mix(r0, r1, f.z)*2. - 1.;
}
float perlin(vec3 p, float a, float b)
{
	float noise = 0.0;
	for (int i = 1; i <= 7; i++)
	{
		float ai = pow(a, float(i)/10.0f);
		float bi = pow(b, float(i)/10.0f);
		noise += (1.0 / ai) * snoise(p, bi);
	}
	return noise;
}
// This is the entry point into the vertex shader
void main()
{	

// Save the world position for rendering the skybox
	worldPosition = inPosition;

	// Transform the vertex spatial position using 
	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(inPosition, 1.0f);
	
	// Get the vertex normal and vertex position in eye coordinates
	 vEyeNorm = normalize(matrices.normalMatrix * inNormal);
	 vEyePosition = matrices.modelViewMatrix * vec4(inPosition, 1.0f);
	if(perlinModeOn){ 

		vec3  xx = vEyeNorm*perlin(gl_Position.xyz,1.0f,2.0f)/10.0f;
		gl_Position.xyz += xx;
	}
	// Apply the Phong model to compute the vertex colour
	//vColour = PhongModel(vEyePosition, vEyeNorm);
	
	// Pass through the texture coordinate
	vTexCoord = inCoord;

} 
	
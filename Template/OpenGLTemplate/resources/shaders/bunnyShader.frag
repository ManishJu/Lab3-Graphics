
#version 400 core

in vec3 vColour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader
in vec3 reflected;
out vec4 vOutputColour;		// The output colour

uniform sampler2D sampler0;  // The texture sampler
uniform samplerCube CubeMapTex;
uniform bool turnOnReflection;


void main()
{

		// Get the texel colour from the texture sampler
		vec4 vTexColour = texture(sampler0, vTexCoord);	

		vOutputColour = vTexColour*vec4(vColour, 1.0f);	// Combine object colour and texture 
		
		// Cubemap reflection that is seen on the sphere
		if(turnOnReflection) vOutputColour = vec4(texture(CubeMapTex, normalize(reflected)));
	
}
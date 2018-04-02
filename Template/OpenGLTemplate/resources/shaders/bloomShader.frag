#version 400 core

in vec3 vColour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 vOutputColour;		// The output colour

uniform sampler2D sampler0;  // The texture sampler
uniform samplerCube CubeMapTex;
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform bool renderSkybox;
uniform float t;
in vec3 worldPosition;


void main()
{
	
	vec2 resol = vec2(1336.0f,768.0f);
	const vec2 blurSize = vec2(1.0f/1336.0f,1.0f/768.0f);
	const float intensity = 0.35f;
	vec4 sum = vec4(0.0f);
   	//vec2 texcoord = vTexCoord.xy/resol.xy;
    vec2 texcoord = vTexCoord;

   	int j;
  	int i;
	vec4 vTexColour;


   sum += texture(sampler0, vec2(texcoord.x - 4.0*blurSize.x, texcoord.y)) * 0.05;
   sum += texture(sampler0, vec2(texcoord.x - 3.0*blurSize.x, texcoord.y)) * 0.09;
   sum += texture(sampler0, vec2(texcoord.x - 2.0*blurSize.x, texcoord.y)) * 0.12;
   sum += texture(sampler0, vec2(texcoord.x - blurSize.x, texcoord.y)) * 0.15;
   sum += texture(sampler0, vec2(texcoord.x, texcoord.y)) * 0.16;
   sum += texture(sampler0, vec2(texcoord.x + blurSize.y, texcoord.y)) * 0.15;
   sum += texture(sampler0, vec2(texcoord.x + 2.0*blurSize.x, texcoord.y)) * 0.12;
   sum += texture(sampler0, vec2(texcoord.x + 3.0*blurSize.x, texcoord.y)) * 0.09;
   sum += texture(sampler0, vec2(texcoord.x + 4.0*blurSize.x, texcoord.y)) * 0.05;
	
	// blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture(sampler0, vec2(texcoord.x, texcoord.y - 4.0*blurSize.y)) * 0.05;
   sum += texture(sampler0, vec2(texcoord.x, texcoord.y - 3.0*blurSize.y)) * 0.09;
   sum += texture(sampler0, vec2(texcoord.x, texcoord.y - 2.0*blurSize.y)) * 0.12;
   sum += texture(sampler0, vec2(texcoord.x, texcoord.y - blurSize.y)) * 0.15;
   sum += texture(sampler0, vec2(texcoord.x, texcoord.y)) * 0.16;
   sum += texture(sampler0, vec2(texcoord.x, texcoord.y + blurSize.y)) * 0.15;
   sum += texture(sampler0, vec2(texcoord.x, texcoord.y + 2.0*blurSize.y)) * 0.12;
   sum += texture(sampler0, vec2(texcoord.x, texcoord.y + 3.0*blurSize.y)) * 0.09;
   sum += texture(sampler0, vec2(texcoord.x, texcoord.y + 4.0*blurSize.y)) * 0.05;

   //increase blur with intensity!
   //vOutputColour = sum*intensity + texture(sampler0, texcoord); 
  

	if (renderSkybox) {
		vOutputColour = texture(CubeMapTex, worldPosition);

	} else {

		// Get the texel colour from the texture sampler

        vTexColour = sum * sint(t) + texture(sampler0, texcoord);
    


		if (bUseTexture)
			vOutputColour = vTexColour*vec4(vColour, 1.0f);	// Combine object colour and texture 
		else
			vOutputColour = vec4(vColour, 1.0f);	// Just use the colour instead

            vOutputColour +=  sum * sint(t) ;

           // if(vOutputColour.x <0.7 && vOutputColour.y <0.7  && vOutputColour.z <0.7) vOutputColour = vec4(0);
	}
	
	
}


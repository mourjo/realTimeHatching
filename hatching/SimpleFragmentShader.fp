#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentNormal;
in vec2 fragmentUV;

// Values that stay constant for the whole mesh
uniform sampler2D myTextureSampler;
uniform mat4 ViewMat; //view matrix
uniform mat4 MV; 
in mat4 MVP;
in vec4 pos;
uniform float count;
// Ouput data
out vec3 color;

void main()
{
	  
	  vec4 lightDirection = ViewMat*vec4(9.,9.,5.,1.)+vec4(sin(count),cos(count),0,0);  //oscillating light source
      lightDirection = MV * lightDirection; 

      vec3 L = normalize(lightDirection.xyz);
      vec3 N = normalize(fragmentNormal);


	  vec3 pos1 = normalize(pos);
	  float depth = pos1.z;
	
	  color = texture(myTextureSampler, ((mod(4.5*fragmentUV,1.) + vec2(max(dot(-N,L),0.), depth*3.))/3.)).rgb;

}
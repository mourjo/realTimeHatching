#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentNormal;

uniform sampler2D shadowMap;

// Ouput data
out vec3 color;

in vec4 ShadowCoord;

// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main()
{
		vec4 lightDirection = vec4(-3.,-3.,-2.,1); 
		vec3 L = normalize(lightDirection.xyz);

		// normal is interpolated between all 3 surrounding vertices, it needs to be normalized
		vec3 N = normalize(fragmentNormal);

		
		float diffuse = max(dot(N,-L),0.);
		color = vec3(diffuse);

}


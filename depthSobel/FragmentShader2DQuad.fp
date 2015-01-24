#version 330 core

// Interpolated values from the vertex shaders
in vec2 fragmentUV;

// Values that stay constant for the whole mesh
uniform sampler2D myTextureSampler;

// Ouput data
out vec3 color;

void main()
{
		//SOBEL
		vec4 s1 = texture(myTextureSampler, fragmentUV - 1.0 / 1200.0 - 1.0 / 800.0);
		vec4 s2 = texture(myTextureSampler, fragmentUV + 1.0 / 1200.0 - 1.0 / 800.0);
		vec4 s3 = texture(myTextureSampler, fragmentUV - 1.0 / 1200.0 + 1.0 / 800.0);
		vec4 s4 = texture(myTextureSampler, fragmentUV + 1.0 / 1200.0 + 1.0 / 800.0);
		vec4 sx = 4.0 * ((s4 + s3) - (s2 + s1));
		vec4 sy = 4.0 * ((s2 + s4) - (s1 + s3));
		vec4 sobel = sqrt(sx * sx + sy * sy);
		color = 1. - sobel;
}
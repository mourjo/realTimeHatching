#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentNormal;

// Ouput data
out vec3 color;

void main()
{
      vec4 lightDirection = vec4(-3.,-3.,-2.,1); 
      vec3 L = normalize(lightDirection.xyz);

      // normal is interpolated between all 3 surrounding vertices, it needs to be normalized
      vec3 N = normalize(fragmentNormal);

      // Diffuse shading
      float diffuse = max(dot(N,-L),0.);

      color = vec3(0.5,1.0,0.0);
}
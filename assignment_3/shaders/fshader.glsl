#version 150

uniform vec4 AmbientProduct;
uniform vec4 DiffuseProduct;
uniform vec4 SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;

in vec4 pos;
in vec4 N;

out vec4 fragColor;

void main()
{
  
  // Ambient
  vec4 ambient = AmbientProduct;
  
  // Diffuse
  float Kd = 1.0;
  vec4  diffuse = Kd*DiffuseProduct;
  
  //Specular
  float Ks = 0.0;
  vec4  specular = Ks * SpecularProduct;
  
  fragColor = ambient + diffuse + specular;
}


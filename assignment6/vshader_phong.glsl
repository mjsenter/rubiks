#version 150

//
// Shader for per-fragment lighting calculation with texture coordinate
//

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;
uniform vec4 mLight;  //moveable light
uniform vec4 sLight;  //stationary light

in  vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

out vec3 fN;
out vec3 fsL;
out vec3 fmL;
out vec3 fV;
out vec2 fTexCoord;

void main() 
{
  fN = normalMatrix * vNormal;
  fsL = (view * sLight - view * model * vPosition).xyz;
  fmL = (view * mLight - view * model * vPosition).xyz;
  fV = -(view * model * vPosition).xyz;
  
  fTexCoord = vTexCoord;
  gl_Position = projection * view * model * vPosition;
} 

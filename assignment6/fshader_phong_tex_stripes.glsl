//
// Shader that uses texture coordinates to make blue and yellow stripes.
// Illustrates simple antialiasing with the smoothstep() function.
//
#version 150

uniform mat3 materialProperties; // not used
uniform mat3 lightProperties;    // not used
uniform float shininess;
uniform sampler2D texture;       // not used

in vec3 fN;
in vec3 fL;
in vec3 fV;
in  vec2 fTexCoord;

out vec4 color;

void main() 
{ 
 // get rows from material and light properties
  mat3 material = transpose(materialProperties);
  mat3 light = transpose(lightProperties);
  vec4 ambientLight = vec4(light[0], 1.0);
  vec4 diffuseLight = vec4(light[1], 1.0);
  vec4 specularLight = vec4(light[2], 1.0);
  vec4 ambientSurface; // surface color set procedurally below
  vec4 diffuseSurface; 
  vec4 specularSurface;

  // procedural texture - blue with yellow stripes
  float scale = 5.0;  // this controls width of stripes
  float fuzz = 0.05;  // this controls area of blending between blue and yellow
  
  // blue, yellow
  vec4 backcolor = vec4(0.0, 0.0, 1.0, 1.0);
  vec4 frontcolor = vec4(1.0, 1.0, 0.0, 1.0);
  
  // Scale the t-coordinate of texture, then take fractional part
  // only (to get stripes)
  float scaledT = fract(fTexCoord.t * scale);
  
#if 1  
  // without antialiasing
  // yellow on the first half of interval, blue on second half
  float m;
  if (scaledT < 0.5)
  {
    m = 0;
  }
  else
  {
    m = 1;
  }  

  // This creates a harsh transition since m is either 0 or 1
  diffuseSurface = (1 - m) * frontcolor + m * backcolor;

  //Alt: use the built-in function mix(frontcolor, backcolor, m);
#endif

#if 0
  // using smoothstep to soften the transitions
  // as above, m is zero for scaledT < 0.5 and 1 for scaledT > 0.5, 
  // but smoothstep is used for the transitions at 0 +/- fuzz, .5 +/- fuzz, 
  // and 1 +/- fuzz.  We're assuming that fuzz is always less than 0.25.

  float m;

  if (scaledT < .25)
  {
    // 1 on the left of zero, 0 on the right of zero, transition downward
	// between -fuzz and +fuzz
    m = 1.0 - smoothstep(-fuzz, fuzz, scaledT);
  }
  else if (scaledT > .75)
  {
    // 1 on the left of 1, 0 on the right of 1, transition downward
	// between 1 - fuzz and 1 + fuzz
    m = 1.0 - smoothstep(1 - fuzz, 1 + fuzz, scaledT);
  }
  else
  {
    // 0 on the left of 0.5, 1 on the right of 0.5, transition
	// upward between 0.5 - fuzz and 0.5 + fuzz
    m = smoothstep(0.5 - fuzz, 0.5 + fuzz, scaledT);
  }

  diffuseSurface = (1 - m) * frontcolor + m * backcolor;
  //Alt: use the built-in function mix(frontcolor, backcolor, m);
#endif  

  diffuseSurface.a = 1.0;
  ambientSurface = diffuseSurface;
  specularSurface = vec4(1.0, 1.0, 1.0, 1.0);

  // rest of lighting calculation is the same as usual

  
  // have to normalize after interpolation
  vec3 N = normalize(fN);
  vec3 L = normalize(fL);
  vec3 V = normalize(fV);
  vec3 R = normalize(reflect(-L, N));
  
  // multiply material by light 
  vec4 ambientProduct = ambientLight * ambientSurface;
  vec4 diffuseProduct = diffuseLight * diffuseSurface;
  vec4 specularProduct = specularLight * specularSurface;  
  
  // ambient intensity
  vec4 ia = ambientProduct;
  
  // diffuse intensity
  float diffuseFactor = max(dot(L, N), 0.0);
  vec4 id = diffuseFactor * diffuseProduct;
  
  // specular intensity
  vec4 is = vec4(0.0, 0.0, 0.0, 1.0);  
  if (dot(L, N) >= 0.0) 
  {
    float specularFactor = pow(max(dot(R, V), 0.0), shininess); 
    is = specularFactor * specularProduct;
  }
  
  color = ia + id + is;
  color.a = 1.0;

} 


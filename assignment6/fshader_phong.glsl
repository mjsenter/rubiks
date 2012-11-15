#version 150

uniform mat3 materialProperties;
uniform mat3 lightProperties;
uniform float shininess;
uniform sampler2D texture;

in vec3 fN;
in vec3 fsL;
in vec3 fmL;
in vec3 fV;
in  vec2 fTexCoord;

out vec4 color;

void main() 
{ 
  
  // get sampled value from texture
  vec4 texColor = texture2D( texture, fTexCoord );
  
  // get rows from material and light properties
  mat3 material = transpose(materialProperties);
  mat3 light = transpose(lightProperties);
  vec4 ambientLight = vec4(light[0], 1.0);
  vec4 diffuseLight = vec4(light[1], 1.0);
  vec4 specularLight = vec4(light[2], 1.0);
  vec4 ambientSurface = vec4(material[0], 1.0);
  vec4 diffuseSurface = vec4(material[1], 1.0);
  vec4 specularSurface = vec4(material[2], 1.0);
  
  // 1)  Use texture as diffuse color
  diffuseSurface = texColor;
  
  // 2) Use texture to modulate diffuse color (try with the tarnish.tga texture)
  //diffuseSurface = diffuseSurface  * texColor;
  //diffuseSurface.a = 1.0;

  // 3) Use texture to modulate specular highlights only (try with the tarnish.tga texture)
  //specularSurface *= texColor;
  //specularSurface.a = 1.0;
  
  // 4) Blend with surface color using image alpha (try with the steve.png)
  //float alpha = texColor.a;
  //diffuseSurface = (1 - alpha) * diffuseSurface + alpha * texColor;
  //diffuseSurface.a = 1.0;
  
  // use same texture in ambient light
  ambientSurface = diffuseSurface; 


  // rest of lighting calculation is the same as usual

  // have to normalize after interpolation
  vec3 N = normalize(fN);
  vec3 sL = normalize(fsL);
  vec3 mL = normalize(fmL);
  vec3 V = normalize(fV);

  vec3 sR = normalize(reflect(-sL, N));
  vec3 mR = normalize(reflect(-mL, N));
  //Or use: vec3 R = normalize(2 * (dot(L, N)) * N - L);
    
  // multiply material by light 
  vec4 ambientProduct = ambientLight * ambientSurface;
  vec4 diffuseProduct = diffuseLight * diffuseSurface;
  vec4 specularProduct = specularLight * specularSurface;

  // ambient intensity
  vec4 ia = ambientProduct;
  
  // diffuse intensity
  float sDiffuseFactor = max(dot(sL, N), 0.0);
  float mDiffuseFactor = max(dot(mL, N), 0.0);
  vec4 id = sDiffuseFactor * diffuseProduct + mDiffuseFactor * diffuseProduct;
  
  // specular intensity
  vec4 is = vec4(0.0, 0.0, 0.0, 1.0);  
  if (dot(sL, N) >= 0.0) 
  {
    float sSpecularFactor = pow(max(dot(sR, V), 0.0), shininess); 
    is = sSpecularFactor * specularProduct;
  }

  if( dot(sL, N) >= 0.0 ) {
	float mSpecularFactor = pow(max(dot(mR, V), 0.0), shininess);
	is += mSpecularFactor * specularProduct;
  }
  
  color = ia + id + is;  
  color.a = 1.0;
} 


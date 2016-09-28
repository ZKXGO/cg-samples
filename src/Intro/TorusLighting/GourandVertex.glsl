#version 330 core

uniform vec4 LightPosition = vec4(0,0,0,1);

uniform vec4 MaterialSpecular = vec4(0.9,0.9,0.9,1.0);
uniform vec4 MaterialAmbient = vec4(0.2,0.2,0.2,1.0);
uniform vec4 MaterialDiffuse = vec4(0.7,0.7,0.7,1.0);
uniform float Shininess = 10.0;

uniform vec4 LightDiffuse = vec4(1.0,1.0,1.0,1.0);
uniform vec4 LightAmbient = vec4(0.2,0.2,0.2,1.0);

uniform mat4 mv;
uniform mat4 mvp;
uniform mat3 nm;

vec4 GetAmbientReflection(
	vec4 surfaceColor,
    vec4 lightColor) 
{
	return lightColor * surfaceColor;
}

vec4 GetDiffuseReflection(
	vec4 surfaceColor,
	vec3 surfaceNormal,
    vec4 lightColor,
	vec3 lightDirection) 
{
	float diffuseFactor = 
		clamp(
			dot(lightDirection, surfaceNormal),0.0,1.0);
	return lightColor * surfaceColor * diffuseFactor;
}

vec4 GetSpecularReflection(
	vec4 surfaceColor,
    float  surfaceShininess,
    vec3 surfaceNormal,
    vec4 lightColor,
    vec3 halfAngle) 
{
	float specularFactor = 
	   pow(clamp(dot(halfAngle, surfaceNormal),0.0,1.0), surfaceShininess);
  
	return lightColor * surfaceColor * specularFactor;       
}

vec4 GetBlinnReflection(
	vec4 ambientSurfaceColor,
    vec4 ambientLightColor,
    vec4 diffuseSurfaceColor,
    vec4 specularSurfaceColor,
    float  surfaceShininess,
    vec3 surfaceNormal,
    vec3 halfAngle,
    vec3 lightDirection,
    vec4 lightColor) {
                       
	vec4 ambient = GetAmbientReflection(ambientSurfaceColor, ambientLightColor);

	vec4 diffuse = GetDiffuseReflection(
		diffuseSurfaceColor, 
		surfaceNormal, 
		lightColor, 
		lightDirection);
                                     
	vec4 specular = GetSpecularReflection(
		specularSurfaceColor, 
		surfaceShininess, 
		surfaceNormal,
		lightColor, 
		halfAngle);

	if (dot(lightDirection, surfaceNormal) <= 0)
	{
		specular = vec4(0.0, 0.0, 0.0, 0.0);
	}
    
	return diffuse + specular + ambient;
}

in vec3 position;
in vec3 normal;
out vec4 vertexColor;

void main()
{
	vec4 pos =	mv * vec4(position, 1);	
	vec3 eyeSurfacePosition = pos.xyz / pos.w;
	vec3 eyeLightPosition   = LightPosition.xyz / LightPosition.w; 

	vec3 eyeViewerDirection = normalize(-eyeSurfacePosition);
	vec3 eyeSurfaceNormal   = normalize(nm * normal);
	vec3 eyeLightDirection  = normalize(eyeLightPosition - eyeSurfacePosition);
	vec3 eyeHalfAngle       = normalize(eyeViewerDirection + eyeLightDirection);

	vertexColor = GetBlinnReflection(
		MaterialDiffuse, 
		LightAmbient,
		MaterialDiffuse, 
		MaterialSpecular, 
		Shininess,
		eyeSurfaceNormal, 
		eyeHalfAngle,
		eyeLightDirection, 
		LightDiffuse);
		
    gl_Position = mvp * vec4(position, 1);
}

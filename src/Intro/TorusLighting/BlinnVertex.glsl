uniform vec4 LightPosition;

varying vec3 eyeSurfaceNormal;
varying vec3 eyeLightDirection;
//varying vec3 eyeViewerDirection;
varying vec3 eyeSurfacePosition;

void main()
{
	vec4 position =	gl_ModelViewMatrix * gl_Vertex;	
	eyeSurfacePosition = position.xyz / position.w;
	vec3 eyeLightPosition   = LightPosition.xyz / LightPosition.w; 

	//eyeViewerDirection = normalize(-eyeSurfacePosition);
	eyeSurfaceNormal   = normalize(gl_NormalMatrix * gl_Normal);
	eyeLightDirection  = normalize(eyeLightPosition - eyeSurfacePosition);
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
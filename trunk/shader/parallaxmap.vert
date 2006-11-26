// The vertex color passed
varying vec4 passColor;

// The transformed light direction and view direction, to pass to the fragment shader
varying vec3 directionalLightDir;
varying vec3 positionalLightDir;
varying vec3 viewDir;

// The inverse tangent, binormal and normal to the geometry int tangent space
attribute vec3 tangent;
attribute vec3 binormal;
attribute vec3 tbnNormal;

void main() 
{
	// Put the color in a varying variable
	passColor = gl_Color;
	
	// Put the vertex in the position passed
	gl_Position = ftransform(); 
	
	// Construct a 3x3 matrix from the geometry’s inverse tangent, binormal, and normal
	mat3 rotmat = mat3(tangent, binormal, tbnNormal);
	
	//rotmat = rotmat * gl_NormalMatrix;
	
	// Rotate the light into tangent space
	directionalLightDir = rotmat * normalize(gl_LightSource[0].position.xyz);
	// NOTE: The gl_LightSource[X].position.xyz is given already in World Position, so
	// we don't need to multiply it by the ModelViewMatrix
	positionalLightDir	= rotmat * normalize(gl_LightSource[1].position.xyz - (gl_ModelViewMatrix * gl_Vertex).xyz);
	
	// first, compute eye dir: it's a different vector from vertex to vertex.
	// the eye is supposedly at (0, 0, 0)
	// second, rotate it into tangent space
	viewDir  = (gl_ModelViewMatrix * gl_Vertex).xyz;
	viewDir  = rotmat * normalize( viewDir );
	
	// Use the first set of texture coordinates in the fragment shader 
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
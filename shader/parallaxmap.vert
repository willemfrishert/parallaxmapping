// The vertex color passed
varying vec4 passColor;

// The transformed light direction and view direction, to pass to the fragment shader
varying vec3 light0Dir;
varying vec3 light1Dir;
varying vec3 light2Dir;
varying vec3 viewDir;
varying float light0Distance;
varying float light1Distance;
varying float light2Distance;

// The inverse tangent, binormal and normal to the geometry int tangent space
attribute vec3 tangent;
attribute vec3 binormal;
attribute vec3 tbnNormal;

// ############### FUNCTIONS ########################
void computeLightParameters(vec3 viewDir, mat3 rotmat);

void main() 
{
	// Put the color in a varying variable
	passColor = gl_Color;
	
	// Put the vertex in the position passed
	gl_Position = ftransform(); 
	
	// Construct a 3x3 matrix from the geometry’s inverse tangent, binormal, and normal
	mat3 rotmat = mat3(tangent, binormal, tbnNormal);

	// Compute eye dir: it's a different vector from vertex to vertex.
	// the eye is supposedly at (0, 0, 0)
	viewDir  = (gl_ModelViewMatrix * gl_Vertex).xyz;
	
	// ##################
	computeLightParameters(viewDir, rotmat);
	
	// Rotate the view direction into tangent space
	viewDir  = rotmat * normalize( viewDir );
	
	// Use the first set of texture coordinates in the fragment shader 
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

void computeLightParameters(vec3 viewDir, mat3 rotmat)
{
	// Rotate the light into tangent space
	// NOTE: The gl_LightSource[X].position.xyz is given already in World Position, so
	// we don't need to multiply it by the ModelViewMatrix
	// 1st: Compute vector from light to vertex
	// 2nd: Compute distance, using the vector's norm
	// 3rd: Transform it into tangent space
	light0Dir = gl_LightSource[0].position.xyz - viewDir;
	light0Distance = length( light0Dir );
	light0Dir = rotmat * normalize( light0Dir );

	light1Dir = gl_LightSource[1].position.xyz - viewDir;
	light1Distance = length( light1Dir );
	light1Dir	= rotmat * normalize( light1Dir );

	light2Dir = gl_LightSource[2].position.xyz - viewDir;
	light2Distance = length( light2Dir );
	light2Dir	= rotmat * normalize( light2Dir );
}
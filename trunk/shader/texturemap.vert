varying vec3 normal;

// The transformed light direction and view direction, to pass to the fragment shader
varying vec3 light0Dir;
varying vec3 light1Dir;
varying vec3 light2Dir;
varying float light0Distance;
varying float light1Distance;
varying float light2Distance;

void computeLightParameters(vec3 viewDir);

void main() 
{
	//Put the color in a varying variable
	normal = gl_NormalMatrix * gl_Normal;
	
	//Put the vertex in the position passed
	gl_Position = ftransform(); 
	
	vec3 viewDir  = (gl_ModelViewMatrix * gl_Vertex).xyz;

	// ##################
	computeLightParameters(viewDir);
	
	//Use the first set of texture coordinates in the fragment shader 
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

void computeLightParameters(vec3 viewDir)
{
	// Rotate the light into tangent space
	// NOTE: The gl_LightSource[X].position.xyz is given already in World Position, so
	// we don't need to multiply it by the ModelViewMatrix
	// 1st: Compute vector from light to vertex
	// 2nd: Compute distance, using the vector's norm
	// 3rd: Transform it into tangent space
	light0Dir = gl_LightSource[0].position.xyz - viewDir;
	light0Distance = length( light0Dir );

	light1Dir = gl_LightSource[1].position.xyz - viewDir;
	light1Distance = length( light1Dir );

	light2Dir = gl_LightSource[2].position.xyz - viewDir;
	light2Distance = length( light2Dir );
}
// The height map and texture map itself 
uniform sampler2D heightMap; 
uniform sampler2D textureMap;

//The vertex color passed
varying vec4 passColor;

// The transformed light direction and view direction
varying vec3 lightDir;
varying vec3 viewDir;

// The geometric normal
varying vec3 normal;

void main() 
{
	vec3 lightDirNormalized = normalize( lightDir );
	vec3 viewDirNormalized = normalize( viewDir );
	vec3 normalNormalized = normalize( normal );
	
	// the initial/input texture coordinate
	// NOTE: this is the coordinate that would be used
	// for basic texture mapping 
	vec3 t0 = vec3(gl_TexCoord[0].xy, 0.0);
	
	// calculate number of samples n
	// n = n_min + N.V_ts*(n_max-n_min)
	// see page 66 ATI parallax paper
	float n = 8 + dot(normalNormalized, viewDirNormalized)*24;
	
	// the step size
	float delta = 1 / n;
	
	// stepVector is the interval stride
	vec3 stepVec = viewDirNormalized * delta;
	
	vec3 prevSampleVec = t0;
	vec3 currSampleVec = t0 + stepVec;
	
	float prevHeight = texture2D(heightMap, prevSampleVec);
	float currHeight = texture2D(heightMap, currSampleVec);
	
	float offsetLimit = something;
	
	while( currSamplePoint < offsetLimit )
	{
		// test for intersection
		// otherwise continue...
		if( marchVector.z > currHeight )
		
		marchVector += stepVector;
		prevSamplePoint = currSamplePoint;
		currSamplePoint += delta;
	}
}
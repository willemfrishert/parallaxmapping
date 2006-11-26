// The height map, normal map and texture map itself 
uniform sampler2D heightMap; 
uniform sampler2D textureMap;
uniform sampler2D normalMap;

//The vertex color passed
varying vec4 passColor;

// The transformed light direction and view direction
varying vec3 directionalLightDir;
varying vec3 positionalLightDir;
varying vec3 viewDir;

// ############### FUNCTIONS ########################
vec3 getScaledBiasedNormal(vec2 texCoord);
float computeDiffuse(vec3 directionalLightDir, vec3 positionalLightDir, 
					vec3 normal, int directionalId, int positionalId);

void main() 
{
	vec3 directionalLightDirNormalized = normalize( directionalLightDir );
	vec3 positionalLightDirNormalized = normalize( positionalLightDir );
	vec3 viewDirNormalized = normalize( viewDir );
	//vec3 normalNormalized = normalize( normal );
	
	// the initial/input texture coordinate
	// NOTE: this is the coordinate that would be used
	// for basic texture mapping 
	vec2 t0 = gl_TexCoord[0].xy;
	
	float hSB = 0.04 * (texture2D(heightMap, t0).x - 0.5);

	// adding the biased and scaled height (multiplied by the cos)
	// ### The texture2D(normalMap, t0).z just shrinks the bump height
	// ### The viewDirNormalized.xy because we just need the XY component
	vec2 tOffset = t0 + hSB * texture2D(normalMap, t0).z * viewDirNormalized.xy;
	
	// Get the displaced normal of the normal map
	vec3 bumpNormal = getScaledBiasedNormal( tOffset );
	
	// Find the dot product between the light direction and the normal
	float NdotL = computeDiffuse(directionalLightDirNormalized, positionalLightDirNormalized, 
					bumpNormal, 0, 1);
	
	vec4 parallaxColor = texture2D(textureMap, tOffset);
	
	// Calculate the final color gl_FragColor
	vec4 diffuse = vec4(NdotL * parallaxColor.rgb, parallaxColor.a);
	//diffuse = vec4(NdotL, NdotL, NdotL, 1.0);

	gl_FragColor = diffuse;
}

vec3 getScaledBiasedNormal(vec2 texCoord)
{
	vec3 bumpNormal = vec3(texture2D(normalMap, texCoord));
	
	// scaling and biasing because the bump normal was reverse scaled
	// to [0, 1] before it was saved
	bumpNormal = (bumpNormal - 0.5) * 2.0;
	bumpNormal = normalize( bumpNormal );

	// the bumpNormal is already in tangent space, so it doesn't need
	// any additional transformation at all
	return bumpNormal;
}

float computeDiffuse(vec3 directionalLightDir, vec3 positionalLightDir, 
					vec3 normal, int directionalId, int positionalId)
{
	// Find the dot product between the light direction and the normal
	float directionalIntensity = max(dot(normal, directionalLightDir), 0.0);
	float positionalIntensity = max(dot(normal, positionalLightDir), 0.0);

	return /*directionalIntensity + */positionalIntensity;
}
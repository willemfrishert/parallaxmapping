// The height map, normal map and texture map itself 
uniform sampler2D heightMap; 
uniform sampler2D textureMap;
uniform sampler2D normalMap;

//The vertex color passed
varying vec4 passColor;

// The transformed light direction and view direction
varying vec3 lightDir;
varying vec3 viewDir;

// The geometric normal
varying vec3 normal;


// ############# FUNCTIONS ###############

void main() 
{
	vec3 lightDirNormalized = normalize( lightDir );
	vec3 viewDirNormalized = normalize( viewDir );
	vec3 normalNormalized = normalize( normal );
	
	// the initial/input texture coordinate
	// NOTE: this is the coordinate that would be used
	// for basic texture mapping 
	vec2 t0 = gl_TexCoord[0].xy;
	
	float hSB = 0.07 * (texture2D(heightMap, t0).x - 0.5);
	
	vec2 tN = t0 + hSB * viewDirNormalized.xy;
	
	// Get the displaced normal of the normal map
	vec3 bumpNormal = vec3(texture2D(normalMap, tN));
	
	// Expand the bump-map into a normalized unsigned vector float
	bumpNormal = (bumpNormal - 0.5) * 2.0;
	
	//bumpNormal = normalize( bumpNormal );
	
	// Find the dot product between the light direction and the normal
	float NdotL = max(dot(bumpNormal, lightDirNormalized), 0.0);
	
	vec4 parallaxColor = texture2D(textureMap, tN);
	
	// Calculate the final color gl_FragColor
	vec3 diffuse = NdotL * parallaxColor.rgb; 
	
	gl_FragColor = vec4(diffuse, parallaxColor.a);
//	gl_FragColor = parallaxColor;
	//gl_FragColor = vec4(NdotL,NdotL,NdotL,1.0);
}
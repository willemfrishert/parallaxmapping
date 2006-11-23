// The height map, normal map and texture map itself 
uniform sampler2D heightMap; 
uniform sampler2D textureMap;
uniform sampler2D normalMap;

//The vertex color passed
varying vec4 passColor;

// The transformed light direction and view direction
varying vec3 lightDir;
varying vec3 viewDir;

void main() 
{
	vec3 lightDirNormalized = normalize( lightDir );
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
	vec3 bumpNormal = vec3(texture2D(normalMap, tOffset));
	
	// scaling and biasing because the bump normal was reverse scaled
	// to [0, 1] before it was saved
	bumpNormal = (bumpNormal - 0.5) * 2.0;
	bumpNormal = normalize( bumpNormal );

	bumpNormal = gl_NormalMatrix * bumpNormal;
	
	// Find the dot product between the light direction and the normal
	float NdotL = max(dot(bumpNormal, lightDirNormalized), 0.0);
	
	vec4 parallaxColor = texture2D(textureMap, tOffset);
	
	// Calculate the final color gl_FragColor
	vec3 diffuse = NdotL * parallaxColor.rgb; 
	
	gl_FragColor = vec4(diffuse, parallaxColor.a);
//	gl_FragColor = vec4(NdotL, NdotL, NdotL, 1.0);
//	gl_FragColor = vec4(bumpNormal, 1.0);
//	gl_FragColor = texture2D(normalMap, t0);
}
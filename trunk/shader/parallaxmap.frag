// The height map, normal map and texture map itself 
uniform sampler2D heightMap; 
uniform sampler2D textureMap;
uniform sampler2D normalMap;

//The vertex color passed
varying vec4 passColor;

// The transformed light direction and view direction
varying vec3 light0Dir;
varying vec3 light1Dir;
varying vec3 light2Dir;
varying float light0Distance;
varying float light1Distance;
varying float light2Distance;

// The view direction in tangent space
varying vec3 viewDir;

// ############### FUNCTIONS ########################
vec3 getScaledBiasedNormal(vec2 texCoord);
vec4 computeDiffuse(vec3 normal, vec4 textureColor);

void main() 
{
	vec3 light0DirNormalized = normalize( light0Dir );
	vec3 light1DirNormalized = normalize( light1Dir );
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
	
	vec4 parallaxColor = texture2D(textureMap, tOffset);

	// Find the dot product between the light direction and the normal
	vec4 color = computeDiffuse(bumpNormal, parallaxColor);
	
	// Calculate the final color gl_FragColor
	//vec4 diffuse = vec4(NdotL * parallaxColor.rgb, parallaxColor.a);
	//vec4 diffuse = vec4(color + parallaxColor.rgb, parallaxColor.a);
	//diffuse = vec4(NdotL, NdotL, NdotL, 1.0);

	gl_FragColor = color;
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

vec4 computeDiffuse(vec3 normal, vec4 textureColor)
{
	// Find the dot product between the light direction and the normal
	float light0Intensity = max(dot(normal, light0Dir), 0.0);
	float light1Intensity = max(dot(normal, light1Dir), 0.0);
	float light2Intensity = max(dot(normal, light2Dir), 0.0);

	vec3 diffuse0 = gl_LightSource[0].diffuse.rgb;
	vec3 diffuse1 = gl_LightSource[1].diffuse.rgb;
	vec3 diffuse2 = gl_LightSource[2].diffuse.rgb;

	vec3 finalColor = light0Intensity * diffuse0 * textureColor.rgb * (1.0 / (light0Distance * 0.3))
		+ light1Intensity * diffuse1 * textureColor.rgb
		+ light2Intensity * diffuse2 * textureColor.rgb * (1.0 / (light2Distance * 0.5));

	return vec4(clamp(finalColor, 0.0, 1.0), textureColor.a);
}
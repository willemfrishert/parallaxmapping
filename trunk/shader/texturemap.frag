uniform sampler2D textureMap;

// The transformed light direction and view direction
varying vec3 light0Dir;
varying vec3 light1Dir;
varying vec3 light2Dir;
varying float light0Distance;
varying float light1Distance;
varying float light2Distance;

varying vec3 normal;

vec4 computeDiffuse(vec3 normal, vec4 textureColor);

void main() 
{
	//Get the color of the texture
	vec4 decalCol = texture2D(textureMap, gl_TexCoord[0].xy);

	// Find the dot product between the light direction and the normal
	vec4 color = computeDiffuse(normal, decalCol);

	//Set the color of the fragment...  If you want specular lighting or other types add it here
	gl_FragColor = vec4(normal, 1.0);
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
		+ light1Intensity * diffuse1 * textureColor.rgb * (1.0 / (light1Distance * 0.15))
		+ light2Intensity * diffuse2 * textureColor.rgb * (1.0 / (light2Distance * 0.15));

	return vec4(clamp(finalColor, 0.0, 1.0), textureColor.a);
}
uniform sampler2D BumpTex; //The bump-map 
uniform sampler2D DecalTex; //The texture
varying vec4 passcolor; //Receiving the vertex color from the vertex shader
varying vec3 LightDir; //Receiving the transformed light direction 
void main() 
{
	//Get the color of the bump-map
	vec3 BumpNorm = vec3(texture2D(BumpTex, gl_TexCoord[0].xy));
	
	//Get the color of the texture
	vec3 DecalCol = vec3(texture2D(DecalTex, gl_TexCoord[0].xy));
	
	//Expand the bump-map into a normalized unsigned vector float
	BumpNorm = (BumpNorm -0.5) * 2.0;
	
	//Find the dot product between the light direction and the normal
	float NdotL = max(dot(BumpNorm, LightDir), 0.0);
	
	//Calculate the final color gl_FragColor
	vec3 diffuse = NdotL * passcolor.xyz * DecalCol;
	
	//Set the color of the fragment...  If you want specular lighting or other types add it here
	vec4(diffuse, passcolor.w);
}
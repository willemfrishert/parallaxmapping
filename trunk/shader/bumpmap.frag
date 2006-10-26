uniform sampler2D bumpTex; //The bump-map 
uniform sampler2D decalTex; //The texture
varying vec4 passColor; //Receiving the vertex color from the vertex shader
varying vec3 lightDir; //Receiving the transformed light direction 
void main() 
{
	//Get the color of the bump-map
	vec3 bumpNorm = vec3(texture2D(bumpTex, gl_TexCoord[0].xy));
	
	//Get the color of the texture
	vec3 decalCol = vec3(texture2D(decalTex, gl_TexCoord[0].xy));
	
	//Expand the bump-map into a normalized unsigned vector float
	bumpNorm = (bumpNorm -0.5) * 2.0;
	
	//Find the dot product between the light direction and the normal
	float NdotL = max(dot(bumpNorm, lightDir), 0.0);
	
	//Calculate the final color gl_FragColor
	vec3 diffuse = NdotL * passColor.xyz * decalCol;
	
	//Set the color of the fragment...  If you want specular lighting or other types add it here
	vec4(diffuse, passColor.w);
}
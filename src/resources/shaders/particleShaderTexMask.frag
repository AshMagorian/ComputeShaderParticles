#version 430 core

in vec2 ex_TexCoord;
in vec4 ex_Color;
uniform sampler2D in_Texture;

void main()
{
	vec4 tex = texture2D(in_Texture, ex_TexCoord);
	if(tex.a < 0.1) {discard;}
		else {	tex.r = ex_Color.r;
				tex.g = ex_Color.g;
				tex.b = ex_Color.b;}
	gl_FragColor = tex;
}
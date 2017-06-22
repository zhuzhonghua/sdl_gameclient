#version 130
//The fragment shader operates on each pixel in a given polygon
in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

uniform sampler2D mySampler;

void main() {
	//vec4 texColor = texture(mySampler, fragmentUV);
	
    color = texture(mySampler, fragmentUV);
}
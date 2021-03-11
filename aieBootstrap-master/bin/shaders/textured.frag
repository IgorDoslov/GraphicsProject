// a simple shader for flat colours
#version 410

uniform sampler2D diffuseTexture;
in vec2 vTexCoord;
out vec4 FragColour;

void main(){
    FragColour = texture(diffuseTexture, vTexCoord);
}
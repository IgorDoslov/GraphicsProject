// a simple shader for flat colours
#version 400

out vec4 FragColour;
in vec4 simpleFlatColour;

void main(){
    FragColour = simpleFlatColour;
    //gl_FragColor = simpleFlatColor;
}


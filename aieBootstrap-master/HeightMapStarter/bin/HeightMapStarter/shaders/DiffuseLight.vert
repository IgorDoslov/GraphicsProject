#version 400

in vec4 vPosition;
in vec4 vColor;
in vec2 vuv;

out vec2 fuv;
out vec4 fColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main ()
{
	fColor = vColor;
	fuv = vuv;
	gl_Position = projection * view * model * vPosition;
}
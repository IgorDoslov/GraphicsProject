// a simple shader
#version 400

in vec4 Position;

uniform mat4 ProjectionViewModel;
uniform vec4 MeshFlatColour;
out vec4 simpleFlatColour;

void main(){
    if(MeshFlatColour != 0)
    {
      simpleFlatColour = MeshFlatColour;
    }
    else
    {
        simpleFlatColour = vec4(1, 0, 1, 1);
    }

    gl_Position = ProjectionViewModel * Position;
}


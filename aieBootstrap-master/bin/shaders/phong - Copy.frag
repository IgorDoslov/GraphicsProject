// Phong shader for simple game lighting
#version 410

in vec4 vPosition;
in vec3 vNormal;

uniform vec3 Ka; // THe ambient colour of the model's material
uniform vec3 Kd; // THe diffuse colour of the model's material
uniform vec3 Ks; // The specular colour of the model's material
uniform float Ns; // The specular power of the model's material

uniform vec3 AmbientColour; // Ambient colour of the light
uniform vec3 LightColour; // Colour of the light
uniform vec3 LightDirection;

uniform vec3 CameraPosition; // Position of the viewport camera for specular calculations

out vec4 FragColour;

void main(){
// Make sure the normal and the light directions have been normalised
vec3 N = normalize(vNormal);
vec3 L = normalize(LightDirection);

// Now we can calculate the lambert term, negative the light direction
float lambertTerm = max(0, min(1,dot(N, -L)));

// Calculate the view vector and the reflection vector
vec3 V = normalize(CameraPosition - vPosition.xyz);
vec3 R = reflect(L, N);

// Determing the value of the specular term
float specularTerm = pow(max(0, dot(R,V)), 32);

// Determine the value of the ambient
vec3 ambient = AmbientColour * Ka;

// Determine the value of the diffuse
vec3 diffuse = LightColour * Kd * lambertTerm;

// Determine the value of the specular
vec3 specular = LightColour * Ks * specularTerm;

//Output the final colour
    FragColour = vec4(ambient + diffuse + specular, 1);
}
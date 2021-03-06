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

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColour[MAX_LIGHTS];
uniform vec3 PointLightPositions[MAX_LIGHTS];

uniform vec3 CameraPosition; // Position of the viewport camera for specular calculations

out vec4 FragColour;

vec3 diffuse(vec3 direction, vec3 colour, vec3 normal)
{
    return colour * max(0, dot(normal, -direction));
}

vec3 specular(vec3 direction, vec3 colour, vec3 normal, vec3 view)
{
    vec3 R = reflect(direction, normal);


    // Calculate the specular term
    float specTerm = pow(max(0, dot(R, view)), Ns);
    return specTerm * colour;
}

void main(){
// Make sure the normal and the light directions have been normalised
vec3 N = normalize(vNormal);
vec3 L = normalize(LightDirection);

// Now we can calculate the lambert term, negative the light direction
float lambertTerm = max(0, min(1,dot(N, -L)));

vec3 diffuseTotal = diffuse(L, LightColour, N);

// Calculate the view vector and the reflection vector
vec3 V = normalize(CameraPosition - vPosition.xyz);
vec3 R = reflect(L, N);

vec3 specularTotal = specular(L, LightColour, N, V);

for(int i = 0; i < numLights; i++)
{
    vec3 direction = vPosition.xyz - PointLightPositions[i];
    float distance = length(direction);
    
    direction = direction / distance;

    // Get the light intensity with the inverse square law
    vec3 colour = PointLightColour[i] / (distance * distance);

    diffuseTotal += diffuse(direction, colour, N);
    specularTotal += specular(direction, colour, N, V);

}

// Determing the value of the specular term
float specularTerm = pow(max(0, dot(R,V)), 32);

// Determine the value of the ambient
vec3 ambient = AmbientColour * Ka;

// Determine the value of the diffuse
vec3 diffuse = Kd * lambertTerm * diffuseTotal;

// Determine the value of the specular
vec3 specular = Ks * specularTotal;

//Output the final colour
    FragColour = vec4(ambient + diffuse + specular, 1);
}
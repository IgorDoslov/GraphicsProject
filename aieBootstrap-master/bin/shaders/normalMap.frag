// Phong shader for simple game lighting
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

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
vec3 T = normalize(vTangent);
vec3 B = normalize(vBiTangent);

mat3 TBN = mat3(T,B,N);

vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
vec3 texNormal = texture(normalTexture, vTexCoord).rgb;

N = TBN * (texNormal * 2 - 1);

// Calculate the diffuse lighting from sunlight
vec3 diffuseTotal = diffuse(L, LightColour, N);

// Now we can calculate the lambert term, negative the light direction
//float lambertTerm = max(0, min(1,dot(N, -L)));

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


// Determine the value of the ambient
vec3 ambient = AmbientColour * Ka * texDiffuse;

// Determine the value of the diffuse
vec3 diffuse = Kd * texDiffuse * diffuseTotal;

// Determine the value of the specular
vec3 specular = Ks * texSpecular *  specularTotal;


// Show the standard normals, turn off: N = TBN * (texNormal * 2 - 1)
 //   FragColour = vec4(N, 1);

// Show the normals
 //   FragColour = vec4(N, 1);

// Show the texture normals
//FragColour = vec4(texNormal, 1);

// Show without texture
//FragColour = vec4(ambient + lambertTerm + specular, 1);

//Output the final colour

FragColour = vec4(ambient + diffuse + specular, 1);
}
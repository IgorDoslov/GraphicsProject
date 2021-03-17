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

const int lightLimit = 3;

uniform vec3 AmbientColour; // Ambient colour of the light
uniform vec3 LightColour[lightLimit]; // Colour of the light
uniform vec3 LightDirection[lightLimit];
uniform int lightCount;
uniform vec3 CameraPosition; // Position of the viewport camera for specular calculations

out vec4 FragColour;

void main(){
// Make sure the normal and the light directions have been normalised
vec3 N = normalize(vNormal); // z axis
vec3 T = normalize(vTangent); // x axis 
vec3 B = normalize(vBiTangent); // y axis

mat3 TBN = mat3(T,B,N); // local space of pixel (basis matrix)

vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
vec3 texNormal = texture(normalTexture, vTexCoord).rgb;

N = TBN * (texNormal * 2 - 1); // Bringing texture's normal into world space

// Determine the value of the ambient
vec3 ambient = AmbientColour * Ka * texDiffuse;

vec3 diffuseTotal = vec3(0.0);
vec3 specularTotal = vec3(0.0);

for(int i = 0; i < lightCount; i++)
{
vec3 L = normalize(LightDirection[i]);
// Now we can calculate the lambert term, negative the light direction
float lambertTerm = clamp(dot(N, -L), 0.0, 1.0);

// Calculate the view vector and the reflection vector
vec3 V = normalize(CameraPosition - vPosition.xyz);
vec3 R = reflect(L, N);

// Determing the value of the specular term
float specularTerm = pow(clamp(-dot(R,V), 0.0, 1.0), Ns) * lambertTerm;

// Determine the value of the diffuse
 diffuseTotal += LightColour[i] * Kd * texDiffuse * lambertTerm;

// Determine the value of the specular
 specularTotal += LightColour[i] * Ks * texSpecular *  specularTerm;
}


// Show the standard normals, turn off: N = TBN * (texNormal * 2 - 1)
 //   FragColour = vec4(N, 1);

// Show the normals
 //   FragColour = vec4(N, 1);

// Show the texture normals
//FragColour = vec4(texNormal, 1);

// Show without texture
//FragColour = vec4(ambient + lambertTerm + specular, 1);

//Output the final colour

FragColour = vec4(ambient + diffuseTotal + specularTotal, 1);
}
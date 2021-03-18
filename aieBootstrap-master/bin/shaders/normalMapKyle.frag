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
 
uniform vec3 Ka; // The ambient color of the model's Material
uniform vec3 Kd; // the diffuse color of the model's Material
uniform vec3 Ks; // The specular color of the model's Material
uniform float Ns; //The specular power of the model's Material
 
uniform vec3 AmbientColour; // Ambient color of the light
uniform vec3 LightColour; //Color of the Light
uniform vec3 LightDirection;
 
const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColour[MAX_LIGHTS];
uniform vec3 PointLightPositions[MAX_LIGHTS];
 
uniform vec3 CameraPosition; // Position of the viewport camera for specular calculations
 
out vec4 FragColour;
 
vec3 diffuse (vec3 direction, vec3 colour, vec3 normal)
{
 
    return color * max(0, dot(normal, -direction));
}
vec3 specular (vec3 direction, vec3 colour, vec3 normal, vec3 view)
{
    vec3 R = reflect(direction, normal);
 
    //Calculate the specular term
 
    float specTerm = pow(max(0, dot(R,view)), Ns);
 
    return specTerm * colour;
}
void main() {
 
    //Make sure the normal and the light directions have been normalised
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);
    vec3 L1 = normalize(LightDirection1);
 
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
    float lambertTerm = max(0, min(1, dot(N,-L)));
    //float lambertTerm1 = max(0, min(1, dot(N,-L1)));
 
    //Calculate the view vector and the reflection vector
    vec3 V = normalize(CameraPosition - vPosition.xyz);
    vec3 R = reflect(L,N);
 
    vec3 specularTotal = specular(L, LightColour, N, V);
    //vec3 R1 = reflect(L1,N);
 
    // Determining the value of the specular term
    //float specularTerm = pow(max(0,dot(R, V)), Ns) * lambertTerm;
    //float specularTerm1 = pow(max(0,dot(R1, V)), Ns) * lambertTerm1;
 
    for(int i = 0; i < numLights; i++)
    {
        vec3 direction = vPosition.xyz - PointLightPositions[i];
        float distance = length(direction);
        direction = direction / distance;
        //Get the lights intensity with the inverse square law
        vec3 colour = PointLightColour[i] / (distance * distance);
 
        diffuseTotal += diffuse(direction, colour, N);
        specularTotal += specular(direction, colour, N, V);
 
    }
    //Determine the value of the ambient
    vec3 ambient = AmbientColour * Ka * texDiffuse;
 
    //Deteremine the valuse of the diffuse
    vec3 diffuse = LightColour * Kd * texDiffuse * diffuseTotal;
    //vec3 diffuse1 = LightColor1 * Kd * texDiffuse * lambertTerm1;
 
    //Determine the value of the specular
    vec3 specular = LightColuor * Ks * texSpecular * specularTotal;
    //vec3 specular1 = LightColor1 * Ks * texSpecular * specularTerm1;
 
    //Show the standard normals, turn off: N = TBN * (texNormal * 2 - 1);
    //FragColor = vec4(N,1);
 
    //Show the standard normals
 
    //FragColor = vec4(N,1);
 
    //Show texture normals
    //FragColor = vec4(texNormal, 1);
 
    //Show without texture
    //FragColor = vec4(ambient + lambertTerm + specular, 1);
 
    //Output the final color
 
    FragColour = vec4(ambient + diffuse + specular, 1);
}
#version 130

in vec4 position; // This will be the position of the vertex in model-space
in vec4 normal; // The normal data may not have been set
in ivec4 material;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec4 worldPosition; //This is being passed to the fragment shader to calculate the normals
//out vec3 normalFromVS;
flat out ivec4 materialFromVS;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
    worldPosition  = position;
    materialFromVS = material;
}

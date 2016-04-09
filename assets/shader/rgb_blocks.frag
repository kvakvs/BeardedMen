#version 150

//in vec4 worldPosition; //Passed in from the vertex shader
//in vec3 normalFromVS;
flat in ivec4 materialFromVS;

out vec4 outputColor;

void main()
{
    outputColor = vec4(materialFromVS.x / 255.0,
                       materialFromVS.y / 255.0,
                       materialFromVS.z / 255.0,
                       materialFromVS.w / 255.0);
}

#version 150

in vec4 worldPosition;
//in vec3 normalFromVS;
flat in ivec4 materialFromVS;

out vec4 outputColor;

void main()
{
    vec3 normal = cross(dFdy(worldPosition.xyz), dFdx(worldPosition.xyz));
    normal = normalize(normal);

    // Quick and dirty lighting, obviously a real implementation
    // should pass light properties as shader parameters, etc.
    vec3 lightDir = vec3(0.33, -0.66, 1.0);
    float diffuse = clamp(dot(lightDir, normal), 0.0, 1.0);
    diffuse *= 0.4; // Dim the diffuse a bit
    float ambient = 0.6; // Add some ambient
    float lightIntensity = diffuse + ambient; // Compute the final light intensity

    outputColor = vec4(materialFromVS.x / 255.0,
                       materialFromVS.y / 255.0,
                       materialFromVS.z / 255.0,
                       materialFromVS.w / 255.0) * lightIntensity;
}

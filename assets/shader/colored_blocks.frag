#version 150

in vec4 worldPosition;
//in vec3 normalFromVS;
flat in ivec2 materialFromVS;

out vec4 outputColor;

void main()
{
	// The first byte of our voxel data is the material.
	// We use this to decide how to color the fragment.
        vec4 surfaceColor;
        switch(materialFromVS.x)
	{
	case 1:
                surfaceColor = vec4(0x4F/255.0, 0x7B/255.0, 0x41/255.0, 1.0); // Swamp/wet grassland
		break;
	case 2:
                surfaceColor = vec4(0x9D/255.0, 0xA6/255.0, 0x7B/255.0, 1.0); // Dry grassland
		break;
	case 3:
                surfaceColor = vec4(0xC2/255.0, 0xBE/255.0, 0x96/255.0, 1.0); // Steppes
		break;
	case 4:
                surfaceColor = vec4(0xCB/255.0, 0x9D/255.0, 0x6B/255.0, 1.0); // Sand land
		break;
	case 5:
                surfaceColor = vec4(0x93/255.0, 0x76/255.0, 0x4F/255.0, 1.0); // Mountain/brown
		break;
        default: // air? mostly air = 0 - pink!
                surfaceColor = vec4(1.0, 0.0, 1.0, 1.0);
		break;
	}

        //outputColor = surfaceColor;

        vec3 normal = cross(dFdy(worldPosition.xyz), dFdx(worldPosition.xyz));
        normal = normalize(normal);

        // Quick and dirty lighting, obviously a real implementation
        // should pass light properties as shader parameters, etc.
        vec3 lightDir = vec3(0.33, -0.66, 1.0);
        float diffuse = clamp(dot(lightDir, normal), 0.0, 1.0);
        diffuse *= 0.7; // Dim the diffuse a bit
        float ambient = 0.3; // Add some ambient
        float lightIntensity = diffuse + ambient; // Compute the final light intensity

        outputColor = surfaceColor * lightIntensity; //Compute final rendered color
}

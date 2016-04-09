#version 150

//in vec4 worldPosition; //Passed in from the vertex shader
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

        // Again, for the purposes of these examples we cannot be sure that per-vertex normals are provided. A sensible fallback
        // is to use this little trick to compute per-fragment flat-shaded normals from the world positions using derivative operations.
        //vec3 normal = normalize(cross(dFdy(worldPosition.xyz), dFdx(worldPosition.xyz)));
        //outputColor = vec4(abs(normal) * 0.5, 1.0) * surfaceColor;

        outputColor = surfaceColor;

        //vec3 lightDir = vec3(0, 0, 1.0);
        //float diffuse = clamp(dot(lightDir, normalFromVS), 0.0, 1.0);
        //diffuse *= 0.7; // Dim the diffuse a bit
        //float ambient = 0.3; // Add some ambient
        //float lightIntensity = diffuse + ambient; // Compute the final light intensity

        //outputColor = surfaceColor * lightIntensity; //Compute final rendered color
}

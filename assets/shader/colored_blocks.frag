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
                surfaceColor = vec4(1.0, 0.3, 0.3, 1.0);
		break;
	case 2:
                surfaceColor = vec4(0.3, 1.0, 0.3, 1.0);
		break;
	case 3:
                surfaceColor = vec4(0.3, 0.3, 1.0, 1.0);
		break;
	case 4:
                surfaceColor = vec4(1.0, 1.0, 0.3, 1.0);
		break;
	case 5:
                surfaceColor = vec4(1.0, 0.3, 1.0, 1.0);
		break;
	default:
                surfaceColor = vec4(1.0, 1.0, 1.0, 1.0);
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

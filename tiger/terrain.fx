cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
}

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
}

Texture2D shaderTexture;
SamplerState SampleType;

struct VertexInputType
{
    float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;    
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

PixelInputType TerrainVertexShader(VertexInputType input)
{
    PixelInputType output;
        
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Pass texture coordinates to pixel shader
	output.tex = input.tex;
        
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	return output;
}

float4 TerrainPixelShader(PixelInputType input) : SV_Target
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	// Sample pixel color from the texture
	textureColor = shaderTexture.Sample(SampleType, input.tex);	

	// Set default output color to ambient light value for all pixels
	color = ambientColor;

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		// Determine final diffuse color
		color += (diffuseColor * lightIntensity);
	}
	
    // Saturate the final color in case it's greater then 1
    color = saturate(color);

	// Multiplay texture pixel with final light color
	color = color * textureColor;
	
    return color;
}

technique10 TerrainTechnique
{
    pass pass0
    {
        SetVertexShader(CompileShader(vs_4_0, TerrainVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, TerrainPixelShader()));
        SetGeometryShader(NULL);
    }
}

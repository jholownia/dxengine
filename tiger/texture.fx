/*
================================
 texture.fx

 Created on: 20 Jan 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

// Globals
matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;
Texture2D shaderTexture;

// Sample states
SamplerState SampleType
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

// Typedefs
struct VertexInput
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// Vertex shader
PixelInput TextureVertexShader(VertexInput input)
{
	PixelInput output;

	// Change the position vector to be 4 units for pronper matrix calculations
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view and projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader
	output.tex = input.tex;
	
	return output;
}

// Pixel shader
float4 TexturePixelShader(PixelInput input) : SV_Target
{
	float4 color;

	// Sample the texture pixel at this location
	color = shaderTexture.Sample(SampleType, input.tex);

	return color;
}

// Technique
technique10 TextureTechnique
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, TextureVertexShader()));
		SetPixelShader(CompileShader(ps_4_0, TexturePixelShader()));
		SetGeometryShader(NULL);
	}
}

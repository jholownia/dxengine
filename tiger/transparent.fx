/*
================================
 transparent.fx

 Created on: 20 Mar 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

// Globals
matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;
Texture2D shaderTexture;

// Blend amount to set transparency
float blendAmount;

// Sample states
SamplerState SampleType
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

// Blending states
BlendState AlphaBlendingOn
{
	BlendEnable[0] = TRUE;
	DestBlend = INV_SRC_ALPHA;
	SrcBlend = SRC_ALPHA;
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
PixelInput TransparentVertexShader(VertexInput input)
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
float4 TransparentPixelShader(PixelInput input) : SV_Target
{
	float4 color;

	// Sample the texture pixel at this location
	color = shaderTexture.Sample(SampleType, input.tex);

	// Use blendAmount variable to set the transparency through alpha value
	color.a = blendAmount;

	return color;
}

// Technique
technique10 TransparentTechnique
{
	pass pass0
	{
		// Here we set blending state to the one created at the top
		// The blending will be turned on until explicitly turned off which means that this shader needs
		// to be used last, or any shaders rendering after this one need to set their own blending states.
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, TransparentVertexShader()));
		SetPixelShader(CompileShader(ps_4_0, TransparentPixelShader()));
		SetGeometryShader(NULL);
	}
}

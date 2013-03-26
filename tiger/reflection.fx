/*
================================
 reflection.fx

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

// Reflection matrix and texture
matrix reflectionMatrix;
Texture2D reflectionTexture;

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
	float4 reflectionPosition : TEXCOORD1;
};

// Vertex shader
PixelInput ReflectionVertexShader(VertexInput input)
{
	PixelInput output;
	matrix reflectProjectWorld;

	// Change the position vector to be 4 units for pronper matrix calculations
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view and projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader
	output.tex = input.tex;

	// Create a matrix for transforming input position into reflection position
	reflectProjectWorld = mul(reflectionMatrix, projectionMatrix);
	reflectProjectWorld = mul(worldMatrix, reflectProjectWorld);

	// Calculate the input position against the reflectProjectWorld matrix
	output.reflectionPosition = mul(input.position, reflectProjectWorld);
		
	return output;
}

// Pixel shader
float4 ReflectionPixelShader(PixelInput input) : SV_Target
{
	float4 textureColor;
	float2 reflectTexCoord;
	float4 reflectionColor;
	float4 color;
	
	// Sample the texture pixel at this location
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	// Calculate the projected reflection texture coordinates
	reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
	reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;

	// Sample the texture pixel from the reflection texture using the projected texture coordinates
	reflectionColor = reflectionTexture.Sample(SampleType, reflectTexCoord);

	// Blend textures using linear interpolation
	color = lerp(textureColor, reflectionColor, 0.2f);

	return color;
}

// Technique
technique10 ReflectionTechnique
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, ReflectionVertexShader()));
		SetPixelShader(CompileShader(ps_4_0, ReflectionPixelShader()));
		SetGeometryShader(NULL);
	}
}

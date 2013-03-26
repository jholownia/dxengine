// Font.fx is the shader for rendering 2D text on top of our 3D scenes.

////////////////////////////////////////////////////////////////////////////////
// Filename: font.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;
Texture2D shaderTexture;

// One of the new global variables in the shader is pixelColor. We use this to control the color of the pixel that will be used to draw the font.
float4 pixelColor;


///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

// The second change to the shader is the new blend state we create called AlphaBlendingState. Blending allows the font to blend with the 3D objects in the background. If we don't turn on blending we see the black triangles behind the text. But with blending turned on only the pixels for the text show up on the screen and the rest of the triangle is completely see-through. The structure for the blend state is called D3D10_BLEND_DESC. In the blend state here I use the default settings except for the destination blend which I change to invert source alpha. I won't get into great detail about blending here but a simple blend was needed for this tutorial to work correctly.

/////////////////////
// BLENDING STATES //
/////////////////////
BlendState AlphaBlendingState
{
    BlendEnable[0] = TRUE;
    DestBlend = INV_SRC_ALPHA;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType FontVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    return output;
}

// The FontPixelShader first samples the font texture to get the pixel. If it samples a pixel that is black then it is just part of the background triangle and not a text pixel. In this case we set the alpha of this pixel to zero so when the blending is calculated it will determine that this pixel should be see-through. If the color of the input pixel is not black then it is a text pixel. In this case we multiply it by the pixelColor to get the pixel colored the way we want and then draw that to the screen.

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 FontPixelShader(PixelInputType input) : SV_Target
{
    float4 color;
	
	
    // Sample the texture pixel at this location.
    color = shaderTexture.Sample(SampleType, input.tex);
	
    // If the color is black on the texture then treat this pixel as transparent.
    if(color.r == 0.0f)
    {
        color.a = 0.0f;
    }
	
    // If the color is other than black on the texture then this is a pixel in the font so draw it using the font pixel color.
    else
    {
        color.a = 1.0f;
        color = color * pixelColor;
    }

    return color;
}

// The FontTechnique is different in that we turn on the alpha blending state using SetBlendState. The first parameter is the blend state we created in this shader. The second two parameters are just defaults. Note that if you do turn on blending here in this shader then it is on permanently until another shader turns it off. Changing the blending state is fairly expensive also so it shouldn't be done too often, the code should be organized such that scenes are drawn with minimal blending state changes.

////////////////////////////////////////////////////////////////////////////////
// Technique
////////////////////////////////////////////////////////////////////////////////
technique10 FontTechnique
{
    pass pass0
    {
        SetBlendState(AlphaBlendingState, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_4_0, FontVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, FontPixelShader()));
        SetGeometryShader(NULL);
    }
}

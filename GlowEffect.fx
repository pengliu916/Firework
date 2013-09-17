Texture2D<float4>    inputTex  : register(t0);
Texture2D<float4>    H_glowTex  : register(t1);
Texture2D<float4>    V_glowTex  : register(t2);

SamplerState samGeneral : register( s0 );

//--------------------------------------------------------------------------------------
// Constants
//--------------------------------------------------------------------------------------
cbuffer cbPerResize : register( b0 )
{
	int DepthWidth;
	int DepthHeight;
	float glow_factor;
	float radius;
}


//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct GS_INPUT
{
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};
//--------------------------------------------------------------------------------------
// Vertex Shader for every filter
//--------------------------------------------------------------------------------------
GS_INPUT VS( )
{
	GS_INPUT output = (GS_INPUT)0;
 
	return output;
}

//--------------------------------------------------------------------------------------
// Geometry Shader for every filter
//--------------------------------------------------------------------------------------
[maxvertexcount(4)]
void GS(point GS_INPUT particles[1], inout TriangleStream<PS_INPUT> triStream)
{
   PS_INPUT output;
	output.Pos=float4(-1.0f,1.0f,0.0f,1.0f);
	output.Tex=float2(0.0f,0.0f);
	triStream.Append(output);

	output.Pos=float4(1.0f,1.0f,0.0f,1.0f);
	output.Tex=float2(DepthWidth,0.0f);
	triStream.Append(output);

	output.Pos=float4(-1.0f,-1.0f,0.0f,1.0f);
	output.Tex=float2(0.0f,DepthHeight);
	triStream.Append(output);

	output.Pos=float4(1.0f,-1.0f,0.0f,1.0f);
	output.Tex=float2(DepthWidth,DepthHeight);
	triStream.Append(output);
}

//--------------------------------------------------------------------------------------
// Pixel Shader just half the distance (test purpose)
//--------------------------------------------------------------------------------------
float4 PS_Glow_V(PS_INPUT input) : SV_Target
{
	int3 currentLocation = int3(input.Tex.xy,0);
	float4 color = float4( 0, 0, 0, 0 );
	for( int i = -1*(int)radius; i <= (int)radius; i++ ){
		float4 col = inputTex.Load( currentLocation + int3( 0, i, 0 )) / (2.0f*(int)radius +1);
		color += col;
	}
	color.a=1;
	return color;
	//return float4 (1,1,1,1);
}

float4 PS_Glow_H(PS_INPUT input) : SV_Target
{
	int3 currentLocation = int3(input.Tex.xy,0);
	float4 color = float4( 0, 0, 0, 0 );
	for( int i = -1*(int)radius; i <= (int)radius; i++ ){
		float4 col = inputTex.Load( currentLocation + int3( i, 0, 0 )) / (2.0f*(int)radius +1);
		color += col;
	}
	color.a=1;
	return color;
}

float4 PS_Glow_ALL(PS_INPUT input) : SV_Target
{
	float4 color = inputTex.Sample( samGeneral, input.Tex / float2( DepthWidth, DepthHeight ))  +
		H_glowTex.Sample( samGeneral, input.Tex / float2( DepthWidth , DepthHeight )) *glow_factor +
		V_glowTex.Sample( samGeneral, input.Tex / float2( DepthWidth , DepthHeight )) *glow_factor ;
	color.a=1;
	return color;
}
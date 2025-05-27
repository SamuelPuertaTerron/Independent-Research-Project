// In your Shaders.hlsl file:
struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

cbuffer TransformBuffer : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewProjectionMatrix;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    float4 worldPos = mul(float4(input.Pos, 1.0f), worldMatrix);
    output.Pos = float4(input.Pos.x + worldMatrix._14, 
        input.Pos.y + worldMatrix._24, 
        input.Pos.z + worldMatrix._34, 
        1.0f);
    output.Pos = mul(output.Pos, viewProjectionMatrix);
    output.Color = input.Color;
    return output;
}
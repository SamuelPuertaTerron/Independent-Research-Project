// VertexShader.hlsl
cbuffer TransformBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 Color;
};

struct VertexInput
{
    float3 Position : POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float3 WorldPosition : TEXCOORD1;
    float3 Normal : NORMAL;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    
    // Calculate world position
    float4 worldPosition = mul(float4(input.Position, 1.0f), World);
    
    // Calculate view position
    float4 viewPosition = mul(worldPosition, View);
    
    // Calculate projected position
    output.Position = mul(viewPosition, Projection);
    
    // Pass through vertex color (multiplied by constant color)
    output.Color = input.Color;
    
    // Transform normal to world space
    output.Normal = mul(input.Normal, (float3x3)transpose(mul(View, World)));
    
    // Pass through texture coordinates
    //output.TexCoord = input.TexCoord;
    
    // Pass world position for lighting calculations
    output.WorldPosition = worldPosition.xyz;
    
    return output;
}
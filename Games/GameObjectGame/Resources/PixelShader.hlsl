// PixelShader.hlsl
struct PixelInput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float3 WorldPosition : TEXCOORD1;
    float3 Normal : NORMAL;
}

// Simple directional light
static const float3 LightDirection = normalize(float3(-1.0f, -1.0f, -1.0f));
static const float4 LightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
static const float AmbientIntensity = 0.2f;

float4 PS(PixelInput input) : SV_TARGET
{
    // Normalize the normal
    float3 normal = normalize(input.Normal);
    //
    //// Calculate diffuse lighting
    float diffuse = max(0.0f, dot(normal, -LightDirection));
    //
    //// Combine ambient and diffuse lighting
    float4 lighting = LightColor * (AmbientIntensity + diffuse);
    //
    //// Apply lighting to the vertex color
    float4 color = input.Color * lighting;
    //
    //// Ensure alpha is preserved
    color.a = input.Color.a;
    
    return float4(normal, 1.0f);
}
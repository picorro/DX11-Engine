struct VSOutput
{
    float4 Position : SV_Position;
    float3 Color : COLOR0;
    float2 Uv : TEXCOORD0;
};

float4 Main(VSOutput input) : SV_Target
{
    return input.Color.xyzz;
}
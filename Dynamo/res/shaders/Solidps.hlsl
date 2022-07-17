cbuffer Color : register(b0)
{
    float3 color;
}

float4 main() : SV_Target
{
    return float4(1, 1, 1, 1.f);
}
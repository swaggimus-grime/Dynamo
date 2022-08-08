struct PIn
{
    float4 pos : SV_Position;
    float3 color : COLOR;
};

float4 main(PIn input) : SV_Target
{
    return float4(input.color, 1.f);
}
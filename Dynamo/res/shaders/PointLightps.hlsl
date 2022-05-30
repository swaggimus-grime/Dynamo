struct PIn
{
    float3 color : COLOR;
    float4 pos : SV_Position;
};

float4 main(PIn input) : SV_TARGET
{
	return float4(input.color, 1.f);
}
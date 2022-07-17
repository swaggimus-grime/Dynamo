struct PIn
{
    float4 pos : SV_Position;
    float2 tc : TexCoord;
};

Texture2D tex : register(t0);
SamplerState samp;

float4 main(PIn input) : SV_Target
{
    return float4(tex.Sample(samp, input.tc).rgb, 1.f);
}
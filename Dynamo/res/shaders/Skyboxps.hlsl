struct PIn
{
    float4 pos : SV_Position;
    float3 tex : TEXCOORD;
};

TextureCube textureCube : register(t0);
SamplerState cubeSampler : register(s0);

float4 main(PIn input) : SV_TARGET
{
    //return float4(input.tex, 1.f);
	return textureCube.Sample(cubeSampler, normalize(input.tex));
}
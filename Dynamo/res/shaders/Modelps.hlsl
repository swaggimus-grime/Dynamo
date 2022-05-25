Texture2D dmap : register(t0);
Texture2D smap : register(t1);
Texture2D nmap : register(t2);
SamplerState samp;

cbuffer Light : register(b1)
{
    float3 LightPos;
    float4 LightColor;
};

struct PIn
{
    float4 pos : SV_POSITION;
    float3 viewPos : POSITION;
    float2 tex : TEXCOORD;
    float3 norm : NORMAL;
    float3 tan : TANGENT;
    float3 bitan : BITANGENT;
};

float4 main(PIn input) : SV_Target
{
    const float4 dtex = dmap.Sample(samp, input.tex);

    if (dot(input.norm, input.viewPos) >= 0.0f)
    {
        input.norm = -input.norm;
    }

    input.norm = normalize(input.norm);
    input.tan = normalize(input.tan);
    input.bitan = normalize(input.bitan);
    const float3x3 TBN = float3x3(input.tan, input.bitan, input.norm);
    const float3 mappedNormal = normalize(mul((nmap.Sample(samp, input.tex).xyz * 2.f - 1.f), TBN));
    input.norm = lerp(input.norm, mappedNormal, 0.f);
    
    float3 lightDir = normalize(LightPos - input.viewPos);
    float4 diffuse = dtex * max(dot(lightDir, input.norm), 0.f);
    // final color = attenuate diffuse & ambient by diffuse texture color and add specular reflected
    return diffuse;
}
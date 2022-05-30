Texture2D dmap : register(t0);
Texture2D smap : register(t1);
Texture2D nmap : register(t2);
SamplerState samp;

cbuffer Model : register(b0)
{
    float SpecIntensity;
    float SpecPower;

}

cbuffer Light : register(b1)
{
    float3 LightPos;
    float3 LightColor;
    float3 Ambient;
    float Intensity;
    float QuadAtt;
    float LinAtt;
    float ConstAtt;
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
    const float4 stex = smap.Sample(samp, input.tex);

    if (dot(input.norm, input.viewPos) >= 0.0f)
    {
        input.norm = -input.norm;
    }

    input.norm = normalize(input.norm);
    input.tan = normalize(input.tan);
    input.bitan = normalize(input.bitan);
    const float3x3 TBN = float3x3(input.tan, input.bitan, input.norm);
    const float3 mappedNormal = normalize(mul((nmap.Sample(samp, input.tex).xyz * 2.f - 1.f), TBN));
    input.norm = lerp(input.norm, mappedNormal, 1.f);
    
    const float3 vToL = LightPos - input.viewPos;
    const float vToLMag = length(vToL);
    const float3 vToLDir = vToL / vToLMag;
    const float att = 1.f / (ConstAtt + LinAtt * vToLMag + QuadAtt * (vToLMag * vToLMag));
    const float3 diffuse = LightColor * Intensity * att * max(dot(vToLDir, input.norm), 0.f);

    const float3 w = input.norm * dot(vToL, input.norm);
    const float3 r = normalize(w * 2.0f - vToL);
    const float3 viewCamToFrag = normalize(input.viewPos);
    const float specularPower = pow(2.0f, stex.a * SpecPower);
    const float3 specular = LightColor * Intensity * stex.rgb * att * SpecIntensity * pow(max(0.0f, dot(-r, viewCamToFrag)), specularPower);

    return float4(saturate((diffuse + Ambient + specular) * dtex.rgb), 1.0f);
    //return float4(SpecIntensity, SpecIntensity, SpecIntensity, 1.f);
}
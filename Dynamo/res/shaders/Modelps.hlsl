Texture2D dmap : register(t0);
Texture2D smap : register(t1);
Texture2D nmap : register(t2);
TextureCube shadowMap : register(t3);

SamplerState samp : register(s0);
SamplerComparisonState ssamp : register(s1);

cbuffer Model : register(b0)
{
    float4 SpecColor;
    float SpecPower;
    bool hasSpecMap;
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

cbuffer Camera : register(b2)
{
    float3 EyePos;
}

struct PIn
{
    float4 pos : SV_POSITION;
    float3 viewPos : POSITION;
    float4 shadowCamScreen : ShadowPosition;
    float2 tex : TEXCOORD;
    float3 norm : NORMAL; 
    float3 tan : TANGENT;
    float3 bitan : BITANGENT;
};

static const float zf = 100.0f;
static const float zn = 0.5f;
static const float c1 = zf / (zf - zn);
static const float c0 = -zn * zf / (zf - zn);

float CalculateShadowDepth(const in float4 shadowPos)
{
    // get magnitudes for each basis component
    const float3 m = abs(shadowPos).xyz;
    // get the length in the dominant axis
    // (this correlates with shadow map face and derives comparison depth)
    const float major = max(m.x, max(m.y, m.z));
    // converting from distance in shadow light space to projected depth
    return (c1 * major + c0) / major;
}

float Shadow(const in float4 shadowPos)
{
    return shadowMap.SampleCmpLevelZero(ssamp, shadowPos.xyz, CalculateShadowDepth(shadowPos));
}

float4 main(PIn input) : SV_Target
{
    float3 specular;
    float3 diffuse;

    const float shadowLevel = Shadow(input.shadowCamScreen);
    if (shadowLevel != 0)
    {
        input.norm = normalize(input.norm);
        
        const float3x3 TBN = float3x3(normalize(input.tan), normalize(input.bitan), input.norm);
        float3 nsamp = nmap.Sample(samp, input.tex).xyz;
        nsamp = 2.f * nsamp - 1.f;
        input.norm = lerp(input.norm, normalize(mul(nsamp, TBN)), 0.5);
    
        const float3 vToL = LightPos - input.viewPos;
        const float vToLMag = length(vToL);
        const float3 vToLDir = vToL / vToLMag;
        const float att = 1.f / (ConstAtt + LinAtt * vToLMag + QuadAtt * (vToLMag * vToLMag));
        
        diffuse = LightColor * Intensity * att * max(dot(vToLDir, input.norm), 0.f);

        float3 specColor = SpecColor;
        float specPower = SpecPower;
        const float4 specSamp = smap.Sample(samp, input.tex);
        specColor = specSamp.rgb;

        const float3 w = input.norm * dot(vToL, input.norm);
        const float3 r = normalize(w * 2.0f - vToL);
        const float3 viewCamToFrag = normalize(input.viewPos);
        specular = att * specColor * Intensity * pow(max(0.0f, dot(-r, viewCamToFrag)), specPower);
        
        diffuse *= shadowLevel;
        specular *= shadowLevel;
    }
    else
    {
        specular = diffuse = float3(0.f, 0.f, 0.f);
    }
    
    return float4(saturate((diffuse + Ambient) * dmap.Sample(samp, input.tex).rgb + specular), 1.0f);
}
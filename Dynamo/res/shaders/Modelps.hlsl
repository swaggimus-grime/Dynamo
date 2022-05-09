Texture2D diffuseTex : register(t0);
Texture2D specularTex : register(t1);
SamplerState samp;

cbuffer Light
{
    float4 LightColor;
    float3 LightPos;
};

float4 main(float4 pos : Pos, float3 norm : Normal, float2 tex : TexCoord) : SV_Target
{
    float4 ambient = LightColor * 0.1f * diffuseTex.Sample(samp, tex);
    float4 lightDir = -normalize(float4(LightPos, 1.f) - pos);
    float4 normal = normalize(float4(norm, 1.f));
    float4 diffuse = LightColor * max(dot(lightDir, normal), 0.0) * diffuseTex.Sample(samp, tex);
    float4 viewDir = normalize(-pos);
    float4 reflectDir = reflect(lightDir, float4(norm, 1.f));
    float4 specular = LightColor * pow(max(dot(viewDir, reflectDir), 0.0), 2) * specularTex.Sample(samp, tex);
    return diffuseTex.Sample(samp, tex);

    //return float4(1.f, 1.f, 1.f, 1.f);
}
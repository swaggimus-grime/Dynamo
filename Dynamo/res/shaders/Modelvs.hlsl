struct VOut
{
    float4 pos : SV_POSITION;
    float3 viewPos : POSITION;
    float4 shadowCamScreen : ShadowPosition;
    float2 tex : TEXCOORD;
    float3 norm : NORMAL;
    float3 tan : TANGENT;
    float3 bitan : BITANGENT;
};

cbuffer Transform : register(b0)
{
    matrix model;
    matrix modelView;
    matrix mvp;
};

cbuffer ShadowTransform : register(b1)
{
    matrix shadowView;
};

VOut main(float3 pos : Pos, float2 tex : Tex, float3 norm : Norm, float3 tan : Tan, float3 bitan : Bitan)
{
    VOut vertex;
    vertex.pos = mul(float4(pos, 1.f), mvp);
    vertex.tex = tex;
    vertex.viewPos = (float3)mul(float4(pos, 1.f), modelView);
    const float4 worldPos = mul(float4(pos, 1.f), model);
    const float4 shadowHomo = mul(worldPos, shadowView);
    vertex.shadowCamScreen = shadowHomo;
    vertex.norm = mul(norm, (float3x3)modelView);
    vertex.tan = mul(tan, (float3x3)modelView);
    vertex.bitan = mul(bitan, (float3x3)modelView);
    return vertex;
}
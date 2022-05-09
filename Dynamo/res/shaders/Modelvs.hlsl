struct VOut
{
    float4 pos : SV_Position;
    float3 norm : Normal;
    float2 tex : TexCoord;
};

cbuffer Transform
{
    matrix mvp;
    matrix modelView;
};

VOut main(float3 pos : Pos, float2 tex : Tex, float3 norm : Norm, float3 tan : Tan, float3 bitan : Bitan)
{
    VOut vertex;
    vertex.pos = mul(float4(pos, 1.f), mvp);
    vertex.tex = tex;
    vertex.norm = mul(norm, (float3x3) modelView);
    return vertex;
}
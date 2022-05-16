struct VOut
{
    float4 pos : SV_Position;
    float3 tex : TEXCOORD;
};

cbuffer Transform : register(b0)
{
    matrix vp;
};

VOut main(float3 pos : Pos)
{
    VOut vertex;
    vertex.tex = pos;
    vertex.pos = mul(float4(pos, 0.f), vp);
    vertex.pos.z = vertex.pos.w;
    return vertex;
}
struct VOut
{
    float4 pos : SV_Position;
    float3 color : COLOR;
};

cbuffer Transform
{
    matrix model;
    matrix modelView;
    matrix mvp;
};

VOut main(float3 pos : Pos)
{
    VOut vertex;
    vertex.pos = mul(float4(pos, 1.f), mvp);
    vertex.color = pos;
    return vertex;
}
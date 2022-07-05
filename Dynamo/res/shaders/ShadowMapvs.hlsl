struct VOut
{
    float4 pos : SV_Position;
};

cbuffer Transform
{
    matrix mvp;
};

VOut main(float3 pos : Pos)
{
    VOut vertex;
    vertex.pos = mul(float4(pos, 1.f), mvp);
    return vertex;
}
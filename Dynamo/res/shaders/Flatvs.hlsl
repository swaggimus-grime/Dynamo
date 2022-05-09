struct VOut
{
    float4 pos : SV_Position;
};

cbuffer Transform
{
    matrix mvp;
};

VOut main(float2 pos : Pos)
{
    VOut vertex;
    vertex.pos = mul(float4(pos, 0.f, 1.f), mvp);
    return vertex;
}
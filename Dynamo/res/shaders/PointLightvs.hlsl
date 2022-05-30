struct VOut
{
    float3 color : COLOR;
    float4 pos : SV_Position;
};

cbuffer Transform
{
    matrix mvp;
    float3 color;
};

VOut main(float3 pos : Pos)
{
    VOut vertex;
    vertex.pos = mul(float4(pos, 1.f), mvp);
    vertex.color = color;
    return vertex;
}
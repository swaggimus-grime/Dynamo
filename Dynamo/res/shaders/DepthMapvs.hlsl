struct VOut
{
    float4 pos : SV_Position;
    float2 tc : TexCoord;
};

VOut main(float2 pos : Pos)
{
    VOut vertex;
    vertex.pos = float4(pos, 0.f, 1.f);
    vertex.tc = float2((pos.x + 1) / 2.f, (1 - pos.y) / 2.f);
    return vertex;
}
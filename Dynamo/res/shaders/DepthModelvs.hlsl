cbuffer Transform
{
    matrix model;
    matrix modelView;
    matrix mvp;
};

float4 main(float3 pos : Pos) : SV_POSITION
{
    return mul(float4(pos, 1.f), mvp);
}
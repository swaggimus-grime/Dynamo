struct PIn
{
    float4 pos : SV_Position;
};

float4 main(PIn input) : SV_Target
{
    return input.pos;
}
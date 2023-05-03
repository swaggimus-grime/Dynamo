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

static const int MAX_BONES = 100;
static const int MAX_BONE_INFLUENCE = 4;

cbuffer Bones : register(b2)
{
    matrix finalBonesMatrices[MAX_BONES];
};

VOut main(float3 pos : Pos, float2 tex : Tex, float3 norm : Norm, float3 tan : Tan, float3 bitan : Bitan, int4 bones : Bones, float4 weights : Weights)
{
    matrix BoneTransform = finalBonesMatrices[bones[0]] * weights[0];
    BoneTransform += finalBonesMatrices[bones[1]] * weights[1];
    BoneTransform += finalBonesMatrices[bones[2]] * weights[2];
    BoneTransform += finalBonesMatrices[bones[3]] * weights[3];
    float4 bonePos = mul(float4(pos, 1), BoneTransform);
    
    VOut vertex;
    vertex.pos = mul(bonePos, mvp);
    vertex.tex = tex;
    vertex.viewPos = (float3) mul(bonePos, modelView);
    const float4 worldPos = mul(bonePos, model);
    const float4 shadowHomo = mul(worldPos, shadowView);
    vertex.shadowCamScreen = shadowHomo;
    vertex.norm = mul(norm, (float3x3) modelView);
    vertex.tan = mul(tan, (float3x3) modelView);
    vertex.bitan = mul(bitan, (float3x3) modelView);
    return vertex;
}
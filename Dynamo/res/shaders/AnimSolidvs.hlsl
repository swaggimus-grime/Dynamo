cbuffer Transform : register(b0)
{
    matrix model;
    matrix modelView;
    matrix mvp;
};

static const int MAX_BONES = 100;
static const int MAX_BONE_INFLUENCE = 4;

cbuffer Bones : register(b2)
{
    matrix finalBonesMatrices[MAX_BONES];
};

float4 main(float3 pos : Pos, int4 bones : Bones, float4 weights : Weights) : SV_Position
{
    matrix BoneTransform = finalBonesMatrices[bones[0]] * weights[0];
    BoneTransform += finalBonesMatrices[bones[1]] * weights[1];
    BoneTransform += finalBonesMatrices[bones[2]] * weights[2];
    BoneTransform += finalBonesMatrices[bones[3]] * weights[3];
    float4 bonePos = mul(float4(pos, 1), BoneTransform);
    
    return mul(bonePos, mvp);
}
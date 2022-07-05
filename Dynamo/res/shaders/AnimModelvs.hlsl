struct VOut
{
    float4 pos : SV_POSITION;
    float3 viewPos : POSITION;
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

static const int MAX_BONES = 100;
static const int MAX_BONE_INFLUENCE = 4;

cbuffer Bones : register(b1)
{
    matrix finalBonesMatrices[MAX_BONES];
};

VOut main(float3 pos : Pos, float2 tex : Tex, float3 norm : Norm, float3 tan : Tan, float3 bitan : Bitan, int4 bones : Bones, float4 weights : Weights)
{
    //float4 totalPosition = float4(0.f, 0.f, 0.f, 0.f);
    //for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    //{
    //    if (bones[i] == -1)
    //        continue;
    //    if (bones[i] >= MAX_BONES)
    //    {
    //        totalPosition = float4(pos, 1.0f);
    //        break;
    //    }

    //    float4 localPosition = mul(float4(pos, 1.0f), finalBonesMatrices[bones[i]]);
    //    totalPosition += localPosition * weights[i];
    //    //vec3 localNormal = mat3(finalBonesMatrices[bones[i]]) * norm;
    //}
    
    matrix BoneTransform = finalBonesMatrices[bones[0]] * weights[0];
    BoneTransform += finalBonesMatrices[bones[1]] * weights[1];
    BoneTransform += finalBonesMatrices[bones[2]] * weights[2];
    BoneTransform += finalBonesMatrices[bones[3]] * weights[3];
    float4 bonePos = mul(float4(pos, 1), BoneTransform);
    
    VOut vertex;
    vertex.pos = mul(bonePos, mvp);
    vertex.tex = tex;
    vertex.viewPos = (float3)mul(bonePos, modelView);
    vertex.norm = mul(norm, (float3x3)modelView);
    vertex.tan = mul(tan, (float3x3)modelView);
    vertex.bitan = mul(bitan, (float3x3)modelView);
    return vertex;
}
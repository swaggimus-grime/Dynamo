#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

struct AssimpNodeData
{
    XMMATRIX transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
public:
    Animation() = default;
    Animation(const aiScene& scene, class AnimModel* model);
    ~Animation();

    class Bone* FindBone(const std::string& name);

    inline float GetTicksPerSecond() { return m_TicksPerSecond; }
    inline float GetDuration() { return m_Duration; }
    inline const AssimpNodeData& GetRootNode() { return m_RootNode; }

    inline const std::map<std::string, struct BoneInfo>& GetBoneIDMap() { return m_BoneInfoMap; }

private:
    void ReadMissingBones(const aiAnimation* animation, AnimModel& model);
    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

private:
    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, struct BoneInfo> m_BoneInfoMap;
};
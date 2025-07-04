#pragma once

#include <unordered_map>
#include <filesystem>
#include <wrl/client.h>
#include "AnimationClip.h"
#include "D2DRender.h"

// 예시입니다.
class AssetManager
{
public:
    using AnimationClips = std::vector<std::pair<std::string, AnimationClip>>;
    using D2DRenderer = sample::D2DRenderer;
   
    AssetManager() = default;
    ~AssetManager() = default;

    const AnimationClips& GetClips(std::wstring assetKey);

    void LoadTexture(D2DRenderer* renderer, std::wstring keyWide, std::filesystem::path fullPath);
    void LoadAseprite(D2DRenderer* renderer, std::wstring keyWide, std::filesystem::path fullPath);
private:

    std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> m_textures;
    std::unordered_map<std::wstring, AnimationClips> m_clipsMap;
};


#include "pch.h"
#include "AnimationClip.h"
#include "AsepriteParser.h"
#include "D2DRender.h"
#include "AssetManager.h"

const AnimationClips& AssetManager::GetClips(std::wstring assetKey)
{
    auto it = m_clipsMap.find(assetKey);
    if (it != m_clipsMap.end())
    {
        return it->second;
    }

    static const AnimationClips empty; 
    return empty;  
}

void AssetManager::LoadTexture(D2DRenderer* renderer, std::wstring keyWide, std::filesystem::path fullPath)
{
    Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;

    std::wstring path = fullPath.wstring();

    renderer->CreateBitmapFromFile(path.c_str(), *bitmap.GetAddressOf());
    m_textures.insert({ keyWide, bitmap });
}

void AssetManager::LoadAseprite(D2DRenderer* renderer, std::wstring keyWide, std::filesystem::path fullPath)
{
    m_clipsMap.insert({ keyWide, AsepriteParser::Load(fullPath) });

    const AnimationClips& constClips = GetClips(keyWide);
    AnimationClips& clips = const_cast<AnimationClips&>(constClips);

    for (auto& clip : clips)
    {
        Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;

        std::filesystem::path keyPath = fullPath.replace_extension(".png");
        std::wstring path = keyPath.wstring();

        renderer->CreateBitmapFromFile(path.c_str(), *bitmap.GetAddressOf());
        clip.second.SetBitmap(bitmap);
        m_textures.insert({ keyWide, bitmap });
    }
}

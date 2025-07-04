#include "pch.h"
#include <fstream>
#include "json.hpp"
#include "AsepriteParser.h"

AnimationClips AsepriteParser::Load(const std::filesystem::path& jsonPath)
{
    // 1) JSON 로드하세요.
    std::ifstream ifs(jsonPath);

    if (!ifs.is_open())
    {
        std::cerr << "파일을 열 수 없습니다.\n";
        return {};
    }

    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    auto parsed = nlohmann::json::parse(content, nullptr, false);

    if (parsed.is_discarded())
    {
        std::cerr << "JSON 파싱 실패 : 유효하지 않은 문서입니다.\n";
        return {};
    }
    const auto& root = parsed;

    AnimationClip clip;

    auto& framesNode = root["frames"];
    if (framesNode.is_object())
    {
        for (auto it = framesNode.begin(); it != framesNode.end(); ++it)
        {
            const auto& f = it.value();
           
            Frame fd;
            fd.srcRect.left = f["frame"]["x"];
            fd.srcRect.top = f["frame"]["y"];
            fd.srcRect.right = f["frame"]["w"];
            fd.srcRect.right += fd.srcRect.left;
            fd.srcRect.bottom = f["frame"]["h"];
            fd.srcRect.bottom += fd.srcRect.top;
            fd.duration = f["duration"] / 1000.f;
            clip.AddFrame(fd);
        }
    }
    else if (framesNode.is_array())
    {
        for (const auto& f : framesNode)
        {
            Frame fd;
            fd.srcRect.left = f["frame"]["x"];
            fd.srcRect.top = f["frame"]["y"];
            fd.srcRect.right = f["frame"]["w"];
            fd.srcRect.right += fd.srcRect.left;
            fd.srcRect.bottom = f["frame"]["h"];
            fd.srcRect.bottom += fd.srcRect.top;
            fd.duration = f["duration"] / 1000.f;
            clip.AddFrame(fd);
        }
    }
    else
    {
        std::cerr << "Unsupported 'frames' format" << '\n';
        return {};
    }

    std::filesystem::path keyPath = jsonPath;
    keyPath.replace_extension();
    std::string key = keyPath.string();

    // 2) 태그별로  AnimationClip 생성합니다.
    AnimationClips clips;

    if (root["meta"].contains("frameTags"))
    {
        for (const auto& t : root["meta"]["frameTags"]) {
            Tag tag;
            tag.m_name = t["name"];
            tag.m_from = t["from"];
            tag.m_to = t["to"];
            tag.m_direction = t["direction"];

            AnimationClip subClip;

            for (int i = tag.m_from; i <= tag.m_to; i++)
            {
                subClip.AddFrame(clip.GetFrame(i));
            }

            clips.push_back({ tag.m_name, subClip });
        }
    }
    else
    {
        clips.push_back({ key, clip });
    }


    return clips;
}

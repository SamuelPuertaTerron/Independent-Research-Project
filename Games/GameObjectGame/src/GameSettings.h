/* The game settings */
/* Note: The errors are with VS and not the Code and it will build successfully */
#pragma once

#include "../../ThirdParty/nlohmann/json.hpp"

namespace GameObject
{
    struct Settings
    {
        Engine::Render::EGraphicsAPI GraphicsAPI = Engine::Render::EGraphicsAPI::OpenGL;
        int MaxChunkSize{ 12 };
        float CameraSpeed{ 10.0f };
        float Volume{ 0.5f };
    };

    inline void to_json(nlohmann::json& json, const Settings& settings)
    {
        json = nlohmann::json{
            {"GraphicsAPI", settings.GraphicsAPI},
            {"MaxChunkSize", settings.MaxChunkSize},
            {"CameraSpeed", settings.CameraSpeed},
            {"Volume", settings.Volume},
        };
    }

    inline void from_json(const nlohmann::json& json, Settings& settings)
    {
        json.at("GraphicsAPI").get_to(settings.GraphicsAPI);
        json.at("MaxChunkSize").get_to(settings.MaxChunkSize);
        json.at("CameraSpeed").get_to(settings.CameraSpeed);
        json.at("Volume").get_to(settings.Volume);
    }

    inline bool LoadSettings(const std::filesystem::path& filename, Settings& settings)
    {
        try {
            std::ifstream file(filename);
            if (!file.is_open()) 
            {
                return false;
            }

            nlohmann::json json;
            file >> json;
            settings = json.get<Settings>();
            return true;
        }
        catch (const std::exception& e) {
            std::stringstream ss;
            ss << "Failed to save settings: " << e.what();
            Engine::Logger::Log(ss.str(), Engine::ELogLevel::Fatal);
            return false;
        }
    }

    inline bool SaveSettings(const std::filesystem::path&& filename, const Settings& settings)
    {
        try {
            std::ofstream file(filename);
            if (!file.is_open()) 
            {
                return false;
            }

            nlohmann::json json = settings;
            file << json.dump(4); // 4 spaces for pretty printing
            return true;
        }
        catch (const std::exception& e) {
            std::stringstream ss;
        	ss << "Failed to load settings: " << e.what();
            Engine::Logger::Log(ss.str(), Engine::ELogLevel::Fatal);
            return false;
        }
    }

}//namespace EntityGame
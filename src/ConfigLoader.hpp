#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

struct Settings
{
    uint32_t width = 1280;
    uint32_t height = 720;
    float nearPlane = 0.1f;
    float farPlane = 1000.f;
    bool fullscreen = false;
    float fovY = 45.0f;
    float cameraSensitivity = 0.005f;
    float movementSpeed = 5.f;

    uint32_t numParticles = 10000;
    uint32_t particleSpawnRadius = 16;
    float sizeOfParticle = 0.01f;
};

class ConfigLoader
{
public:
    /*
    Format Type:

    width 1024
    height 768
    maxFps 60
    fullscreen 1

    */
    ConfigLoader(const std::string &filename)
    {
        std::ifstream file(filename, std::ios::in);

        if (!file) {
            std::cerr << "Error: Cannot open config file! Make sure there is a config file with the name \"" << filename << "\" in the same directory as the executable.\n";
            std::cerr << "Falling back to default values!\n";
            return;
        }

        std::string line, value, descr;
        std::istringstream token;

        while (!file.eof()) {
            getline(file, line);
            if (!line.empty()) {
                token = std::istringstream(line);
                getline(token, descr, ' ');
                getline(token, value, ' ');
                if (descr == "width"){ m_settings.width = std::stoi(value); }
                else if (descr == "height"){ m_settings.height = std::stoi(value); }
                else if (descr == "fovY"){ m_settings.fovY = std::stof(value); }
                else if (descr == "nearPlane"){ m_settings.nearPlane = std::stof(value); }
                else if (descr == "farPlane"){ m_settings.farPlane = std::stof(value); }
                else if (descr == "fullscreen"){ m_settings.fullscreen = std::stoi(value); }
                else if (descr == "cameraSensitivity"){ m_settings.cameraSensitivity = std::stof(value); }
                else if (descr == "movementSpeed"){ m_settings.movementSpeed = std::stof(value); }
                else if (descr == "numParticles"){ m_settings.numParticles = std::stoi(value); }
                else if (descr == "particleSpawnRadius"){ m_settings.particleSpawnRadius = std::stoi(value); }
                else if (descr == "sizeOfParticle"){ m_settings.sizeOfParticle = std::stof(value); }
            }
        }
    }

    Settings settings() const { return m_settings; }

private:
    Settings m_settings;
};
#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct Texture {
    uint32_t id = 0;
    std::vector<uint8_t> data;
    int width = 0;
    int height = 0;
    int format = 0;
    int internalFormat = 0;

    void Load(const std::string& filename);
    void GenerateWhiteTexture();

    const uint8_t* GetRawData() const { return data.data(); }
};

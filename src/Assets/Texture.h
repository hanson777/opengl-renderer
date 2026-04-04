#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct Texture {
    uint32_t m_id = 0;
    std::vector<uint8_t> m_data;
    int m_width = 0;
    int m_height = 0;
    int m_format = 0;
    int m_internalFormat = 0;

    void Load(const std::string& filename);
    void GenerateWhiteTexture();

    uint32_t GetId() const { return m_id; }
    const uint8_t* GetRawData() const { return m_data.data(); }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetFormat() const { return m_format; }
    int GetInternalFormat() const { return m_internalFormat; }
};

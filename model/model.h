#pragma once
#include "../mesh/mesh.h"

class Model {
public:
    Model(const std::string& path);

    void Draw(Shader& shader);

private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;

    void LoadModel(const std::string& path);
};

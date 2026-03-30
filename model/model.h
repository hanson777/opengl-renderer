#pragma once
#include "../mesh/mesh.h"

class Model {
public:
    Model(const std::string& path);
    void draw(Shader& shader);

private:
    std::vector<Mesh> m_meshes;
    void loadModel(const std::string& path);
    std::string m_directory;
};

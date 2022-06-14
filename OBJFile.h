#ifndef OBJFILE_H
#define OBJFILE_H

#include <cassert>
#include <fstream>
#include <glm/glm.hpp>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

class OBJFile
{
public:
    using Vertices = std::vector<glm::vec3>;
    using TextureCoordinates = std::vector<float>;
    using Normals = std::vector<float>;
    using Indices = std::vector<unsigned short>;

private:
    //Vertices m_VertexPositions;
    Vertices m_Vertex;
    TextureCoordinates m_TextureCoordinates;
    Normals m_Normals;
    Indices m_Indices;

public:
    OBJFile(const std::string &filename);
    const Vertices &GetVertices() const;
    const TextureCoordinates &GetTextureCoordinates() const;
    const Normals &GetNormals() const;
    const Indices &GetIndices() const;
};

#endif // OBJFILE_H
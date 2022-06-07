#ifndef OBJFILE_H
#define OBJFILE_H

#include <fstream>
#include <glm/glm.hpp>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <vector>
#include <iostream>
#include <iomanip>

//Marco Antonio Quiña Mamani

class OBJFile
{
public:
    using Vertices = std::vector<glm::vec3>;
    using Normals =  std::vector<glm::vec3>;
    using Uvs = std::vector<glm::vec2>;
    using IndicesVertices =  std::vector<unsigned short>;
    using IndicesNormals =  std::vector<unsigned short>;
    using IndicesUvs = std::vector<unsigned short>;
private:
    Vertices m_VertexPositions;
    Normals m_Normals;
    Uvs m_Uvs;
    IndicesVertices m_IndicesVertex;
    IndicesNormals m_IndicesNormals;
    IndicesUvs m_IndicesUvs;
public:
    OBJFile();
    void readFileOBJ(const std::string& filename);
    void writeFrames(const std::string& directory, unsigned index);
    void writeFramesInVTK(const std::string& directory, unsigned index);
    Vertices& GetVertices() ;
    Normals& GetNormals();
    Uvs& GetUvs();
    IndicesVertices& GetIndicesVertices();
    IndicesNormals& GetIndicesNormals();
    IndicesUvs& GetIndicesUvs();
};

#endif // OBJFILE_H

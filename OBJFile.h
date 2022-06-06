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
    using TextureCoordinates =  std::vector<float>;
    using Normals =  std::vector<float>;
    using Indices =  std::vector<unsigned short>;
private:
    Vertices m_VertexPositions;
    TextureCoordinates m_TextureCoordinates;
    Normals m_Normals;
    Indices m_Indices;
public:
    OBJFile();
    void readFileOBJ(const std::string& filename);
    void writeFrames(const std::string& directory, unsigned index);
    void writeFramesInVTK(const std::string& directory, unsigned index);
    Vertices& GetVertices() ;
    TextureCoordinates& GetTextureCoordinates();
    Normals& GetNormals();
    Indices& GetIndices();
};

#endif // OBJFILE_H

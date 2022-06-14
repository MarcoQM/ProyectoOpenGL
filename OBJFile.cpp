#include "OBJFile.h"

OBJFile::OBJFile(const std::string &filename)
{
    std::ifstream fileStream{filename, std::ios_base::in};

    if (fileStream.is_open())
    {
        while (!fileStream.eof())
        {
            std::string line;
            getline(fileStream, line);
            std::stringstream lineStream{line};
            std::string firstSymbol;
            lineStream >> firstSymbol;
            if (firstSymbol == "v")
            {
                float vertexPosition{};
                float x, y, z;
                lineStream >> x >> y >> z;
                glm::vec3 vertice(x, y, z);
                m_Vertex.emplace_back(vertice);
                
                /*for (unsigned int i = 0; i < 3; ++i)
                {

                    lineStream >> vertexPosition;
                    m_VertexPositions.emplace_back(vertexPosition);
                }*/
            }
            else if (firstSymbol == "vt")
            {
                float textureCoordinate{};
                for (unsigned int i = 0; i < 2; ++i)
                {
                    lineStream >> textureCoordinate;
                    m_TextureCoordinates.emplace_back(textureCoordinate);
                }
            }
            else if (firstSymbol == "vn")
            {
                float normal{};
                for (unsigned int i = 0; i < 3; ++i)
                {
                    lineStream >> normal;
                    m_Normals.emplace_back(normal);
                }
            }
            else if (firstSymbol == "f")
            {
                char separator;
                unsigned short index{};
                for (unsigned int i = 0; i < 3; ++i)
                {
                    // for (unsigned int j = 0; j < 3; ++j)
                    //{
                    lineStream >> index;

                    m_Indices.emplace_back(index);
                    //if (j < 2)
                    //{
                    //    lineStream >> separator;
                    //}
                    //}
                }
            }
        }
    }
}

const OBJFile::Vertices &OBJFile::GetVertices() const
{
    //return m_VertexPositions;
    return m_Vertex;
}

const OBJFile::TextureCoordinates &OBJFile::GetTextureCoordinates() const
{
    return m_TextureCoordinates;
}

const OBJFile::Normals &OBJFile::GetNormals() const
{
    return m_Normals;
}

const OBJFile::Indices &OBJFile::GetIndices() const
{
    return m_Indices;
}
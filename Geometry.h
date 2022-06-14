#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "OBJFile.h"

class Geometry 
{
public:
    using Vertices = std::vector<glm::vec3>;
    using Indices = std::vector<unsigned short>;

private:
    Vertices m_Vertices;
    Indices m_Indices;
    unsigned int numVertexPositionElements;

public:
    Geometry() = default;
    Geometry(const OBJFile& objFile);
    ~Geometry() = default;
    
    void SetVertices(const Vertices& vertices);
    const Vertices &GetVertices() const;
    void SetIndices(const Indices& indices);
    const Indices &GetIndices() const;
    const unsigned int &GetNumVertexPositionElements() const;
};
#endif // GEOMETRY_H
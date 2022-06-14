#include "Geometry.h"

Geometry::Geometry(const OBJFile& objFile)
{
    const OBJFile::Indices& objIndices{objFile.GetIndices()};
    const OBJFile::Vertices& objVertex{objFile.GetVertices()};
    numVertexPositionElements = objVertex.size();
    unsigned short index;

    for (unsigned int i = 0; i < objIndices.size(); ++i)
    {
        index = objIndices[i];
        m_Vertices.emplace_back(objVertex[index-1]);
        m_Indices.emplace_back(objIndices[i]);
    }
}

void Geometry::SetVertices(const Vertices& vertices)
{
    m_Vertices = vertices;
}

const Geometry::Vertices &Geometry::GetVertices() const
{
    return m_Vertices;
}

void Geometry::SetIndices(const Indices& indices)
{
    m_Indices = indices;
}

const Geometry::Indices &Geometry::GetIndices() const
{
    return m_Indices;
}

const unsigned int &Geometry::GetNumVertexPositionElements() const
{
    return numVertexPositionElements;
}
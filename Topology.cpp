#include "Topology.h"

Topology::Topology(const Indices& indices) 
{
    //m_Edges.emplace_back(Edge(4,7));
    unsigned short v1, v2, v3;

    for (unsigned int i = 0; i < indices.size(); i+=3)
    {
        v1 = indices[i];
        v2 = indices[i+1];
        v3 = indices[i+2];

        Edge edge1(v1, v2);
        Edge edge2(v2, v3);
        Edge edge3(v1, v3);

        if(!util::existDataInVector(m_Edges, edge1))
            m_Edges.emplace_back(edge1);
        if(!util::existDataInVector(m_Edges, edge2))
            m_Edges.emplace_back(edge2);
        if(!util::existDataInVector(m_Edges, edge3))
            m_Edges.emplace_back(edge3);
    }


}

void Topology::SetEdges(const Edges& edges)
{
    m_Edges = edges;
}
    
const Topology::Edges &Topology::GetEdges() const
{
    return m_Edges;
}
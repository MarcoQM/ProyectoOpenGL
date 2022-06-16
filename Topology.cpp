#include "Topology.h"

Topology::Topology(const Indices& indices) 
{
    //m_Edges.emplace_back(Edge(4,7));


    for (unsigned int i = 0; i < indices.size(); i+=3)
    {
        m_Edges.emplace_back(Edge(indices[i], indices[i+1]));
        m_Edges.emplace_back(Edge(indices[i+1], indices[i+2]));
        m_Edges.emplace_back(Edge(indices[i], indices[i+2]));
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
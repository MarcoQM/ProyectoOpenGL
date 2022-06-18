#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <iostream>
#include <vector>
#include "Util.h"

class Topology
{
    struct Edge
    {
        unsigned short VertexA;
        unsigned short VertexB;

        Edge(unsigned short vertexA, unsigned short vertexB) : VertexA(vertexA), VertexB(vertexB) {}

        const unsigned short &GetVertexA() const
        {
            return VertexA;
        }

        const unsigned short &GetVertexB() const
        {
            return VertexB;
        }

        bool operator==(const Edge &otherEdge) const
        {
            return VertexA == otherEdge.VertexA &&
                       VertexB == otherEdge.VertexB ||
                   VertexA == otherEdge.VertexB &&
                       VertexB == otherEdge.VertexA;
        }

        friend std::ostream &operator<<(std::ostream &output, const Edge &e)
        {
            output << "(" << e.VertexA << ", " << e.VertexB << ")";
            return output;
        }
    };

public:
    using Edges = std::vector<Edge>;
    using Indices = std::vector<unsigned short>;

private:
    Edges m_Edges;

public:
    Topology() = default;
    Topology(const Indices &indices);
    ~Topology() = default;

    void SetEdges(const Edges &edges);
    const Edges &GetEdges() const;
};

#endif // TOPOLOGY_H
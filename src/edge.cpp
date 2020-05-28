class Edge {

public:
    int src, dest;

    bool operator== (const Edge &edge) const {
        return ((src == edge.src) && (dest == edge.dest)) || ((src == edge.dest) && (dest == edge.src));
    }

    bool operator<(const Edge& edge) const {
        return !(((src == edge.src) && (dest == edge.dest)) || ((src == edge.dest) && (dest == edge.src)));
    }

    Edge(int src, int dest) {
        this->src = src;
        this->dest = dest;
    }
};


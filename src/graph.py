class Graph:
    NULL_LABEL = 0xffffffff

    class Edge:
        def __init__(self, label=NULL_LABEL):
            self.label = label
            self.source = None
            self.target = None
            self.directed = True
        
    class Vertex:
        def __init__(self, label=NULL_LABEL):
            self.id = 0
            self.label = label
            self.edges = []

    def __init__(self):
        self.vertices = []

    def addVertex(self, label=None):
        vertex = self.Vertex(label)
        assert vertex is not None
        self.vertices.append(vertex)
        return vertex

    def connectVertices(self, source, target, directed=False, label=NULL_LABEL):
        edge = self.Edge(label)
        assert edge is not None
        edge.source = source
        edge.target = target
        edge.directed = directed
        edge.label = label
        source.edges.append(edge)
        # self-edges
        if source != target:
            target.edges.append(edge)
        return edge

    def getVertex(self, label):
        for vertex in self.vertices:
            if vertex.label == label:
                return vertex
        return None

    def clear(self):
        self.vertices.clear()

    def clone(self):
        new_graph = Graph()
        vertex_mapping = {}
        for old_vertex in self.vertices:
            new_vertex = new_graph.addVertex(old_vertex.label)
            new_vertex.id = old_vertex.id
            vertex_mapping[id(old_vertex)] = new_vertex
        for old_vertex, new_vertex in zip(self.vertices, new_graph.vertices):
            for old_edge in old_vertex.edges:
                new_edge = self.Edge(old_edge.label)
                new_edge.directed = old_edge.directed
                new_edge.source = vertex_mapping[id(old_edge.source)]
                new_edge.target = vertex_mapping[id(old_edge.target)]
                new_vertex.edges.append(new_edge)
        
        return new_graph

    def id(self):
        for i, v in enumerate(self.vertices):
            v.id = i
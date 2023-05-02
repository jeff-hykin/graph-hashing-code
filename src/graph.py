import collections
from hashlib import md5 
import functools
import pickle

# NOTES:
    # .id never seems to be used 
    # NO_HASH_VERTEX_LABEL never seems to be used, and seems like it might be a rare edgecase bug

code = type(compile('1','','single'))

def consistent_hash(value):
    if isinstance(value, bytes):
        return md5(value).hexdigest()
    
    if isinstance(value, str):
        return md5(("@"+value).encode('utf-8')).hexdigest()
    
    if isinstance(value, (bool, int, float, type(None))):
        return md5(str(value).encode('utf-8')).hexdigest()
        
    else:
        return md5(pickle.dumps(value, protocol=4)).hexdigest()

class Graph:
    class Edge:
        def __init__(self, label=None):
            self.label = label
            self.source = None
            self.target = None
            self.directed = True
        
    class Vertex:
        def __init__(self, label=None):
            self.label = label
            self.edges = []

    def __init__(self):
        self.vertices = []

    def addVertex(self, label=None):
        vertex = self.Vertex(label)
        assert vertex is not None
        self.vertices.append(vertex)
        return vertex

    def connectVertices(self, source, target, directed=False, label=None):
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
            vertex_mapping[id(old_vertex)] = new_vertex
        for old_vertex, new_vertex in zip(self.vertices, new_graph.vertices):
            for old_edge in old_vertex.edges:
                new_edge = self.Edge(old_edge.label)
                new_edge.directed = old_edge.directed
                new_edge.source = vertex_mapping[id(old_edge.source)]
                new_edge.target = vertex_mapping[id(old_edge.target)]
                new_vertex.edges.append(new_edge)
        
        return new_graph


class GraphHash:
    def __init__(self):
        self.code = consistent_hash(tuple())
        self.vertexCoder = None
        self.partitionCount = 0
    
    def hash(self, graph):
        vertexBranch = []
        self.code = consistent_hash(tuple())
        self.vertexCoder = None
        self.partitionCount = 0

        self.vertexCoder = self.VertexCoder()
        for vertex in graph.vertices:
            child = self.VertexCoder(vertex, vertexBranch)
            link = self.VertexCoderLink(None, child)
            self.vertexCoder.children.append(link)
        self.vertexCoder.encode()
        self.code = self.vertexCoder.code
        self.partitionCount = 1 + sum(
            1 for prev_child, child in zip(self.vertexCoder.children[0:-1], self.vertexCoder.children[1:])
                if prev_child.coder.code != child.coder.code
        )
        return self.code
    
    class VertexCoderLink:
        def __init__(self, edge=None, coder=None):
            self.edge = edge
            self.coder = coder
        
    class VertexCoder:
        def __init__(self, vertex=None, vertexBranch=None):
            self.vertex = vertex
            self.vertexBranch = vertexBranch or []
            self.code = consistent_hash(tuple())
            self.codeValid = False
            self.newcode = consistent_hash(tuple())
            self.children = []
        
        def encode(self):
            number_of_children = len(self.children)
            for child_index, child in enumerate(self.children):
                coder = child.coder
                coder.code = consistent_hash(tuple())
                vertex = coder.vertex
                for edge_index, edge in enumerate(vertex.edges):
                    if vertex == edge.source:
                        other_vertex = edge.target
                    else:
                        other_vertex = edge.source
                    for other_child in self.children:
                        if other_child.coder.vertex == other_vertex:
                            link = GraphHash.VertexCoderLink(edge, other_child.coder)
                            coder.children.append(link)
                            break
            
            for _ in range(number_of_children):
                for each_child in self.children:
                    coder = each_child.coder
                    coder.children.sort(key=functools.cmp_to_key(self.cmpCoderLinks))
                    coder.newcode = consistent_hash(tuple(each.coder.code for each in coder.children))
                
                for each_child in self.children:
                    coder = each_child.coder
                    coder.code = coder.newcode
                    coder.codeValid = True
            
            self.children.sort(key=functools.cmp_to_key(self.cmpCoderLinks))
            self.code = consistent_hash(tuple(each.coder.code for each in self.children))
            self.codeValid = True

        # private helper methods
        @classmethod
        def ltcmpCoderLinks(cls, a, b):
            return cls.cmpCoderLinks(a, b) < 0
        
        @classmethod
        def ltcmpCoderLinksLabeled(cls, a, b):
            result = cls.cmpCoderLinks(a, b)

            if result < 0:
                return True
            elif result == 0:
                if a.edge == None:
                    return False
                else:
                    return a.edge.label < b.edge.label
            else:
                return False
        
        @classmethod
        def cmpCoderLinks(cls, a, b):
            if a.coder.code < b.coder.code:
                return -1
            elif a.coder.code > b.coder.code:
                return 1    
            else:
                if a.edge is None:
                    return 0
                else:
                    if a.edge.directed:
                        if b.edge.directed:
                            if a.edge.source == a.coder.vertex:
                                if b.edge.source == b.coder.vertex:
                                    return 0
                                else:
                                    return -1
                            else:
                                if b.edge.source == b.coder.vertex:
                                    return 1
                                else:
                                    return 0
                        else:
                            return -1
                    else:
                        if b.edge.directed:
                            return 1
                        else:
                            return 0


def testGraphs():
    # Create graph.
    graph = Graph()

    # Add vertices.
    a = graph.addVertex()
    b = graph.addVertex()
    c = graph.addVertex()
    d = graph.addVertex()
    e = graph.addVertex()
    f = graph.addVertex()
    g = graph.addVertex()
    h = graph.addVertex()
    i = graph.addVertex()
    j = graph.addVertex()
    k = graph.addVertex()
    l = graph.addVertex()
    m = graph.addVertex()
    n = graph.addVertex()
    o = graph.addVertex()
    p = graph.addVertex()
    q = graph.addVertex()
    r = graph.addVertex()
    s = graph.addVertex()
    t = graph.addVertex()
    u = graph.addVertex()
    v = graph.addVertex()
    w = graph.addVertex()
    x = graph.addVertex()
    y = graph.addVertex()
    z = graph.addVertex()

    # Connect edges.
    graph.connectVertices(a, b, False)
    graph.connectVertices(b, d, False)
    graph.connectVertices(d, e, False)
    graph.connectVertices(e, f, False)
    graph.connectVertices(f, c, False)
    graph.connectVertices(c, a, False)

    graph.connectVertices(g, c, False)
    graph.connectVertices(f, j, False)
    graph.connectVertices(j, i, False)
    graph.connectVertices(i, h, False)
    graph.connectVertices(h, g, False)

    graph.connectVertices(e, s, False)
    graph.connectVertices(s, r, False)
    graph.connectVertices(r, l, False)
    graph.connectVertices(l, j, False)

    graph.connectVertices(l, n, False)
    graph.connectVertices(n, m, False)
    graph.connectVertices(m, k, False)
    graph.connectVertices(k, i, False)

    graph.connectVertices(s, t, False)
    graph.connectVertices(t, u, False)
    graph.connectVertices(u, v, False)
    graph.connectVertices(v, q, False)
    graph.connectVertices(q, r, False)

    graph.connectVertices(q, p, False)
    graph.connectVertices(p, o, False)
    graph.connectVertices(o, n, False)


    graph1 = graph.clone()
    graph1.connectVertices(u, z, False)
    graph1.connectVertices(z, y, False)
    graph1.connectVertices(y, x, False)
    graph1.connectVertices(x, w, False)
    graph1.connectVertices(w, v, False)

    graph2 = graph.clone()
    graph2.connectVertices(t, z, False)
    # graph2.connectVertices(z, y, False)
    # graph2.connectVertices(y, x, False)
    # graph2.connectVertices(x, w, False)
    # graph2.connectVertices(w, u, False)

    graph1_hash = GraphHash().hash(graph1)
    graph2_hash = GraphHash().hash(graph2)
    print(f'''graph1_hash = {graph1_hash}''')
    print(f'''graph2_hash = {graph2_hash}''')
    
testGraphs()
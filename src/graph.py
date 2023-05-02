import collections
from hashlib import md5 
import functools
import pickle

# NOTES:
    # .id never seems to be used 
    # NO_HASH_VERTEX_LABEL never seems to be used, and seems like it might be a rare edgecase bug
    # .vertex_branch is never used
    # .labelClass is never used

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

    def add_vertex(self, label=None):
        vertex = self.Vertex(label)
        assert vertex is not None
        self.vertices.append(vertex)
        return vertex

    def connect_vertices(self, source, target, directed=False, label=None):
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

    def get_vertex(self, label):
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
            new_vertex = new_graph.add_vertex(old_vertex.label)
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
        self.vertex_coder = None
    
    def hash(self, graph):
        self.code = consistent_hash(tuple())
        self.vertex_coder = None

        self.vertex_coder = self.VertexCoder()
        for vertex in graph.vertices:
            child = self.VertexCoder(vertex)
            link = self.VertexCoderLink(None, child)
            self.vertex_coder.children.append(link)
        self.vertex_coder.encode()
        self.code = self.vertex_coder.code
        return self.code
    
    class VertexCoderLink:
        def __init__(self, edge=None, coder=None):
            self.edge = edge
            self.coder = coder
        
    class VertexCoder:
        def __init__(self, vertex=None,):
            self.vertex = vertex
            self.code = consistent_hash(tuple())
            self.code_valid = False
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
                    coder.children.sort(key=functools.cmp_to_key(self.cmp_coder_links))
                    coder.newcode = consistent_hash(tuple(each.coder.code for each in coder.children))
                
                for each_child in self.children:
                    coder = each_child.coder
                    coder.code = coder.newcode
                    coder.code_valid = True
            
            self.children.sort(key=functools.cmp_to_key(self.cmp_coder_links))
            self.code = consistent_hash(tuple(each.coder.code for each in self.children))
            self.code_valid = True

        # private helper methods
        @classmethod
        def ltcmp_coder_links(cls, a, b):
            return cls.cmp_coder_links(a, b) < 0
        
        @classmethod
        def ltcmp_coder_links_labeled(cls, a, b):
            result = cls.cmp_coder_links(a, b)

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
        def cmp_coder_links(cls, a, b):
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
    a = graph.add_vertex()
    b = graph.add_vertex()
    c = graph.add_vertex()
    d = graph.add_vertex()
    e = graph.add_vertex()
    f = graph.add_vertex()
    g = graph.add_vertex()
    h = graph.add_vertex()
    i = graph.add_vertex()
    j = graph.add_vertex()
    k = graph.add_vertex()
    l = graph.add_vertex()
    m = graph.add_vertex()
    n = graph.add_vertex()
    o = graph.add_vertex()
    p = graph.add_vertex()
    q = graph.add_vertex()
    r = graph.add_vertex()
    s = graph.add_vertex()
    t = graph.add_vertex()
    u = graph.add_vertex()
    v = graph.add_vertex()
    w = graph.add_vertex()
    x = graph.add_vertex()
    y = graph.add_vertex()
    z = graph.add_vertex()

    # Connect edges.
    graph.connect_vertices(a, b, False)
    graph.connect_vertices(b, d, False)
    graph.connect_vertices(d, e, False)
    graph.connect_vertices(e, f, False)
    graph.connect_vertices(f, c, False)
    graph.connect_vertices(c, a, False)

    graph.connect_vertices(g, c, False)
    graph.connect_vertices(f, j, False)
    graph.connect_vertices(j, i, False)
    graph.connect_vertices(i, h, False)
    graph.connect_vertices(h, g, False)

    graph.connect_vertices(e, s, False)
    graph.connect_vertices(s, r, False)
    graph.connect_vertices(r, l, False)
    graph.connect_vertices(l, j, False)

    graph.connect_vertices(l, n, False)
    graph.connect_vertices(n, m, False)
    graph.connect_vertices(m, k, False)
    graph.connect_vertices(k, i, False)

    graph.connect_vertices(s, t, False)
    graph.connect_vertices(t, u, False)
    graph.connect_vertices(u, v, False)
    graph.connect_vertices(v, q, False)
    graph.connect_vertices(q, r, False)

    graph.connect_vertices(q, p, False)
    graph.connect_vertices(p, o, False)
    graph.connect_vertices(o, n, False)


    graph1 = graph.clone()
    graph1.connect_vertices(u, z, False)
    graph1.connect_vertices(z, y, False)
    graph1.connect_vertices(y, x, False)
    graph1.connect_vertices(x, w, False)
    graph1.connect_vertices(w, v, False)

    graph2 = graph.clone()
    graph2.connect_vertices(t, z, False)
    graph2.connect_vertices(z, y, False)
    graph2.connect_vertices(y, x, False)
    graph2.connect_vertices(x, w, False)
    graph2.connect_vertices(w, u, False)

    graph1_hash = GraphHash().hash(graph1)
    graph2_hash = GraphHash().hash(graph2)
    print(f'''graph1_hash = {graph1_hash}''')
    print(f'''graph2_hash = {graph2_hash}''')
    
testGraphs()
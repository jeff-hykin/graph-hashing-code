import collections
from hashlib import md5 
import pickle

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
        for index, vertex in enumerate(self.vertices):
            vertex.id = index


class GraphHash:
    NO_HASH_VERTEX_LABEL = 0xfffffffe
    def __init__(self):
        self.graph = None
        self.code = None
        self.vertexCoder = None
    
    def getHash(self):
        pass
    
    class VertexCoder:
        def __init__(self, vertex=None, vertexBranch=None):
            self.vertex = vertex
            self.vertexBranch = vertexBranch or []
            self.code = None
            self.codeValid = False
            self.newcode = None
            self.children = []
            self.labelClass = None # int
        
        def encode(self):
            pass
            
            consistent_hash
        
        @staticmethod
        def ltcmpCoderLinks(VertexCoderLink *a, VertexCoderLink *b):
            pass
        @staticmethod
        def ltcmpCoderLinksLabeled(VertexCoderLink *a, VertexCoderLink *b):
            pass
        @staticmethod
        def cmpCoderLinks(VertexCoderLink *a, VertexCoderLink *b):
            pass
    
    class VertexCoderLink:
        def __init__(self, edge=None, coder=None):
            self.edge = edge
            self.coder = coder
        
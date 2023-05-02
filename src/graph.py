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
    
    class VertexCoderLink:
        def __init__(self, edge=None, coder=None):
            self.edge = edge
            self.coder = coder
        
    class VertexCoder:
        def __init__(self, vertex=None, vertexBranch=None):
            self.vertex = vertex
            self.vertexBranch = vertexBranch or []
            self.code = tuple()
            self.codeValid = False
            self.newcode = tuple()
            self.children = []
            self.labelClass = 0
        
        def encode(self):
            number_of_children = len(self.children)
            for child_index, child in enumerate(self.children):
                coder = child.coder
                coder.code = tuple()
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
                    coder.code = copy(coder.newcode)
                    coder.codeValid = True
            
            self.children.sort(key=functools.cmp_to_key(self.cmpCoderLinks))
            self.code = consistent_hash(tuple(each.coder.code for each in coder.children))
            self.codeValid = True

        def printCode(self, fp=None):
            fp = fp or sys.stdout
            print("".join(f"{b:02x}" for b in self.code), file=fp)
        
        # private helper methods
        @classmethod
        def ltcmpCoderLinks(cls, a, b):
            return cls.cmpCoderLinks(a, b) < 0
        
        @classmethod
        def ltcmpCoderLinksLabeled(cls, a, b):
            return cls.cmpCoderLinks(a, b) < 0
        
        @classmethod
        def cmpCoderLinks(cls, a, b):
            if a.edge.label < b.edge.label:
                return -1
            elif a.edge.label > b.edge.label:
                return 1
            else:
                return 0

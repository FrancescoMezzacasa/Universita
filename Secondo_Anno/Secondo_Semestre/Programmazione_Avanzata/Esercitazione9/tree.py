import random
from ctypes import cdll, c_bool, c_float, c_int, pointer, POINTER, Structure


class CTreeNode(Structure):
    pass


CTreeNode._fields_ = [("key", c_int),
                      ("value", c_float),
                      ("left", POINTER(CTreeNode)),
                      ("right", POINTER(CTreeNode))]


class CTree:

    code = None

    def __init__(self):
        if CTree.code is None:
            CTree.code = cdll.LoadLibrary("libtree.so")
            CTree.code.insert.argtypes = [POINTER(CTreeNode), c_int, c_float]
            CTree.code.insert.restype = POINTER(CTreeNode)
            CTree.code.search.argtypes = [POINTER(CTreeNode),
                                          c_int, POINTER(c_float)]
            CTree.code.search.restype = c_bool
            CTree.code.destroy.argtypes = [POINTER(CTreeNode)]
        self.root = POINTER(CTreeNode)()

    def search(self, key):
        pass

    def insert(self, key, value):
        pass

    def __str__(self):
        pass

    def __del__(self):
        CTree.code.destroy(self.root)


class Node:

    def __init__(self, key, value):
        self.key = key
        self.value = value
        self.dx = None
        self.sx = None

    def search(self, key):
        if(self.key == key):
            return self.value
        elif(key < self.key and self.sx != None):
            return(self.sx.search(key))
        elif(key > self.key and self.dx != None):
            return(self.dx.search(key))
        return None

    def insert(self, key, value):
        if(self.key == key):
            self.value = value
        elif(key < self.key):
            if(self.sx == None):
                self.sx = Node(key, value)
            else:
                self.sx.insert(key, value)
        elif(key > self.key):
            if(self.dx == None):
                self.dx = Node(key, value)
            else:
                self.dx.insert(key, value)

    def __str__(self):
        if(self.key == None):
            return("None ")
        else:
            stringa = f"( {self.key} "
            if(self.sx != None):
                stringa += self.sx.__str__()
            else:
                stringa += "None "
            
            if(self.dx != None):
                stringa += self.dx.__str__()
            else:
                stringa += "None "
            stringa += ")"
            return(stringa)

        


class BinarySearchTree:

    def __init__(self):
        self.root = None

    def search(self, key):
        if(self.root == None):
            return None
        return(self.root.search(key))

    def insert(self, key, value):
        if(self.root == None):#se il nodo radice è null, quindi tipo albero vuoto
            self.root = Node(key, value)
        else:
            self.root.insert(key, value)

    def __str__(self):
        if(self.root == None):
            return("None ")
        return(self.root.__str__())
        


if __name__ == "__main__":
    bst = BinarySearchTree()
    for i in range(0, 20):
        bst.insert(random.randint(0, 30), random.random())
    for i in range(0, 10):
        k = random.randint(0, 30)
        print(f"Searching for {k}: {bst.search(k)}")
    print(bst)

    # Decommentare per la parte extra
    # cbst = CTree()
    # for i in range(0, 20):
    #     cbst.insert(random.randint(0, 30), random.random())
    # for i in range(0, 10):
    #     k = random.randint(0, 30)
    #     print(f"Searching for {k}: {cbst.search(k)}")
    # print(cbst)

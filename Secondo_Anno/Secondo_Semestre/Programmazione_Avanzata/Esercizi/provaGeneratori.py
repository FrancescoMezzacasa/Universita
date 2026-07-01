class fib():
    def __init__(self):
        pass
    
    def __iter__(self):
        self.primo = 0
        self.secondo = 1
        return self
    
    def __next__(self):
        fib = self.primo + self.secondo
        self.primo = self.secondo
        self.secondo = fib
        return fib
    
classe = fib()
it = iter(classe)
valori = [next(it) for i in range(10)]
print(valori)
from collections import defaultdict


class Polynomial:

    def __init__(self, coeffs):
        self.coeffs = defaultdict(int)#cosi se non trova la chiave da 0
        for i in coeffs.keys():
            self.coeffs[i] = coeffs[i]

    def __getitem__(self, k):
        return(self.coeffs[k])

    def __setitem__(self, k, v):
        self.coeffs[k] = v

    def __call__(self, x):
        somma = 0
        for i in self.coeffs.keys():
            somma += self.coeffs[i]*(x**i)
        return somma

    def __add__(self, other):
        ris = defaultdict(int)
        for i in self.coeffs.keys():
            ris[i] += self.coeffs[i]
        for i in other.coeffs.keys():
            ris[i] += other.coeffs[i]
        return Polynomial(ris)

    def __sub__(self, other):
        ris = defaultdict(int)
        for i in self.coeffs.keys():
            ris[i] -= self.coeffs[i]
        for i in other.coeffs.keys():
            ris[i] -= other.coeffs[i]
        return Polynomial(ris)

    def __mul__(self, other):
        #devo moltiplicare ogni termine
        ris = defaultdict(int)
        for i in self.coeffs.keys():
            for j in other.coeffs.keys():
                ris[i+j] += self.coeffs[i]*other.coeffs[j]
        return Polynomial(ris)

    def __pow__(self, n):
        #vedo la potenza come una moltiplicazione con se stesso
        if(n == 0):
            return Polynomial({0: 1})#alla 0 fa 1
        ris = Polynomial(self.coeffs)
        for i in range(n-1):
            ris *= self
        return ris

    def derivative(self):
        ris = defaultdict(int)
        for grado, coeff in self.coeffs.items():#cosi ogni volta mi salva grado e coefficiente
            if(grado != 0):
                ris[grado-1] = coeff * grado
        return Polynomial(ris)

    def __str__(self):
        stringa = ""
        for i in reversed(sorted(self.coeffs.keys())):#cosi stampa sempre prima il grado maggiore
            stringa += f"{self.coeffs[i]}x^{i}" + " + "
        return stringa[:-3]#tolgo il + messo alla fine


def newton_raphson(p, x, n_iter=20):
    derivata = p.derivative()
    for i in range(n_iter):
        x = x - (p(x)/derivata(x))
    return x


p1 = Polynomial({4: 2, 3: 5, 1: 6, 0: -2})
p2 = Polynomial({3: 1, 2: 0.5, 1: -1, 0: 3})
print(p1)
# print(p1.derivative())
# print(p2)
# print(p2.derivative())

# p1[3] = 10
# print(p1)
#print(p2[1])

#print(p1 + p2)
#print(p1 * p2)
#print(p1**2)
#print(p1(3))
# print((p1+p2)(20))
# 
x = newton_raphson(p1, 5)
print(f"p({x}) = {p1(x)}")

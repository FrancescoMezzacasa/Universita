#voglio decorare la somma
def decoratore(funz):#funz = somma
    def wrapper(*args, **kwargs):#args = (3, 4)
        print(f"Gli addendi sono {args}")
        ris = funz(*args, **kwargs)
        print(f"Ho fatto la somma e vale {ris}")
        return ris
    return wrapper

@decoratore
def somma(a, b):
    return a + b

s = somma(3, 4)
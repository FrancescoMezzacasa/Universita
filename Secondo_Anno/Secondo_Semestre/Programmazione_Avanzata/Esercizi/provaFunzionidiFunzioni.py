from functools import partial

def potenze(base, esp):
    return base ** esp

pot2 = partial(potenze, esp=2)
print([pot2(i) for i in range(10)])
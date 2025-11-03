def prodottoCartesiano(a, b):
    '''
    (set, set) -> set
    '''
    ris = set()
    for i in a:
        for j in b:
            ris.add((i, j))
    return ris

print(prodottoCartesiano({1, 2}, {3, 4}))
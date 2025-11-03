def tabelline(N):
    '''
    <int> -> <dict<int><int>>
    '''
    dizionario = dict()
    for i in range(1, 13):
        multipli = []
        for j in range(N):
            multipli.append(j*i)
        dizionario[i] = multipli
    return dizionario

N = int(input("Inserire un numero"))

diz = tabelline(N)

for k, v in diz.items():
    print(f"{k:2}", v)
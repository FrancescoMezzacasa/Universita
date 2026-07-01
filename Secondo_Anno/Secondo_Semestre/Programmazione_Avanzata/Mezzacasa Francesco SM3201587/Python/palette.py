#FRANCESCO MEZZACASA SM3201587
import json
from exceptions import PaletteError
import numpy as np

class Palette():
    def __init__(self, pathpalette):
        #apre il file con la palette, fa i controlli per capire che sia conforme a quello che vogliamo
        #e poi mette la lista di colori ottenuta in self.lista
        try:
            with open(pathpalette, "r") as palettefile:
                lista = json.load(palettefile)
        except FileNotFoundError:
            raise PaletteError("File palette non trovato")
        except json.JSONDecodeError:#JSONDecodeError è l'errore che si crea quando il file dato non è in formato json valido
            raise PaletteError("File JSON palette non valido")

        if(type(lista) != list):
            raise PaletteError("Il file JSON deve contenere una lista")
        if(len(lista) != 16):
            raise PaletteError("La palette deve contenere 16 colori")
        for i in lista:
            if(type(i) != list):
                raise PaletteError("La palette deve contenere colori rappresentati come liste")
            if(len(i) != 3):
                raise PaletteError("I colori devono essere rappresentati in formato RGB, quindi richiedono 3 numeri")
            for j in i:#scorro sui numeri nei singoli elementi della palette
                if(type(j) != int or j < 0 or j > 255):
                    raise PaletteError("I singoli valori devono essere rappresentati da numeri interi da 0 a 255")
        #se sono qua ho la lista di colori giusta
        self.lista = np.array(lista, dtype=np.uint8)

    
    def __getitem__(self, indice):
        return self.lista[indice]
    

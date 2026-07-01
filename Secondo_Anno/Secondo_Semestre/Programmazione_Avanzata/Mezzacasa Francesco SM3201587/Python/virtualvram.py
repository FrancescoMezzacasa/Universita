#FRANCESCO MEZZACASA SM3201587
import numpy as np
from exceptions import VRAMError

class VirtualVRAM():
    def __init__(self, tilepath, spritepath):
        self.tiles = self._decodifica_sheet(tilepath)
        self.sprites = self._decodifica_sheet(spritepath)
    
    #uso un metodo protetto
    #che sarà unico per sprite e tile perché tanto sono matrici della stessa dimensione
    def _decodifica_sheet(self, filepath):
        #qua le variabili le dichiaro senza self. perche tanto non mi interessa che rimangano come attributi
        try:
            sheet = np.fromfile(filepath, dtype=np.uint8)
        except FileNotFoundError:
            raise VRAMError("File sheet non trovato")
        if(len(sheet) != 32768):
            raise VRAMError(f"Dimensione del file {filepath} errata")
        #per prendere o i primi o gli ultimi 4 bit faccio l'and con 11110000 e 00001111
        #però per i primi otterrei il numero che voglio ma seguito da 4 zeri, quindi uso l'operatore di shift >>
        ultimi = sheet & 0b00001111 #0b indica numero binario
        primi = (sheet & 0b11110000) >> 4 #cosi se ero a 00110000 diventa 00000011
        #ora devo "combinarli" nella matrice finale che ritornerò
        #la creo vuota tanto so la dimensione
        matrice = np.empty(65536, dtype=np.uint8)
        #poi per non stare a fare il ciclo for vedo che i primi vanno sempre negli indici pari
        #e gli ultimi in quelli dispari
        matrice[0::2] = primi
        matrice[1::2] = ultimi
        #ora ho correttamente interpretato i numeri dei file binari e li ho messi in una lista piatta
        #ora faccio il reshape e cosi ho la matrice
        return matrice.reshape((256, 256))
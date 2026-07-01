#FRANCESCO MEZZACASA SM3201587
import numpy as np
from blitter import Blitter
from PIL import Image

class RenderingPipeline():
    def __init__(self, scena, vram, palette):#gli passo istanze che ho creato nel main
        #il blitter invece non lo passo perché mi serve solo ad uso "interno" quindi lo istanzio direttamente qua
        self.scena = scena
        self.vram = vram
        self.palette = palette

        #creo il frame buffer (di fatto per ora solo una matrice)
        self.framebuffer = np.zeros(shape=(480, 640), dtype=np.uint8)

        self.blitter = Blitter(self.framebuffer, self.vram)
        #ora ho tutto quello che mi serve per fare il rendering
    
    def render(self, pathoutput):
        #innanzitutto inizio a mettere i tile sullo sfondo
        #che tile posizionare e dove lo trovo nel campo tile_map della scena
        for i in range(len(self.scena.tile_map)):#scorro tile_map che è lista di liste
            for j in range(len(self.scena.tile_map[i])):
                self.blitter.posizionaTile(self.scena.tile_map[i][j], i, j)#l'id lo trovo proprio nella tilemap


        #ora devo piazzare gli sprite
        for sprite in self.scena.sprites:#il campo sprites era la lista di sprite
            self.blitter.posizionaSprite(sprite["id"], sprite["x"], sprite["y"], sprite["flip_h"], sprite["flip_v"], sprite["rotation"], self.scena.transparent_index)
        
        #ora ho il frame buffer che è un array di indici di colori, ora riconverto gli indici ai colori veri perché pillow vuole gli RGB
        framebufColorato = self.palette[self.framebuffer]
        im = Image.fromarray(framebufColorato)
        im.save(pathoutput)


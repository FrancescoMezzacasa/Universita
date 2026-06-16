#FRANCESCO MEZZACASA SM3201587
import sys#per gli argomenti da riga di comando
from palette import Palette
from virtualvram import VirtualVRAM
from sceneparser import SceneParser
from renderingpipeline import RenderingPipeline

from exceptions import PaletteError, VRAMError, SceneParsingError, BlitterError

#main
if(len(sys.argv) != 6):
    print("Numero di argomenti errato, sono richiesti 5 path")
    exit(1)

#prendo i path passati (nell'ordine giusto)
pathpalette = sys.argv[1]
pathscena = sys.argv[2]
pathtiles = sys.argv[3]
pathsprites = sys.argv[4]
pathoutput = sys.argv[5]

#ora istanzio quello che serve a rendering pipeline e faccio l'esecuzione vera e propria
#metto tutto in un try except cosi qualsiasi eccezione si generi la prendo
try:
    print("Inizio esecuzione")
    scena = SceneParser(pathscena)
    palette = Palette(pathpalette)
    vram = VirtualVRAM(pathtiles, pathsprites)

    #ora ho tutto quello che serve a rendering pipeline quindi lo chiamo
    pipeline = RenderingPipeline(scena, vram, palette)
    #chiamo l'effettiva funzinoe di rendering
    pipeline.render(pathoutput)
    print("Immagine creata con successo")
except (PaletteError, VRAMError, SceneParsingError, BlitterError) as error:
    print(f"Errore: {error}")
    exit(1)


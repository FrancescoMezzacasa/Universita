#!/bin/bash

# Si realizzi in Bash una semplice Container Engineche funziona senza richiedere 
# privilegi di amministratore.Essa deve permettere di eseguire un programma in un 
# ambiente isolato.

# Sintassi: container-run.sh conf-file command

# conf-file: file testualeche elenca (gli unici) fileo cartelleche devono essere 
# disponibili all’internocontainer. La Container Engine deve costruire un opportuno 
# albero di cartelle e file come specificato in tale file.

# Esempio di conf-file (dov'è e dove metterlo):
# /usr/bin/ls /bin/ls
# /usr/bin/ps /bin/ps
# /bin/bash /bin/bash
# /lib /lib
# /lib64 /lib64

# Esempio di comando:
# ./container-run.sh conf-file /bin/ls -lh /lib

# Creazione Directory temporanea:
path_c="/tmp/container"
mkdir "$path_c"

# Uso bindfs --no-allow-other$ORIGIN $DST per montare le cartelle

file=$1
command="${@:2}" # Per il comando e gli argomenti uso "${@:2}"
echo "Comando: $command"

# Controllo se il file di configurazione esiste e non è vuoto
if [[ ! -f "$file" ]]; then
    echo "Error: '$file' not found"
    exit 1
fi

if [[ ! -s "$file" ]]; then
    echo "Error: '$file' is empty"
    exit 1
fi




while read -r line || [[ -n "$line" ]]; do

    # Estrazione del percorso sorgente e destinazione
    src=$(echo "$line" | cut -d ' ' -f 1)
    dst=$(echo "$line" | cut -d ' ' -f 2)
    dst="$path_c$dst"

    # Vedo se è una cartella
    if [[ -d "$src" ]]; then

        echo "Montando $src su $dst (directory)"

        # Creazione della directory di destinazione
        mkdir -p "$dst"

        # Montaggio del file o della cartella
        bindfs --no-allow-other "$src" "$dst"
    else
        # Se è un file, lo copio nella destinazione
        echo "Copiando $src su $dst (file)"

        dst_dir=$(dirname "$dst") 
        mkdir -p "$dst_dir"
        cp "$src" "$dst"
    fi
done < "$file"

ls -l "$path_c"
ls -l "$path_c/bin"

echo "Path: $path_c"
echo "Comando: $command"

fakechroot chroot "$path_c" $command
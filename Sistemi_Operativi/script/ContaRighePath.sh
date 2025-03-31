#!/bin/bash
while true; do
    echo "Inserire un file (path): "
    read file

    if [[ $file == "quit" ]]; then
        echo "Esco dal programma"
        break
    elif [[ -f $file ]]; then
        conto=$(wc -l < $file)
        #echo $conto
        echo "$file ha $conto righe"
    else
        echo "Il file $file non esiste"
    fi
done
#!/bin/bash
if  [ "$#" != "2" ]
then
    echo "Inserire due argomenti"
else
    find . -name $1 > $2
fi
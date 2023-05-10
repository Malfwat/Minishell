#! /bin/bash

nb=$(echo $#)

if [ ! "$nb" -eq 2 ]; then
    echo 'example of usage: my_push.sh "MAJ" "modifications"'
    exit
fi

git add . && git commit -m "[$1]: $2" && git push
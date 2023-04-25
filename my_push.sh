#! /bin/bash

if [$# != 2]; then
    echo "example of usage\nmy_push.sh 'MAJ' 'moficications'"
    exit
fi

git add . && git commit -m "[$1]: $2" && git push
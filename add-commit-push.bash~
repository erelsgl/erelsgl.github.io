#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR
echo ==== erel-sites ====
date
echo -- add --
/usr/bin/git add -A
echo -- commit --
/usr/bin/git commit -m "$1"
echo -- pull --
/usr/bin/git pull
echo -- push --
/usr/bin/git push


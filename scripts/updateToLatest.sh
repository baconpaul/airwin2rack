#!/bin/bash

pushd libs/airwindows
git fetch origin 
git reset origin/master --hard
popd

perl scripts/configure.pl

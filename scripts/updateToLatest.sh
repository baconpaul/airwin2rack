#!/bin/bash

pushd libs/airwindows
git pull origin master
popd

perl scripts/configure.pl
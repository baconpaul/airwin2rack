#!/bin/bash

GHASH=`git rev-parse --short HEAD`
sed -e "s/HASH/$GHASH/" scripts/check-update/check-update.html.in

# All the Airwindows

It's all the [airwindows](https://www.airwindows.com/) in a single
module for [VCV Rack](https://www.vcvrack.com/) coded in collaboration
with the [Surge Synth Team](https://surge-synthesizer.github.io).

Have fun!

## Updating the airwindows sub-library

To update the airwindows library

1. Pull to the latest airwindows plugins

```bash
cd libs/airwindows
git pull origin master
cd ../..
```
2. run `scripts/configure.pl`. This will read the latest airwindowpedia and what.txt from the git repo
3. Do a test build
```bash
RACK_DIR=(path-to-sdk) make -j clean
RACK_DIR=(path-to-sdk) make -j install
```
4. Commit src libs and the infile and push to github

## Licensing

This is MIT licensed, like Airwin, but the combined work with rack is of course
GPL3 as distributed here.

The clipper airwindows graphic is freely distributed by airwindows

the jakarta and fira mono font are both openSIL
https://tokotype.github.io/plusjakarta-sans/

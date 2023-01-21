# All the Airwindows

It's all the [airwindows](https://www.airwindows.com/) in a single
module for [VCV Rack](https://www.vcvrack.com/) coded in collaboration
with the [Surge Synth Team](https://surge-synthesizer.github.io).

Have fun!

## Notes on building

We are using @qno's excellent cmake SDK. This means the makefile works
like any other rack project. 

But if you pull and want to clean build, make sure to run both the `clean` and `cleandep`
targets to rebuild fully.

## Updating the airwindows sub-library

To update the airwindows library

1. Pull to the latest airwindows plugins

```bash
./scripts/updateToLatest.sh
```
3. Do a test build
```bash
RACK_DIR=(path-to-sdk) make -j cleandep
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

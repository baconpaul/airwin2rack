# All the Airwindows Consolidated

It's all the [airwindows](https://www.airwindows.com) presented in three lovely flavors

1. As a static library with a uniform registry and access pattern for you to use
   as a submodule to expose the airwindows
2. As module for [VCV Rack](https://www.vcvrack.com/) in the [Rack library](https://library.vcvrack.com/Airwin2Rack/Airwin2Rack) or as a [Rack nightly build](https://github.com/baconpaul/airwin2rack/releases/tag/Nightly)
3. As a [CLAP/VST3/AU/LV2/Standalone](https://github.com/baconpaul/airwin2rack/releases/tag/DAWPlugin) plugin for your DAW

Have fun! If you are a user the above links are everything you need. If you are a dev read on!

## The Library Version

The target `airwin-registry` builds a static library for you containing
all of the airwindows under a uniform api. Documenting this is still a
todo, but if you link this target, it will automatically populate the
datastructures so`AirwinRegistry.h` does what you would expect, which is 
give you a map to create `airwin2rackbase` operator objects. 

Have a question? Open an issue!

## The Rack Plugin

We are using @qno's excellent cmake SDK. This means the makefile works
like any other rack project.

But if you pull and want to clean build, make sure to run both the `clean` and `cleandep`
targets to rebuild fully.

## The JUCE plugin

```bask
cmake -Bignore/daw-plugin -DBUILD_JUCE_PLUGIN=TRUE -DCMAKE_BUILD_TYPE=Release
cmake --build ignore/daw-plugin --target awcons-products
```

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

## A note on licensing

Airwindows is MIT licensed and the source code here is also. But using the
GPL3 JUCE and VST3 sdk results in a combined work licensed under GPL3, and
similarly the Rack plugin is a GPL3 dependency

We made the standalone consoldiated effecfs library MIT so that you can build *just* the static library
and use the code in your MIT or closed source project, though.

Still unsure what you can use in a closed source environment?  The answer
is basicaly `AirwinRegistry.h` and its dependencies, the cmake target
`airwin-registry` and the documentation in `res/awdoc`. Still stil unsure?
Open an issue and ask!

The clipper airwindows graphic is freely distributed by airwindows

the jakarta and fira mono font are both openSIL
https://tokotype.github.io/plusjakarta-sans/

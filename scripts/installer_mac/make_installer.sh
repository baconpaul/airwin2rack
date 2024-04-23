#!/bin/bash

# Documentation for pkgbuild and productbuild: https://developer.apple.com/library/archive/documentation/DeveloperTools/Reference/DistributionDefinitionRef/Chapters/Distribution_XML_Ref.html

# preflight check
PRODUCT=$1
INDIR=$2
RESOURCESDIR=$3
TARGET_DIR=$4
VERSION=$5

TMPDIR="./installer-tmp"
mkdir -p $TMPDIR

echo "MAKE from $INDIR $RESOURCESDIR into $TARGET_DIR with $VERSION"

VST3="${PRODUCT}.vst3"
AU="${PRODUCT}.component"
CLAP="${PRODUCT}.clap"
APP="${PRODUCT}.app"

PRODUCTFILE=`echo $PRODUCT | tr ' ' '-' | tr '[:upper:]' '[:lower:]'`
echo "PRODUCTFILE is ${PRODUCTFILE}"

mkdir $TMPDIR/AppResourcesPackageScript
cat > $TMPDIR/AppResourcesPackageScript/postinstall << PIEND
#!/bin/bash
rsync -r --delete "/tmp/sst-installer/$APP" /Applications
chown -R $USER:staff "/Applications/$APP";
# rm -rf /tmp/sst-installer
date > /tmp/sst-installer/handled
PIEND

chmod 755 $TMPDIR/AppResourcesPackageScript/postinstall


if [ "$VERSION" == "" ]; then
    echo "You must specify the version you are packaging!"
    echo "eg: ./make_installer.sh 1.0.6b4"
    exit 1
fi


OUTPUT_BASE_FILENAME="${PRODUCTFILE}-macOS-$VERSION"

build_flavor()
{
    flavor=$1
    flavorprod=$2
    ident=$3
    loc=$4
    scripts=$5

    echo --- BUILDING ${PRODUCTFILE}_${flavor}.pkg from "$flavorprod" ---

    workdir=$TMPDIR/$flavor
    mkdir -p $workdir

    # In the past we would pkgbuild --analyze first to make a plist file, but we are perfectly fine with the
    # defaults, so we skip that step here. http://thegreyblog.blogspot.com/2014/06/os-x-creating-packages-from-command_2.html
    # was pretty handy in figuring that out and man pkgbuild convinced us to not do it, as did testing.
    #
    # The defaults only work if a component is a sole entry in a staging directory though, so synthesize that
    # by moving the product to a tmp dir

    cp -r "$INDIR/$flavorprod" "$workdir"
    ls -l $workdir

    sca=""
    if [[ ! -z $scripts ]]; then
      sca="--scripts $scripts"
    fi

    if [[ ! -z $MAC_SIGNING_CERT ]]; then
      [[ -z $MAC_INSTALLING_CERT ]] && echo "You need an installing cert too " && exit 2
      codesign --force -s "$MAC_SIGNING_CERT" -o runtime --deep "$workdir/$flavorprod"
      codesign -vvv "$workdir/$flavorprod"

      pkgbuild --sign "$MAC_INSTALLING_CERT" --root $workdir --identifier $ident --version $VERSION --install-location "$loc" "$TMPDIR/${PRODUCTFILE}_${flavor}.pkg" $sca || exit 1
      echo pkgbuild --sign "$MAC_INSTALLING_CERT" --root $workdir --identifier $ident --version $VERSION --install-location "$loc" "$TMPDIR/${PRODUCTFILE}_${flavor}.pkg" $sca || exit 1
    else
      pkgbuild --root $workdir --identifier $ident --version $VERSION --install-location "$loc" "$TMPDIR/${PRODUCTFILE}_${flavor}.pkg" $sca || exit 1
    fi

    #rm -rf $workdir
}


if [[ -d $INDIR/$VST3 ]]; then
    build_flavor "VST3" "$VST3" "org.surge-synth-team.${PRODUCTFILE}.vst3.pkg" "/Library/Audio/Plug-Ins/VST3"
fi

if [[ -d $INDIR/$AU ]]; then
    build_flavor "AU" "$AU" "org.surge-synth-team.${PRODUCTFILE}.component.pkg" "/Library/Audio/Plug-Ins/Components"
fi

if [[ -d $INDIR/$APP ]]; then
    build_flavor "APP" "$APP" "org.surge-synth-team.${PRODUCTFILE}.app.pkg" "/tmp/sst-installer" $TMPDIR/AppResourcesPackageScript
fi

if [[ -d $INDIR/$CLAP ]]; then
    build_flavor "CLAP" "$CLAP" "org.surge-synth-team.${PRODUCTFILE}.clap.pkg" "/Library/Audio/Plug-Ins/CLAP"
fi

echo --- Sub Packages Created ---
ls -l "${TMPDIR}"

# create distribution.xml

if [[ -d $INDIR/$VST3 ]]; then
	VST3_PKG_REF="<pkg-ref id=\"org.surge-synth-team.${PRODUCTFILE}.vst3.pkg\"/>"
	VST3_CHOICE="<line choice=\"org.surge-synth-team.${PRODUCTFILE}.vst3.pkg\"/>"
	VST3_CHOICE_DEF="<choice id=\"org.surge-synth-team.${PRODUCTFILE}.vst3.pkg\" visible=\"true\" start_selected=\"true\" title=\"${PRODUCT} VST3\"><pkg-ref id=\"org.surge-synth-team.${PRODUCTFILE}.vst3.pkg\"/></choice><pkg-ref id=\"org.surge-synth-team.${PRODUCTFILE}.vst3.pkg\" version=\"${VERSION}\" onConclusion=\"none\">${PRODUCTFILE}_VST3.pkg</pkg-ref>"
fi
if [[ -d $INDIR/$AU ]]; then
	AU_PKG_REF="<pkg-ref id=\"org.surge-synth-team.${PRODUCTFILE}.component.pkg\"/>"
	AU_CHOICE="<line choice=\"org.surge-synth-team.${PRODUCTFILE}.component.pkg\"/>"
	AU_CHOICE_DEF="<choice id=\"org.surge-synth-team.${PRODUCTFILE}.component.pkg\" visible=\"true\" start_selected=\"true\" title=\"${PRODUCT} Audio Unit\"><pkg-ref id=\"org.surge-synth-team.${PRODUCTFILE}.component.pkg\"/></choice><pkg-ref id=\"org.surge-synth-team.${PRODUCTFILE}.component.pkg\" version=\"${VERSION}\" onConclusion=\"none\">${PRODUCTFILE}_AU.pkg</pkg-ref>"
fi
if [[ -d $INDIR/$CLAP ]]; then
	CLAP_PKG_REF="<pkg-ref id=\"org.surge-synth-team.${PRODUCTFILE}.clap.pkg\"/>"
	CLAP_CHOICE="<line choice=\"org.surge-synth-team.${PRODUCTFILE}.clap.pkg\"/>"
	CLAP_CHOICE_DEF="<choice id=\"org.surge-synth-team.${PRODUCTFILE}.clap.pkg\" visible=\"true\" start_selected=\"true\" title=\"${PRODUCT} CLAP\"><pkg-ref id=\"org.surge-synth-team.${PRODUCTFILE}.clap.pkg\"/></choice><pkg-ref id=\"org.surge-synth-team.${PRODUCTFILE}.clap.pkg\" version=\"${VERSION}\" onConclusion=\"none\">${PRODUCTFILE}_CLAP.pkg</pkg-ref>"
fi
if [[ -d $INDIR/$APP ]]; then
	APP_PKG_REF="<pkg-ref id=\"org.surge-synth-team.${PRODUCTFILE}.app.pkg\"/>"
	APP_CHOICE="<line choice=\"org.surge-synth-team.${PRODUCTFILE}.app.pkg\"/>"
	APP_CHOICE_DEF="<choice id=\"org.surge-synth-team.${PRODUCTFILE}.app.pkg\" visible=\"true\" start_selected=\"true\" title=\"${PRODUCT} App\"><pkg-ref id=\"org.surge-synth-team.${PRODUCTFILE}.app.pkg\"/></choice><pkg-ref id=\"org.surge-synth-team.${PRODUCTFILE}.app.pkg\" version=\"${VERSION}\" onConclusion=\"none\">${PRODUCTFILE}_APP.pkg</pkg-ref>"
fi


cat > $TMPDIR/distribution.xml << XMLEND
<?xml version="1.0" encoding="utf-8"?>
<installer-gui-script minSpecVersion="1">
    <title>${PRODUCT} ${VERSION}</title>
    <license file="License.txt" />
    <readme file="Readme.rtf" />
    ${VST3_PKG_REF}
    ${AU_PKG_REF}
    ${CLAP_PKG_REF}
    ${APP_PKG_REF}
    <options require-scripts="false" customize="always" hostArchitectures="x86_64,arm64" />
    <choices-outline>
        ${VST3_CHOICE}
        ${AU_CHOICE}
        ${CLAP_CHOICE}
        ${APP_CHOICE}
    </choices-outline>
    ${VST3_CHOICE_DEF}
    ${AU_CHOICE_DEF}
    ${CLAP_CHOICE_DEF}
    ${APP_CHOICE_DEF}
</installer-gui-script>
XMLEND

# build installation bundle

pushd ${TMPDIR}
if [[ ! -z $MAC_INSTALLING_CERT ]]; then
  echo "Building SIGNED PKG"
  echo productbuild --sign "$MAC_INSTALLING_CERT" --distribution "distribution.xml" --package-path "." --resources ${RESOURCESDIR} "$OUTPUT_BASE_FILENAME.pkg"
  productbuild --sign "$MAC_INSTALLING_CERT" --distribution "distribution.xml" --package-path "." --resources ${RESOURCESDIR} "$OUTPUT_BASE_FILENAME.pkg"
else
  echo "Building UNSIGNED PKG"
  productbuild --distribution "distribution.xml" --package-path "." --resources ${RESOURCESDIR}/ "$OUTPUT_BASE_FILENAME.pkg"
fi

popd

Rez -append ${RESOURCESDIR}/icns.rsrc -o "${TMPDIR}/${OUTPUT_BASE_FILENAME}.pkg"
SetFile -a C "${TMPDIR}/${OUTPUT_BASE_FILENAME}.pkg"
mkdir -p "${TMPDIR}/${PRODUCTFILE}"

mv "${TMPDIR}/${OUTPUT_BASE_FILENAME}.pkg" "${TMPDIR}/${PRODUCTFILE}"
# create a DMG if required

if [[ -f "${TARGET_DIR}/$OUTPUT_BASE_FILENAME.dmg" ]]; then
  rm "${TARGET_DIR}/$OUTPUT_BASE_FILENAME.dmg"
fi
hdiutil create /tmp/tmp.dmg -ov -volname "$OUTPUT_BASE_FILENAME" -fs HFS+ -srcfolder "${TMPDIR}/${PRODUCTFILE}/"
hdiutil convert /tmp/tmp.dmg -format UDZO -o "${TARGET_DIR}/$OUTPUT_BASE_FILENAME.dmg"

if [[ ! -z $MAC_SIGNING_CERT ]]; then
  codesign --force -s "$MAC_SIGNING_CERT" --timestamp "${TARGET_DIR}/$OUTPUT_BASE_FILENAME.dmg"
  codesign -vvv "${TARGET_DIR}/$OUTPUT_BASE_FILENAME.dmg"
  xcrun notarytool submit "${TARGET_DIR}/$OUTPUT_BASE_FILENAME.dmg" --apple-id ${MAC_SIGNING_ID} --team-id ${MAC_SIGNING_TEAM} --password ${MAC_SIGNING_1UPW} --wait

  xcrun stapler staple "${TARGET_DIR}/${OUTPUT_BASE_FILENAME}.dmg"
fi

# clean up

#rm distribution.xml
#rm Surge_*.pkg

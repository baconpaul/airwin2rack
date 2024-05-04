#define MyAppPublisher "Airwindows (with BaconPaul and members of the Surge Synth Team)"
#define MyAppURL "https://airwindows.com/"
#define MyAppName "Airwindows Consolidated"
#define MyAppNameCondensed "AirwindowsConsolidated"
#define MyID "1762E8B5-648B-4581-93D8-41CCE83DE14B"

#ifndef MyAppVersion
#define MyAppVersion "0.0.0"
#endif

;uncomment these two lines if building the installer locally!
;#define AWCONS_SRC "..\..\"
;#define AWCONS_BIN "..\..\build\"

[Setup]
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
AppId={#MyID}
AppName={#MyAppName}
AppVerName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppPublisher}\{#MyAppName}\
DefaultGroupName={#MyAppName}
DisableDirPage=yes
DisableProgramGroupPage=yes
AlwaysShowDirOnReadyPage=yes
LicenseFile={#AWCONS_SRC}\LICENSE.md
OutputBaseFilename={#MyAppNameCondensed}-{#MyAppVersion}-Windows-64bit-setup
UninstallDisplayIcon={uninstallexe}
UsePreviousAppDir=yes
Compression=lzma
SolidCompression=yes
UninstallFilesDir={autoappdata}\{#MyAppName}\uninstall
CloseApplicationsFilter=*.exe,*.vst3
WizardStyle=modern
WizardSizePercent=100
WizardImageFile={#AWCONS_SRC}\scripts\installer_win\empty.bmp
WizardSmallImageFile={#AWCONS_SRC}\scripts\installer_win\empty.bmp
WizardImageAlphaFormat=defined

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Types]
Name: "full"; Description: "Full installation"
Name: "clap"; Description: "CLAP installation"
Name: "vst3"; Description: "VST3 installation"
Name: "standalone"; Description: "Standalone installation"
Name: "custom"; Description: "Custom"; Flags: iscustom

[Components]
Name: "CLAP"; Description: "{#MyAppName} CLAP (64-bit)"; Types: full clap custom
Name: "VST3"; Description: "{#MyAppName} VST3 (64-bit)"; Types: full vst3 custom
Name: "SA"; Description: "{#MyAppName} Standalone (64-bit)"; Types: full standalone custom

[Files]
Source: "{#AWCONS_BIN}\awcons-products\{#MyAppName}.clap"; DestDir: "{autocf}\CLAP\{#MyAppPublisher}\"; Components: CLAP; Flags: ignoreversion
Source: "{#AWCONS_BIN}\awcons-products\{#MyAppName}.vst3\*"; DestDir: "{autocf}\VST3\{#MyAppPublisher}\{#MyAppName}.vst3\"; Components: VST3; Flags: ignoreversion recursesubdirs
Source: "{#AWCONS_BIN}\awcons-products\{#MyAppName}.exe"; DestDir: "{app}"; Components: SA; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppName}.exe"; Flags: createonlyiffileexists

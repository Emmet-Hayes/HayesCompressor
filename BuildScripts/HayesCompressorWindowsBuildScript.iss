; Script generated by the Inno Setup Script Wizard.

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{93951FDE-7122-4745-90CE-EBB71DD44D28}}
AppName=HayesCompressor
AppVersion=1.0.0
DefaultDirName={pf64}\Common Files\VST3\HayesCompressor\VST3
DefaultGroupName=EmmetSoft
OutputDir=userdocs:HayesInstallers
OutputBaseFilename=HayesCompressorInstaller
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\Builds\VisualStudio2019\x64\Release\VST3\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\HayesCompressor"; Filename: "{app}\HayesCompressor.exe"
Name: "{commondesktop}\HayesCompressor"; Filename: "{app}\HayesCompressor.exe"; Tasks: desktopicon


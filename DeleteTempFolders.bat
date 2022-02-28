Echo deleting folders

@RD /S /Q "Binaries"
@RD /S /Q "Intermediate"
@RD /S /Q "DerivedDataCache"
@RD /S /Q "Build"
@RD /S /Q "Script"
@RD /S /Q ".vs"
DEL "*.sln" /s /f /q
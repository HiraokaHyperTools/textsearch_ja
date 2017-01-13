echo VS2012 x86 Native Tools コマンド プロンプト
call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x86 || pause

nmake PGV=9.1 CPU=x64 || pause
nmake PGV=9.2 CPU=x64 || pause
nmake PGV=9.3 CPU=x64 || pause
nmake PGV=9.4 CPU=x64 pack || pause
nmake PGV=9.4 CPU=x86 pack || pause
nmake PGV=9.5 CPU=x64 pack || pause
nmake PGV=9.5 CPU=x86 pack || pause
nmake PGV=9.6 CPU=x64 pack || pause
nmake PGV=9.6 CPU=x86 pack || pause

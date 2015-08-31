echo VS2012 x86 Native Tools コマンド プロンプト
call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x86 || pause

nmake PGVER=PG91 CPU=x64 || pause
nmake PGVER=PG92 CPU=x64 || pause
nmake PGVER=PG93 CPU=x64 || pause
nmake PGVER=PG94 CPU=x64 || pause

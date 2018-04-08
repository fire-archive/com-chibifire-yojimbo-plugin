set current=%cd%
set godot=%current%\thirdparty\godot
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64 && cd /D %godot% &&call scons p=windows target=debug -j%NUMBER_OF_PROCESSORS% use_lto=no gdnative_wrapper=yes vsproj=yes deprecated=no
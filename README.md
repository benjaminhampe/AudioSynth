# AudioSynth

Is a cmake toolchain that builds ca. 100 libs and app using these libs on Windows 64 ( 7,8.1,10 ) and Linux 64.
This C++ toolchain does not use Microsoft Visual Studio. All libs and apps are built for Windows but dont need any MS tool.

1. Builds <project-dir>/bin/win64/nasm.exe
2. Builds libs and apps using nasm.exe ( audio and video libs need nasm.exe )

# Workflow

User opens a terminal on Win64 or Linux64, writes 'a_rel' and presses enter. 
Thats it, building starts when all is configured correctly.

# Prerequisites on Windows 64

- <must> Install QtSdk5 ( enable tools/MinGW 7.3.1 x64 + QtCreator + extra libs u like to use aswell  )  
- <must> Install git ( contains the terminal i use )
- <must> Install cmake ( atleast 3.14 )
- <optional> Install msys1 ( years ago i used its make.exe, but now i use the mingw32-make.exe from QtSdk as i recall )
- <optional> Install ConEmu ( another Linux terminal emulator for Windows, maybe u like it better than git-terminal )

Install all into C:\SDK ( u can install anywhere u like, but then adopt the scripts and shell commands to your folders )

C:\SDK\Qt5.12.3
C:\SDK\CMake-3.14.0
C:\SDK\Git

# Add stuff to bash.bashrc

Open and modify C:\SDK\Git\etc\bash.bashrc

The terminal with bash shell is from GitOnWindows and should be installed to C:\SDK\Git

```bash

# ============= AudioSynth-qt ================

alias a_dir='cd C:/Users/firestarter/Documents/GitHub/AudioSynth'
alias a_exe='cd C:/Users/firestarter/Documents/GitHub/AudioSynth/bin/win64'

function a_clean()
{
	DIR=$(pwd)
	a_dir
	echo $(pwd)
	./clean_all.sh
	cd $DIR
}

function a_rel()
{
	DIR=$(pwd)
	a_dir
	echo $(pwd)
	./build_m3_win_release_shared.sh
	cd $DIR
}

function a_rel_static()
{
	DIR=$(pwd)
	a_dir
	echo $(pwd)
	./build_m3_win_release_static.sh
	cd $DIR
}

function a_debug_static()
{
	DIR=$(pwd)
	a_dir
	echo $(pwd)
	./build_m3_win_debug_static.sh
	cd $DIR
}

function a_relwithdebuginfo()
{
	DIR=$(pwd)
	a_dir
	echo $(pwd)
	./build_m3_win_relwithdebinfo_shared.sh
	cd $DIR
}
```
# Adopt the build_<>.sh files and bash.rc to your installation of tools and downloaded source code

# Check

Open terminal and test is following commands work:

g++ --version
cmake --version
make --version 
or other make i.e. mingw32-make

if all works typs a_rel|a_rel_static|a_debug|a_debug_static|a_relwithdebinfo

Have fun.

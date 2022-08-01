# @title Benni's C/C++ toolchain ( GCC on Windows64 using QtSDK+MinGW7.3.1+cmake-benni
# BUILD=x86_64-pc-mingw64 x86_64-w64-mingw32
# HOST=x86_64-pc-mingw64 x86_64-w64-mingw32
# @author Benjamin Hampe <benjaminhampe@gmx.de>
# @date 2014-20210
# @platforms Windows64, Linux64

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(AudioSynth-sdk-22.01-benni++17)

# @buildenv Setup C++ Toolchain on Window64
#           1. Install QtSDK to C:/SDK/Qt5.12.3
#           1.1. Select QtCreator
#           1.2. Select QtLibs ( only select MinGW compiler built libs, to be compatible with benni magic )
#           2.3. Select Tools/MinGW7.3.1 and get a modern GNU C++ compiler on Win64, convincingly simple.
#           2.4. DONT select Tools/StrawberryPerl because it overwrites PATH var with crap (very old g++ that is delivered with perl)
#           2. Install CMake to C:/SDK/CMake
#           3. Install Git   to C:/SDK/Git ( i use its bash terminal and .bashrc to add my fasterwork commands )
#
#  licht-sdk   Benni explains his directory layout.
#       |
#       +-- bin   <Result> Runnable apps and runtimes. ( .exe and .dll )
#       +-- build <Result> Intermediate object files. ( .a, .lib, .o )
#       |
#       +-- doc   <Optional> Directory with infos, if any.
#       +-- media <Optional> Used by some Benni apps|tests, if any. ( .jpg, .png, .ttf, .svg, .x, .dxf, .gcode, etc.. )
#       |
#       +-- src   <Sources> Never delete this. Carefully adapted sources of over 50-100 libs
#       |         every programmer encounters in his life. Each lib got a Benni cmake recipe ( which is the magic ).
#       |         FullSourceLibs: e.g. glm, zlib, png, jpeg, zip2, zlib, unzip, minizip, kissfft, rapidjson, etc..
#       |         PrecompiledLibs: e.g. boost, fmod, ffmpeg, Qt, wxWidgets hopefully.
#       |         FullSourceTools: e.g. 'nasm.exe', a ASM compiler compiled by a C compiler.
#       |         These recipes are simple and similar for different uses, therefore easy to adapt to your needs.
#       |         These libs are always essentiell for any app or kernel, but i had to rewrite/reorder them to be compatible.
#       |         Libraries are built from sources because its makes the toolchain stable for the next 30 years.
#       |         I give you the power to test and fix all libs to the point it can bring you to Mars and back safely.
#
# The ./clean.sh script never deletes the 'src', 'media' and 'doc' directories.
# But the script deletes all dirs created by cmake, the 'bin' and 'build' directories.
#
# Building static apps really tries to link with runtime libs to have less dependencies ( ideally one exe only ).
# Building shared (default) is fasted and best for rapid development ( used 99,9% of time ), can result in many files.
# Also shared offers many public or actually unneeded symbols still loaded to memory, which could be unwanted for
# performance, memory or security reasons.
#
# If you like to copy the 'bin' folder (apps and runtimes) then probably also copy the 'media' folder
#
# But try to keep the folder struct where <bin> and <media> are on the same directory level.
#
# BenniApp(s) tend to call media files in the manner "../../media/fonts/arial.ttf"
#
#  licht-sdk
#       |
#       +--+ bin ( The compilation results )
#       |  |
#       |  +--+ win64 ( contains debug|release|etc. in same dir, for now )
#       |     |
#       |     +-- BenniApp<d><s>.exe    somewhere calls ../../media/fonts/arial.ttf"
#       |
#       +--+ media
#       |  |
#       |  +--+ fonts
#       |     |
#       |     +--+ arial.ttf -> means this file


# The 'src' contains source codes of many basic libraries like zlib, lzma, jpeg, png, etc.
# we all need everyday andis to have all sources and always build from these ( adapted and optimized because the originals are all kinda shit and not benni compatible )
# Each tool ( e.g. nasm ) and sub-library ( e.g. freetype2, jpeg, png, zip ) is build from its included sources from the vip 'src' directory
# Except fmod, ffmpeg, QtSDK maybe wxWidgets.
#
# Libraries that are built from included C/C++ sources
#       Each lib has its own custom CMake recipe, all in the same manner ( easier to add new libs )
#
#       1 GLM - OpenGL math ( my most favourite lib ) | de-glm
#       2 tinyxml2 ( 2 files, one .hpp and one .cpp )
#       3 rapidjson
#       4 z-1.2.8 ( needed for all multimedia libs, e.g. libjpeg8d
#       5 bzip2
#       6 lzma-4.32.7
#       7 minizip
#       8 unzip
#       9 utf8cpp
#      10 kissfft
#      11 jpeg-8d
#      12 png-1.6.23
#      13 tiff4
#      14 giflib-5.2.1
#      15 freetype-2.10.0
#      16 opengles3
#      18. DarkImage ( benniLib ) -> Imports and Exports images, draws many shapes, texts, gradients and rainbows. Has filesystem, filemagic ( to correct wrong file extensions from thingyverse images ) and nice logging macros.
#      19. DarkGPU ( benniLib ) -> Uses libGLESv2.dll and libEGL.dll from QtSDK. Works on many computers, even Win w/o installed gfx drivers!
#      20. DarkAssimp ( benniLib ) -> Converts assimp models to benni models ( aiScene* -> de::gpu::SMesh* )
#       20.1 poly2tri -> could be useful for other, maybe extract
#       20.2 clipper -> could be useful for other, maybe extract
#       20.3 irrXML
#       20.4 open3dgc
#       20.5 openddlparser
#       20.6 libAssimp ( load 3d <CAD> models )
#       + DarkGPU ( benni OpenGLES pi - platform independent )
# Included Libraries from binaries:
# + QtSDK
# + fmod
# + ffmpeg
# The 3D engine DarkGPU is based on the 2d module DarkImage, links to it. DarkImage is a "complete" jpg,*png,*gif,*bmp etc image library.
# It is small, fast, easy extensible, many importers, some exporters, even fancy ones like a image as html table where each <td> is a pixel.
# Also has libfreetype2 for font and text rendering 2d ( DarkImage ) and hardware accelerated texture based text rendering in DarkGPU de::gpu::IFontAtlas
# all dependencies are building up from sources
# on all supported platforms: for now Windows and Linux amd64 (yet), hello raspi.
#


# set(BUILD_SHARED=1) # Enable to force shared ( 'most' targets use this var, except the ones that are always built static due to cyclic deps )
set(BENNI_SANITIZE 1)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS} -D_FILE_OFFSET_BITS=64) # enable fseeko64()
# set(CMAKE_UNITY_BUILD 1)  # Available only in cmake >= 3.16
#include(CheckPIESupportrd)
#check_pie_supported()

# target_precompiled_header # Available only in cmake >= 3.16

if(NOT MSVC)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

message("${BoldWhite}")

# Tell MSVC to use main instead of WinMain for Windows subsystem executables
#if (MSVC)
# set_target_properties(${GUI_ONLY_BINARIES} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
#endif()
execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                OUTPUT_VARIABLE PACKAGE_GIT_VERSION
                ERROR_QUIET
                OUTPUT_STRIP_TRAILING_WHITESPACE)

#execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
#WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#RESULT_VARIABLE GIT_SUBMOD_RESULT)
#if(NOT GIT_SUBMOD_RESULT EQUAL "0")
# message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
#endif()

#execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
#                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
#                OUTPUT_VARIABLE PACKAGE_GIT_VERSION
#                ERROR_QUIET
#                OUTPUT_STRIP_TRAILING_WHITESPACE)

# set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
# set(BUILD_SHARED_LIBS OFF)
# set(CMAKE_EXE_LINKER_FLAGS "-static")
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# =============================================================
# Can be set outside cmake
# =============================================================
if(BUILD_SHARED)
   message("[benni] BUILD_SHARED")
   # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
else()
   message("[benni] BUILD_STATIC")
   set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()

if(WIN32)
   # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
endif()

# =============================================================
# Detect debug type, e.g. build Release, Debug, MinSizeRel=default
# =============================================================
if(CMAKE_BUILD_TYPE STREQUAL "")
   set(CMAKE_BUILD_TYPE "MinSizeRel")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   set(BUILD_DEBUG=1) # Used in applications to add "d" postfix to TARGET_NAME
   add_definitions(-D_DEBUG=1)
   add_definitions(-DDEBUG=1)
   add_definitions(-DBUILD_DEBUG=1)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
   add_definitions(-DNDEBUG=1)
   add_definitions(-DBUILD_RELEASE=1)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Release")
   add_definitions(-DNDEBUG=1)
   add_definitions(-DBUILD_RELEASE=1)
endif()

message("[benni] CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

# =============================================================
# Add more switches here, Os for MinSizRel, etc..
# =============================================================

set(MY_C_FLAGS "-fpic -mtune=native -march=native")
set(MY_CXX_FLAGS "${MY_C_FLAGS} -Wall -Wfatal-errors -fext-numeric-literals") #  -Werror=no-return
set(MY_LD_FLAGS "")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   set(MY_C_FLAGS "${MY_C_FLAGS} -O0 -g")
   #set(MY_C_FLAGS "${MY_C_FLAGS} -fsanitize=address")
   #set(MY_LD_FLAGS "${MY_LD_FLAGS} -lasan")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
   set(MY_C_FLAGS "${MY_C_FLAGS} -O0 -g")
   #set(MY_C_FLAGS "${MY_C_FLAGS} -fsanitize=address")
   #set(MY_LD_FLAGS "${MY_LD_FLAGS} -lasan")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
   set(MY_C_FLAGS "${MY_C_FLAGS} -mfpmath=sse -msse3 -mmmx -O2")
   set(MY_C_FLAGS "${MY_C_FLAGS} -Wno-unused-but-set-variable")
   set(MY_C_FLAGS "${MY_C_FLAGS} -Wno-unknown-pragmas")
   set(MY_C_FLAGS "${MY_C_FLAGS} -Wno-sign-compare")
   set(MY_C_FLAGS "${MY_C_FLAGS} -Wno-unused-variable")
   set(MY_C_FLAGS "${MY_C_FLAGS} -Wno-unused-function")
   set(MY_C_FLAGS "${MY_C_FLAGS} -Wno-comment")
   set(MY_C_FLAGS "${MY_C_FLAGS} -Wno-strict-aliasing")
   set(MY_C_FLAGS "${MY_C_FLAGS} -Wno-maybe-uninitialized")
   set(MY_CXX_FLAGS "${MY_CXX_FLAGS} -fexpensive-optimizations -ftree-vectorize")
   # set(MY_CXX_FLAGS "${MY_CXX_FLAGS} -fno-rtti")
   # set(MY_C_FLAGS "${MY_C_FLAGS} -fno-exceptions")
   # set(MY_C_FLAGS "${MY_C_FLAGS} -fstrict-aliasing")
   #set(MY_C_FLAGS "${MY_C_FLAGS} -ffast-math")
   #set(MY_C_FLAGS "${MY_C_FLAGS} -fassociative-math")
   #set(MY_C_FLAGS "${MY_C_FLAGS} -funsafe-math-optimizations")
   #set(MY_C_FLAGS "${MY_C_FLAGS} -funroll-loops")
endif()

message("[benni] MY_C_FLAGS = ${MY_C_FLAGS}")
message("[benni] MY_CXX_FLAGS = ${MY_CXX_FLAGS}")
message("[benni] MY_LD_FLAGS = ${MY_LD_FLAGS}")
message("[benni] Before CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")
message("[benni] Before CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message("[benni] Before CMAKE_LD_FLAGS = ${CMAKE_LD_FLAGS}")
message("[benni] Before CMAKE_EXE_LINKER_FLAGS = ${CMAKE_EXE_LINKER_FLAGS}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MY_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS} ${MY_C_FLAGS}")
set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} ${MY_LD_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MY_LD_FLAGS}")

message("[benni] After CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")
message("[benni] After CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message("[benni] After CMAKE_LD_FLAGS = ${CMAKE_LD_FLAGS}")
message("[benni] After CMAKE_EXE_LINKER_FLAGS = ${CMAKE_EXE_LINKER_FLAGS}")

# set(CMAKE_LINKER_FLAGS "-pthread" CACHE STRING "Linker Flags" FORCE)
# add_definitions(MY_CXX)

# Depending on the boolean chosen above, set CPU_OPTS to the correct cpu opts
#IF (CPUINTEL_native) SET(CPU_OPTS "-mtune=native -march=native -mfpmath=sse -msse3 -mmmx -ftree-vectorize -ffast-math -fassociative-math -funsafe-math-optimizations -funroll-loops ")
#ELSEIF (CPUAMD_k8) SET(CPU_OPTS "-mtune=athlon64 -mfpmath=sse -msse2 -mmmx -m3dnow -ftree-vectorize -ffast-math -fassociative-math -funsafe-math-optimizations -fprefetch-loop-arrays ")
#ELSEIF (CPUAMD_K9) SET(CPU_OPTS "-mtune=k8-sse3 -mfpmath=sse -msse3 -mmmx -m3dnow -ftree-vectorize -ffast-math -fassociative-math -funsafe-math-optimizations -funroll-loops ")
#ELSEIF (CPUINTEL_p4) SET(CPU_OPTS "-mtune=pentium4 -mfpmath=sse -msse2 -mmmx -ftree-vectorize -ffast-math -fassociative-math -funsafe-math-optimizations -funroll-loops ")
#ELSEIF (CPUINTEL_prescott) SET(CPU_OPTS "-mtune=prescott -mfpmath=sse -msse3 -mmmx -ftree-vectorize -ffast-math -fassociative-math -funsafe-math-optimizations -funroll-loops " )
#ELSEIF (CPUINTEL_nocona) SET(CPU_OPTS "-mtune=nocona -mfpmath=sse -msse3 -mmmx -ftree-vectorize -ffast-math -fassociative-math -funsafe-math-optimizations -funroll-loops ")
#ELSEIF (CPUINTEL_core2) SET(CPU_OPTS "-mtune=core2 -mfpmath=sse -msse3 -mmmx -ftree-vectorize -ffast-math -fassociative-math -funsafe-math-optimizations -funroll-loops ")
#ELSEIF (CPUAMD_vishera) SET(CPU_OPTS "-march=bdver2 -mfpmath=both -mmmx -msse4a -msse4.2 -maes -mpclmul -mfsgsbase -mrdrnd -mf16c -mfma -mfma4 -m3dnow -mxop -mlwp -mtbm -mavx2 -mvzeroupper -msahf -mcx16 -funsafe-loop-optimizations ")
#ENDIF (CPUAMD_k8)

#IF (CPU_SMP GREATER 1)
#    SET(CPU_OPTS " ${CPU_OPTS} -ftree-parallelize-loops=${CPU_SMP} ")
#ENDIF (CPU_SMP GREATER 1)

# =============================================================
# Detect host OS name, e.g. Linux/Win/Mac
# =============================================================
message("[benni] Detect OS - operating system ...")
if (WIN32)
   message("[benni] WIN32")
   set(BENNI_OS_NAME "win")
   message("[benni] BENNI_OS_NAME = win")
   add_definitions(-DBENNI_OS_WINDOWS=1)
   add_definitions(-DWIN32_LEAN_AND_MEAN)
   add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
   add_definitions(-D_CRT_SECURE_NO_WARNINGS) 	# Shut up about unsafe stuff
   add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
   if (MSYS)
      message("[benni] Has MSYS")
   endif()
   if (MINGW)
      message("[benni] Has MINGW")
   endif()
   if (CYGWIN)
      message("[benni] Has CYGWIN")
   endif()
endif()
if (UNIX)
   message("[benni] UNIX")
   set(BENNI_OS_NAME "linux")
   message("[benni] BENNI_OS_NAME = win")
   add_definitions(-DBENNI_OS_LINUX=1)
endif()
if (APPLE)
   message("[benni] APPLE")
   set(BENNI_OS_NAME "apple")
   message("[benni] BENNI_OS_NAME = apple")
   add_definitions(-DBENNI_OS_MAC=1)
endif()

message("[benni] System       : ${CMAKE_SYSTEM_NAME}")
message("[benni] Processor    : ${CMAKE_SYSTEM_PROCESSOR}")
message("[benni] C compiler   : ${CMAKE_C_COMPILER_ID}")
message("[benni] C++ compiler : ${CMAKE_CXX_COMPILER_ID}")
message("[benni] CMAKE_HOST_SYSTEM_PROCESSOR = ${CMAKE_HOST_SYSTEM_PROCESSOR} ")
# =============================================================
# Detect machine word size:
# =============================================================
# 64 bit host
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
   set(BENNI_MACHINE 64)
   add_definitions(-DBENNI_MACHINE=64)
   message("[benni] Machine x64_86")
# 32 bit host
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
   set(BENNI_MACHINE 32)
   add_definitions(-DBENNI_MACHINE=32)
   message("[benni] Machine x86")
endif()

# =============================================================
# Combine os name + address size 32|64
# =============================================================
set(BENNI_HOST "${BENNI_OS_NAME}${BENNI_MACHINE}") # linux64, win64
message("[benni] BENNI_HOST = ${BENNI_HOST}")

# BENNI_BUILD_DIR: <win64|linux64>_<shared|static>_<Debug|Release|MinSizeRel>
if (BUILD_SHARED)
   set(BUILD_NAME "${BENNI_HOST}_${CMAKE_BUILD_TYPE}_shared")
else()
   set(BUILD_NAME "${BENNI_HOST}_${CMAKE_BUILD_TYPE}_static")
endif()
message("[benni] BENNI_BUILD = ${BUILD_NAME}")

# BENNI_BUILD_DIR: build/<win|linux|mac><32|64>_<shared|static>_<Debug|Release|MinSizeRel>
set(BUILD_DIR "build/${BUILD_NAME}")
message("[benni] BENNI_BUILD_DIR = ${BUILD_DIR}")

# BENNI_OUTPUT_DIR
set(BENNI_OUTPUT_DIR "${CMAKE_CURRENT_LIST_DIR}/bin/${BENNI_HOST}")
message("[benni] BENNI_OUTPUT_DIR = ${BENNI_OUTPUT_DIR}")

# Output directories for build artifacts
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BENNI_OUTPUT_DIR}) # Where cmake puts .exe files.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BENNI_OUTPUT_DIR}") # Where cmake puts .dll files.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/ar") # Where cmake puts .a/.lib files.
set(CMAKE_LIBRARY_PATH "${CMAKE_BINARY_DIR}") # Separated list of directories for the find_library() command.
set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}") # ?
set(CMAKE_INSTALL_INCLUDEDIR "${CMAKE_CURRENT_LIST_DIR}/install/include")
set(CMAKE_INSTALL_LIBDIR "${CMAKE_CURRENT_LIST_DIR}/install/lib")
set(CMAKE_INSTALL_BINDIR "${CMAKE_CURRENT_LIST_DIR}/install/bin")
if (WIN32)
   set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} C:/SDK/Qt5.12.3/5.12.3/mingw73_64/lib/cmake")
endif()

message("[benni] CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
message("[benni] CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
message("[benni] CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message("[benni] CMAKE_LIBRARY_OUTPUT_DIRECTORY = ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message("[benni] CMAKE_ARCHIVE_OUTPUT_DIRECTORY = ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message("[benni] CMAKE_LIBRARY_PATH = ${CMAKE_LIBRARY_PATH}")
message("[benni] CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message("[benni] CMAKE_INSTALL_INCLUDEDIR = ${CMAKE_INSTALL_INCLUDEDIR}")
message("[benni] CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")
message("[benni] CMAKE_INSTALL_BINDIR = ${CMAKE_INSTALL_BINDIR}")

# NEW 2021: This flag is now automated, but it needs the Moved

# Benni: Set to '1' when tools like the assembler are needed,
# The tools get built into <bin/win64> folder and can be used now.
# After building tools set to '0' again and build normally.
# 1 = build tools, including nasm, required to build normally with audio libs
# 0 = build normally ( all activated projects and libraries ).
#set(BENNI_BUILD_NASM 0)   # Build normal ( libs + apps, they need nasm )
set(BENNI_BUILD_NASM 1) # Build nasm tools
if(EXISTS ${BENNI_OUTPUT_DIR}/nasm)
   set(BENNI_BUILD_NASM 0)
   message("[benni] FOUND ASSEMBLER nasm, build normal")
endif()
if(EXISTS ${BENNI_OUTPUT_DIR}/nasm.exe)
   set(BENNI_BUILD_NASM 0)
   message("[benni] FOUND ASSEMBLER nasm.exe, build normal")
endif()
if(BENNI_BUILD_NASM)
   message("[benni] NO ASSEMBLER FOUND, BUILD NOW nasm.exe, THEN START AGAIN to build normal")
endif()

# Tell cmake to use our custom built Assembler tools/nasm.exe

set(CMAKE_ASM_NASM_COMPILER "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/nasm.exe")
set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS nasm asm)
set(CMAKE_ASM_OBJECT_FORMAT elf64)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

if(NOT CMAKE_ASM_NASM_OBJECT_FORMAT)
  if(WIN32)
    if(CMAKE_C_SIZEOF_DATA_PTR EQUAL 8)
      set(CMAKE_ASM_NASM_OBJECT_FORMAT win64)
    else()
      set(CMAKE_ASM_NASM_OBJECT_FORMAT win32)
    endif()
  elseif(APPLE)
    if(CMAKE_C_SIZEOF_DATA_PTR EQUAL 8)
      set(CMAKE_ASM_NASM_OBJECT_FORMAT macho64)
    else()
      set(CMAKE_ASM_NASM_OBJECT_FORMAT macho)
    endif()
  else()
    if(CMAKE_C_SIZEOF_DATA_PTR EQUAL 8)
      set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
    else()
      set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)
    endif()
  endif()
endif()

if (0)
   set(CMAKE_NASM_COMPILER "${CMAKE_ASM_NASM_COMPILER}")
   set(CMAKE_NASM_COMPILER_ENV_VAR "${CMAKE_NASM_COMPILER}")
   set(CMAKE_ASM_OBJECT_FORMAT ${CMAKE_ASM_NASM_OBJECT_FORMAT})
   set(CMAKE_ASM_COMPILER ${CMAKE_ASM_NASM_COMPILER})

   # support for the asm assembler
   set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> <INCLUDES> \
       <FLAGS> -f ${CMAKE_ASM_NASM_OBJECT_FORMAT} -o <OBJECT> <SOURCE>")

   # support for the nasm assembler
   set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> \
       <FLAGS> -f ${CMAKE_ASM_NASM_OBJECT_FORMAT} -o <OBJECT> <SOURCE>")

   # set_source_files_properties(src/strlen.asm PROPERTIES COMPILE_FLAGS "-g -Fdwarf")
   if(CMAKE_BUILD_TYPE STREQUAL "Debug")
       set(CMAKE_ASM_NASM_FLAGS "${ASM_NASM_FLAGS} -g -Fdwarf")
   else()
       set(CMAKE_ASM_NASM_FLAGS "${ASM_NASM_FLAGS}")
   endif()

   # support for the nasm assembler
   set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> \
       <FLAGS> -f ${CMAKE_ASM_NASM_OBJECT_FORMAT} -o <OBJECT> <SOURCE>")

   message("[benni] CMAKE_ASM_NASM_FLAGS = ${CMAKE_ASM_NASM_FLAGS}")
   message("[benni] CMAKE_ASM_NASM_COMPILER = ${CMAKE_ASM_NASM_COMPILER}")
   message("[benni] CMAKE_ASM_COMPILER = ${CMAKE_ASM_COMPILER}")
   message("[benni] CMAKE_ASM_COMPILER_ENV_VAR = ${CMAKE_ASM_COMPILER_ENV_VAR}")
   message("[benni] CMAKE_NASM_COMPILER = ${CMAKE_NASM_COMPILER}")
   message("[benni] CMAKE_NASM_COMPILER_ENV_VAR = ${CMAKE_NASM_COMPILER_ENV_VAR}")
   message("[benni] CMAKE_ASM_NASM_OBJECT_FORMAT = ${CMAKE_ASM_NASM_OBJECT_FORMAT}")
   message("[benni] CMAKE_ASM_NASM_COMPILE_OBJECT = ${CMAKE_ASM_NASM_COMPILE_OBJECT}")
   message("${ColourReset}")

   enable_language(ASM_NASM)
endif(0)

# on windows:
if(BENNI_OS_NAME STREQUAL "win")

   # Hardcoded is easier here
   set(BENNI_QTSDK_DIR "C:/SDK/Qt5.12.3/5.12.3/mingw73_64")

   # Get runtime dlls from MinGW GCC Compiler ( g++-7.3.0 or so ) in QtSDK
   file(COPY "${BENNI_QTSDK_DIR}/bin/libgcc_s_seh-1.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )  # gcc runtime for windows
   file(COPY "${BENNI_QTSDK_DIR}/bin/libstdc++-6.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})      # gcc runtime for windows
   file(COPY "${BENNI_QTSDK_DIR}/bin/libwinpthread-1.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})  # gcc runtime for windows

   # Get qtwidget dlls from installed QtSDK (with integrated MinGW C++ Compiler)
   ### Debug mode
   if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      file(COPY "${BENNI_QTSDK_DIR}/bin/Qt5Cored.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Debug
      file(COPY "${BENNI_QTSDK_DIR}/bin/Qt5Guid.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Debug
      file(COPY "${BENNI_QTSDK_DIR}/bin/Qt5Widgetsd.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Debug
      file(COPY "${BENNI_QTSDK_DIR}/bin/Qt5WinExtrasd.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Debug
      file(COPY "${BENNI_QTSDK_DIR}/bin/Qt5Svgd.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Debug
      file(COPY "${BENNI_QTSDK_DIR}/plugins/platforms/qwindowsd.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/platforms) # Debug
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qgifd.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qicnsd.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qicod.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qjpegd.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qsvgd.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qtgad.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qtiffd.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qwbmpd.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qwebpd.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
   ### Release mode
   else()
      file(COPY "${BENNI_QTSDK_DIR}/bin/Qt5Core.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Release|MinSizeRel
      file(COPY "${BENNI_QTSDK_DIR}/bin/Qt5Gui.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Release|MinSizeRel
      file(COPY "${BENNI_QTSDK_DIR}/bin/Qt5Widgets.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Release|MinSizeRel
      file(COPY "${BENNI_QTSDK_DIR}/bin/Qt5WinExtras.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Release|MinSizeRel
      file(COPY "${BENNI_QTSDK_DIR}/bin/Qt5Svg.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
      file(COPY "${BENNI_QTSDK_DIR}/plugins/platforms/qwindows.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/platforms)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qgif.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qgif.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qicns.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qico.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qjpeg.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qsvg.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qtga.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qtiff.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qwbmp.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
      file(COPY "${BENNI_QTSDK_DIR}/plugins/imageformats/qwebp.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imageformats)
   endif()
endif()

add_subdirectory(src)

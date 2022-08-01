set(APP_NAME fontview-qt)
project(${APP_NAME}-dev)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(APP_NAME "${APP_NAME}d")
endif()

if( BUILD_SHARED EQUAL 1 )
else()
  set(APP_NAME "${APP_NAME}s") # static suffix
endif()

if (TARGET ${APP_NAME})
   message("${BoldRed}[benni] HEADER_GUARD: ${APP_NAME}${ColourReset}")
else()
   message("${BoldYellow}[benni] APPLICATION: ${APP_NAME}${ColourReset}")

   #add_subdirectory(lib)

   enable_language(RC)

   set(APP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   file( GLOB_RECURSE APP_SOURCES "${APP_SOURCE_DIR}/*.[h,c]pp")
   list(APPEND APP_SOURCES ${CMAKE_CURRENT_LIST_DIR}/icon.rc)

   set(CMAKE_AUTOMOC ON)
   # set(CMAKE_AUTORCC OFF)
   # set(CMAKE_AUTOUIC OFF)

   find_package(Qt5Widgets)
   find_package(Qt5 COMPONENTS Core Widgets)

   include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS})
   add_definitions(${Qt5Widgets_DEFINITIONS})
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

   add_executable(${APP_NAME} ${APP_SOURCES})
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR})

   target_compile_options(${APP_NAME} PRIVATE -Wno-deprecated-declarations)
   target_compile_options(${APP_NAME} PRIVATE -Wno-unused-variable)

   if (WIN32)  # on Windows:
      target_link_libraries(${APP_NAME} PRIVATE comctl32 user32 gdi32 kernel32)
   else()      # on Linux:
      target_link_libraries(${APP_NAME} PRIVATE X11 Xxf86vm Xi Xcursor)
   endif()
   target_link_libraries(${APP_NAME} PRIVATE stdc++fs)   # Filesystem c++17
   target_link_libraries(${APP_NAME} PRIVATE pthread)    # Threads
   target_link_libraries(${APP_NAME} PRIVATE m)          # Math

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   # include_directories(SYSTEM ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
   target_link_libraries(${APP_NAME} PRIVATE Qt5::Widgets)
else()
   target_link_libraries(${APP_NAME} PRIVATE Qt5::Widgets)
endif()

   target_link_libraries(${APP_NAME} PRIVATE DarkImage)  # 2D image io, glm, logger, freetype text, fontawesome, xml, draw
endif()




#cmake_minimum_required( VERSION 3.0 )

#set(APP_NAME fontview-qt)

#project(${APP_NAME})

#set(APP_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
#set(APP_SOURCES
#	"${APP_SOURCE_DIR}/main.cpp"
#	"${APP_SOURCE_DIR}/FontGlyphInfoWidget.hpp"
#	"${APP_SOURCE_DIR}/FontGlyphInfoWidget.cpp") # Sources without moc for qt?

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fext-numeric-literals" ) # Add compiler flags for building executables (-fPIE)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

#find_package(Qt5Core CONFIG REQUIRED) # Find the QtWidgets library
#find_package(Qt5Widgets CONFIG REQUIRED) # Find the QtWidgets library
#find_package(Qt5Gui CONFIG REQUIRED) # Find the QtWidgets library
#include_directories(SYSTEM ${Qt5Core_INCLUDE_DIRS}) # Add the include directories for the Qt 5 Widgets module to the compile lines.
#include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS}) # Add the include directories for the Qt 5 Widgets module to the compile lines.
#include_directories(SYSTEM ${Qt5Gui_INCLUDE_DIRS}) # Add the include directories for the Qt 5 Widgets module to the compile lines.
#add_definitions(${Qt5Core_DEFINITIONS}) # Use the compile definitions defined in the Qt 5 Widgets module
#add_definitions(${Qt5Widgets_DEFINITIONS}) # Use the compile definitions defined in the Qt 5 Widgets module
#add_definitions(${Qt5Gui_DEFINITIONS}) # Use the compile definitions defined in the Qt 5 Widgets module
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}" )
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}" )
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Gui_EXECUTABLE_COMPILE_FLAGS}" )

## QT5_WRAP_CPP(MOC_FILES
## 			"${FV_SOURCE_DIR}/FontGlyphInfoWidget.hpp"
## 			"${FV_SOURCE_DIR}/FontGlyphInfoWidget.cpp")
## QT5_WRAP_UI( FONTVIEW_UI "${FV_SOURCE_DIR}/FontGlyphInfoWidget.ui" )

#QT5_WRAP_CPP(MOC_FILES "${APP_SOURCES}")
#set(${APP_SOURCES} "${APP_SOURCES} ${MOC_FILES}")

#message("===============================================")
#message("${BoldBlue}PROJECT_NAME: ${PROJECT_NAME}${ColourReset}")
#message("===============================================")
#message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
#message("APP_SOURCE_DIR = ${APP_SOURCE_DIR}")
#message("APP_SOURCES = ${APP_SOURCES}")
#message("MOC_FILES = ${MOC_FILES}")

###### Add project
#add_executable(${APP_NAME})
#set_target_properties(${APP_NAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE) # -fPic
#target_include_directories(${APP_NAME} PRIVATE "${APP_SOURCE_DIR}")
#target_include_directories(${APP_NAME} PUBLIC "${PROJECT_BINARY_DIR}") # For QT ?
#target_sources(${APP_NAME} PRIVATE "${APP_SOURCES}")
#target_link_libraries(${APP_NAME}
#	PUBLIC
#	Qt5::Widgets
#    Qt5::Core
#    Qt5::Gui) #     Qt5::OpenGL

## file(COPY ${CMAKE_CURRENT_LIST_DIR}/themes/darcula.css DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) ### copy resource files

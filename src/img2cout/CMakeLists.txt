set(APP_NAME img2cout)
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
   list(APPEND APP_SOURCES ${CMAKE_CURRENT_LIST_DIR}/res/resource.rc)

# Begin QtSDK
   set(CMAKE_AUTOMOC ON)
   # set(CMAKE_AUTORCC OFF)
   # set(CMAKE_AUTOUIC OFF)

   find_package(Qt5Widgets)
   find_package(Qt5 COMPONENTS Core Widgets)

   include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS})
   add_definitions(${Qt5Widgets_DEFINITIONS})
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")


   # file(GLOB_RECURSE APP_SOURCES "${APP_SOURCE_DIR}/*.[h,c]pp")

   # file( GLOB_RECURSE APP_SOURCES "${APP_SOURCE_DIR}/*.[h,c]pp")

   # set(APP_SOURCES ${APP_SOURCE_DIR}/Canvas3DToolPanel.cpp


#   set(APP_SOURCES ${APP_SOURCE_DIR}/Canvas3DToolPanel.cpp
#                   ${APP_SOURCE_DIR}/Canvas3D.cpp
#                   ${APP_SOURCE_DIR}/MainWindow.cpp
#                   ${APP_SOURCE_DIR}/LibreCAD.cpp
#                   ${APP_SOURCE_DIR}/main.cpp )

#   QT5_WRAP_CPP( MOC_FILES
#      ${APP_SOURCE_DIR}/MainWindow.cpp
#      ${APP_SOURCE_DIR}/Canvas3DToolPanel.cpp
#      ${APP_SOURCE_DIR}/Canvas3D.cpp
#      ${APP_SOURCE_DIR}/main.cpp )
#   list(APPEND APP_SOURCES ${MOC_FILES})

   #QT5_WRAP_UI( APP_UIF "ui/mainwindow.ui" )
   #set(RESOURCE_FILES "res/icons.qrc")
   #qt5_add_resources(APP_SOURCES ${APP_UIF})
   # list(APPEND APP_SOURCES ${APP_UIF} )

   # copy theme resource files
   #ADD_CUSTOM_COMMAND( TARGET "${APP_NAME}"
   #   POST_BUILD
   #   COMMAND "echo" "Copying theme.."
   #      COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/themes" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/themes"
   #   )
# Ende QtSDK

   add_executable(${APP_NAME} ${APP_SOURCES})
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR})

   # target_sources(${APP_NAME} PRIVATE ${APP_SOURCES})
   target_compile_options(${APP_NAME} PUBLIC -Wno-deprecated-declarations)
   target_link_options(${APP_NAME} PRIVATE -mwindows)

   if (WIN32)  # on Windows:
      target_link_libraries(${APP_NAME} PRIVATE comctl32 user32 gdi32 kernel32)
   else()      # on Linux:
      target_link_libraries(${APP_NAME} PRIVATE X11 Xxf86vm Xi Xcursor)
   endif()

   # target_link_libraries(${APP_NAME} PRIVATE stdc++fs)   # Filesystem c++17
   # target_link_libraries(${APP_NAME} PRIVATE pthread)    # Threads
   # target_link_libraries(${APP_NAME} PRIVATE m)          # Math

   # target_link_libraries(${APP_NAME} PRIVATE Qt5::Widgets)
   target_link_libraries(${APP_NAME} PRIVATE DarkImage)

endif()

set(APP_NAME vlc-qt)
project(${APP_NAME}-dev)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(APP_NAME "${APP_NAME}d")
endif()

if(NOT BUILD_SHARED EQUAL 1)
  set(APP_NAME "${APP_NAME}s") # static suffix
endif()

if (TARGET ${APP_NAME})
	message("${BoldYellow}[benni] HEADER_GUARD: ${APP_NAME}${ColourReset}")
else()
   message("${BoldRed}[benni] APPLICATION: ${APP_NAME}${ColourReset}")

# add_subdirectory(lib)

   set(APP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
   file(GLOB_RECURSE APP_SOURCES "${APP_SOURCE_DIR}/*.[c,h]pp")
   list(APPEND APP_SOURCES ${CMAKE_CURRENT_LIST_DIR}/icon.rc)

# # # # [QT] BEGIN
   set(CMAKE_AUTOMOC ON)
   set(CMAKE_AUTORCC ON)
   set(CMAKE_AUTOUIC ON)
   find_package(Qt5Widgets)
   find_package(Qt5Svg)
   find_package(Qt5 COMPONENTS Core Widgets)
   include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS})
   # add_definitions(${Qt5Widgets_DEFINITIONS})
   # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
   #QT5_WRAP_CPP( APP_MOC_FILES_CPP "mainwindow.hpp" )
   #QT5_WRAP_UI( APP_UI_FILES "ui/mainwindow.ui" )
# # # # [QT] END

   add_executable(${APP_NAME} "")
   target_sources(${APP_NAME} PRIVATE ${APP_SOURCES})
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR})

# # # # [QT] BEGIN
   # set(RESOURCE_FILES "res/icons.qrc")
   # qt5_add_resources(APP_SOURCES ${RESOURCE_FILES})
   target_link_libraries( ${APP_NAME} PRIVATE "Qt5::Widgets")
   target_link_libraries( ${APP_NAME} PRIVATE "Qt5::Svg")
   # copy theme resource files
   #ADD_CUSTOM_COMMAND( TARGET "${APP_NAME}"
   #   POST_BUILD
   #   COMMAND "echo" "Copying theme.."
   #      COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/themes" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/themes"
   #   )
# # # # [QT] END

   target_link_libraries( ${APP_NAME} PRIVATE DarkVLC)

endif()

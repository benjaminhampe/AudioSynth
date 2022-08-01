set(APP_NAME SoundStretch)
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
   message("${Green}[benni] APPLICATION: ${APP_NAME}${ColourReset}")

   set(APP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   file(GLOB_RECURSE APP_SOURCES "${APP_SOURCE_DIR}/*.[c,h]pp")
   list(APPEND APP_SOURCES ${CMAKE_CURRENT_LIST_DIR}/icon.rc)

   add_executable(${APP_NAME} "")
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR})
   target_sources(${APP_NAME} PRIVATE ${APP_SOURCES})
   target_link_libraries(${APP_NAME} PUBLIC de_soundtouch)
   target_link_libraries(${APP_NAME} PUBLIC winmm m)

   target_compile_options(${APP_NAME} PUBLIC -Wno-deprecated-declarations )
endif()

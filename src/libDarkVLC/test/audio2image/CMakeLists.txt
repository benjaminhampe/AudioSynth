set(APP_NAME audio2image)
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
   enable_language(RC)

   set(APP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   set(APP_SOURCES ${APP_SOURCE_DIR}/main_audio2image.cpp)
   list(APPEND APP_SOURCES ${CMAKE_CURRENT_LIST_DIR}/icon.rc)

   add_executable(${APP_NAME} "")
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR})
   target_sources(${APP_NAME} PRIVATE ${APP_SOURCES})
   target_link_libraries(${APP_NAME} PRIVATE DarkVLC)
endif()

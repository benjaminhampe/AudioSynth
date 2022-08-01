set(APP_NAME 03.CustomSceneNode)
project(${APP_NAME}-dev)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(APP_NAME "${APP_NAME}d")
endif()

if( BUILD_SHARED EQUAL 0 )
  set(APP_NAME "${APP_NAME}s") # static suffix
endif()

if (TARGET ${APP_NAME})
   message("${BoldBlue}[benni] HEADER_GUARD: ${APP_NAME}${ColourReset}")
else()
   message("${Green}[benni] APPLICATION: ${APP_NAME}${ColourReset}")

   enable_language(RC)

   add_executable(${APP_NAME} main.cpp icon.rc)

   target_link_libraries(${APP_NAME} PRIVATE de_irrlicht)
endif()

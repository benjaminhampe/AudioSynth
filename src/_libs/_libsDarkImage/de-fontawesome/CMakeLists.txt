### FontAwesome 463
set(LIB_FA de_fontawesome)
project(${LIB_FA} CXX)

if (TARGET ${LIB_FA})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_FA}${ColourReset}")
else()
   message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_FA}${ColourReset}")

   set(FA_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
   set(FA_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

   set(FA_HEADERS ${FA_HEADER_DIR}/de_fontawesome.hpp)
   set(FA_SOURCES ${FA_SOURCE_DIR}/de_fontawesome.cpp)

   add_library(${LIB_FA} STATIC "")
   target_sources(${LIB_FA} PUBLIC ${FA_HEADERS} PRIVATE ${FA_SOURCES})
   target_include_directories(${LIB_FA} PUBLIC ${FA_HEADER_DIR})

   # add_custom_command( TARGET ${LIB_FA} COMMAND "echo" "Copy fontawesome.ttf" COMMAND ${CMAKE_COMMAND} -E copy
         # ${CMAKE_CURRENT_LIST_DIR}/fontawesome.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fontawesome.ttf)

#   add_custom_command( TARGET ${LIB_FA} COMMAND "echo" "Copy fontawesome463.ttf" COMMAND ${CMAKE_COMMAND} -E copy
#         ${CMAKE_CURRENT_LIST_DIR}/fontawesome463.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fontawesome463.ttf)

#   add_custom_command( TARGET ${LIB_FA} COMMAND "echo" "Copy la-brands-400.ttf" COMMAND ${CMAKE_COMMAND} -E copy
#         ${CMAKE_CURRENT_LIST_DIR}/la-brands-400.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/la-brands-400.ttf)

#   add_custom_command( TARGET ${LIB_FA} COMMAND "echo" "Copy la-regular-400.ttf" COMMAND ${CMAKE_COMMAND} -E copy
#         ${CMAKE_CURRENT_LIST_DIR}/la-regular-400.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/la-regular-400.ttf)

#   add_custom_command( TARGET ${LIB_FA} COMMAND "echo" "Copy la-solid-900.ttf" COMMAND ${CMAKE_COMMAND} -E copy
#         ${CMAKE_CURRENT_LIST_DIR}/la-solid-900.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/la-solid-900.ttf)
endif()

# copy fonts:
file(COPY ${CMAKE_CURRENT_LIST_DIR}/fontawesome463.ttf DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/la-brands-400.ttf DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/la-regular-400.ttf DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/la-solid-900.ttf DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# file(COPY "${BHD}/Qt5Core.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Release
# file(COPY "${BHD}/Qt5Gui.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Release
# file(COPY "${BHD}/Qt5Widgets.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Release
# file(COPY "${BHD}/Qt5WinExtras.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Release

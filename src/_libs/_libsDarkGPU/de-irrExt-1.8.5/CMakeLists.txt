set(LIB_NAME de_irrExt)
project(${LIB_NAME}-dev CXX)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   set(IRREXT_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
   set(IRREXT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

   file(GLOB_RECURSE IRREXT_HEADERS "${IRREXT_HEADER_DIR}/*.hpp")
   file(GLOB_RECURSE IRREXT_SOURCES "${IRREXT_SOURCE_DIR}/*.cpp")

   # set(IRREXT_HEADERS
      # ${IRREXT_HEADER_DIR}/AutoMeshBuffer.hpp
      # ${IRREXT_HEADER_DIR}/AutoMeshBufferUtils.hpp
      # ${IRREXT_HEADER_DIR}/BaseEventReceiver.hpp
      # ${IRREXT_HEADER_DIR}/BaseWindow.hpp
      # ${IRREXT_HEADER_DIR}/Camera.hpp
      # ${IRREXT_HEADER_DIR}/CGUITTFont.hpp
      # ${IRREXT_HEADER_DIR}/CGUIWindow.hpp
      # ${IRREXT_HEADER_DIR}/createDevice.hpp
      # ${IRREXT_HEADER_DIR}/createRoundRect.hpp
      # ${IRREXT_HEADER_DIR}/CXMLWriterUTF8.hpp
      # ${IRREXT_HEADER_DIR}/FontUtils.hpp
      # ${IRREXT_HEADER_DIR}/ImageUtils.hpp
      # ${IRREXT_HEADER_DIR}/irrExt.hpp
      # ${IRREXT_HEADER_DIR}/libFreeType.hpp
      # ${IRREXT_HEADER_DIR}/sinCosTable.hpp
      # ${IRREXT_HEADER_DIR}/StringUtils.hpp
      # ${IRREXT_HEADER_DIR}/ustring.hpp
      # ${IRREXT_HEADER_DIR}/MeshLoader.hpp)

   # set(IRREXT_SOURCES
      # ${IRREXT_SOURCE_DIR}/AutoMeshBuffer.cpp
      # ${IRREXT_SOURCE_DIR}/AutoMeshBufferUtils.cpp
      # ${IRREXT_SOURCE_DIR}/BaseEventReceiver.cpp
      # ${IRREXT_SOURCE_DIR}/BaseWindow.cpp
      # ${IRREXT_SOURCE_DIR}/Camera.cpp
      # ${IRREXT_SOURCE_DIR}/CGUITTFont.cpp
      # ${IRREXT_SOURCE_DIR}/CGUIWindow.cpp
      # ${IRREXT_SOURCE_DIR}/createDevice.cpp
      # ${IRREXT_SOURCE_DIR}/createRoundRect.cpp
      # ${IRREXT_SOURCE_DIR}/CXMLWriterUTF8.cpp
      # ${IRREXT_SOURCE_DIR}/FontUtils.cpp
      # ${IRREXT_SOURCE_DIR}/ImageUtils.cpp
      # ${IRREXT_SOURCE_DIR}/irrExt.cpp
      # ${IRREXT_SOURCE_DIR}/libFreeType.cpp
      # ${IRREXT_SOURCE_DIR}/StringUtils.cpp
      # ${IRREXT_SOURCE_DIR}/MeshLoader.cpp)

   if (BUILD_SHARED)
      add_library(${LIB_NAME} SHARED "")
      message("${BoldBlue}[benni] SHARED_LIB: ${LIB_NAME}${ColourReset}")
   else()
      add_library(${LIB_NAME} STATIC "")
      message("${BoldBlue}[benni] STATIC_LIB: ${LIB_NAME}${ColourReset}")
   endif()

   target_include_directories(${LIB_NAME} PUBLIC ${IRREXT_HEADER_DIR})
   target_include_directories(${LIB_NAME} PRIVATE ${IRREXT_SOURCE_DIR})
   target_sources(${LIB_NAME} PUBLIC ${IRREXT_HEADERS} PRIVATE ${IRREXT_SOURCES})
   #target_compile_definitions(${LIB_NAME} PUBLIC -D_IRR_STATIC_LIB_=1)
   target_link_libraries(${LIB_NAME} PUBLIC de_irrlicht)
   target_link_libraries(${LIB_NAME} PUBLIC de_freetype2)
   target_link_libraries(${LIB_NAME} PUBLIC de_glm)

endif()

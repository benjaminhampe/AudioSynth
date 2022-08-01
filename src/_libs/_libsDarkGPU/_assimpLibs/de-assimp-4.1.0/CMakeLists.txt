set(LIB_NAME de_assimp)
project(${LIB_NAME}-dev CXX)

# Depends on: utf8cpp clipper poly2tri open3dgc openddlparser irrXML rapidjson unzip minizip z

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   set(ASSIMP_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/assimp)
   set(ASSIMP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   set(ASSIMP_INCLUDE_DIR ${ASSIMP_HEADER_DIR}/..)

   # All supported options ###############################################
   option(BUILD_SHARED_LIBS "Build package with shared libraries." ON )
   option(BUILD_FRAMEWORK "Build package as Mac OS X Framework bundle." OFF)
   option(ASSIMP_DOUBLE_PRECISION "Set to ON to enable double precision processing" OFF)
   option(ASSIMP_OPT_BUILD_PACKAGES "Set to ON to generate CPack configuration files and packaging targets" OFF)
   option(ASSIMP_ANDROID_JNIIOSYSTEM "Android JNI IOSystem support is active" OFF)
   option(ASSIMP_NO_EXPORT "Disable Assimp's export functionality." OFF)
   option(ASSIMP_BUILD_ZLIB "Build your own zlib" OFF)
   option(ASSIMP_BUILD_ASSIMP_TOOLS "If the supplementary tools for Assimp are built in addition to the library." ON)
   option(ASSIMP_BUILD_SAMPLES "If the official samples are built as well (needs Glut)." OFF)
   option(ASSIMP_BUILD_TESTS "If the test suite for Assimp is built in addition to the library." ON)
   option(ASSIMP_COVERALLS "Enable this to measure test coverage." OFF)
   option(ASSIMP_WERROR "Treat warnings as errors." OFF)
   option(ASSIMP_ASAN "Enable AddressSanitizer." OFF)
   option(ASSIMP_UBSAN "Enable Undefined Behavior sanitizer." OFF)
   option(SYSTEM_IRRXML "Use system installed Irrlicht/IrrXML library." OFF)
   option(BUILD_DOCS "Build documentation using Doxygen." OFF)

   ##### add_definitions(-DASSIMP_DOUBLE_PRECISION)

   # Needed for openddl_parser config, no use of c++11 at this moment
   # add_definitions( -DOPENDDL_NO_USE_CPP11 )
   # set_property( GLOBAL PROPERTY CXX_STANDARD 11 )

   # add_definitions( -DASSIMP_BUILD_DLL_EXPORT )
   add_definitions( -DASSIMP_BUILD_NO_OWN_ZLIB )
   add_definitions( -DASSIMP_BUILD_NO_C4D_IMPORTER ) # currently only msvc?
   #add_definitions( -DASSIMP_BUILD_NO_IFC_IMPORTER )
   #add_definitions( -DASSIMP_BUILD_NO_XGL_IMPORTER )
   #add_definitions( -DASSIMP_BUILD_NO_FBX_IMPORTER )
   #add_definitions( -DASSIMP_BUILD_NO_GLTF_IMPORTER )
   #add_definitions( -DASSIMP_BUILD_NO_GLTF_EXPORTER )
   #add_definitions( -DASSIMP_BUILD_NO_CSM_IMPORTER )
   #add_definitions( -DASSIMP_BUILD_NO_3D_IMPORTER )
   #add_definitions( -DASSIMP_BUILD_NO_OGRE_IMPORTER )
   #add_definitions( -DASSIMP_BUILD_NO_OPENGEX_IMPORTER )
   #add_definitions( -DASSIMP_BUILD_NO_X3D_IMPORTER )
   #add_definitions( -DASSIMP_BUILD_NO_X3D_EXPORTER )
   #add_definitions( -DASSIMP_BUILD_NO_MMD_IMPORTER )

   # Define here the needed parameters
   set(LINK_SEARCH_START_STATIC TRUE)
   set(ASSIMP_VERSION_MAJOR 4)
   set(ASSIMP_VERSION_MINOR 1)
   set(ASSIMP_VERSION_PATCH 0)
   set(ASSIMP_VERSION ${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH})
   set(ASSIMP_SOVERSION 4)
   set(PROJECT_VERSION "${ASSIMP_VERSION}")
   set(ASSIMP_PACKAGE_VERSION "0" CACHE STRING "the package-specific version used for uploading the sources" )

   # LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules" )
   # set(LIBASSIMP_COMPONENT     "libassimp${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH}" )
   # set(LIBASSIMP-DEV_COMPONENT "libassimp${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH}-dev" )
   # set(CPACK_COMPONENTS_ALL assimp-bin ${LIBASSIMP_COMPONENT} ${LIBASSIMP-DEV_COMPONENT} assimp-dev)
   set(ASSIMP_LIBRARY_SUFFIX "" CACHE STRING "Suffix to append to library names")

   # Get the current working branch
   EXECUTE_PROCESS(
     COMMAND git rev-parse --abbrev-ref HEAD
     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
     OUTPUT_VARIABLE GIT_BRANCH
     OUTPUT_STRIP_TRAILING_WHITESPACE
     ERROR_QUIET
   )

   # Get the latest abbreviated commit hash of the working branch
   EXECUTE_PROCESS(
     COMMAND git log -1 --format=%h
     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
     OUTPUT_VARIABLE GIT_COMMIT_HASH
     OUTPUT_STRIP_TRAILING_WHITESPACE
     ERROR_QUIET
   )

   IF (NOT GIT_COMMIT_HASH)
      set(GIT_COMMIT_HASH 0)
   ENDIF()

   CONFIGURE_FILE(
     ${ASSIMP_HEADER_DIR}/revision.h.in
     ${ASSIMP_HEADER_DIR}/revision.h
   )

   CONFIGURE_FILE(
     ${ASSIMP_HEADER_DIR}/config.h.in
     ${ASSIMP_HEADER_DIR}/config.h
   )

   set(ASSIMP_HEADERS
      ${ASSIMP_HEADER_DIR}/Compiler/pushpack1.h
      ${ASSIMP_HEADER_DIR}/Compiler/poppack1.h
      ${ASSIMP_HEADER_DIR}/Compiler/pstdint.h
      ${ASSIMP_HEADER_DIR}/anim.h
      ${ASSIMP_HEADER_DIR}/ai_assert.h
      ${ASSIMP_HEADER_DIR}/camera.h
      ${ASSIMP_HEADER_DIR}/color4.h
      ${ASSIMP_HEADER_DIR}/color4.inl
      ${ASSIMP_HEADER_DIR}/config.h
      ${ASSIMP_HEADER_DIR}/defs.h
      ${ASSIMP_HEADER_DIR}/Defines.h
      ${ASSIMP_HEADER_DIR}/cfileio.h
      ${ASSIMP_HEADER_DIR}/light.h
      ${ASSIMP_HEADER_DIR}/material.h
      ${ASSIMP_HEADER_DIR}/material.inl
      ${ASSIMP_HEADER_DIR}/matrix3x3.h
      ${ASSIMP_HEADER_DIR}/matrix3x3.inl
      ${ASSIMP_HEADER_DIR}/matrix4x4.h
      ${ASSIMP_HEADER_DIR}/matrix4x4.inl
      ${ASSIMP_HEADER_DIR}/mesh.h
      ${ASSIMP_HEADER_DIR}/postprocess.h
      ${ASSIMP_HEADER_DIR}/quaternion.h
      ${ASSIMP_HEADER_DIR}/quaternion.inl
      ${ASSIMP_HEADER_DIR}/scene.h
      ${ASSIMP_HEADER_DIR}/metadata.h
      ${ASSIMP_HEADER_DIR}/texture.h
      ${ASSIMP_HEADER_DIR}/types.h
      ${ASSIMP_HEADER_DIR}/vector2.h
      ${ASSIMP_HEADER_DIR}/vector2.inl
      ${ASSIMP_HEADER_DIR}/vector3.h
      ${ASSIMP_HEADER_DIR}/vector3.inl
      ${ASSIMP_HEADER_DIR}/version.h
      ${ASSIMP_HEADER_DIR}/cimport.h
      ${ASSIMP_HEADER_DIR}/importerdesc.h
      ${ASSIMP_HEADER_DIR}/Importer.hpp
      ${ASSIMP_HEADER_DIR}/DefaultLogger.hpp
      ${ASSIMP_HEADER_DIR}/ProgressHandler.hpp
      ${ASSIMP_HEADER_DIR}/IOStream.hpp
      ${ASSIMP_HEADER_DIR}/IOSystem.hpp
      ${ASSIMP_HEADER_DIR}/Logger.hpp
      ${ASSIMP_HEADER_DIR}/LogStream.hpp
      ${ASSIMP_HEADER_DIR}/NullLogger.hpp
      ${ASSIMP_HEADER_DIR}/cexport.h
      ${ASSIMP_HEADER_DIR}/Exporter.hpp
      ${ASSIMP_HEADER_DIR}/DefaultIOStream.h
      ${ASSIMP_HEADER_DIR}/DefaultIOSystem.h
      ${ASSIMP_HEADER_DIR}/SceneCombiner.h
   )

   set(ASSIMP_SOURCES
   # Core_SRCS
      ${ASSIMP_SOURCE_DIR}/Assimp.cpp
   # Logging_SRCS
      ${ASSIMP_HEADER_DIR}/DefaultLogger.hpp
      ${ASSIMP_HEADER_DIR}/LogStream.hpp
      ${ASSIMP_HEADER_DIR}/Logger.hpp
      ${ASSIMP_HEADER_DIR}/NullLogger.hpp
      ${ASSIMP_SOURCE_DIR}/Win32DebugLogStream.h
      ${ASSIMP_SOURCE_DIR}/DefaultLogger.cpp
      ${ASSIMP_SOURCE_DIR}/FileLogStream.h
      ${ASSIMP_SOURCE_DIR}/StdOStreamLogStream.h
      ${ASSIMP_SOURCE_DIR}/irrXMLWrapper.h
   # Common_SRCS
      ${ASSIMP_SOURCE_DIR}/fast_atof.h
      ${ASSIMP_SOURCE_DIR}/qnan.h
      ${ASSIMP_SOURCE_DIR}/BaseImporter.cpp
      ${ASSIMP_SOURCE_DIR}/BaseImporter.h
      ${ASSIMP_SOURCE_DIR}/BaseProcess.cpp
      ${ASSIMP_SOURCE_DIR}/BaseProcess.h
      ${ASSIMP_SOURCE_DIR}/Importer.h
      ${ASSIMP_SOURCE_DIR}/ScenePrivate.h
      ${ASSIMP_SOURCE_DIR}/PostStepRegistry.cpp
      ${ASSIMP_SOURCE_DIR}/ImporterRegistry.cpp
      ${ASSIMP_SOURCE_DIR}/ByteSwapper.h
      ${ASSIMP_SOURCE_DIR}/DefaultProgressHandler.h
      ${ASSIMP_SOURCE_DIR}/DefaultIOStream.cpp
      ${ASSIMP_SOURCE_DIR}/DefaultIOSystem.cpp
      ${ASSIMP_SOURCE_DIR}/CInterfaceIOWrapper.cpp
      ${ASSIMP_SOURCE_DIR}/CInterfaceIOWrapper.h
      ${ASSIMP_SOURCE_DIR}/Hash.h
      ${ASSIMP_SOURCE_DIR}/Importer.cpp
      ${ASSIMP_SOURCE_DIR}/IFF.h
      ${ASSIMP_SOURCE_DIR}/MemoryIOWrapper.h
      ${ASSIMP_SOURCE_DIR}/ParsingUtils.h
      ${ASSIMP_SOURCE_DIR}/StreamReader.h
      ${ASSIMP_SOURCE_DIR}/StreamWriter.h
      ${ASSIMP_SOURCE_DIR}/StringComparison.h
      ${ASSIMP_SOURCE_DIR}/StringUtils.h
      ${ASSIMP_SOURCE_DIR}/SGSpatialSort.cpp
      ${ASSIMP_SOURCE_DIR}/SGSpatialSort.h
      ${ASSIMP_SOURCE_DIR}/VertexTriangleAdjacency.cpp
      ${ASSIMP_SOURCE_DIR}/VertexTriangleAdjacency.h
      ${ASSIMP_SOURCE_DIR}/GenericProperty.h
      ${ASSIMP_SOURCE_DIR}/SpatialSort.cpp
      ${ASSIMP_SOURCE_DIR}/SpatialSort.h
      ${ASSIMP_SOURCE_DIR}/SceneCombiner.cpp
      ${ASSIMP_SOURCE_DIR}/ScenePreprocessor.cpp
      ${ASSIMP_SOURCE_DIR}/ScenePreprocessor.h
      ${ASSIMP_SOURCE_DIR}/SkeletonMeshBuilder.cpp
      ${ASSIMP_SOURCE_DIR}/SkeletonMeshBuilder.h
      ${ASSIMP_SOURCE_DIR}/SplitByBoneCountProcess.cpp
      ${ASSIMP_SOURCE_DIR}/SplitByBoneCountProcess.h
      ${ASSIMP_SOURCE_DIR}/ScaleProcess.cpp
      ${ASSIMP_SOURCE_DIR}/ScaleProcess.h
      ${ASSIMP_SOURCE_DIR}/SmoothingGroups.h
      ${ASSIMP_SOURCE_DIR}/StandardShapes.cpp
      ${ASSIMP_SOURCE_DIR}/StandardShapes.h
      ${ASSIMP_SOURCE_DIR}/TargetAnimation.cpp
      ${ASSIMP_SOURCE_DIR}/TargetAnimation.h
      ${ASSIMP_SOURCE_DIR}/RemoveComments.cpp
      ${ASSIMP_SOURCE_DIR}/RemoveComments.h
      ${ASSIMP_SOURCE_DIR}/Subdivision.cpp
      ${ASSIMP_SOURCE_DIR}/Subdivision.h
      ${ASSIMP_SOURCE_DIR}/scene.cpp
      ${ASSIMP_SOURCE_DIR}/Vertex.h
      ${ASSIMP_SOURCE_DIR}/LineSplitter.h
      ${ASSIMP_SOURCE_DIR}/TinyFormatter.h
      ${ASSIMP_SOURCE_DIR}/Profiler.h
      ${ASSIMP_SOURCE_DIR}/LogAux.h
      ${ASSIMP_SOURCE_DIR}/Bitmap.cpp
      ${ASSIMP_SOURCE_DIR}/Bitmap.h
      ${ASSIMP_SOURCE_DIR}/XMLTools.h
      ${ASSIMP_SOURCE_DIR}/Version.cpp
      ${ASSIMP_SOURCE_DIR}/IOStreamBuffer.h
      ${ASSIMP_SOURCE_DIR}/CreateAnimMesh.h
      ${ASSIMP_SOURCE_DIR}/CreateAnimMesh.cpp
      # IFC ( do i need it for STEP? )
       ${ASSIMP_SOURCE_DIR}/IFCLoader.cpp
       ${ASSIMP_SOURCE_DIR}/IFCLoader.h
       ${ASSIMP_SOURCE_DIR}/IFCReaderGen1.cpp
       ${ASSIMP_SOURCE_DIR}/IFCReaderGen2.cpp
       ${ASSIMP_SOURCE_DIR}/IFCReaderGen.h
       ${ASSIMP_SOURCE_DIR}/IFCUtil.h
       ${ASSIMP_SOURCE_DIR}/IFCUtil.cpp
       ${ASSIMP_SOURCE_DIR}/IFCGeometry.cpp
       ${ASSIMP_SOURCE_DIR}/IFCMaterial.cpp
       ${ASSIMP_SOURCE_DIR}/IFCProfile.cpp
       ${ASSIMP_SOURCE_DIR}/IFCCurve.cpp
       ${ASSIMP_SOURCE_DIR}/IFCBoolean.cpp
       ${ASSIMP_SOURCE_DIR}/IFCOpenings.cpp
      # GLTF
       ${ASSIMP_SOURCE_DIR}/glTFAsset.h
       ${ASSIMP_SOURCE_DIR}/glTFAsset.inl
       ${ASSIMP_SOURCE_DIR}/glTFAssetWriter.h
       ${ASSIMP_SOURCE_DIR}/glTFAssetWriter.inl
       ${ASSIMP_SOURCE_DIR}/glTFImporter.cpp
       ${ASSIMP_SOURCE_DIR}/glTFImporter.h
       ${ASSIMP_SOURCE_DIR}/glTFExporter.h
       ${ASSIMP_SOURCE_DIR}/glTFExporter.cpp
       ${ASSIMP_SOURCE_DIR}/glTF2Asset.h
       ${ASSIMP_SOURCE_DIR}/glTF2Asset.inl
       ${ASSIMP_SOURCE_DIR}/glTF2AssetWriter.h
       ${ASSIMP_SOURCE_DIR}/glTF2AssetWriter.inl
       ${ASSIMP_SOURCE_DIR}/glTF2Importer.cpp
       ${ASSIMP_SOURCE_DIR}/glTF2Importer.h
       ${ASSIMP_SOURCE_DIR}/glTF2Exporter.h
       ${ASSIMP_SOURCE_DIR}/glTF2Exporter.cpp
      # FBX ?
       ${ASSIMP_SOURCE_DIR}/FBXImporter.cpp
       ${ASSIMP_SOURCE_DIR}/FBXCompileConfig.h
       ${ASSIMP_SOURCE_DIR}/FBXImporter.h
       ${ASSIMP_SOURCE_DIR}/FBXParser.cpp
       ${ASSIMP_SOURCE_DIR}/FBXParser.h
       ${ASSIMP_SOURCE_DIR}/FBXTokenizer.cpp
       ${ASSIMP_SOURCE_DIR}/FBXTokenizer.h
       ${ASSIMP_SOURCE_DIR}/FBXImportSettings.h
       ${ASSIMP_SOURCE_DIR}/FBXConverter.h
       ${ASSIMP_SOURCE_DIR}/FBXConverter.cpp
       ${ASSIMP_SOURCE_DIR}/FBXUtil.h
       ${ASSIMP_SOURCE_DIR}/FBXUtil.cpp
       ${ASSIMP_SOURCE_DIR}/FBXDocument.h
       ${ASSIMP_SOURCE_DIR}/FBXDocument.cpp
       ${ASSIMP_SOURCE_DIR}/FBXProperties.h
       ${ASSIMP_SOURCE_DIR}/FBXProperties.cpp
       ${ASSIMP_SOURCE_DIR}/FBXMeshGeometry.h
       ${ASSIMP_SOURCE_DIR}/FBXMeshGeometry.cpp
       ${ASSIMP_SOURCE_DIR}/FBXMaterial.cpp
       ${ASSIMP_SOURCE_DIR}/FBXModel.cpp
       ${ASSIMP_SOURCE_DIR}/FBXAnimation.cpp
       ${ASSIMP_SOURCE_DIR}/FBXNodeAttribute.cpp
       ${ASSIMP_SOURCE_DIR}/FBXDeformer.cpp
       ${ASSIMP_SOURCE_DIR}/FBXBinaryTokenizer.cpp
       ${ASSIMP_SOURCE_DIR}/FBXDocumentUtil.cpp
      # Non Free C4D sources
      ${ASSIMP_SOURCE_DIR}/C4DImporter.cpp
      ${ASSIMP_SOURCE_DIR}/C4DImporter.h
      # OFF
      ${ASSIMP_SOURCE_DIR}/OFFLoader.cpp
      ${ASSIMP_SOURCE_DIR}/OFFLoader.h
      # AMF
      ${ASSIMP_SOURCE_DIR}/AMFImporter.hpp
      ${ASSIMP_SOURCE_DIR}/AMFImporter_Macro.hpp
      ${ASSIMP_SOURCE_DIR}/AMFImporter_Node.hpp
      ${ASSIMP_SOURCE_DIR}/AMFImporter.cpp
      ${ASSIMP_SOURCE_DIR}/AMFImporter_Geometry.cpp
      ${ASSIMP_SOURCE_DIR}/AMFImporter_Material.cpp
      ${ASSIMP_SOURCE_DIR}/AMFImporter_Postprocess.cpp
      # 3DS
      ${ASSIMP_SOURCE_DIR}/3DSConverter.cpp
      ${ASSIMP_SOURCE_DIR}/3DSHelper.h
      ${ASSIMP_SOURCE_DIR}/3DSLoader.cpp
      ${ASSIMP_SOURCE_DIR}/3DSLoader.h
      ${ASSIMP_SOURCE_DIR}/3DSExporter.h
      ${ASSIMP_SOURCE_DIR}/3DSExporter.cpp
      # STEP
      ${ASSIMP_SOURCE_DIR}/STEPFile.h
      ${ASSIMP_SOURCE_DIR}/STEPFileReader.h
      ${ASSIMP_SOURCE_DIR}/STEPFileReader.cpp
      ${ASSIMP_SOURCE_DIR}/STEPFileEncoding.cpp
      ${ASSIMP_SOURCE_DIR}/STEPFileEncoding.h
      # PLY
      ${ASSIMP_SOURCE_DIR}/PlyLoader.cpp
      ${ASSIMP_SOURCE_DIR}/PlyLoader.h
      ${ASSIMP_SOURCE_DIR}/PlyParser.cpp
      ${ASSIMP_SOURCE_DIR}/PlyParser.h
      ${ASSIMP_SOURCE_DIR}/PlyExporter.cpp
      ${ASSIMP_SOURCE_DIR}/PlyExporter.h
      # MS3D
      ${ASSIMP_SOURCE_DIR}/MS3DLoader.cpp
      ${ASSIMP_SOURCE_DIR}/MS3DLoader.h
      # COB
      ${ASSIMP_SOURCE_DIR}/COBLoader.cpp
      ${ASSIMP_SOURCE_DIR}/COBLoader.h
      ${ASSIMP_SOURCE_DIR}/COBScene.h
      # BLEND
      ${ASSIMP_SOURCE_DIR}/BlenderLoader.cpp
      ${ASSIMP_SOURCE_DIR}/BlenderLoader.h
      ${ASSIMP_SOURCE_DIR}/BlenderDNA.cpp
      ${ASSIMP_SOURCE_DIR}/BlenderDNA.h
      ${ASSIMP_SOURCE_DIR}/BlenderDNA.inl
      ${ASSIMP_SOURCE_DIR}/BlenderScene.cpp
      ${ASSIMP_SOURCE_DIR}/BlenderScene.h
      ${ASSIMP_SOURCE_DIR}/BlenderSceneGen.h
      ${ASSIMP_SOURCE_DIR}/BlenderIntermediate.h
      ${ASSIMP_SOURCE_DIR}/BlenderModifier.h
      ${ASSIMP_SOURCE_DIR}/BlenderModifier.cpp
      ${ASSIMP_SOURCE_DIR}/BlenderBMesh.h
      ${ASSIMP_SOURCE_DIR}/BlenderBMesh.cpp
      ${ASSIMP_SOURCE_DIR}/BlenderTessellator.h
      ${ASSIMP_SOURCE_DIR}/BlenderTessellator.cpp

      # B3D
      ${ASSIMP_SOURCE_DIR}/B3DImporter.cpp
      ${ASSIMP_SOURCE_DIR}/B3DImporter.h
      # BVH
      ${ASSIMP_SOURCE_DIR}/BVHLoader.cpp
      ${ASSIMP_SOURCE_DIR}/BVHLoader.h
      # DXF
      ${ASSIMP_SOURCE_DIR}/DXFLoader.cpp
      ${ASSIMP_SOURCE_DIR}/DXFLoader.h
      ${ASSIMP_SOURCE_DIR}/DXFHelper.h
      # COLLADA
      ${ASSIMP_SOURCE_DIR}/ColladaHelper.h
      ${ASSIMP_SOURCE_DIR}/ColladaLoader.cpp
      ${ASSIMP_SOURCE_DIR}/ColladaLoader.h
      ${ASSIMP_SOURCE_DIR}/ColladaParser.cpp
      ${ASSIMP_SOURCE_DIR}/ColladaParser.h
      ${ASSIMP_SOURCE_DIR}/ColladaExporter.h
      ${ASSIMP_SOURCE_DIR}/ColladaExporter.cpp
      # IRRMESH
      ${ASSIMP_SOURCE_DIR}/IRRMeshLoader.cpp
      ${ASSIMP_SOURCE_DIR}/IRRMeshLoader.h
      ${ASSIMP_SOURCE_DIR}/IRRShared.cpp
      ${ASSIMP_SOURCE_DIR}/IRRShared.h
      # IRR
      ${ASSIMP_SOURCE_DIR}/IRRLoader.cpp
      ${ASSIMP_SOURCE_DIR}/IRRLoader.h
      ${ASSIMP_SOURCE_DIR}/IRRShared.cpp
      ${ASSIMP_SOURCE_DIR}/IRRShared.h
      # LWO
      ${ASSIMP_SOURCE_DIR}/LWOAnimation.cpp
      ${ASSIMP_SOURCE_DIR}/LWOAnimation.h
      ${ASSIMP_SOURCE_DIR}/LWOBLoader.cpp
      ${ASSIMP_SOURCE_DIR}/LWOFileData.h
      ${ASSIMP_SOURCE_DIR}/LWOLoader.cpp
      ${ASSIMP_SOURCE_DIR}/LWOLoader.h
      ${ASSIMP_SOURCE_DIR}/LWOMaterial.cpp
      # LWS
      ${ASSIMP_SOURCE_DIR}/LWSLoader.cpp
      ${ASSIMP_SOURCE_DIR}/LWSLoader.h
      # MD2
      ${ASSIMP_SOURCE_DIR}/MD2FileData.h
      ${ASSIMP_SOURCE_DIR}/MD2Loader.cpp
      ${ASSIMP_SOURCE_DIR}/MD2Loader.h
      ${ASSIMP_SOURCE_DIR}/MD2NormalTable.h
      # MD3
      ${ASSIMP_SOURCE_DIR}/MD3FileData.h
      ${ASSIMP_SOURCE_DIR}/MD3Loader.cpp
      ${ASSIMP_SOURCE_DIR}/MD3Loader.h
      # Extra_SRCS
      ${ASSIMP_SOURCE_DIR}/MD4FileData.h
      # MD5
      ${ASSIMP_SOURCE_DIR}/MD5Loader.cpp
      ${ASSIMP_SOURCE_DIR}/MD5Loader.h
      ${ASSIMP_SOURCE_DIR}/MD5Parser.cpp
      ${ASSIMP_SOURCE_DIR}/MD5Parser.h
      # MDC
      ${ASSIMP_SOURCE_DIR}/MDCFileData.h
      ${ASSIMP_SOURCE_DIR}/MDCLoader.cpp
      ${ASSIMP_SOURCE_DIR}/MDCLoader.h
      ${ASSIMP_SOURCE_DIR}/MDCNormalTable.h
      # MDL
      ${ASSIMP_SOURCE_DIR}/MDLDefaultColorMap.h
      ${ASSIMP_SOURCE_DIR}/MDLFileData.h
      ${ASSIMP_SOURCE_DIR}/MDLLoader.cpp
      ${ASSIMP_SOURCE_DIR}/MDLLoader.h
      ${ASSIMP_SOURCE_DIR}/MDLMaterialLoader.cpp
      # CSM
      ${ASSIMP_SOURCE_DIR}/CSMLoader.cpp
      ${ASSIMP_SOURCE_DIR}/CSMLoader.h
      # HMP
      ${ASSIMP_SOURCE_DIR}/HMPFileData.h
      ${ASSIMP_SOURCE_DIR}/HMPLoader.cpp
      ${ASSIMP_SOURCE_DIR}/HMPLoader.h
      ${ASSIMP_SOURCE_DIR}/HalfLifeFileData.h
      # NFF
      ${ASSIMP_SOURCE_DIR}/NFFLoader.cpp
      ${ASSIMP_SOURCE_DIR}/NFFLoader.h
      # NDO
      ${ASSIMP_SOURCE_DIR}/NDOLoader.cpp
      ${ASSIMP_SOURCE_DIR}/NDOLoader.h
      # OBJ
      ${ASSIMP_SOURCE_DIR}/ObjFileData.h
      ${ASSIMP_SOURCE_DIR}/ObjFileImporter.cpp
      ${ASSIMP_SOURCE_DIR}/ObjFileImporter.h
      ${ASSIMP_SOURCE_DIR}/ObjFileMtlImporter.cpp
      ${ASSIMP_SOURCE_DIR}/ObjFileMtlImporter.h
      ${ASSIMP_SOURCE_DIR}/ObjFileParser.cpp
      ${ASSIMP_SOURCE_DIR}/ObjFileParser.h
      ${ASSIMP_SOURCE_DIR}/ObjTools.h
      ${ASSIMP_SOURCE_DIR}/ObjExporter.h
      ${ASSIMP_SOURCE_DIR}/ObjExporter.cpp
      # AC
      ${ASSIMP_SOURCE_DIR}/ACLoader.cpp
      ${ASSIMP_SOURCE_DIR}/ACLoader.h
      # ASE
      ${ASSIMP_SOURCE_DIR}/ASELoader.cpp
      ${ASSIMP_SOURCE_DIR}/ASELoader.h
      ${ASSIMP_SOURCE_DIR}/ASEParser.cpp
      ${ASSIMP_SOURCE_DIR}/ASEParser.h
      # ASSBIN
      ${ASSIMP_SOURCE_DIR}/AssbinExporter.h
      ${ASSIMP_SOURCE_DIR}/AssbinExporter.cpp
      ${ASSIMP_SOURCE_DIR}/AssbinLoader.h
      ${ASSIMP_SOURCE_DIR}/AssbinLoader.cpp
      # ASSXML
      ${ASSIMP_SOURCE_DIR}/AssxmlExporter.h
      ${ASSIMP_SOURCE_DIR}/AssxmlExporter.cpp
      # XGL
      ${ASSIMP_SOURCE_DIR}/XGLLoader.cpp
      ${ASSIMP_SOURCE_DIR}/XGLLoader.h
      # OPENGEX
      ${ASSIMP_SOURCE_DIR}/OpenGEXExporter.cpp
      ${ASSIMP_SOURCE_DIR}/OpenGEXExporter.h
      ${ASSIMP_SOURCE_DIR}/OpenGEXImporter.cpp
      ${ASSIMP_SOURCE_DIR}/OpenGEXImporter.h
      ${ASSIMP_SOURCE_DIR}/OpenGEXStructs.h
      # OGRE
      ${ASSIMP_SOURCE_DIR}/OgreImporter.h
      ${ASSIMP_SOURCE_DIR}/OgreStructs.h
      ${ASSIMP_SOURCE_DIR}/OgreParsingUtils.h
      ${ASSIMP_SOURCE_DIR}/OgreBinarySerializer.h
      ${ASSIMP_SOURCE_DIR}/OgreXmlSerializer.h
      ${ASSIMP_SOURCE_DIR}/OgreImporter.cpp
      ${ASSIMP_SOURCE_DIR}/OgreStructs.cpp
      ${ASSIMP_SOURCE_DIR}/OgreBinarySerializer.cpp
      ${ASSIMP_SOURCE_DIR}/OgreXmlSerializer.cpp
      ${ASSIMP_SOURCE_DIR}/OgreMaterial.cpp
      # Q3D
      ${ASSIMP_SOURCE_DIR}/Q3DLoader.cpp
      ${ASSIMP_SOURCE_DIR}/Q3DLoader.h
      # Q3BSP
      ${ASSIMP_SOURCE_DIR}/Q3BSPFileData.h
      ${ASSIMP_SOURCE_DIR}/Q3BSPFileParser.h
      ${ASSIMP_SOURCE_DIR}/Q3BSPFileParser.cpp
      ${ASSIMP_SOURCE_DIR}/Q3BSPFileImporter.h
      ${ASSIMP_SOURCE_DIR}/Q3BSPFileImporter.cpp
      ${ASSIMP_SOURCE_DIR}/Q3BSPZipArchive.h
      ${ASSIMP_SOURCE_DIR}/Q3BSPZipArchive.cpp
      # RAW
      ${ASSIMP_SOURCE_DIR}/RawLoader.cpp
      ${ASSIMP_SOURCE_DIR}/RawLoader.h
      # SIB
      ${ASSIMP_SOURCE_DIR}/SIBImporter.cpp
      ${ASSIMP_SOURCE_DIR}/SIBImporter.h
      # SMD
      ${ASSIMP_SOURCE_DIR}/SMDLoader.cpp
      ${ASSIMP_SOURCE_DIR}/SMDLoader.h
      # STL
      ${ASSIMP_SOURCE_DIR}/STLLoader.cpp
      ${ASSIMP_SOURCE_DIR}/STLLoader.h
      ${ASSIMP_SOURCE_DIR}/STLExporter.h
      ${ASSIMP_SOURCE_DIR}/STLExporter.cpp
      # TERRAGEN
      ${ASSIMP_SOURCE_DIR}/TerragenLoader.cpp
      ${ASSIMP_SOURCE_DIR}/TerragenLoader.h
      # 3D
      ${ASSIMP_SOURCE_DIR}/UnrealLoader.cpp
      ${ASSIMP_SOURCE_DIR}/UnrealLoader.h
      # X
      ${ASSIMP_SOURCE_DIR}/XFileHelper.h
      ${ASSIMP_SOURCE_DIR}/XFileImporter.cpp
      ${ASSIMP_SOURCE_DIR}/XFileImporter.h
      ${ASSIMP_SOURCE_DIR}/XFileParser.cpp
      ${ASSIMP_SOURCE_DIR}/XFileParser.h
      ${ASSIMP_SOURCE_DIR}/XFileExporter.h
      ${ASSIMP_SOURCE_DIR}/XFileExporter.cpp
      # FI
      ${ASSIMP_SOURCE_DIR}/FIReader.hpp
      ${ASSIMP_SOURCE_DIR}/FIReader.cpp
      # X3D
      ${ASSIMP_SOURCE_DIR}/X3DExporter.cpp
      ${ASSIMP_SOURCE_DIR}/X3DExporter.hpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter.hpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Geometry2D.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Geometry3D.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Group.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Light.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Macro.hpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Metadata.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Networking.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Node.hpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Postprocess.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Rendering.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Shape.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Texturing.cpp
      ${ASSIMP_SOURCE_DIR}/X3DVocabulary.cpp
      # 3MF
      ${ASSIMP_SOURCE_DIR}/D3MFImporter.h
      ${ASSIMP_SOURCE_DIR}/D3MFImporter.cpp
      ${ASSIMP_SOURCE_DIR}/D3MFExporter.h
      ${ASSIMP_SOURCE_DIR}/D3MFExporter.cpp
      ${ASSIMP_SOURCE_DIR}/D3MFOpcPackage.h
      ${ASSIMP_SOURCE_DIR}/D3MFOpcPackage.cpp
      ${ASSIMP_SOURCE_DIR}/3MFXmlTags.h
      # MMD
      ${ASSIMP_SOURCE_DIR}/MMDCpp14.h
      ${ASSIMP_SOURCE_DIR}/MMDImporter.cpp
      ${ASSIMP_SOURCE_DIR}/MMDImporter.h
      ${ASSIMP_SOURCE_DIR}/MMDPmdParser.h
      ${ASSIMP_SOURCE_DIR}/MMDPmxParser.h
      ${ASSIMP_SOURCE_DIR}/MMDPmxParser.cpp
      ${ASSIMP_SOURCE_DIR}/MMDVmdParser.h
      # Step_SRCS
      ${ASSIMP_SOURCE_DIR}/StepExporter.h
      ${ASSIMP_SOURCE_DIR}/StepExporter.cpp
      # Exporter_SRCS
      ${ASSIMP_SOURCE_DIR}/Exporter.cpp
      ${ASSIMP_SOURCE_DIR}/AssimpCExport.cpp
      ${ASSIMP_SOURCE_DIR}/BlobIOSystem.h
      # MaterialSystem_SRCS
      ${ASSIMP_SOURCE_DIR}/MaterialSystem.cpp
      ${ASSIMP_SOURCE_DIR}/MaterialSystem.h

      # PostProcessing_SRCS
      ${ASSIMP_SOURCE_DIR}/CalcTangentsProcess.cpp
      ${ASSIMP_SOURCE_DIR}/CalcTangentsProcess.h
      ${ASSIMP_SOURCE_DIR}/ComputeUVMappingProcess.cpp
      ${ASSIMP_SOURCE_DIR}/ComputeUVMappingProcess.h
      ${ASSIMP_SOURCE_DIR}/ConvertToLHProcess.cpp
      ${ASSIMP_SOURCE_DIR}/ConvertToLHProcess.h
      ${ASSIMP_SOURCE_DIR}/FindDegenerates.cpp
      ${ASSIMP_SOURCE_DIR}/FindDegenerates.h
      ${ASSIMP_SOURCE_DIR}/FindInstancesProcess.cpp
      ${ASSIMP_SOURCE_DIR}/FindInstancesProcess.h
      ${ASSIMP_SOURCE_DIR}/FindInvalidDataProcess.cpp
      ${ASSIMP_SOURCE_DIR}/FindInvalidDataProcess.h
      ${ASSIMP_SOURCE_DIR}/FixNormalsStep.cpp
      ${ASSIMP_SOURCE_DIR}/FixNormalsStep.h
      ${ASSIMP_SOURCE_DIR}/GenFaceNormalsProcess.cpp
      ${ASSIMP_SOURCE_DIR}/GenFaceNormalsProcess.h
      ${ASSIMP_SOURCE_DIR}/GenVertexNormalsProcess.cpp
      ${ASSIMP_SOURCE_DIR}/GenVertexNormalsProcess.h
      ${ASSIMP_SOURCE_DIR}/PretransformVertices.cpp
      ${ASSIMP_SOURCE_DIR}/PretransformVertices.h
      ${ASSIMP_SOURCE_DIR}/ImproveCacheLocality.cpp
      ${ASSIMP_SOURCE_DIR}/ImproveCacheLocality.h
      ${ASSIMP_SOURCE_DIR}/JoinVerticesProcess.cpp
      ${ASSIMP_SOURCE_DIR}/JoinVerticesProcess.h
      ${ASSIMP_SOURCE_DIR}/LimitBoneWeightsProcess.cpp
      ${ASSIMP_SOURCE_DIR}/LimitBoneWeightsProcess.h
      ${ASSIMP_SOURCE_DIR}/RemoveRedundantMaterials.cpp
      ${ASSIMP_SOURCE_DIR}/RemoveRedundantMaterials.h
      ${ASSIMP_SOURCE_DIR}/RemoveVCProcess.cpp
      ${ASSIMP_SOURCE_DIR}/RemoveVCProcess.h
      ${ASSIMP_SOURCE_DIR}/SortByPTypeProcess.cpp
      ${ASSIMP_SOURCE_DIR}/SortByPTypeProcess.h
      ${ASSIMP_SOURCE_DIR}/SplitLargeMeshes.cpp
      ${ASSIMP_SOURCE_DIR}/SplitLargeMeshes.h
      ${ASSIMP_SOURCE_DIR}/TextureTransform.cpp
      ${ASSIMP_SOURCE_DIR}/TextureTransform.h
      ${ASSIMP_SOURCE_DIR}/TriangulateProcess.cpp
      ${ASSIMP_SOURCE_DIR}/TriangulateProcess.h
      ${ASSIMP_SOURCE_DIR}/ValidateDataStructure.cpp
      ${ASSIMP_SOURCE_DIR}/ValidateDataStructure.h
      ${ASSIMP_SOURCE_DIR}/OptimizeGraph.cpp
      ${ASSIMP_SOURCE_DIR}/OptimizeGraph.h
      ${ASSIMP_SOURCE_DIR}/OptimizeMeshes.cpp
      ${ASSIMP_SOURCE_DIR}/OptimizeMeshes.h
      ${ASSIMP_SOURCE_DIR}/DeboneProcess.cpp
      ${ASSIMP_SOURCE_DIR}/DeboneProcess.h
      ${ASSIMP_SOURCE_DIR}/ProcessHelper.h
      ${ASSIMP_SOURCE_DIR}/ProcessHelper.cpp
      ${ASSIMP_SOURCE_DIR}/PolyTools.h
      ${ASSIMP_SOURCE_DIR}/MakeVerboseFormat.cpp
      ${ASSIMP_SOURCE_DIR}/MakeVerboseFormat.h
   )

   if (BUILD_SHARED)
       message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
       add_library(${LIB_NAME} SHARED "")
   else()
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
      add_library(${LIB_NAME} STATIC "")
   endif()

   target_sources(${LIB_NAME} PUBLIC ${ASSIMP_HEADERS} PRIVATE ${ASSIMP_SOURCES})

   # Enables ai_assert()
   if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      target_compile_definitions(${LIB_NAME} PUBLIC -DASSIMP_BUILD_DEBUG=1)
   endif()

   # IFC specific flags
   if (CMAKE_COMPILER_IS_MINGW)
      set_source_files_properties(
         ${ASSIMP_SOURCE_DIR}/IFCReaderGen1.cpp
         ${ASSIMP_SOURCE_DIR}/IFCReaderGen2.cpp
         PROPERTIES COMPILE_FLAGS "-Wa,-mbig-obj")
   endif()

   # if you like to use these libs (e.g. de_clipper) your own, then
   # just link to them directly, dont expect this de_assimp lib to publicly offer the other libs.
   # That would increase parser times uneccessarily and would flood root namespace with C-symbols.
   target_link_libraries(${LIB_NAME} PRIVATE de_rapidjson de_utf8cpp de_clipper de_poly2tri)
   target_link_libraries(${LIB_NAME} PRIVATE de_unzip de_minizip de_z)
   target_link_libraries(${LIB_NAME} PRIVATE de_open3dgc de_openddlparser de_irrXML)

   target_include_directories(${LIB_NAME} PUBLIC ${ASSIMP_HEADER_DIR}/..)
   target_include_directories(${LIB_NAME} PRIVATE ${ASSIMP_HEADER_DIR} ${ASSIMP_SOURCE_DIR})

#   file(COPY ${ASSIMP_HEADERS} DESTINATION ${ASSIMP_INCLUDE_DIR}) # For the SDK
#   file(COPY ${ASSIMP_HEADERS_II} DESTINATION ${ASSIMP_INCLUDE_DIR}/Compiler) # For the SDK

endif()


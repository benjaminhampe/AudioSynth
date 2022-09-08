set(LIB_NAME de_irrlicht)
project(lib-${LIB_NAME}-1.8.5-dev)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   set(IRR_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
   set(IRR_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

   file(GLOB_RECURSE IRR_HEADERS "${IRR_HEADER_DIR}/*.h")

   list(APPEND IRR_HEADERS "${IRR_HEADER_DIR}/de_irrlicht.hpp")

   # file(GLOB_RECURSE IRR_SOURCES "${IRR_SOURCE_DIR}/*.cpp")

   set(IRR_SOURCES_EXTERN
      # links with de_z, commented out because of multiple definitions.
#      ${IRR_SOURCE_DIR}/zlib/adler32.c
#      ${IRR_SOURCE_DIR}/zlib/compress.c
#      ${IRR_SOURCE_DIR}/zlib/crc32.c
#      ${IRR_SOURCE_DIR}/zlib/deflate.c
#      ${IRR_SOURCE_DIR}/zlib/gzclose.c
#      ${IRR_SOURCE_DIR}/zlib/gzlib.c
#      ${IRR_SOURCE_DIR}/zlib/gzread.c
#      ${IRR_SOURCE_DIR}/zlib/gzwrite.c
#      ${IRR_SOURCE_DIR}/zlib/infback.c
#      ${IRR_SOURCE_DIR}/zlib/inffast.c
#      ${IRR_SOURCE_DIR}/zlib/inflate.c
#      ${IRR_SOURCE_DIR}/zlib/inftrees.c
#      ${IRR_SOURCE_DIR}/zlib/trees.c
#      ${IRR_SOURCE_DIR}/zlib/uncompr.c
#      ${IRR_SOURCE_DIR}/zlib/zutil.c
      # links with de_bzip2, commented out because of multiple definitions.
#      ${IRR_SOURCE_DIR}/bzip2/blocksort.c
#      ${IRR_SOURCE_DIR}/bzip2/bzcompress.c
#      ${IRR_SOURCE_DIR}/bzip2/bzlib.c
#      ${IRR_SOURCE_DIR}/bzip2/crctable.c
#      ${IRR_SOURCE_DIR}/bzip2/decompress.c
#      ${IRR_SOURCE_DIR}/bzip2/huffman.c
#      ${IRR_SOURCE_DIR}/bzip2/randtable.c
      #####
      # ${IRR_SOURCE_DIR}/bzip2/bzip2.c
      # ${IRR_SOURCE_DIR}/bzip2/bzip2recover.c
      # ${IRR_SOURCE_DIR}/bzip2/dlltest.c # main()
      # ${IRR_SOURCE_DIR}/bzip2/mk251.c
      # ${IRR_SOURCE_DIR}/bzip2/spewG.c
      # ${IRR_SOURCE_DIR}/bzip2/unzcrash.c
      ${IRR_SOURCE_DIR}/jpeglib/cdjpeg.c
      # ${IRR_SOURCE_DIR}/jpeglib/cjpeg.c
      # ${IRR_SOURCE_DIR}/jpeglib/ckconfig.c
      # ${IRR_SOURCE_DIR}/jpeglib/djpeg.c
      ${IRR_SOURCE_DIR}/jpeglib/jaricom.c
      ${IRR_SOURCE_DIR}/jpeglib/jcapimin.c
      ${IRR_SOURCE_DIR}/jpeglib/jcapistd.c
      ${IRR_SOURCE_DIR}/jpeglib/jcarith.c
      ${IRR_SOURCE_DIR}/jpeglib/jccoefct.c
      ${IRR_SOURCE_DIR}/jpeglib/jccolor.c
      ${IRR_SOURCE_DIR}/jpeglib/jcdctmgr.c
      ${IRR_SOURCE_DIR}/jpeglib/jchuff.c
      ${IRR_SOURCE_DIR}/jpeglib/jcinit.c
      ${IRR_SOURCE_DIR}/jpeglib/jcmainct.c
      ${IRR_SOURCE_DIR}/jpeglib/jcmarker.c
      ${IRR_SOURCE_DIR}/jpeglib/jcmaster.c
      ${IRR_SOURCE_DIR}/jpeglib/jcomapi.c
      ${IRR_SOURCE_DIR}/jpeglib/jcparam.c
      ${IRR_SOURCE_DIR}/jpeglib/jcprepct.c
      ${IRR_SOURCE_DIR}/jpeglib/jcsample.c
      ${IRR_SOURCE_DIR}/jpeglib/jctrans.c
      ${IRR_SOURCE_DIR}/jpeglib/jdapimin.c
      ${IRR_SOURCE_DIR}/jpeglib/jdapistd.c
      ${IRR_SOURCE_DIR}/jpeglib/jdarith.c
      ${IRR_SOURCE_DIR}/jpeglib/jdatadst.c
      ${IRR_SOURCE_DIR}/jpeglib/jdatasrc.c
      ${IRR_SOURCE_DIR}/jpeglib/jdcoefct.c
      ${IRR_SOURCE_DIR}/jpeglib/jdcolor.c
      ${IRR_SOURCE_DIR}/jpeglib/jddctmgr.c
      ${IRR_SOURCE_DIR}/jpeglib/jdhuff.c
      ${IRR_SOURCE_DIR}/jpeglib/jdinput.c
      ${IRR_SOURCE_DIR}/jpeglib/jdmainct.c
      ${IRR_SOURCE_DIR}/jpeglib/jdmarker.c
      ${IRR_SOURCE_DIR}/jpeglib/jdmaster.c
      ${IRR_SOURCE_DIR}/jpeglib/jdmerge.c
      ${IRR_SOURCE_DIR}/jpeglib/jdpostct.c
      ${IRR_SOURCE_DIR}/jpeglib/jdsample.c
      ${IRR_SOURCE_DIR}/jpeglib/jdtrans.c
      ${IRR_SOURCE_DIR}/jpeglib/jerror.c
      ${IRR_SOURCE_DIR}/jpeglib/jfdctflt.c
      ${IRR_SOURCE_DIR}/jpeglib/jfdctfst.c
      ${IRR_SOURCE_DIR}/jpeglib/jfdctint.c
      ${IRR_SOURCE_DIR}/jpeglib/jidctflt.c
      ${IRR_SOURCE_DIR}/jpeglib/jidctfst.c
      ${IRR_SOURCE_DIR}/jpeglib/jidctint.c
      ${IRR_SOURCE_DIR}/jpeglib/jinclude.h
      ${IRR_SOURCE_DIR}/jpeglib/jmemansi.c
      # ${IRR_SOURCE_DIR}/jpeglib/jmemdos.c
      # ${IRR_SOURCE_DIR}/jpeglib/jmemmac.c
      ${IRR_SOURCE_DIR}/jpeglib/jmemmgr.c
      ${IRR_SOURCE_DIR}/jpeglib/jmemname.c
      # ${IRR_SOURCE_DIR}/jpeglib/jmemnobs.c
      # ${IRR_SOURCE_DIR}/jpeglib/jpegtran.c
      ${IRR_SOURCE_DIR}/jpeglib/jquant1.c
      ${IRR_SOURCE_DIR}/jpeglib/jquant2.c
      ${IRR_SOURCE_DIR}/jpeglib/jutils.c
      ${IRR_SOURCE_DIR}/jpeglib/rdbmp.c
      ${IRR_SOURCE_DIR}/jpeglib/rdcolmap.c
      ${IRR_SOURCE_DIR}/jpeglib/rdgif.c
      # ${IRR_SOURCE_DIR}/jpeglib/rdjpgcom.c
      ${IRR_SOURCE_DIR}/jpeglib/rdppm.c
      ${IRR_SOURCE_DIR}/jpeglib/rdrle.c
      ${IRR_SOURCE_DIR}/jpeglib/rdswitch.c
      ${IRR_SOURCE_DIR}/jpeglib/rdtarga.c
      ${IRR_SOURCE_DIR}/jpeglib/transupp.c
      ${IRR_SOURCE_DIR}/jpeglib/wrbmp.c
      ${IRR_SOURCE_DIR}/jpeglib/wrgif.c
      # ${IRR_SOURCE_DIR}/jpeglib/wrjpgcom.c
      ${IRR_SOURCE_DIR}/jpeglib/wrppm.c
      ${IRR_SOURCE_DIR}/jpeglib/wrrle.c
      ${IRR_SOURCE_DIR}/jpeglib/wrtarga.c

   # MacOSX/
      ${IRR_SOURCE_DIR}/libpng/example.c
      ${IRR_SOURCE_DIR}/libpng/png.c
      ${IRR_SOURCE_DIR}/libpng/pngerror.c
      ${IRR_SOURCE_DIR}/libpng/pngget.c
      ${IRR_SOURCE_DIR}/libpng/pngmem.c
      ${IRR_SOURCE_DIR}/libpng/pngpread.c
      ${IRR_SOURCE_DIR}/libpng/pngread.c
      ${IRR_SOURCE_DIR}/libpng/pngrio.c
      ${IRR_SOURCE_DIR}/libpng/pngrtran.c
      ${IRR_SOURCE_DIR}/libpng/pngrutil.c
      ${IRR_SOURCE_DIR}/libpng/pngset.c
      ${IRR_SOURCE_DIR}/libpng/pngtest.c
      ${IRR_SOURCE_DIR}/libpng/pngtrans.c
      ${IRR_SOURCE_DIR}/libpng/pngwio.c
      ${IRR_SOURCE_DIR}/libpng/pngwrite.c
      ${IRR_SOURCE_DIR}/libpng/pngwtran.c
      ${IRR_SOURCE_DIR}/libpng/pngwutil.c
   )

   set(IRR_SOURCES
      ${IRR_SOURCE_DIR}/C3DSMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CAnimatedMeshHalfLife.cpp
      ${IRR_SOURCE_DIR}/CAnimatedMeshMD2.cpp
      ${IRR_SOURCE_DIR}/CAnimatedMeshMD3.cpp
      ${IRR_SOURCE_DIR}/CAnimatedMeshSceneNode.cpp
      ${IRR_SOURCE_DIR}/CAttributes.cpp
      ${IRR_SOURCE_DIR}/CB3DMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CBillboardSceneNode.cpp
      ${IRR_SOURCE_DIR}/CBoneSceneNode.cpp
      ${IRR_SOURCE_DIR}/CBSPMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CBurningShader_Raster_Reference.cpp
      ${IRR_SOURCE_DIR}/CCameraSceneNode.cpp
      ${IRR_SOURCE_DIR}/CCgMaterialRenderer.cpp
      ${IRR_SOURCE_DIR}/CColladaFileLoader.cpp
      ${IRR_SOURCE_DIR}/CColladaMeshWriter.cpp
      ${IRR_SOURCE_DIR}/CColorConverter.cpp
      ${IRR_SOURCE_DIR}/CCSMLoader.cpp
      ${IRR_SOURCE_DIR}/CCubeSceneNode.cpp
      ${IRR_SOURCE_DIR}/CD3D8Driver.cpp
      ${IRR_SOURCE_DIR}/CD3D8NormalMapRenderer.cpp
      ${IRR_SOURCE_DIR}/CD3D8ParallaxMapRenderer.cpp
      ${IRR_SOURCE_DIR}/CD3D8ShaderMaterialRenderer.cpp
      ${IRR_SOURCE_DIR}/CD3D8Texture.cpp
      ${IRR_SOURCE_DIR}/CD3D9CgMaterialRenderer.cpp
      ${IRR_SOURCE_DIR}/CD3D9Driver.cpp
      ${IRR_SOURCE_DIR}/CD3D9HLSLMaterialRenderer.cpp
      ${IRR_SOURCE_DIR}/CD3D9NormalMapRenderer.cpp
      ${IRR_SOURCE_DIR}/CD3D9ParallaxMapRenderer.cpp
      ${IRR_SOURCE_DIR}/CD3D9ShaderMaterialRenderer.cpp
      ${IRR_SOURCE_DIR}/CD3D9Texture.cpp
      ${IRR_SOURCE_DIR}/CDefaultGUIElementFactory.cpp
      ${IRR_SOURCE_DIR}/CDefaultSceneNodeAnimatorFactory.cpp
      ${IRR_SOURCE_DIR}/CDefaultSceneNodeFactory.cpp
      ${IRR_SOURCE_DIR}/CDepthBuffer.cpp
      ${IRR_SOURCE_DIR}/CDMFLoader.cpp
      ${IRR_SOURCE_DIR}/CDummyTransformationSceneNode.cpp
      ${IRR_SOURCE_DIR}/CEmptySceneNode.cpp
      ${IRR_SOURCE_DIR}/CFileList.cpp
      ${IRR_SOURCE_DIR}/CFileSystem.cpp
      ${IRR_SOURCE_DIR}/CFPSCounter.cpp
      ${IRR_SOURCE_DIR}/CGeometryCreator.cpp
      ${IRR_SOURCE_DIR}/CGUIButton.cpp
      ${IRR_SOURCE_DIR}/CGUICheckBox.cpp
      ${IRR_SOURCE_DIR}/CGUIColorSelectDialog.cpp
      ${IRR_SOURCE_DIR}/CGUIComboBox.cpp
      ${IRR_SOURCE_DIR}/CGUIContextMenu.cpp
      ${IRR_SOURCE_DIR}/CGUIEditBox.cpp
      ${IRR_SOURCE_DIR}/CGUIEnvironment.cpp
      ${IRR_SOURCE_DIR}/CGUIFileOpenDialog.cpp
      ${IRR_SOURCE_DIR}/CGUIFont.cpp
      ${IRR_SOURCE_DIR}/CGUIImage.cpp
      ${IRR_SOURCE_DIR}/CGUIImageList.cpp
      ${IRR_SOURCE_DIR}/CGUIInOutFader.cpp
      ${IRR_SOURCE_DIR}/CGUIListBox.cpp
      ${IRR_SOURCE_DIR}/CGUIMenu.cpp
      ${IRR_SOURCE_DIR}/CGUIMeshViewer.cpp
      ${IRR_SOURCE_DIR}/CGUIMessageBox.cpp
      ${IRR_SOURCE_DIR}/CGUIModalScreen.cpp
      ${IRR_SOURCE_DIR}/CGUIScrollBar.cpp
      ${IRR_SOURCE_DIR}/CGUISkin.cpp
      ${IRR_SOURCE_DIR}/CGUISpinBox.cpp
      ${IRR_SOURCE_DIR}/CGUISpriteBank.cpp
      ${IRR_SOURCE_DIR}/CGUIStaticText.cpp
      ${IRR_SOURCE_DIR}/CGUITabControl.cpp
      ${IRR_SOURCE_DIR}/CGUITable.cpp
      ${IRR_SOURCE_DIR}/CGUIToolBar.cpp
      ${IRR_SOURCE_DIR}/CGUITreeView.cpp
      ${IRR_SOURCE_DIR}/CGUIWindow.cpp
      ${IRR_SOURCE_DIR}/CImage.cpp
      ${IRR_SOURCE_DIR}/CImageLoaderBMP.cpp
      ${IRR_SOURCE_DIR}/CImageLoaderDDS.cpp
      ${IRR_SOURCE_DIR}/CImageLoaderJPG.cpp
      ${IRR_SOURCE_DIR}/CImageLoaderPCX.cpp
      ${IRR_SOURCE_DIR}/CImageLoaderPNG.cpp
      ${IRR_SOURCE_DIR}/CImageLoaderPPM.cpp
      ${IRR_SOURCE_DIR}/CImageLoaderPSD.cpp
      ${IRR_SOURCE_DIR}/CImageLoaderRGB.cpp
      ${IRR_SOURCE_DIR}/CImageLoaderTGA.cpp
      ${IRR_SOURCE_DIR}/CImageLoaderWAL.cpp
      ${IRR_SOURCE_DIR}/CImageWriterBMP.cpp
      ${IRR_SOURCE_DIR}/CImageWriterJPG.cpp
      ${IRR_SOURCE_DIR}/CImageWriterPCX.cpp
      ${IRR_SOURCE_DIR}/CImageWriterPNG.cpp
      ${IRR_SOURCE_DIR}/CImageWriterPPM.cpp
      ${IRR_SOURCE_DIR}/CImageWriterPSD.cpp
      ${IRR_SOURCE_DIR}/CImageWriterTGA.cpp
      ${IRR_SOURCE_DIR}/CIrrDeviceConsole.cpp
      ${IRR_SOURCE_DIR}/CIrrDeviceFB.cpp
      ${IRR_SOURCE_DIR}/CIrrDeviceLinux.cpp
      ${IRR_SOURCE_DIR}/CIrrDeviceSDL.cpp
      ${IRR_SOURCE_DIR}/CIrrDeviceStub.cpp
      ${IRR_SOURCE_DIR}/CIrrDeviceWin32.cpp
      ${IRR_SOURCE_DIR}/CIrrDeviceWinCE.cpp
      ${IRR_SOURCE_DIR}/CIrrMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CIrrMeshWriter.cpp
      ${IRR_SOURCE_DIR}/CLightSceneNode.cpp
      ${IRR_SOURCE_DIR}/CLimitReadFile.cpp
      ${IRR_SOURCE_DIR}/CLMTSMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CLogger.cpp
      ${IRR_SOURCE_DIR}/CLWOMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CMD2MeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CMD3MeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CMemoryFile.cpp
      ${IRR_SOURCE_DIR}/CMeshCache.cpp
      ${IRR_SOURCE_DIR}/CMeshManipulator.cpp
      ${IRR_SOURCE_DIR}/CMeshSceneNode.cpp
      ${IRR_SOURCE_DIR}/CMetaTriangleSelector.cpp
      ${IRR_SOURCE_DIR}/CMountPointReader.cpp
      ${IRR_SOURCE_DIR}/CMS3DMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CMY3DMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CNPKReader.cpp
      ${IRR_SOURCE_DIR}/CNullDriver.cpp
      ${IRR_SOURCE_DIR}/COBJMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/COBJMeshWriter.cpp
      ${IRR_SOURCE_DIR}/COCTLoader.cpp
      ${IRR_SOURCE_DIR}/COctreeSceneNode.cpp
      ${IRR_SOURCE_DIR}/COctreeTriangleSelector.cpp
      ${IRR_SOURCE_DIR}/COgreMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/COpenGLCgMaterialRenderer.cpp
      ${IRR_SOURCE_DIR}/COpenGLDriver.cpp
      ${IRR_SOURCE_DIR}/COpenGLExtensionHandler.cpp
      ${IRR_SOURCE_DIR}/COpenGLNormalMapRenderer.cpp
      ${IRR_SOURCE_DIR}/COpenGLParallaxMapRenderer.cpp
      ${IRR_SOURCE_DIR}/COpenGLShaderMaterialRenderer.cpp
      ${IRR_SOURCE_DIR}/COpenGLSLMaterialRenderer.cpp
      ${IRR_SOURCE_DIR}/COpenGLTexture.cpp
      ${IRR_SOURCE_DIR}/COSOperator.cpp
      ${IRR_SOURCE_DIR}/CPakReader.cpp
      ${IRR_SOURCE_DIR}/CParticleAnimatedMeshSceneNodeEmitter.cpp
      ${IRR_SOURCE_DIR}/CParticleAttractionAffector.cpp
      ${IRR_SOURCE_DIR}/CParticleBoxEmitter.cpp
      ${IRR_SOURCE_DIR}/CParticleCylinderEmitter.cpp
      ${IRR_SOURCE_DIR}/CParticleFadeOutAffector.cpp
      ${IRR_SOURCE_DIR}/CParticleGravityAffector.cpp
      ${IRR_SOURCE_DIR}/CParticleMeshEmitter.cpp
      ${IRR_SOURCE_DIR}/CParticlePointEmitter.cpp
      ${IRR_SOURCE_DIR}/CParticleRingEmitter.cpp
      ${IRR_SOURCE_DIR}/CParticleRotationAffector.cpp
      ${IRR_SOURCE_DIR}/CParticleScaleAffector.cpp
      ${IRR_SOURCE_DIR}/CParticleSphereEmitter.cpp
      ${IRR_SOURCE_DIR}/CParticleSystemSceneNode.cpp
      ${IRR_SOURCE_DIR}/CPLYMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CPLYMeshWriter.cpp
      ${IRR_SOURCE_DIR}/CQ3LevelMesh.cpp
      ${IRR_SOURCE_DIR}/CQuake3ShaderSceneNode.cpp
      ${IRR_SOURCE_DIR}/CReadFile.cpp
      ${IRR_SOURCE_DIR}/CSceneCollisionManager.cpp
      ${IRR_SOURCE_DIR}/CSceneLoaderIrr.cpp
      ${IRR_SOURCE_DIR}/CSceneManager.cpp
      ${IRR_SOURCE_DIR}/CSceneNodeAnimatorCameraFPS.cpp
      ${IRR_SOURCE_DIR}/CSceneNodeAnimatorCameraMaya.cpp
      ${IRR_SOURCE_DIR}/CSceneNodeAnimatorCollisionResponse.cpp
      ${IRR_SOURCE_DIR}/CSceneNodeAnimatorDelete.cpp
      ${IRR_SOURCE_DIR}/CSceneNodeAnimatorFlyCircle.cpp
      ${IRR_SOURCE_DIR}/CSceneNodeAnimatorFlyStraight.cpp
      ${IRR_SOURCE_DIR}/CSceneNodeAnimatorFollowSpline.cpp
      ${IRR_SOURCE_DIR}/CSceneNodeAnimatorRotation.cpp
      ${IRR_SOURCE_DIR}/CSceneNodeAnimatorTexture.cpp
      ${IRR_SOURCE_DIR}/CShadowVolumeSceneNode.cpp
      ${IRR_SOURCE_DIR}/CSkinnedMesh.cpp
      ${IRR_SOURCE_DIR}/CSkyBoxSceneNode.cpp
      ${IRR_SOURCE_DIR}/CSkyDomeSceneNode.cpp
      ${IRR_SOURCE_DIR}/CSMFMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CSoftwareDriver.cpp
      ${IRR_SOURCE_DIR}/CSoftwareDriver2.cpp
      ${IRR_SOURCE_DIR}/CSoftwareTexture.cpp
      ${IRR_SOURCE_DIR}/CSoftwareTexture2.cpp
      ${IRR_SOURCE_DIR}/CSphereSceneNode.cpp
      ${IRR_SOURCE_DIR}/CSTLMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CSTLMeshWriter.cpp
      ${IRR_SOURCE_DIR}/CTarReader.cpp
      ${IRR_SOURCE_DIR}/CTerrainSceneNode.cpp
      ${IRR_SOURCE_DIR}/CTerrainTriangleSelector.cpp
      ${IRR_SOURCE_DIR}/CTextSceneNode.cpp
      ${IRR_SOURCE_DIR}/CTRFlat.cpp
      ${IRR_SOURCE_DIR}/CTRFlatWire.cpp
      ${IRR_SOURCE_DIR}/CTRGouraud.cpp
      ${IRR_SOURCE_DIR}/CTRGouraud2.cpp
      ${IRR_SOURCE_DIR}/CTRGouraudAlpha2.cpp
      ${IRR_SOURCE_DIR}/CTRGouraudAlphaNoZ2.cpp
      ${IRR_SOURCE_DIR}/CTRGouraudWire.cpp
      ${IRR_SOURCE_DIR}/CTriangleBBSelector.cpp
      ${IRR_SOURCE_DIR}/CTriangleBBSelector.h
      ${IRR_SOURCE_DIR}/CTriangleSelector.cpp
      ${IRR_SOURCE_DIR}/CTriangleSelector.h
      ${IRR_SOURCE_DIR}/CTRNormalMap.cpp
      ${IRR_SOURCE_DIR}/CTRStencilShadow.cpp
      ${IRR_SOURCE_DIR}/CTRTextureBlend.cpp
      ${IRR_SOURCE_DIR}/CTRTextureDetailMap2.cpp
      ${IRR_SOURCE_DIR}/CTRTextureFlat.cpp
      ${IRR_SOURCE_DIR}/CTRTextureFlatWire.cpp
      ${IRR_SOURCE_DIR}/CTRTextureGouraud.cpp
      ${IRR_SOURCE_DIR}/CTRTextureGouraud.h
      ${IRR_SOURCE_DIR}/CTRTextureGouraud2.cpp
      ${IRR_SOURCE_DIR}/CTRTextureGouraudAdd.cpp
      ${IRR_SOURCE_DIR}/CTRTextureGouraudAdd2.cpp
      ${IRR_SOURCE_DIR}/CTRTextureGouraudAddNoZ2.cpp
      ${IRR_SOURCE_DIR}/CTRTextureGouraudAlpha.cpp
      ${IRR_SOURCE_DIR}/CTRTextureGouraudAlphaNoZ.cpp
      ${IRR_SOURCE_DIR}/CTRTextureGouraudNoZ.cpp
      ${IRR_SOURCE_DIR}/CTRTextureGouraudNoZ2.cpp
      ${IRR_SOURCE_DIR}/CTRTextureGouraudVertexAlpha2.cpp
      ${IRR_SOURCE_DIR}/CTRTextureGouraudWire.cpp
      ${IRR_SOURCE_DIR}/CTRTextureLightMap2_Add.cpp
      ${IRR_SOURCE_DIR}/CTRTextureLightMap2_M1.cpp
      ${IRR_SOURCE_DIR}/CTRTextureLightMap2_M2.cpp
      ${IRR_SOURCE_DIR}/CTRTextureLightMap2_M4.cpp
      ${IRR_SOURCE_DIR}/CTRTextureLightMapGouraud2_M4.cpp
      ${IRR_SOURCE_DIR}/CTRTextureWire2.cpp
      ${IRR_SOURCE_DIR}/CVideoModeList.cpp
      ${IRR_SOURCE_DIR}/CVolumeLightSceneNode.cpp
      ${IRR_SOURCE_DIR}/CWADReader.cpp
      ${IRR_SOURCE_DIR}/CWaterSurfaceSceneNode.cpp
      ${IRR_SOURCE_DIR}/CWriteFile.cpp
      ${IRR_SOURCE_DIR}/CXMeshFileLoader.cpp
      ${IRR_SOURCE_DIR}/CXMLReader.cpp
      ${IRR_SOURCE_DIR}/CXMLWriter.cpp
      ${IRR_SOURCE_DIR}/CZBuffer.cpp
      ${IRR_SOURCE_DIR}/CZipReader.cpp
      ${IRR_SOURCE_DIR}/IBurningShader.cpp
      ${IRR_SOURCE_DIR}/Irrlicht.cpp
      ${IRR_SOURCE_DIR}/irrXML.cpp
      ${IRR_SOURCE_DIR}/os.cpp

      ${IRR_SOURCE_DIR}/aesGladman/pwd2key.cpp
      ${IRR_SOURCE_DIR}/aesGladman/aescrypt.cpp
      ${IRR_SOURCE_DIR}/aesGladman/aeskey.cpp
      ${IRR_SOURCE_DIR}/aesGladman/aestab.cpp
      ${IRR_SOURCE_DIR}/aesGladman/fileenc.cpp
      ${IRR_SOURCE_DIR}/aesGladman/hmac.cpp
      ${IRR_SOURCE_DIR}/aesGladman/prng.cpp
      ${IRR_SOURCE_DIR}/aesGladman/sha1.cpp
      ${IRR_SOURCE_DIR}/aesGladman/sha2.cpp

      ${IRR_SOURCE_DIR}/lzma/LzmaDec.c
   )

   if (BUILD_SHARED)
      add_library(${LIB_NAME} SHARED "")
      message("${BoldBlue}[benni] SHARED_LIB: ${LIB_NAME}${ColourReset}")
   else()
      add_library(${LIB_NAME} STATIC "")
      message("${BoldBlue}[benni] STATIC_LIB: ${LIB_NAME}${ColourReset}")
   endif()

   target_compile_definitions(${LIB_NAME} PUBLIC -DUSE_IRRLICHT=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -D_IRR_STATIC_LIB_=1)

   target_include_directories(${LIB_NAME} PUBLIC  ${IRR_HEADER_DIR})
   target_include_directories(${LIB_NAME} PUBLIC ${IRR_HEADER_DIR}/Irrlicht)
   target_include_directories(${LIB_NAME} PRIVATE ${IRR_SOURCE_DIR})
   target_sources(${LIB_NAME} PUBLIC ${IRR_HEADERS} PRIVATE ${IRR_SOURCES})
   target_link_libraries(${LIB_NAME} PUBLIC stdc++fs)
   target_link_libraries(${LIB_NAME} PUBLIC pthread)
   target_link_libraries(${LIB_NAME} PUBLIC m)

   if(WIN32)
      target_link_libraries(${LIB_NAME} PUBLIC glu32 opengl32
               uuid ole32 oleaut32 ComCtl32
               SetupAPI winmm user32 gdi32 kernel32 advapi32 shell32 )
   else() # on Linux:
      target_link_libraries(${LIB_NAME} PUBLIC GLU GL)
      target_link_libraries(${LIB_NAME} PRIVATE X11 Xxf86vm Xi Xcursor)
   endif()

   target_link_libraries(${LIB_NAME} PUBLIC de_bzip2)
   target_link_libraries(${LIB_NAME} PUBLIC de_z)
   target_link_libraries(${LIB_NAME} PUBLIC de_png)
   target_link_libraries(${LIB_NAME} PUBLIC de_jpeg8d)
#   target_link_libraries(${LIB_NAME} PUBLIC de_lzma)
#   target_link_libraries(${LIB_NAME} PUBLIC de_aesgladman)

#   target_link_libraries(${LIB_NAME} PUBLIC de_tiff4)
#   target_link_libraries(${LIB_NAME} PUBLIC de_freetype2)
#   target_link_libraries(${LIB_NAME} PUBLIC de_opengles)
#   target_link_libraries(${LIB_NAME} PUBLIC de_fontawesome)
#   target_link_libraries(${LIB_NAME} PUBLIC de_assimp)

endif()

add_subdirectory(test)

if (BENNI_BUILD_NASM)
   add_subdirectory(tools)
else()
   #add_subdirectory(tests)
   add_subdirectory(_libs)
   #add_subdirectory(_libs/_libsDarkImage)
   #add_subdirectory(_libs/_libsDarkAudio)
   add_subdirectory(libDarkImage)
   add_subdirectory(BenMath)
   add_subdirectory(convert2mp3)
#   add_subdirectory(Hypnotoad)
#   add_subdirectory(HexEdit-qt)
   add_subdirectory(MidiParser)
   add_subdirectory(fontview-qt)
#   add_subdirectory(AbletonLivePluginScanner-qt)

   #add_subdirectory(AbletonLive64-PluginScanner-qt)
   add_subdirectory(AbletonLive64-Lite-qt)

   # add_subdirectory(F:/AbletonLive64-Lite-qt) binary directory must be explicitly specified

   add_subdirectory(minimal_vst2x_host)
   add_subdirectory(captureScreen)
   
   #add_subdirectory(AudioRecorder-qt)

# NOT good in Debugmode
   # add_subdirectory(_libs/_libsDarkGPU)
   add_subdirectory(de-id3v2)
   add_subdirectory(libDarkGPU)
   add_subdirectory(AudioSynth-qt)


   # add_subdirectory(libDarkAudio)

   #add_subdirectory(brickVST) # 1st de_juce project
   # add_subdirectory(AbletonLive-qt5)
   #add_subdirectory(AudioLordVaDAW)
   #add_subdirectory(AudioLordVaDAW)
endif()

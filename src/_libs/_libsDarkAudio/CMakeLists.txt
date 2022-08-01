add_subdirectory(de-asiosdk-2.3) # Steinberg ASIO driver ( Win/Mac )
add_subdirectory(de-rtaudio-5.1.0)     # Audio Engine, works really well in AudioSynth
add_subdirectory(de-rtmidi)            # MIDI Engine ( No virtual MidiPorts in Windows wtf? )
add_subdirectory(de-kissfft)           # fft, used atleast in AudioSynth
add_subdirectory(de-fftw3-static)             # fft, not easy to use and bad license? Candidate for rejection.
add_subdirectory(de-soundtouch-1.9.2)                 # Works really well in AudioSynth
add_subdirectory(_libsndfile)                         # does it encode flac and ogg?
add_subdirectory(de-lame-3.100)                       # for encoding MP3 files only, depends on: sndfile, mpg123
add_subdirectory(de-ffmpeg-4.3.2-win64-gcc-shared)    # for decoding any important Audio/Video multimedia container.
#add_subdirectory(de-encoder-mp3-twolame-0.4.0)       # depends on: sndfile, mpg123
add_subdirectory(de-vstsdk3610_11_06_2018_build_37)   # VST plugin programming and hosting
#add_subdirectory(de-juce-1.51)

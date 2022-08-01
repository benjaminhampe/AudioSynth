################################
# SNDFILE <- depends on libs: ogg, flac, opus, theora, vorbis
################################
add_subdirectory(de-ogg-1.3.4)
add_subdirectory(de-opus-1.3.1)
add_subdirectory(de-theora-1.1.1)     # depends on: ogg.
add_subdirectory(de-vorbis-1.3.6)     # depends on: ogg.
add_subdirectory(de-flac-1.3.3)       # depends on: ogg.
add_subdirectory(de-sndfile-1.0.28)   # depends on: ogg, flac.

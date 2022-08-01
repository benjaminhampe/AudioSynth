### ========== DarkImage : private libs ( optional, less compile time ) ==========
add_subdirectory(de-bzip2)
add_subdirectory(de-giflib-5.2.1)
add_subdirectory(de-jpeg-8d)
add_subdirectory(de-lzma-4.32.7)
add_subdirectory(de-png-1.6.23)
add_subdirectory(de-tiff4)
add_subdirectory(de-z-1.2.8)

add_subdirectory(de-freetype-2.10.0)
add_subdirectory(de-minizip)
add_subdirectory(de-unzip)

### ========== DarkGeometry ==========
# add_subdirectory(de-fltk-1.3.5)           # gui
# add_subdirectory(de-glfw-3.3.2)           # Like GLUT
# add_subdirectory(de-irrlicht-1.8.5)       # 3d engine, gui, input, joystick
# add_subdirectory(de-irrExt-1.8.5)         # benni addons utf8 + freetype rendering + glm
# add_subdirectory(de-wx-3.1.5)
# add_subdirectory(de-boost_1_75_0-static)
# add_subdirectory(de-gmp-6.1.2-static)     # bigint
# add_subdirectory(de-mpfr-4.0.2-static)    # bigint
# add_subdirectory(de-CGAL-5.0.2)           # CSG constructive solid geometry
#add_subdirectory(de-concaveman)           # concaveman.h:212:43: error: call of overloaded 'make_unique<rtree<double, 2, 16, std::array<double, 2> >::TreeType>(rtree<double, 2, 16, std::array<double, 2> >::DataType&, const bounds_type&)' is ambiguous
# add_subdirectory(de-earcut)
# add_subdirectory(de-expat-2.2.9)          # XML lib
# add_subdirectory(de-cork)                 # CSG booleans, #include <mpir.h>? uses GMP?
# add_subdirectory(de-ginac)               # Ti89 CAS,  fatal error: cln/complex.h: No such file or directory
# add_subdirectory(de-sqlite3)            # Can i exclude the big file from Qt parsing? ( to make loading faster )


# ========== DarkGeometry ==========

#add_subdirectory(de-boost_1_75_0-static)

#add_subdirectory(de-gmp-6.1.2-static)     # bigint
#add_subdirectory(de-mpfr-4.0.2-static)    # bigint
#add_subdirectory(de-CGAL-5.0.2)           # CSG constructive solid geometry
#add_subdirectory(de-expat-2.2.9)          # XML lib
#add_subdirectory(de-concaveman)           # concaveman.h:212:43: error: call of overloaded 'make_unique<rtree<double, 2, 16, std::array<double, 2> >::TreeType>(rtree<double, 2, 16, std::array<double, 2> >::DataType&, const bounds_type&)' is ambiguous
#add_subdirectory(de-earcut)

# add_subdirectory(de-cork)                 # CSG booleans, #include <mpir.h>? uses GMP?
# add_subdirectory(de-ginac)               # Ti89 CAS,  fatal error: cln/complex.h: No such file or directory

add_subdirectory(de-gmp-6.1.2)      # Cork
add_subdirectory(de-cork)           # Cork

### ========== DarkDVD ==========
# add_subdirectory(de-dvdcss)
# add_subdirectory(de-dvdread)
# add_subdirectory(de-dvdnav)

# ========== DarkSQL ==========

# add_subdirectory(de-sqlite3)            # Can i exclude the big file from Qt parsing? ( to make loading faster )

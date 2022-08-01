#include <de/FileSystem.hpp>
#include <codecvt>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <fstream>

// STL C++17
// #include <filesystem>
// namespace fs = std::filesystem;

// GCC 7.3.0 needs extra link with -lstdc++fs
#include <experimental/filesystem> // I use GCC 7.3.0 and link with -lstdc++fs
namespace { namespace fs = std::experimental::filesystem; }

// Boost
// #include <boost/filesystem.hpp>
// namespace fs = boost::filesystem;

namespace de {

std::string
FileSystem::loadText( std::string const & uri )
{
   std::ifstream fin( uri.c_str() );
   std::stringstream s;
   if ( fin.is_open() )
   {
      s << fin.rdbuf();
   }
   else
   {
      s << "Not a file " << uri;
   }

   return s.str();
}

std::vector< std::string >
FileSystem::loadTextLnFromMemory( char const * data )
{
   PerformanceTimer timer;
   timer.start();
   std::string content = data;
   dbStrReplace( content, "\r\n", "\n" );
   dbStrReplace( content, "\n\r", "\n" );
   std::vector< std::string > lines = dbStrSplit( content, '\n' );
   timer.stop();
   DE_DEBUG("Loaded MemoryText in ", timer.ms(), "ms, size = ",content.size() )
   return lines;
}


std::vector< std::string >
FileSystem::loadTextLines( std::string const & uri )
{
   PerformanceTimer timer;
   timer.start();
   std::string content = dbLoadText( uri );
   dbStrReplace( content, "\r\n", "\n" );
   dbStrReplace( content, "\n\r", "\n" );
   std::vector< std::string > lines = dbStrSplit( content, '\n' );
   timer.stop();
   DE_DEBUG("Loaded TextFile in ", timer.ms(), "ms, ", uri )
   return lines;
}

bool
FileSystem::saveText( std::string const & uri, std::string const & txt )
{
   std::ofstream fout( uri );
   if ( !fout.is_open() ) return false;
   fout << txt;
   fout.close();
   return true;
}

bool
FileSystem::existFile( std::string const & uri )
{
   //DE_DEBUG("uri = ",uri)
   if ( uri.empty() )
   {
      //DE_DEBUG("empty uri")
      return false;
   }
   fs::file_status s = fs::status( uri );
   if ( !fs::exists( s ) )
   {
      //DE_DEBUG("No such fs entry, uri(",uri,")")
      return false;
   }

   if ( !fs::is_regular_file( s ) )
   {
      DE_DEBUG("Not a regular file, uri(",uri,")")
      return false;
   }

   return true;
}

bool
FileSystem::existFile( std::wstring const & uri )
{
   //DE_DEBUG("uri = ",uri)
   if ( uri.empty() )
   {
      //DE_DEBUG("empty uri")
      return false;
   }
   fs::file_status s = fs::status( fs::path(uri) );
   if ( !fs::exists( s ) )
   {
      //DE_DEBUG("No such fs entry, uri(",uri,")")
      return false;
   }

   if ( !fs::is_regular_file( s ) )
   {
      //DE_DEBUG("Not a regular file, uri(",uri,")")
      return false;
   }

   return true;
}

bool
FileSystem::existDirectory( std::string const & fileName )
{
   if ( fileName.empty() ) return false;
   fs::file_status s = fs::status( fileName );
   bool ok = fs::exists( s ) && fs::is_directory( s );
   return ok;
}

bool
FileSystem::existDirectory( std::wstring const & fileName )
{
   if ( fileName.empty() ) return false;
   fs::file_status s = fs::status( fileName );
   bool ok = fs::exists( s ) && fs::is_directory( s );
   return ok;
}

std::string
FileSystem::parentDir( std::string const & uri )
{
   fs::path p( uri );
   if ( p.is_relative() )
   {
      p = fs::absolute( p );
   }

   // DE_DEBUG("uri = ", uri, " -> is_relative make canonical" )
   if ( !fs::is_directory( p ) )
   {
      if ( p.has_parent_path() )
      {
         p = p.parent_path();
      }
   }

   std::string tmp = p.string();
   dbStrReplace( tmp, "\\", "/" );
   if ( dbStrEndsWith( tmp, "/" ) )
   {
      //DE_WARN("Remove trailing '/'")
      tmp = tmp.substr( 0, tmp.size() - 1 );
   }
   return tmp;
}

std::wstring
FileSystem::parentDir( std::wstring const & uri )
{
   fs::path p( uri );
   if ( p.is_relative() )
   {
      p = fs::absolute( p );
   }

   // DE_DEBUG("uri = ", uri, " -> is_relative make canonical" )
   if ( !fs::is_directory( p ) )
   {
      if ( p.has_parent_path() )
      {
         p = p.parent_path();
      }
   }

   std::wstring tmp = p.wstring();
   dbStrReplace( tmp, L"\\", L"/" );
   if ( dbStrEndsWith( tmp, L"/" ) )
   {
      //DE_WARN("Remove trailing '/'")
      tmp = tmp.substr( 0, tmp.size() - 1 );
   }
   return tmp;
}

std::string
FileSystem::fileSuffix( std::string const & uri )
{
   std::string suffix;
   if ( uri.empty() )
   {
      // DE_WARN("Empty uri has no suffix, uri(",uri,")" )
      return suffix;
   }

   std::string ext = fs::path( uri ).extension().string();
   if ( ext.empty() )
   {
      // DE_WARN("URI has no suffix, ext(",ext,"), uri(",uri,")" )
      return suffix;
   }

   // DE_DEBUG("Found suffix for ext(",ext,"), uri(",uri,")" )
   if (ext[0] == '.')
   {
      ext.erase( 0, 1 );
      // DE_DEBUG("Erase prefix '.' for ext(",ext,"), uri(",uri,")" )
   }

   // DE_DEBUG("Make lowercase for ext(",ext,"), uri(",uri,")" )
   dbStrLowerCase( ext );
   return ext;
}

std::wstring
FileSystem::fileSuffix( std::wstring const & uri )
{
   std::wstring suffix;
   if ( uri.empty() )
   {
      // DE_WARN("Empty uri has no suffix, uri(",uri,")" )
      return suffix;
   }

   std::wstring ext = fs::path( uri ).extension().wstring();
   if ( ext.empty() )
   {
      // DE_WARN("URI has no suffix, ext(",ext,"), uri(",uri,")" )
      return suffix;
   }

   // DE_DEBUG("Found suffix for ext(",ext,"), uri(",uri,")" )
   if (ext[0] == L'.')
   {
      ext.erase( 0, 1 );
      // DE_DEBUG("Erase prefix '.' for ext(",ext,"), uri(",uri,")" )
   }

   // DE_DEBUG("Make lowercase for ext(",ext,"), uri(",uri,")" )
   dbStrLowerCase( ext );
   return ext;
}

bool
FileSystem::isAbsolute( std::string const & uri )
{
   return fs::path( uri ).is_absolute();
}

bool
FileSystem::isAbsolute( std::wstring const & uri )
{
   return fs::path( uri ).is_absolute();
}

std::string
FileSystem::fileBase( std::string const & uri )
{
   return fs::path( uri ).stem().string();
}

std::wstring
FileSystem::fileBase( std::wstring const & uri )
{
   return fs::path( uri ).stem().wstring();
}

int64_t
FileSystem::fileSize( std::string const & uri )
{
   return int64_t( fs::file_size( uri ) );
}

int64_t
FileSystem::fileSize( std::wstring const & uri )
{
   return int64_t( fs::file_size( uri ) );
}

std::string
FileSystem::fileName( std::string const & uri )
{
   auto fn = fs::path( uri ).filename().string();
   if ( fn.empty() ) return uri;
   return fn;
}

std::wstring
FileSystem::fileName( std::wstring const & uri )
{
   auto fn = fs::path( uri ).filename().wstring();
   if ( fn.empty() ) return uri;
   return fn;
}

std::string
FileSystem::cwd()
{
   return fs::absolute( "." ).string();
}

std::string
FileSystem::makeAbsolute( std::string const & uri, std::string const & baseDir )
{
   std::string s = uri;

   try {

   if ( baseDir.empty() )
   {
      s = fs::canonical( fs::path( uri ) ).string();
      dbStrReplace( s, "\\", "/" );
   }
   else
   {
      fs::path dir( baseDir );
      if ( !dir.is_absolute() )
      {
         throw std::runtime_error( "BaseDir must be absolute" );
      }

      s = fs::absolute( dir.concat( uri ) ).string();
      dbStrReplace( s, "\\", "/" );
   }

#if 0
   if ( uri != s )
   {
      DE_DEBUG("Changed uri = ",uri, "\t :: TO :: \t",s)
   }
#endif

   }
   catch ( std::exception & e )
   {
      //DE_DEBUG("Got filesystem exception ", e.what(), ", uri = ", uri )
      s = uri;
   }

   return s;
}

bool
FileSystem::copyFile( std::string const & src, std::string const & dst )
{
   if ( src.empty() )
   {
      // DE_DEBUG("Empty filename src")
      return false;
   }
   if ( dst.empty() )
   {
      // DE_DEBUG("Empty filename dst")
      return false;
   }

   fs::path p0( src );
   fs::file_status s0 = fs::status( p0 );
   if ( !fs::exists( s0 ) ) {
      // DE_DEBUG("No existing file src")
      return false;
   }
   if ( !fs::is_regular_file( s0 ) ) {
      // DE_DEBUG("No regular file src")
      return false;
   }

   fs::path p1( dst );
//   fs::path p1 = dst.parent_path();

//   std::error_code e;
//   fs::create_directories( p_dir, e );
//   if ( e )
//   {
//      std::cout << "[Error] " << __func__ << "( uri:" << uri << ") :: Got error " << e.message() << std::endl;
//   }

   return fs::copy_file( p0, p1 ); //, fs::copy_options::recursive );
}

bool
FileSystem::copyDirectory( std::string const & src, std::string const & dst )
{
//   std::error_code e;
//   fs::create_directories( fs::path( uri ), e );
//   if ( e )
//   {
//      std::cout << "[Error] " << __func__ << "( uri:" << uri << ") :: Got error " << e.message() << std::endl;
//   }
   return false;
}

void
FileSystem::entryList(  std::string baseDir, bool recursive, bool withFiles, bool withDirs,
               std::function< void( std::string const & ) > const & userFunc )
{
   baseDir = FileSystem::makeAbsolute( baseDir );
   //DE_DEBUG("AbsoluteDir = ", baseDir )

   if ( !FileSystem::existDirectory( baseDir ) )
   {
      //DE_ERROR("Dir not found baseDir(", baseDir, ")" )
      return;
   }

   if ( recursive )
   {
      fs::recursive_directory_iterator it( baseDir );
      while ( it != fs::recursive_directory_iterator() )
      {
         fs::path p = it->path();
         std::string absUri = fs::canonical( p ).string();
         dbStrReplace( absUri, "\\", "/" ); // make posix path

         if ( withDirs && fs::is_directory( p ) )
         {
            userFunc( absUri );
         }

         if ( withFiles && fs::is_regular_file( p ) )
         {
            userFunc( absUri );
         }

         std::error_code ec;
         it.increment( ec );
         if ( ec )
         {
            // DE_ERROR("Recursive find : ",absUri," :: ",ec.message() )
            break;
         }
      }
   }
   else
   {
      fs::directory_iterator it( baseDir );
      while ( it != fs::directory_iterator() )
      {
         fs::path p = it->path();
         std::string absUri = fs::canonical( p ).string();
         dbStrReplace( absUri, "\\", "/" ); // make posix path

         if ( withDirs && fs::is_directory( p ) )
         {
            userFunc( absUri );
         }

         if ( withFiles && fs::is_regular_file( p ) )
         {
            userFunc( absUri );
         }

         std::error_code ec;
         it.increment( ec );
         if ( ec )
         {
            // DE_ERROR("Iterative find : ",absUri," :: ",ec.message() )
            break;
         }
      }
   }
}

void
FileSystem::createDirectory( std::string const & uri )
{
   if ( uri.empty() ) return;

   fs::path p( uri );

   fs::file_status s = fs::status( p );

   if ( fs::exists( s ) && fs::is_directory( s ) )
   {
      // std::cout << "[Warn] " << __func__ << "( uri:" << uri << ") :: Dir already exists." << std::endl;
      return;
   }

   std::error_code e;
   fs::create_directories( p, e );
   if ( e )
   {
      std::cout << "[Error] " << __func__ << "( uri:" << uri << ") :: Got error " << e.message() << std::endl;
   }
}


void
FileSystem::removeFile( std::string const & uri )
{
   std::error_code e;
   fs::remove( uri, e );
   if ( e )
   {
      std::cout << "[Error] " << __func__ << "(" << uri << ") :: " << e.message() << std::endl;
   }
}

void
FileSystem::removeFile( std::wstring const & uri )
{
   std::error_code e;
   fs::remove( uri, e );
   if ( e )
   {
      std::wcout << "[Error] " << __func__ << "(" << uri << ") :: " << e.message().c_str() << std::endl;
   }
}


void
FileSystem::removeDirectory( std::string const & uri )
{
   std::error_code e;
   fs::remove_all( fs::path( uri ), e );
   if ( e )
   {
      std::cout << "[Error] " << __func__ << "( uri:" << uri << ") :: Got error " << e.message() << std::endl;
   }
   fs::remove( uri, e );
   if ( e )
   {
      std::cout << "[Error] " << __func__ << "( uri:" << uri << ") :: Got 2nd error " << e.message() << std::endl;
   }
}

} // end namespace de

size_t
dbGetEntryCount( std::string const & uri, bool recursive, bool withFiles, bool withDirs )
{
   size_t entryCount = 0;
   dbGetEntries( uri, recursive, withFiles, withDirs, [&] ( std::string const & ) { ++entryCount; } );
   // DE_DEBUG("Found (", entryCount, ") entries in uri(", uri, "), recursive(", recursive, ")" )
   return entryCount;
}

std::vector< std::string >
dbGetFilesAndDirs( std::string const & uri, bool recursive, bool withFiles, bool withDirs )
{
   size_t entryCount = dbGetEntryCount( uri, recursive, withFiles, withDirs );
   if ( entryCount < 1 )
   {
      return {};
   }

   std::vector< std::string > fileList;
   fileList.reserve( entryCount );
   dbGetEntries( uri, recursive, withFiles, withDirs, [&] ( std::string const & f ) { fileList.emplace_back( f ); } );
   // DE_DEBUG("Found (", entryCount, ") entries in uri(", uri, "), recursive(", recursive, ")" )
   return fileList;
}

std::vector< std::string >
dbGetDirs( std::string const & uri, bool recursive )
{
   return dbGetFilesAndDirs( uri, recursive, false, true );
}

std::vector< std::string >
dbGetFiles( std::string const & uri, bool recursive )
{
   return dbGetFilesAndDirs( uri, recursive, true, false );
}

std::vector< std::string >
dbGetFilteredFiles( std::string const & uri, bool recursive, std::vector< std::string > const & exts )
{
   if ( uri.empty() )
   {
      return {};
   }

   if ( exts.empty())
   {
      return dbGetFiles( uri, recursive );
   }

   std::vector< std::string > files;

   dbGetEntries( uri, recursive, true, false,
      [&] ( std::string const & fileName )
      {
         std::string const ext = dbGetFileSuffix( fileName );

         int32_t found = -1;

         for ( size_t i = 0; i < exts.size(); ++i )
         {
            if ( ext == exts[ i ] )
            {
               found = int32_t( i );
               break;
            }
         }

         if ( found > -1 )
         {
            // DE_DEBUG( "Found file(", files.size(),") with extension(", ext, ") in fileName(", fileName, ")")
            files.emplace_back( fileName ); // extension is in given list.
         }
      }
   );

   //DE_DEBUG("Found (",files.size(),") files with given exts(", exts.size(), ")" )
   return files;
}


std::string
dbFirstSubDirectory( std::string const & uri, bool recursive )
{
   fs::path p( uri );
   if ( fs::is_regular_file( p ) )
   {
      p = p.parent_path();
   }
   if ( !fs::is_directory( p ) )
   {
      return "";
   }
   if ( !fs::exists( p ) )
   {
      return "";
   }

   if ( recursive )
   {
      fs::recursive_directory_iterator it( p );
      while ( it != fs::recursive_directory_iterator() )
      {
         fs::path p = it->path();
         if ( fs::is_directory( p ) )
         {
            return fs::canonical( p ).string();
         }
         std::error_code ec;
         it.increment( ec );
         if ( ec )
         {
            break;
         }
      }
   }
   else
   {
      fs::directory_iterator it( p );
      while ( it != fs::directory_iterator() )
      {
         fs::path p = it->path();
         if ( fs::is_directory( p ) )
         {
            return fs::canonical( p ).string();
         }
         std::error_code ec;
         it.increment( ec );
         if ( ec )
         {
            break;
         }
      }
   }
   return "";
}

#if 0

std::vector< std::string >
dbGetAbsoluteFileNames( std::vector< std::string > fileNames, std::string baseDir )
{
   // make filenames absolute using dir from given filename
   //std::string baseDir = getAbsoluteFilePath( fileName );

   makePosixPath( baseDir );

   if ( baseDir.empty() )
   {
      baseDir = dbCWD();
   }

   if ( !endsWith( baseDir, "/" ) )
   {
      baseDir += "/";
   }

   for ( size_t i = 0; i < fileNames.size(); ++i )
   {
      std::string & fileName = fileNames[ i ];
      fileName = dbMakeAbsolute( baseDir + fileName );
   }

   return fileNames;
}

std::string
dbStringifyFilePermsissions( fs::perms const perm )
{
   std::stringstream s;
   s << ((perm & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
     << ((perm & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
     << ((perm & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
     << ((perm & fs::perms::group_read) != fs::perms::none ? "r" : "-")
     << ((perm & fs::perms::group_write) != fs::perms::none ? "w" : "-")
     << ((perm & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
     << ((perm & fs::perms::others_read) != fs::perms::none ? "r" : "-")
     << ((perm & fs::perms::others_write) != fs::perms::none ? "w" : "-")
     << ((perm & fs::perms::others_exec) != fs::perms::none ? "x" : "-");
   return s.str();
}

#endif

// #include "GetPluginDatabase.hpp"
// #include "GetPluginInfo.hpp"


namespace de {
namespace audio {

// PluginDatabase
// getPluginInfoList( std::wstring vstDir, bool searchSubFolders )
// {
   // auto qtdir = QString::fromStdWString( vstDir );
   // QDirIterator dirIt( qtdir, searchSubFolders ?
                              // QDirIterator::Subdirectories :
                              // QDirIterator::NoIteratorFlags );

   // PluginDatabase out;
   // out.m_vstDir = vstDir;
   // out.m_vstDirMB = qtdir.toStdString();

   // std::vector< PluginInfo > & pluginList = out.m_list;

   // while ( dirIt.hasNext() )
   // {
      // QString elem = dirIt.next();
      // QFileInfo fi = dirIt.fileInfo();
      // if ( fi.isFile() )
      // {
         // auto ext = fi.suffix().toLower();
         // if ( ext == "dll" )
         // {
            // try {
               // PluginInfo pin = getPluginInfo( elem.toStdWString() );
               // if ( pin.m_isPlugin )
               // {
                  // pluginList.emplace_back( std::move(pin) );
               // }
            // }
            // catch (...)
            // {
            // }

         // }
      // }
   // }

   // return out;
// }


} // end namespace audio
} // end namespace de

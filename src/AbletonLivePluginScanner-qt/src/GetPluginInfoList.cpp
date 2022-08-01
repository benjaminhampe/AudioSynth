#include "GetPluginInfoList.hpp"

// #include <QWidget>
// #include <QImage>
// #include <QDebug>
// #include <cstdint>
// #include <sstream>
// #include <vector>
// #include <map>
// #include <functional>
// #include <algorithm>
// #include <memory>
// #include <mutex>
// #include <atomic>
// #include <thread>

// #include <QRect>

// inline QString
// toQString( std::string const & msg )
// {
   // return QString::fromStdString( msg );
// }

// inline QString
// toQString( std::wstring const & msg )
// {
   // return QString::fromStdWString( msg );
// }

// #include <DarkImage.hpp>
#include <QFileInfo>
#include <QDirIterator>
#include <iostream>

namespace de {
namespace audio {

PluginInfoList
getPluginInfoList( std::wstring vstDir, bool searchSubFolders )
{
   auto qtdir = QString::fromStdWString( vstDir );
   QDirIterator dirIt( qtdir, searchSubFolders ?
                              QDirIterator::Subdirectories :
                              QDirIterator::NoIteratorFlags );

   PluginInfoList out;
   out.m_vstDir = vstDir;
   out.m_vstDirMB = qtdir.toStdString();

   std::vector< PluginInfo > & pluginList = out.m_list;

   while ( dirIt.hasNext() )
   {
      QString elem = dirIt.next();
      QFileInfo fi = dirIt.fileInfo();
      if ( fi.isFile() )
      {
         auto ext = fi.suffix().toLower();
         if ( ext == "dll" )
         {
            try {
               PluginInfo pin = getPluginInfo( elem.toStdWString() );
               if ( pin.m_isPlugin )
               {
                  pluginList.emplace_back( std::move(pin) );
               }
            }
            catch (...)
            {
            }

         }
      }
   }

   return out;
}


} // end namespace audio
} // end namespace de

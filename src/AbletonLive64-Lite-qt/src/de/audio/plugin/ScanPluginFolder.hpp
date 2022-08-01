#ifndef DE_AUDIO_SCAN_PLUGIN_FOLDER_HPP
#define DE_AUDIO_SCAN_PLUGIN_FOLDER_HPP

#include "ScanPlugin.hpp"
#include <QFileInfo>
#include <QDirIterator>

namespace de {
namespace audio {

typedef std::vector< PluginInfo* > PluginInfos;

struct IPluginScannerListener
{
   virtual ~IPluginScannerListener();

};

inline PluginInfos
scanPluginFolder( QString vstDir, bool recursive )
{
   PluginInfos infos;

   QDirIterator dirIt( vstDir, recursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags );
   while ( dirIt.hasNext() )
   {
      QString uri = dirIt.next();
      QFileInfo fi = dirIt.fileInfo();
      if ( fi.isFile() )
      {
         auto ext = fi.suffix().toLower();
         if ( ext == "dll" )
         {
            try
            {
               PluginInfo* pin = scanPlugin( uri.toStdWString() );
               if ( pin )
               {
                  infos.emplace_back( std::move(pin) );
               }
            }
            catch (...)
            {
            }

         }
      }
   }

   return infos;
}

} // end namespace audio
} // end namespace de

#endif // G_LOPASS1_HPP

#include "App.hpp"

App::App()
{
   if ( !m_pluginDb.loadXml( "plugin_db.xml" ) )
   {
      std::cout << "Cant load plugin_sb.xml" << std::endl;
   }

   // Track1
   {
      Track trk;
      trk.m_isAudioTrack = false;
      trk.addClip( 0, 1, "Clip0-1");
      trk.addClip( 1, 3, "Clip1-3");
      trk.addClip( 3, 4, "Clip3-4");
      trk.addClip( 5, 8, "Clip5-8");
      trk.addClip( 10, 16, "Clip10-16");
      trk.addClip( 17, 20, "Clip17-20");
      m_trackList.addTrack( trk );
   }

   // Track2
   {
      Track trk;
      trk.m_isAudioTrack = true;
      trk.addClip( 0, 2, "Clip0-2");
      trk.addClip( 4, 6, "Clip4-6");
      trk.addClip( 8, 10, "Clip8-10");
      trk.addClip( 16, 17, "Clip16-18");
      trk.addClip( 18, 19, "Clip18-20");
      m_trackList.addTrack( trk );
   }

   // Track3
   {
      Track trk;
      trk.addClip( 0, 10, "Clip0-10");
      trk.addClip( 10, 20, "Clip10-32");
      m_trackList.addTrack( trk );
   }

   // Track4
   {
      Track trk;
      trk.m_isAudioTrack = true;
      trk.addClip( 0, 2, "Clip0-2");
      trk.addClip( 4, 6, "Clip4-6");
      trk.addClip( 8, 10, "Clip8-10");
      trk.addClip( 16, 17, "Clip16-18");
      trk.addClip( 18, 19, "Clip18-20");
      m_trackList.addTrack( trk );
   }

   // Track5
   {
      Track trk;
      trk.m_isAudioTrack = true;
      trk.addClip( 0, 0.25, "Clip0-1");
      trk.addClip( 0.5, 0.75, "Clip0-2");
      trk.addClip( 1, 1.25, "Clip1-1");
      trk.addClip( 1.5, 1.75, "Clip1-2");
      trk.addClip( 2, 2.25, "Clip2-1");
      trk.addClip( 2.5, 2.75, "Clip2-2");
      trk.addClip( 3, 3.25, "Clip3-1");
      trk.addClip( 3.5, 3.75, "Clip3-2");
      trk.addClip( 4, 4.25, "Clip4-1");
      trk.addClip( 4.5, 4.75, "Clip4-2");
      trk.addClip( 5, 5.25, "Clip5-1");
      trk.addClip( 5.5, 5.75, "Clip5-2");
      trk.addClip( 6, 6.25, "Clip6-1");
      trk.addClip( 6.5, 6.75, "Clip6-2");
      trk.addClip( 7, 7.25, "Clip7-1");
      trk.addClip( 7.5, 7.75, "Clip7-2");
      m_trackList.addTrack( trk );
   }
}

App::~App()
{}

void
App::reset()
{
}

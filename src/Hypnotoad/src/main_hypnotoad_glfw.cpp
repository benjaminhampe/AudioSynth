#include <DarkGPU.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


#include <de/audio/Music.hpp>
#include <de/audio/Engine.hpp>
#include <de/audio/Mixer.hpp>

namespace de {
namespace gpu {

// =====================================================================
struct AppContext
// =====================================================================
{
   DE_CREATE_LOGGER("AppContext")

   GLFWwindow* window;
   int m_NormalW = 1024;
   int m_NormalH = 768;

   IVideoDriver* driver;

   de::audio::Music m_audioPlayer;
   de::audio::Engine m_audioEngine;
   de::audio::Mixer m_audioMixer;

   std::array< bool, 256*4 > keys;
   int mouseX = 0;
   int mouseY = 0;
   int mouseXLast = 0;
   int mouseYLast = 0;
   int mouseMoveX = 0;
   int mouseMoveY = 0;
   bool isFullscreen = true;
   bool isLeftPressed = false;
   bool isRightPressed = false;
   bool isMiddlePressed = false;
   bool isDragging = false;

   double time;
   double timeLastTitleUpdate;
   double timeLastRenderUpdate;
   AppContext()
      : window( nullptr )
      , m_NormalW( 1024 )
      , m_NormalH( 768 )
      , driver( nullptr )
      , m_Node( nullptr )
      , m_IsPerfOverlayVisible( false )
   {
      DE_DEBUG("")
   }

   SMeshSceneNode* m_Node;
   bool m_IsPerfOverlayVisible;
};

inline void
toggleFullscreen( AppContext & app )
{
   if ( app.window )
   {
      app.isFullscreen = !app.isFullscreen;
      int dx = dbDesktopWidth();
      int dy = dbDesktopHeight();

      if ( app.isFullscreen )
      {
         glfwSetWindowAttrib( app.window, GLFW_DECORATED, GLFW_FALSE );
         glfwSetWindowAttrib( app.window, GLFW_RESIZABLE, GLFW_FALSE );
         glfwSetWindowAttrib( app.window, GLFW_MAXIMIZED, GLFW_TRUE );
         glfwSetWindowSize( app.window, dx, dy );
         glfwSetWindowPos( app.window, 0, 0 );
      }
      else
      {
         glfwSetWindowAttrib( app.window, GLFW_DECORATED, GLFW_TRUE );
         glfwSetWindowAttrib( app.window, GLFW_MAXIMIZED, GLFW_FALSE );
         glfwSetWindowAttrib( app.window, GLFW_RESIZABLE, GLFW_TRUE );

         int x = ( dx - app.m_NormalW ) / 2;
         int y = ( dy - app.m_NormalH ) / 2;
         glfwSetWindowSize( app.window, app.m_NormalW, app.m_NormalH );
         glfwSetWindowPos( app.window, x, y );
      }
   }
}

}
}

de::gpu::AppContext gApp;

void createAudio()
{
   gApp.m_audioPlayer.open( "../../media-hypnotoad/hypnotoad.mp4");
   gApp.m_audioMixer.setInputSignal( 0, &gApp.m_audioPlayer );
   gApp.m_audioEngine.setPlayer( &gApp.m_audioMixer );
   gApp.m_audioEngine.playAudio();
   gApp.m_audioPlayer.play();

   auto driver = gApp.driver;
   if ( !driver )
   {
      DEM_ERROR("Inv no driver II")
   }

   int w = driver->getScreenWidth();
   int h = driver->getScreenHeight();

}

void on_resize( GLFWwindow* window, int w, int h )
{
   static_cast< void >( window );
   if ( gApp.driver )
   {
      gApp.driver->resize( w, h );
   }
}

void on_keyboard( GLFWwindow* window, int key, int scancode, int action, int mods )
{
   if ( key == GLFW_KEY_ESCAPE )
   {
      glfwSetWindowShouldClose( window, GLFW_TRUE );
      return;
   }

   // if ( action == GLFW_PRESS )
   // {
      // gApp.keys[ key ] = true;
   // }
   // else if ( action == GLFW_RELEASE )
   // {
      // gApp.keys[ key ] = false;
   // }

   // if ( gApp.keys[ GLFW_KEY_F1 ] )
   // {
      // de::gpu::toggleFullscreen( gApp );
   // }
}

void on_mouse( GLFWwindow* window, int button, int action, int mods )
{
   //glfwGetCursorPos( window, &m_MouseX, &m_MouseY );
   if (button == GLFW_MOUSE_BUTTON_LEFT)
   {
      if (action == GLFW_PRESS)
      {
         //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
         //glfwGetCursorPos( window, &m_MouseX, &m_MouseY );
         gApp.isLeftPressed = true;
      }
      else
      {
         //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
         gApp.isLeftPressed = false;
      }
   }
}

void on_cursor( GLFWwindow* window, double x, double y )
{
   static_cast< void >( window );

   if ( glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED )
   {
      //        alpha += (GLfloat) (x - cursorX) / 10.f;
      //        beta += (GLfloat) (y - cursorY) / 10.f;
      //m_MouseX = x;
      //m_MouseY = y;
   }

   if ( gApp.driver )
   {
      de::SEvent post;
      post.type = de::EET_MOUSE_EVENT;
      post.mouseEvent.m_Flags = de::SMouseEvent::Moved;
      post.mouseEvent.m_X = x;
      post.mouseEvent.m_Y = y;
      post.mouseEvent.m_Wheel = 0.0f;
      gApp.driver->postEvent( post );
   }

   gApp.mouseX = x;
   gApp.mouseMoveX = gApp.mouseX - gApp.mouseXLast;
   gApp.mouseXLast = gApp.mouseX;

   gApp.mouseY = y;
   gApp.mouseMoveY = gApp.mouseY - gApp.mouseYLast;
   gApp.mouseYLast = gApp.mouseY;
}

void on_scroll( GLFWwindow* window, double x, double y )
{
   static_cast< void >( window );
   static_cast< void >( x );
   static_cast< void >( y );
}

void on_error( int error, const char* description )
{
   fprintf(stderr, "GLFW Error: (%d) %s\n", error, description);
}

//========================================================================
int main(int argc, char** argv)
//========================================================================
{
   glfwSetErrorCallback( on_error );
   if ( !glfwInit() ) exit(EXIT_FAILURE);

   glfwDefaultWindowHints();
   glfwWindowHint( GLFW_VISIBLE, GLFW_TRUE );
   glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );

   if ( gApp.isFullscreen )
   {
      glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );   // -> make fullscreen
      glfwWindowHint( GLFW_MAXIMIZED, GLFW_FALSE );    // -> make fullscreen
      glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
   }
// #ifdef USE_OPENGLES_AND_EGL

   glfwWindowHint( GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API );
   glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );

   DEM_DEBUG("Uses EGL + OpenGLES")
// #else

   // glfwWindowHint( GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API );
   // glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
   // glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
   // glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

   // DEM_DEBUG("Uses Desktop OpenGL 4.5")
// #endif
   int w = dbDesktopWidth();   // -> make fullscreen
   int h = dbDesktopHeight(); // -> make fullscreen
   gApp.window = glfwCreateWindow( w, h, "MeshViewer (c) 2021-2022 Benjamin Hampe", nullptr, nullptr );

   if ( !gApp.window )
   {
      glfwTerminate();
      exit(EXIT_FAILURE);
   }

   glfwSetKeyCallback( gApp.window, on_keyboard );
   glfwSetFramebufferSizeCallback( gApp.window, on_resize );
   glfwSetMouseButtonCallback( gApp.window, on_mouse );
   glfwSetCursorPosCallback( gApp.window, on_cursor );
   glfwSetScrollCallback( gApp.window, on_scroll );
   glfwMakeContextCurrent( gApp.window );
   glfwSwapInterval(1); // min

   glfwGetFramebufferSize( gApp.window, &w, &h );

   // Create VideoDriver
   gApp.driver = de::gpu::createVideoDriver( w, h );

   if ( gApp.isFullscreen )
   {
      int dx = dbDesktopWidth();
      int dy = dbDesktopHeight();
      glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );   // -> make fullscreen
      glfwWindowHint( GLFW_MAXIMIZED, GLFW_FALSE );    // -> make fullscreen
      glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
      glfwSetWindowSize( gApp.window, dx, dy );
      glfwSetWindowPos( gApp.window, 0, 0 );
   }

   // Camera speeds
   float moveSpeed = 5.3f;
   float strafeSpeed = moveSpeed;
   float elevateSpeed = moveSpeed;
   float turnSpeed = 0.21f;

   gApp.time = gApp.driver->getTimer();
   gApp.timeLastRenderUpdate = gApp.time;
   gApp.timeLastTitleUpdate = gApp.time;

   createAudio();

   while ( !glfwWindowShouldClose( gApp.window ) )
   {
      auto & driver = gApp.driver;
      auto & camera = *gApp.driver->getActiveCamera();
      auto & keys = gApp.keys;

      // ============================== Skybox ==============================
      gApp.time = driver->getTimer();

      // Update FPS Camera
      int w = driver->getScreenWidth();
      int h = driver->getScreenHeight();
      camera.setScreenSize( w, h );

      // Limit to 20 fps to save energy
      if ( gApp.time - gApp.timeLastRenderUpdate > 0.05 )
      {
         if ( keys[ GLFW_KEY_W ] )         { camera.move( moveSpeed ); }
         if ( keys[ GLFW_KEY_A ] )         { camera.strafe( -strafeSpeed ); }
         if ( keys[ GLFW_KEY_S ] )         { camera.move( -moveSpeed ); }
         if ( keys[ GLFW_KEY_D ] )         { camera.strafe( strafeSpeed ); }
         if ( keys[ GLFW_KEY_UP ] )        { camera.move( moveSpeed ); }
         if ( keys[ GLFW_KEY_LEFT ] )      { camera.strafe( -strafeSpeed ); }
         if ( keys[ GLFW_KEY_DOWN ] )      { camera.move( -moveSpeed ); }
         if ( keys[ GLFW_KEY_RIGHT ] )     { camera.strafe( strafeSpeed ); }
         if ( keys[ GLFW_KEY_PAGE_UP ] )   { camera.elevate( elevateSpeed ); }
         if ( keys[ GLFW_KEY_Y ] )         { camera.elevate( elevateSpeed ); }
         if ( keys[ GLFW_KEY_PAGE_DOWN ])  { camera.elevate( -elevateSpeed ); }
         if ( keys[ GLFW_KEY_X ] )         { camera.elevate( -elevateSpeed ); }
         if ( gApp.isLeftPressed )
         {
            if ( gApp.mouseMoveX != 0 )
            {
               camera.yaw( turnSpeed * gApp.mouseMoveX );
               gApp.mouseMoveX = 0;
            }
            if ( gApp.mouseMoveY != 0 )
            {
               camera.pitch( turnSpeed * gApp.mouseMoveY );
               gApp.mouseMoveY = 0;
            }
         }

         driver->beginRender();
         driver->getSceneManager()->render();
         driver->endRender();
         glfwSwapBuffers( gApp.window );

         gApp.timeLastRenderUpdate = gApp.time;
         // ++framecount;
      }

      if ( gApp.time - gApp.timeLastTitleUpdate > 0.132 )
      {
         std::ostringstream s;
         s <<
            "FPS(" << driver->getFPS() << "), "
            "Screen(" << driver->getScreenWidth() << ","
                      << driver->getScreenHeight() << "), "
                      << driver->getScreenFormat().toString() << "), "
            "Frames(" << driver->getFrameCount() << "), "
            "Time(" << gApp.time << "), ";

   //      if ( m_Camera )
   //      {
            s << "Camera-Pos(" << glm::ivec3( camera.getPos()) << ")+";
            s << "Ang(" << glm::ivec3(camera.getAng()) << ")+";
            s << "Dir(" << camera.getDir() << ")+";
            s << "Up(" << camera.getUp() << ")+";
            s << "Right(" << camera.getRight() << ")";
   //      }

         glfwSetWindowTitle( gApp.window, s.str().c_str() );
         gApp.timeLastTitleUpdate = gApp.time;
      }



      glfwPollEvents();
   }

   gApp.window = nullptr;
   SAFE_DELETE( gApp.driver );

   glfwTerminate();
   exit(EXIT_SUCCESS);
}


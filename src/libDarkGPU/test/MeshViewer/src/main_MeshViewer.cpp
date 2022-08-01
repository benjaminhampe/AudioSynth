#include <DarkGPU.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

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

int
CAD_addObjFromFile( de::gpu::AppContext & app, std::string uri )
{
   static int createCounter = 0;
   createCounter++;
   int id = createCounter;

   auto smgr = app.driver->getSceneManager();
   auto mesh = smgr->getMesh( uri );
   if ( !mesh )
   {
      DEM_ERROR("Cant load CAD obj ", uri)
      return 0;
   }

   app.m_Node = smgr->addMeshSceneNode( mesh, nullptr, id );
   return id;
}

de::gpu::AppContext g_ctx;

void createGUI( de::gpu::AppContext & app )
{
   auto driver = app.driver;
   if ( !driver )
   {
      DEM_ERROR("Inv no driver II")
   }

   int w = driver->getScreenWidth();
   int h = driver->getScreenHeight();

/*
   using namespace de::gpu;
   UI* ui = driver->getUI();
   Label* lblRes = new Label( "X:", ui );
   Button* btnResPrev = new Button( "<", ui );
   EditBox* edtRes = new EditBox( "Hello", ui );
   Button* btnResNext = new Button( ">", ui );

   Panel* panel = new Panel( ui );
   panel->setSize( 300, 200 );
   panel->setPos( w/2-150, h/2-100 );
   panel->addChild( lblRes );
   panel->addChild( btnResPrev );
   panel->addChild( edtRes );
   panel->addChild( btnResNext );

   lblRes->setPos( 10,10 );
   btnResPrev->setPos( 50,10 );
   edtRes->setPos( 100,10 );
   btnResNext->setPos( 210,10 );

   ui->addTopLevelWidget( panel );
*/
}

void on_resize( GLFWwindow* window, int w, int h )
{
   static_cast< void >( window );
   if ( g_ctx.driver )
   {
      g_ctx.driver->resize( w, h );
   }
}

void on_keyboard( GLFWwindow* window, int key, int scancode, int action, int mods )
{
   if ( key == GLFW_KEY_ESCAPE )
   {
      glfwSetWindowShouldClose( window, GLFW_TRUE );
      return;
   }

   if ( action == GLFW_PRESS )
   {
      g_ctx.keys[ key ] = true;
   }
   else if ( action == GLFW_RELEASE )
   {
      g_ctx.keys[ key ] = false;
   }

   if ( g_ctx.keys[ GLFW_KEY_F1 ] )
   {
      de::gpu::toggleFullscreen( g_ctx );
   }

   if ( g_ctx.keys[ GLFW_KEY_F2 ] )
   {
      g_ctx.m_IsPerfOverlayVisible = !g_ctx.m_IsPerfOverlayVisible;
   }

//   if ( g_ctx.keys[ GLFW_KEY_F2 ] )
//   {
//      g_ctx.driver->getSceneManager()->cleanUp();
//   }

   // F5 - Load Object3D
   if ( g_ctx.keys[ GLFW_KEY_F5 ] )
   {
      std::string uri = dbOpenFileDlg( "Open model file");
      CAD_addObjFromFile( g_ctx, uri );
   }

   // F6 - Show debug info
   if ( g_ctx.keys[ GLFW_KEY_F6 ] )
   {
      auto node = g_ctx.m_Node;
      if ( node ) node->setDebugData( de::gpu::VisualDebugData::ALL );
   }

   // F7 - Hide debug info
   if ( g_ctx.keys[ GLFW_KEY_F7 ] )
   {
      auto node = g_ctx.m_Node;
      if ( node ) node->setDebugData( de::gpu::VisualDebugData::NONE );
   }

   // F8/F9 -/+ Lighting
   if ( g_ctx.keys[ GLFW_KEY_F8 ] )
   {
      auto node = g_ctx.m_Node;
      if ( node ) node->setLighting( std::max( 0, node->getLighting() - 1 ) );
   }
   if ( g_ctx.keys[ GLFW_KEY_F9 ] )
   {
      auto node = g_ctx.m_Node;
      if ( node ) node->setLighting( std::min( 6, node->getLighting() + 1 ) );
   }

   if ( g_ctx.keys[ GLFW_KEY_DELETE ] )
   {
      //g_ctx.objs.clear();
   }

   if ( g_ctx.keys[ GLFW_KEY_SPACE ] )
   {

   }
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
         g_ctx.isLeftPressed = true;
      }
      else
      {
         //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
         g_ctx.isLeftPressed = false;
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

   if ( g_ctx.driver )
   {
      de::SEvent post;
      post.type = de::EET_MOUSE_EVENT;
      post.mouseEvent.m_Flags = de::SMouseEvent::Moved;
      post.mouseEvent.m_X = x;
      post.mouseEvent.m_Y = y;
      post.mouseEvent.m_Wheel = 0.0f;
      g_ctx.driver->postEvent( post );
   }

   g_ctx.mouseX = x;
   g_ctx.mouseMoveX = g_ctx.mouseX - g_ctx.mouseXLast;
   g_ctx.mouseXLast = g_ctx.mouseX;

   g_ctx.mouseY = y;
   g_ctx.mouseMoveY = g_ctx.mouseY - g_ctx.mouseYLast;
   g_ctx.mouseYLast = g_ctx.mouseY;
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

   if ( g_ctx.isFullscreen )
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
   g_ctx.window = glfwCreateWindow( w, h, "MeshViewer (c) 2021-2022 Benjamin Hampe", nullptr, nullptr );

   if ( !g_ctx.window )
   {
      glfwTerminate();
      exit(EXIT_FAILURE);
   }

   glfwSetKeyCallback( g_ctx.window, on_keyboard );
   glfwSetFramebufferSizeCallback( g_ctx.window, on_resize );
   glfwSetMouseButtonCallback( g_ctx.window, on_mouse );
   glfwSetCursorPosCallback( g_ctx.window, on_cursor );
   glfwSetScrollCallback( g_ctx.window, on_scroll );
   glfwMakeContextCurrent( g_ctx.window );
   glfwSwapInterval(1); // min

   glfwGetFramebufferSize( g_ctx.window, &w, &h );

   // Create VideoDriver
   g_ctx.driver = de::gpu::createVideoDriver( w, h );

   if ( g_ctx.isFullscreen )
   {
      int dx = dbDesktopWidth();
      int dy = dbDesktopHeight();
      glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );   // -> make fullscreen
      glfwWindowHint( GLFW_MAXIMIZED, GLFW_FALSE );    // -> make fullscreen
      glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
      glfwSetWindowSize( g_ctx.window, dx, dy );
      glfwSetWindowPos( g_ctx.window, 0, 0 );
   }

   // Camera speeds
   float moveSpeed = 5.3f;
   float strafeSpeed = moveSpeed;
   float elevateSpeed = moveSpeed;
   float turnSpeed = 0.21f;

   createGUI( g_ctx );

   if ( argc > 1 )
   {
      CAD_addObjFromFile( g_ctx, argv[ 1 ] );
   }

   g_ctx.time = g_ctx.driver->getTimer();
   g_ctx.timeLastRenderUpdate = g_ctx.time;
   g_ctx.timeLastTitleUpdate = g_ctx.time;

   while ( !glfwWindowShouldClose( g_ctx.window ) )
   {
      auto & driver = g_ctx.driver;
      auto & camera = *g_ctx.driver->getActiveCamera();
      auto & keys = g_ctx.keys;

      // ============================== Skybox ==============================
      g_ctx.time = driver->getTimer();

      // Update FPS Camera
      int w = driver->getScreenWidth();
      int h = driver->getScreenHeight();
      camera.setScreenSize( w, h );



      // Limit to 20 fps to save energy
      if ( g_ctx.time - g_ctx.timeLastRenderUpdate > 0.05 )
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
         if ( g_ctx.isLeftPressed )
         {
            if ( g_ctx.mouseMoveX != 0 )
            {
               camera.yaw( turnSpeed * g_ctx.mouseMoveX );
               g_ctx.mouseMoveX = 0;
            }
            if ( g_ctx.mouseMoveY != 0 )
            {
               camera.pitch( turnSpeed * g_ctx.mouseMoveY );
               g_ctx.mouseMoveY = 0;
            }
         }

         driver->beginRender();
         driver->getSceneManager()->render();

   //      if ( g_ctx.pickObj )
   //      {

   //         if ( g_ctx.pickObjBufferIndex > -1 &&
   //               g_ctx.pickObjBufferIndex < g_ctx.pickObj->mesh.getMeshBufferCount() )
   //         {
   //            driver->draw3DLineBox( g_ctx.pickObj->mesh.getMeshBuffer( g_ctx.pickObjBufferIndex ).getBoundingBox(), de::randomColor() );
   //         }

   //      }

         if ( g_ctx.m_IsPerfOverlayVisible )
         {
            driver->draw2DPerfOverlay5x8();
         }

         driver->getUI()->render();
         driver->endRender();
         glfwSwapBuffers( g_ctx.window );

         g_ctx.timeLastRenderUpdate = g_ctx.time;
         // ++framecount;
      }

      if ( g_ctx.time - g_ctx.timeLastTitleUpdate > 0.132 )
      {
         std::ostringstream s;
         s <<
            "FPS(" << driver->getFPS() << "), "
            "Screen(" << driver->getScreenWidth() << ","
                      << driver->getScreenHeight() << "), "
                      << driver->getScreenFormat().toString() << "), "
            "Frames(" << driver->getFrameCount() << "), "
            "Time(" << g_ctx.time << "), ";

   //      if ( m_Camera )
   //      {
            s << "Camera-Pos(" << glm::ivec3( camera.getPos()) << ")+";
            s << "Ang(" << glm::ivec3(camera.getAng()) << ")+";
            s << "Dir(" << camera.getDir() << ")+";
            s << "Up(" << camera.getUp() << ")+";
            s << "Right(" << camera.getRight() << ")";
   //      }

         glfwSetWindowTitle( g_ctx.window, s.str().c_str() );
         g_ctx.timeLastTitleUpdate = g_ctx.time;
      }



      glfwPollEvents();
   }

   g_ctx.window = nullptr;
   SAFE_DELETE( g_ctx.driver );

   glfwTerminate();
   exit(EXIT_SUCCESS);
}


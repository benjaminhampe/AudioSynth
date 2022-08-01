#include "LibreCAD.hpp"

// ========================================================================
int
CAD_findObj( de::gpu::AppContext & app, int objId )
{
   for ( size_t i = 0; i < app.objs.size(); ++i )
   {
      if ( app.objs[ i ].id == objId )
      {
         return int( i );
      }
   }
   return -1;
}

de::gpu::CADObj &
CAD_getObj( de::gpu::AppContext & app, int objId )
{
   int found = CAD_findObj( app, objId );
   if (found < 0)
   {
      std::stringstream s;
      s << "CAD_getObj(" << objId << ")";
      throw std::runtime_error( s.str() );
   }

   return app.objs[ found ];
}



namespace de {
namespace gpu {

/*
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
*/

}
}
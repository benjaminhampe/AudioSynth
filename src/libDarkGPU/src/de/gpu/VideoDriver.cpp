#include <de/gpu/VideoDriver.hpp>
#include <de_opengl.hpp>

#ifdef USE_OPENGLES_AND_EGL
   #include <de/gpu/EGL.hpp>
#endif

namespace de {
namespace gpu {

namespace {
   constexpr float const CONST_Z_INIT = 0.975f; // At 1.0 it disappears, not inside frustum.
   constexpr float const CONST_Z_STEP = -0.00001f; // enough for 200.000 elements, TODO: test more.
}

// Uses and creates new internally created window as surface,
// creates platform depending ASyncWindow
IVideoDriver*
createVideoDriver( int w, int h, uint64_t winHandle, SurfaceFormat const & fmt, bool debug )
{
   auto driver = new VideoDriverGLESv3();
   if ( !driver->open( w, h, winHandle, fmt, debug ) )
   {
      delete driver;
      return nullptr;
   }
   //driver->setVSync( vsync );
   return driver;
}

// ###########################
// ###                     ###
// ###       Driver        ###
// ###                     ###
// ###########################

VideoDriverGLESv3::VideoDriverGLESv3()
   : m_IsOpen( false )
   , m_IsDebug( false )
   , m_Width( 0 )
   , m_Height( 0 )
   , m_MouseX( 0 )
   , m_MouseY( 0 )
   , m_MouseMoveX( 0 )
   , m_MouseMoveY( 0 )
   , m_WindowHandle( 0 )
#ifdef USE_OPENGLES_AND_EGL
   , m_eglDisplay( nullptr )
   , m_eglConfig( nullptr )
   , m_eglSurface( nullptr )
   , m_eglContext( nullptr )
#endif
   , m_VSync( 0 )
   , m_Time( dbSeconds() )
   , m_FrameCount( 0 )
   , m_FPS( 0.0f )
   , m_TimeLastFpsUpdate( 0.0 )
   // , m_Screen( nullptr )
   // , m_MSAA16( nullptr )
   , m_VersionMajor(-1)
   , m_VersionMinor(-1)
   , m_Shader( nullptr )
   , m_ProgramId( 0 )
   , m_ModelMatrix( 1.0 )
   , m_Camera( nullptr )
   , m_MaterialRenderer( this )
   , m_ScreenRenderer( this )
#ifdef USE_LIB_DARKFONT
   , m_FontRenderer( this )
#endif
   , m_PerfOverlay( this )
   , m_SceneManager( this )
   //,m_GUIEnvironment( this )
{
   m_VSync.curr = 1;
}

VideoDriverGLESv3::~VideoDriverGLESv3()
{
   close();
//   if ( m_SceneManager )
//   {
//      delete m_SceneManager;
//      m_SceneManager = nullptr;
//   }
}

bool
VideoDriverGLESv3::isDebug() const { return m_IsDebug; }
void
VideoDriverGLESv3::setDebug( bool debug ) { m_IsDebug = debug; }

int32_t
VideoDriverGLESv3::getScreenWidth() const { return m_Width; }
int32_t
VideoDriverGLESv3::getScreenHeight() const { return m_Height; }
SurfaceFormat const &
VideoDriverGLESv3::getScreenFormat() const { return m_ScreenFormat; }
int
VideoDriverGLESv3::getVSync() const { return m_VSync.curr; }
void
VideoDriverGLESv3::setVSync( int vsync ) { m_VSync.curr = vsync; }

void
VideoDriverGLESv3::setMSAA( int msaa )
{
   if ( msaa < 0 ) msaa = 0;
   if ( msaa > 16 ) msaa = 16;
   resizeMSAA( m_MSAA, getScreenWidth(), getScreenHeight(), msaa, this );
}

int
VideoDriverGLESv3::getMSAA() const { return m_MSAA.msaa; }

ICamera*
VideoDriverGLESv3::getActiveCamera() { return m_Camera; }

void
VideoDriverGLESv3::setActiveCamera( ICamera* camera )
{
   //SAFE_DELETE(m_Camera);
   m_Camera = camera;
}

ISceneManager const*
VideoDriverGLESv3::getSceneManager() const { return &m_SceneManager; }
ISceneManager*
VideoDriverGLESv3::getSceneManager() { return &m_SceneManager; }
// ###    GUIEnvironment   ###
UI const*
VideoDriverGLESv3::getUI() const { return &m_GUIEnvironment; }
UI*
VideoDriverGLESv3::getUI() { return &m_GUIEnvironment; }
// ### Input for Collision  ###
int32_t
VideoDriverGLESv3::getMouseX() const { return m_MouseX.curr; }
int32_t
VideoDriverGLESv3::getMouseY() const { return m_MouseY.curr; }
int32_t
VideoDriverGLESv3::getMouseMoveX() const { return m_MouseMoveX; }
int32_t
VideoDriverGLESv3::getMouseMoveY() const { return m_MouseMoveY; }
/*
void
VideoDriverGLESv3::setMousePos( int32_t x, int32_t y, bool isInit )
{
   if ( isInit )
   {
      m_MouseX = x;
      m_MouseY = y;
      m_MouseMoveX = 0;
      m_MouseMoveY = 0;
   }
   else
   {
      m_MouseX.curr = x;
      m_MouseY.curr = y;
   }
}
*/


// ###########################
// ###                     ###
// ###    StateManager     ###
// ###                     ###
// ###########################
void
VideoDriverGLESv3::setClearColor( glm::vec4 const & clearColor )
{
   m_ClearColor = clearColor;
}

State
VideoDriverGLESv3::getState() const
{
   return m_State.curr;
}

void
VideoDriverGLESv3::setState( State const & state )
{
   m_State.last = m_State.curr;
   m_State.curr = GLES::applyState( m_State.curr, state );
}
//void
//VideoDriverGLESv3::setCullTest( Culling const & neu )
//{
//   m_State.curr.culling = neu;
//   m_Culling.curr = GLES::applyCulling( m_Culling.curr, neu );
//}

//void
//VideoDriverGLESv3::setDepthTest( Depth const & neu )
//{
//   m_Depth.last = m_Depth.curr;
//   m_Depth.curr = GLES::applyDepth( m_Depth.curr, neu );
//}

//void
//VideoDriverGLESv3::setStencilTest( Stencil const & neu )
//{
//   m_Stencil.last = m_Stencil.curr;
//   m_Stencil.curr = GLES::applyStencil( m_Stencil.curr, neu );
//}

//void
//VideoDriverGLESv3::setBlendTest( Blend const & neu )
//{
//   m_Blend.last = m_Blend.curr;
//   m_Blend.curr = GLES::applyBlend( m_Blend.curr, neu );
//}

//void
//VideoDriverGLESv3::setLineWidth( LineWidth const & neu )
//{
//   m_LineWidth.last = m_LineWidth.curr;
//   //m_LineWidth.curr = GLES::applyLineWidth( m_LineWidth.curr, neu );
//}

//void
//VideoDriverGLESv3::setPointSize( PointSize const & neu )
//{
//   m_PointSize.curr = neu;
//   // Set in SMaterialRenderer, if any.
//   //m_PointSize.last = m_PointSize.curr;
//   //m_PointSize.curr = GLES::applyPointSize( m_PointSize.curr, neu );
//}

//void
//VideoDriverGLESv3::setPolygonOffset( PolygonOffset const & neu )
//{
//   m_PolygonOffset.last = m_PolygonOffset.curr;
//   m_PolygonOffset.curr = GLES::applyPolygonOffset( m_PolygonOffset.curr, neu );
//}

//void
//VideoDriverGLESv3::setRasterizerDiscard( RasterizerDiscard const & neu )
//{
//   m_RasterizerDiscard.last = m_RasterizerDiscard.curr;
//   m_RasterizerDiscard.curr = GLES::applyRasterizerDiscard( m_RasterizerDiscard.curr, neu );
//}

//void
//VideoDriverGLESv3::drawPrimitiveList( PrimitiveType::EType primType,
//    void const* vertices, uint32_t vCount, FVF const & fvf,
//    void const* indices, uint32_t iCount, IndexType::EType indexType )
//{
//   GLES::drawPrimitiveList( __func__, primType, vertices, vCount, fvf,  indices, iCount, indexType, m_IsDebug );
//}


//TexAtlas const* getTexAtlas() const { return &m_TexAtlas; }
//TexAtlas* getTexAtlas() { return &m_TexAtlas; }

//CollisionManager const* getCollisionManager() const { return &m_CollisionManager; }
//CollisionManager* getCollisionManager() { return &m_CollisionManager; }

// ###########################
// ###                     ###
// ###   TextureManager    ###
// ###                     ###
// ###########################
TexManager*
VideoDriverGLESv3::getTexManager()
{
   return &m_TexManager;
}
TexManager const*
VideoDriverGLESv3::getTexManager() const
{
   return &m_TexManager;
}

double
VideoDriverGLESv3::getTimer()
{
   m_Time.curr = dbSeconds();
   return m_Time.curr - m_Time.init;
}

int64_t
VideoDriverGLESv3::getFrameCount() const
{
   return m_FrameCount.curr;
}

float
VideoDriverGLESv3::getFPS() const
{
   return m_FPS;
}

// typ = 0 -> MeshBuffers per Frame
// typ = 1 -> Vertices per Frame
// typ = 2 -> Indices per Frame
// typ = 3 -> Primitives per Frame
// typ = 4 -> Total rendered Frames
// typ = 5 -> Total rendered MeshBuffers
// typ = 6 -> Total rendered Vertices
// typ = 7 -> Total rendered Indices
// typ = 8 -> Total rendered Primitives
uint64_t
VideoDriverGLESv3::getRenderStat( int typ ) const
{
   switch ( typ )
   {
      case 0: return m_MaterialRenderer.m_FrameBuffers;
      case 1: return m_MaterialRenderer.m_FrameVertices;
      case 2: return m_MaterialRenderer.m_FrameIndices;
      case 3: return m_MaterialRenderer.m_FramePrimitives;
      case 4: return m_MaterialRenderer.m_TotalFrames;
      case 5: return m_MaterialRenderer.m_TotalBuffers;
      case 6: return m_MaterialRenderer.m_TotalVertices;
      case 7: return m_MaterialRenderer.m_TotalIndices;
      case 8: return m_MaterialRenderer.m_TotalPrimitives;
      default: return 0;
   }
}

void
VideoDriverGLESv3::makeCurrent()
{
#ifdef USE_OPENGLES_AND_EGL
   //if ( !m_IsOpen ) { DE_WARN("Not open") return; }
   if ( m_eglContext )
   {
      if ( !m_eglDisplay ){ DE_WARN("Not display") return; }
      if ( !m_eglSurface ){ DE_WARN("Not surface") return; }
      if ( !m_eglContext ){ DE_WARN("Not context") return; }
      ::eglMakeCurrent( m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext );
      EGL_VALIDATE
   }
#endif
}


bool
VideoDriverGLESv3::is_open() const
{
   return m_IsOpen;
}

void
VideoDriverGLESv3::close()
{
   if ( !m_IsOpen ) { return; }  // Already closed.
   // dump();
   clearTextures(); // m_TexManager.clear();
   updateTextures();
   destroyMSAA( m_MSAA, this );
   clearShaders();
   //m_BufferManager.clear();
   //clearStates();
   m_SceneManager.clear();
   m_GUIEnvironment.clearWidgets();
   //SAFE_DELETE(m_Camera);

   //saveFonts();

#ifdef USE_OPENGLES_AND_EGL
   if ( m_eglContext )
   {
      EGL::destroyContext( m_eglDisplay, m_eglContext );
      EGL::destroySurface( m_eglDisplay, m_eglSurface );
      EGL::destroyDisplay( m_eglDisplay );
   }
#endif

   // clearMeshes();
//   if ( m_Window )
//   {
//      m_Window->setEventReceiver( nullptr ); // Prevent us from getting deleted.
//      if ( m_IsWindowOwner )
//      {
//         // m_IsWindowOwner = false;
//         delete m_Window;
//      }
//      m_Window = nullptr;
//   }

   m_IsOpen = false;
}

bool
VideoDriverGLESv3::open( int w, int h, uint64_t winHandle, SurfaceFormat const & fmt, bool debug )
{
   if ( m_IsOpen )
   {
      return false; // Already open
   }

// ###########################
// ###                     ###
// ###       Driver        ###
// ###                     ###
// ###########################
   m_Width = w;
   m_Height = h;
   m_Time = dbSeconds();
   m_FrameCount = 0;
   m_FPS = 0.0f;
   m_IsDebug = debug;
   m_ScreenFormat = fmt;
   m_WindowHandle = winHandle;
   //setMousePos( w/2,h/2, true );

#ifdef USE_OPENGLES_AND_EGL
   // [EGL]
   if ( m_WindowHandle )
   {
      DE_DEBUG("Create EGL stuff because winHandle != 0")

      // [EGL] Display
      m_eglDisplay = EGL::createDisplay();
      if ( !m_eglDisplay )
      {
         DE_ERROR("No EGL display");
      }

      // [EGL] Screen0
      m_eglSurface = EGL::createWindowSurface( m_eglDisplay,
                              m_Width, m_Height, m_WindowHandle,
                              m_ScreenFormat, &m_eglConfig );
      if ( !m_eglSurface )
      {
         DE_ERROR("No EGL surface")
      }

      // [EGL] Context
      m_eglContext = EGL::createContext( m_eglDisplay, m_eglSurface, m_eglConfig );
      if ( !m_eglContext )
      {
         DE_ERROR("No EGL context")
      }

      makeCurrent();
   }
#endif

   // [ShaderManager]
   m_VersionMajor = glGetIntegerDE( GL_MAJOR_VERSION );
   m_VersionMinor = glGetIntegerDE( GL_MINOR_VERSION );
   m_Shader = nullptr;
   m_ProgramId = 0;

   // [StateManager]
   m_DepthRange = GLES::queryDepthRange();
   m_State = GLES::queryState();
   //   m_Culling = GLES::queryCulling();
   //   m_Depth = GLES::queryDepth();
   //   m_Stencil = GLES::queryStencil();
   //   m_Blend = GLES::queryBlend();
   //   m_LineWidth = GLES::queryLineWidth();
   //   m_PointSize = GLES::queryPointSize();
   //   m_PolygonOffset = GLES::queryPolygonOffset();
   //   m_RasterizerDiscard = GLES::queryRasterizerDiscard();
   // [TexManager]
   m_TexManager.init();
   // [RTManager]
   // m_RTManager.init();
   // IRenderTarget* msaa = addRenderTexture( "aa4", 4 * w, 4 * h );
   // m_MSAA16 = reinterpret_cast< GLES_RenderTexture* >( msaa );
   // w = m_MSAA16->getWidth();
   // h = m_MSAA16->getHeight();
   // DE_DEBUG("Created RT [aa4] w(",w,"), h(",h,"), fmt(",fmt.toString(),")")
   if ( !createMSAA( m_MSAA, m_Width, m_Height, 0, this ) )
   {
      DE_ERROR("No MSAA 4x")
   }

   // [CameraManager]
   if ( !getActiveCamera() )
   {
      m_Camera = new ICamera();
   }

   // [SceneManager]
   if ( getSceneManager() )
   {
      getSceneManager()->init( w,h );
   }

   // m_StippleLineRenderer.init();
   m_Font5x8Renderer.setDriver( this );
   m_GUIEnvironment.setDriver( this );

   DE_DEBUG("[OpenGLES] ScreenSize = ", w, " x ", h )
   DE_DEBUG("[OpenGLES] ShaderVersion = ", m_VersionMajor, ".", m_VersionMinor )
   DE_DEBUG("[OpenGLES] GL_VERSION = ", (char*)glGetString( GL_VERSION ) )
   DE_DEBUG("[OpenGLES] GL_VENDOR = ", (char*)glGetString( GL_VENDOR ) )
   DE_DEBUG("[OpenGLES] GL_RENDERER = ", (char*)glGetString( GL_RENDERER ) )
   DE_DEBUG("[OpenGLES] GL_SHADING_LANGUAGE_VERSION = ", (char*)glGetString( GL_SHADING_LANGUAGE_VERSION ) )
   DE_DEBUG("[OpenGLES] TextureUnitCount = ", m_TexManager.getUnitCount() )
   DE_DEBUG("[OpenGLES] TextureUnpackAlignment = 1" )
   DE_DEBUG("[OpenGLES] DepthRange = ", m_DepthRange.curr.toString() )
   DE_DEBUG("[OpenGLES] State = ", m_State.curr.toString() )

   if ( m_IsDebug )
   {
      std::string extString = (char*)glGetString( GL_EXTENSIONS );
      DE_DEBUG("[OpenGLES] GL_EXTENSIONS = ", extString )
   #if 0
      auto extVector = dbStrSplit( extString, ' ' );
      DE_DEBUG("GL_EXTENSIONS = ", extVector.size() )
      for ( size_t i = 0; i < extVector.size(); ++i )
      {
         DE_DEBUG("GL_EXTENSION[",i,"] = ", extVector[ i ] )
      }
   #endif
      /*
      DE_DEBUG( "[Init] CullTest: ", m_Culling.curr.toString() )
      DE_DEBUG( "[Init] DepthTest: ", m_Depth.curr.toString() )
      DE_DEBUG( "[Init] StencilTest: ", m_Stencil.curr.toString() )
      DE_DEBUG( "[Init] BlendTest: ", m_Blend.curr.toString() )
      DE_DEBUG( "[Init] LineWidth: ", m_LineWidth.curr.toString() )
      DE_DEBUG( "[Init] PointSize: ", m_PointSize.curr.toString() )
      DE_DEBUG( "[Init] PolygonOffset: ", m_PolygonOffset.curr.toString() )
      DE_DEBUG( "[Init] RasterizerDiscard: ", m_RasterizerDiscard.curr.toString() )*/

      DE_DEBUG( "[Sizeof] State(", sizeof( State ), ")" )
      DE_DEBUG( "[Sizeof] = Viewport(", sizeof( Viewport ), ")" )
      DE_DEBUG( "[Sizeof] + Scissor(", sizeof( Scissor ), ")" )
      DE_DEBUG( "[Sizeof] + Clear(", sizeof( Clear ), ")" )
      DE_DEBUG( "[Sizeof] + DepthRange(", sizeof( DepthRange ), ")" )
      DE_DEBUG( "[Sizeof] + Culling(", sizeof( Culling ), ")" )
      DE_DEBUG( "[Sizeof] + Blend(", sizeof( Blend ), ")" )
      DE_DEBUG( "[Sizeof] + Depth(", sizeof( Depth ), ")" )
      DE_DEBUG( "[Sizeof] + Stencil(", sizeof( Stencil ), ")" )
      DE_DEBUG( "[Sizeof] + LineWidth(", sizeof( LineWidth ), ")" )
      DE_DEBUG( "[Sizeof] + PointSize(", sizeof( PointSize ), ")" )
      DE_DEBUG( "[Sizeof] + PolygonOffset(", sizeof( PolygonOffset ), ")" )
      DE_DEBUG( "[Sizeof] + RasterizerDiscard(", sizeof( RasterizerDiscard ), ")" )
      DE_FLUSH
   }


   m_IsOpen = true; // Can use VideoDriverGLESv3 now.
   return m_IsOpen;
}

void
VideoDriverGLESv3::postEvent( SEvent event )
{
   if ( event.type == EET_MOUSE_EVENT )
   {
      m_MouseX.curr = event.mouseEvent.x();
      m_MouseY.curr = event.mouseEvent.y();

      //DE_ERROR("MousePos(",m_MouseX.curr,",",m_MouseY.curr,")")

      if ( m_MouseX.curr != m_MouseX.last ) // Some button is clicked...
      {
         m_MouseMoveX = m_MouseX.curr - m_MouseX.last; // current mouse pos
         m_MouseX.last = m_MouseX.curr;
      }
      else
      {
         m_MouseMoveX = 0;
      }

      if ( m_MouseY.curr != m_MouseY.last )
      {
         m_MouseMoveY = m_MouseY.curr - m_MouseY.last; // current mouse pos
         m_MouseY.last = m_MouseY.curr;
      }
      else
      {
         m_MouseMoveY = 0;
      }
   }

   if ( getActiveCamera() )
   {
      getActiveCamera()->onEvent( event );
   }
   else
   {
      DE_ERROR("No camera")
   }

   m_GUIEnvironment.postEvent( event );
   m_SceneManager.postEvent( event );

}

void
VideoDriverGLESv3::resize( int32_t w, int32_t h )
{
   m_Width = w;
   m_Height = h;
   if ( getActiveCamera() )
   {
      getActiveCamera()->setScreenSize( w,h );
   }

   m_SceneManager.resize( w,h );
   //DE_DEBUG("w(",w,"), h(",h,")")
}

bool
VideoDriverGLESv3::beginRender()
{
   int w = getScreenWidth();
   int h = getScreenHeight();

   if ( m_MouseX.curr != m_MouseX.last )
   {
      m_MouseMoveX = m_MouseX.curr - m_MouseX.last;
   }

   if ( m_MouseY.curr != m_MouseY.last )
   {
      m_MouseMoveY = m_MouseY.curr - m_MouseY.last;
   }

//   if ( rt )
//   {
//      w = rt->getWidth();
//      h = rt->getHeight();
//   }


   if (m_MSAA.msaa > 0 )
   {
      // [FBO] Bind MSAA fbo.
      ::glBindFramebuffer( GL_FRAMEBUFFER, m_MSAA.fbo ); GL_VALIDATE

      // [Viewport] Entire MSAA texture?
      ::glViewport( 0, 0, m_MSAA.getWidth(), m_MSAA.getHeight() );

      if ( m_Camera )
      {
         m_Camera->setScreenSize( m_MSAA.getWidth(), m_MSAA.getHeight() );
      }

   }
   else
   {
      // [FBO] Entire screen.
      ::glBindFramebuffer( GL_FRAMEBUFFER, 0 ); GL_VALIDATE

      // [Viewport] Entire screen.
      ::glViewport( 0, 0, getScreenWidth(), getScreenHeight() );

      if ( m_Camera )
      {
         m_Camera->setScreenSize( getScreenWidth(), getScreenHeight() );
      }
   }

   // [Clear] Color always different to see better fps rate.
   //auto rainbow = de::RainbowColor::computeColor128( dbAbs( sin( 0.13 * dbSeconds() ) ) );
   ::glClearColor( m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a );
   ::glClearDepthf( 1.0f );
   ::glClearStencil( 0 );
   ::glClear( GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   m_Time.curr = dbSeconds();
   //return m_Time.curr - m_Time.init;
   //m_TimeFrameStart = m_Time;

   m_MaterialRenderer.beginFrame();
   if ( getActiveCamera() )
   {
      getActiveCamera()->onAnimate( getTimer() );
   }
   //m_Render3DMaterial.animate( driverTime );
   //return m_RTManager.beginRender( rt ); // manager eats what it gets.

   if ( getSceneManager() )
   {
      getSceneManager()->onFrameBegin();
   }

   return true;
}

// ===========================================================================
#pragma pack( push )
#pragma pack( 1 )

struct MSAA_Vertex
{
   glm::vec3 pos;
   glm::vec2 tex;    // 12 + 12 + 4 + 8 = 36 Bytes if packed

   MSAA_Vertex()
      : pos(), tex()
   {}

   MSAA_Vertex( float x, float y, float z, float u, float v )
      : pos( x,y,z ), tex( u,v )
   {}

   MSAA_Vertex( glm::vec3 const & p, glm::vec2 const & tex0 )
      : pos( p ), tex( tex0 )
   {}

   static FVF const &
   getFVF()
   {
      static FVF const fvf( sizeof( MSAA_Vertex ),
         {  VertexAttrib( VertexAttribType::F32, 3, VertexAttribSemantic::Pos ),
            VertexAttrib( VertexAttribType::F32, 2, VertexAttribSemantic::Tex )
         }
      );
      return fvf;
   }

   glm::vec3 const & getPos() const { return pos; }
   glm::vec3 & getPos() { return pos; }
   glm::vec2 const & getTexCoord( int stage = 0 ) const { return tex; }

   void setPos( glm::vec3 const & position ) { pos = position; }
   void setTexCoord( glm::vec2 const & texCoord, int stage = 0 ) { tex = texCoord; }

};

#pragma pack( pop )

void
VideoDriverGLESv3::endRender()
{
   m_MaterialRenderer.endFrame();

   if ( m_MSAA.msaa > 0 )
   {
      // [FBO] Entire screen.
      ::glBindFramebuffer( GL_FRAMEBUFFER, 0 ); GL_VALIDATE

      // [Viewport] Entire screen.
      ::glViewport( 0, 0, getScreenWidth(), getScreenHeight() ); GL_VALIDATE

      // [Clear] Color always different to see better fps rate.
      //auto rainbow = de::RainbowColor::computeColor128( dbAbs( sin( 0.13 * dbSeconds() ) ) );
      ::glClearColor( 0.f, 0.f, 0.f, 1.f ); GL_VALIDATE
      ::glClearDepthf( 1.f ); GL_VALIDATE
      ::glClearStencil( 0 ); GL_VALIDATE
      ::glClear( GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); GL_VALIDATE

      std::string name = "msaa";
      IShader* shader = getShader( name );
      if ( !shader )
      {
         std::stringstream vs;
         vs << R"(
            precision highp float;

            in vec3 a_pos;
            in vec2 a_tex;

            out vec2 v_tex;

            void main()
            {
               gl_Position = vec4( a_pos.x, a_pos.y, -1.0, 1.0 );
               v_tex = a_tex;
            }
         )";

         std::stringstream fs;
         fs << R"(
            precision highp float;

            uniform sampler2D u_tex;

            in vec2 v_tex;

            out vec4 color;

            void main()
            {
               color = texture( u_tex, v_tex );
            }
         )";

         shader = addShader( name, vs.str(), fs.str() );
      }

      if ( !shader )
      {
         DE_ERROR("No shader name ",name)
         //return false;
      }

      if ( !setActiveShader( shader ) )
      {
         DE_ERROR("No shader set, name ",name )
         //return false;
      }

      auto stage = bindTexture( m_MSAA.texR8G8B8A8 );
      if ( stage < 0 )
      {
         DE_ERROR("No tex unit activated")
         //return false;
      }

      if ( !shader->setUniformi( "u_tex", stage ) )
      {
         DE_ERROR("Sampler u_tex not activated" )
         //return false;
      }

      auto tex_aspect = double( m_MSAA.getWidth() ) / double( m_MSAA.getHeight() );
      auto qw = 2.0;
      auto qh = 2.0*tex_aspect;

      SMeshBuffer rect( PrimitiveType::Triangles );
      //rect.setTexture( 0, m_MSAA.texR8G8B8A8 );

      // B---C
      // |  /|
      // | / |
      // |/  |
      // A---D
      float z =  -1.0f;
      float x1 = -1.0f;
      float y1 = -1.0f;
      float x2 =  1.0f;
      float y2 =  1.0f;

      rect.Vertices.emplace_back( S3DVertex( x1,y1,z, 0,0,-1, 0xFFFFFFFF, 0,0 ) ); // A
      rect.Vertices.emplace_back( S3DVertex( x1,y2,z, 0,0,-1, 0xFFFFFFFF, 0,1 ) ); // B
      rect.Vertices.emplace_back( S3DVertex( x2,y2,z, 0,0,-1, 0xFFFFFFFF, 1,1 ) ); // C
      rect.Vertices.emplace_back( S3DVertex( x2,y1,z, 0,0,-1, 0xFFFFFFFF, 1,0 ) ); // D
      rect.Indices.emplace_back( 0 );
      rect.Indices.emplace_back( 1 );
      rect.Indices.emplace_back( 2 );  // Triangle ABC
      rect.Indices.emplace_back( 0 );
      rect.Indices.emplace_back( 2 );
      rect.Indices.emplace_back( 3 );  // Triangle ACD
      State state;
      state.depth = Depth::disabled();
      state.culling = Culling::disabled();
      //state.lineWidth.m_Now = getMSAA();
      setState( state );

      resetModelMatrix();
      drawPrimitiveList( rect );


      //draw2DRect( );
   }

#ifdef USE_OPENGLES_AND_EGL
   // [VSync]
   if ( m_eglSurface )
   {
      EGLBoolean ok;
      if ( m_VSync.last != m_VSync.curr )
      {
         ok = ::eglSwapInterval( m_eglDisplay, m_VSync.curr ); EGL_VALIDATE
         if ( ok != EGL_TRUE ) { DE_ERROR("Swap interval error") }
         m_VSync.last = m_VSync.curr;
      }

      ok = ::eglSwapBuffers( m_eglDisplay, m_eglSurface ); EGL_VALIDATE
   }
#endif

   if ( getSceneManager() )
   {
      getSceneManager()->onFrameEnd();
   }

   m_Time.curr = dbSeconds();
   double driverTime = m_Time.curr - m_Time.init;
   ++m_FrameCount.curr;

   if ( driverTime - m_TimeLastFpsUpdate >= 1.0 )
   {
      m_FPS = double( m_FrameCount.curr - m_FrameCount.last );
      m_FrameCount.last = m_FrameCount.curr;
      m_TimeLastFpsUpdate = driverTime;
   }

}

// ###########################
// ###                     ###
// ###    ShaderManager    ###
// ###                     ###
// ###########################
int32_t
VideoDriverGLESv3::getShaderVersionMajor() const { return m_VersionMajor; }

int32_t
VideoDriverGLESv3::getShaderVersionMinor() const { return m_VersionMinor; }

std::string
VideoDriverGLESv3::getShaderVersionHeader() const
{
   std::ostringstream s; s << "#version " << m_VersionMajor << m_VersionMinor << "0 es\n";
   return s.str();
}

void
VideoDriverGLESv3::clearShaders()
{
   for ( IShader* p : m_Shaders ) { if ( p ) delete p; }
   m_Shaders.clear();
   //m_Shader = nullptr;
   m_ProgramId = 0;
}


IShader*
VideoDriverGLESv3::getActiveShader() { return m_Shader; }

bool
VideoDriverGLESv3::setActiveShader( IShader* shader )
{
   GLuint programId = 0;
   if ( shader )
   {
      programId = shader->getProgramId();
   }

   m_Shader = shader;

   if ( m_ProgramId == programId )
   {
      //DE_DEBUG("No program change ", m_ProgramId)
      return true;
   }

   m_ProgramId = programId;

   ::glUseProgram( programId );
   bool ok = GL_VALIDATE;
   if ( ok )
   {
      //DE_DEBUG("Use programId = ", programId)
   }
   else
   {
      DE_ERROR("Cant use programId:", programId)
   }
   return ok;
}


int32_t
VideoDriverGLESv3::findShader( std::string const & name ) const
{
   if ( name.empty() ) return -1;
   for ( size_t i = 0; i < m_Shaders.size(); ++i )
   {
      IShader* p = m_Shaders[ i ];
      if ( p && p->getName() == name ) { return int32_t( i ); }
   }
   return -1;
}

IShader*
VideoDriverGLESv3::getShader( std::string const & name )
{
   int32_t found = findShader( name );
   if ( found < 0 ) return nullptr;
   return m_Shaders[ found ];
}

void
VideoDriverGLESv3::addShader( IShader* shader )
{
   if ( !shader ) { DE_ERROR("Got nullptr") return; }
   std::string const & name = shader->getName();
   int32_t found = findShader( name );
   if ( found >= 0 ) { DE_ERROR("Shader already cached(", name,").") return; }
   m_Shaders.push_back( shader );
   if ( m_IsDebug )
   {
      DE_DEBUG("[+] New managed Shader(", shader->getProgramId(), "), name(",name,")")
   }
}

IShader*
VideoDriverGLESv3::addShader( std::string const & name, std::string const & vs, std::string const & fs )
{
   int32_t found = findShader( name );
   if ( found > -1 ) { DE_ERROR( "Shader already exist at index. (",name,")" ) return nullptr; }
   std::ostringstream s_vs; s_vs << getShaderVersionHeader() << vs;
   std::ostringstream s_fs; s_fs << getShaderVersionHeader() << fs;
   //bool oldDebug = m_IsDebug;
   //m_IsDebug = true;
   Shader* shader = Shader::create( name, s_vs.str(), s_fs.str(), m_IsDebug );
   //m_IsDebug = oldDebug;
   if ( !shader ) { DE_ERROR( "No shader created. (",name,")" ) return nullptr; }
   if ( m_IsDebug )
   {
      DE_DEBUG("Created shader(", name, "), programId(", shader->getProgramId(), ")")
   }
   m_Shaders.push_back( shader );
   //setActiveShader( shader );
   return shader;
}

// ###########################
// ###                     ###
// ###   TextureManager    ###
// ###                     ###
// ###########################

bool
VideoDriverGLESv3::createScreenShot( Image & img )
{
   int w = getScreenWidth();
   int h = getScreenHeight();

   img.clear();
   img.setFormat( ColorFormat::RGBA8888 );
   img.resize( w,h ); // should also zero init data ( like a fill )

   std::stringstream s;
   s << "DarkFBO_ScreenShot_" << dbMilliseconds() << ".png";
   img.setUri( s.str() );

   GLenum texFormat = GL_RGBA;
   GLenum texType = GL_UNSIGNED_BYTE;
   if ( img.getFormat() == ColorFormat::RGB888 )
   {
      texFormat = GL_RGB;
      texType = GL_UNSIGNED_BYTE;
   }

//   glReadBuffer( GL_FRONT ); // GL_FRONT_LEFT, GL_FRONT_RIGHT, GL_AUX0+i
//   GL_VALIDATE

   glReadPixels( 0, 0, w, h, texFormat, texType, img.writePtr< void >() );
   GL_VALIDATE

   glReadBuffer( GL_BACK ); // GL_FRONT_LEFT, GL_FRONT_RIGHT, GL_AUX0+i
   GL_VALIDATE

   img.flipVertical();

   DE_DEBUG("Created screenshot ", img.toString() )
   return true;
}


} // end namespace gpu.
} // end namespace de.

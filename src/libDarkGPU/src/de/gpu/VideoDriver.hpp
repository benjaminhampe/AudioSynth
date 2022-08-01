#ifndef DE_GPU_GLESV3_VIDEODRIVER_V5_HPP
#define DE_GPU_GLESV3_VIDEODRIVER_V5_HPP

#include <de/gpu/IVideoDriver.hpp>
#include <de/gpu/MSAA.hpp>
#include <de/gpu/TexManager.hpp>
#include <de/gpu/SMaterialRenderer.hpp>
#include <de/gpu/ScreenRenderer.hpp>

#ifdef USE_LIB_DARKFONT
#include <de/gpu/FontRenderer.hpp>
#endif

#include <de/gpu/PerfOverlay.hpp>
#include <de/gpu/Draw2DDemo.hpp>
//#include <de_opengl.hpp>
//#include <de/gpu/CollisionManager.hpp>

namespace de {
namespace gpu {

//===========================================================
struct VideoDriverGLESv3 : public IVideoDriver
//===========================================================
{
   DE_CREATE_LOGGER( "de.gpu.VideoDriverGLESv3" )
   bool m_IsOpen;
   bool m_IsDebug;
   int32_t m_Width;    // Screen Width
   int32_t m_Height;   // Screen Height
   // Camera control
   std::array< bool, 256 > m_KeyStates;
   SM3< int32_t > m_MouseX;   // Mouse Pos X
   SM3< int32_t > m_MouseY;   // Mouse Pos Y
   int32_t m_MouseMoveX;
   int32_t m_MouseMoveY;
   bool m_IsDragging;
   bool m_LMB; // left mb pressed
   bool m_RMB;
   bool m_MMB;
   SurfaceFormat m_ScreenFormat;
   MSAA_Combined_DepthStencil_Target_Info m_MSAA;
   uint64_t m_WindowHandle;
   EGLDisplay m_eglDisplay;   ///< Selected display - Currently used EGL display
   EGLConfig m_eglConfig;   ///< Selected config - Currently used EGL config
   EGLSurface m_eglSurface;   ///< Selected surface - Currently used EGL surface
   EGLContext m_eglContext;   ///< Selected context - Currently used EGL context
   SM3< int32_t > m_VSync;    // Screen VSync - Vertical sync determined by [SwapMin,SwapMax] using EGL ( which is very flexible in this regard ). 0 - disabled ( singlebuffer, high CPU usage ), 1 - DoubleBuffer, 2 - TripleBuffer, 3 - etc. in EGL [SwapMin,SwapMax] range
   SM3< double > m_Time;
   SM2< int64_t > m_FrameCount;
   float m_FPS;
   double m_TimeLastFpsUpdate;
   glm::vec4 m_ClearColor;          // ###    StateManager     ###
   SM3< DepthRange > m_DepthRange;  // ###    StateManager     ###
   SM3< State > m_State;            // ###    StateManager     ###
   /*
   SM3< Viewport > m_Viewport; // ScreenViewport, r=x, g=y, b=width, a=height
   SM3< Clear > m_Clear;       // ScreenClear
   SM3< Culling > m_Culling;
   SM3< Depth > m_Depth;
   SM3< Stencil > m_Stencil;
   SM3< Blend > m_Blend;
   SM3< LineWidth > m_LineWidth;
   SM3< PointSize > m_PointSize;
   SM3< PolygonOffset > m_PolygonOffset;
   SM3< RasterizerDiscard > m_RasterizerDiscard;
   */
   int32_t m_VersionMajor;          // ###  ShaderManager  ###
   int32_t m_VersionMinor;          // ###  ShaderManager  ###
   IShader* m_Shader;               // ###  ShaderManager  ###
   GLuint m_ProgramId;              // ###  ShaderManager  ###
   std::vector< IShader* > m_Shaders;// ###  ShaderManager  ###
   TexManager m_TexManager;   // ###  TexManager  ###
   glm::dmat4 m_ModelMatrix;   // ###  CameraManager  ###
   ICamera* m_Camera;   // ###  CameraManager  ###
   SMaterialRenderer m_MaterialRenderer;     // 3D material renderer ( textured + lighting and fog, etc.. )
   ScreenRenderer m_ScreenRenderer;          // 2D material ( textured, but no lighting and fog )
   Font5x8Renderer m_Font5x8Renderer;        // 2D 5x8 lcd matrix font ( max. 40 quads per glyph, ca. 130 ascii glyphs )
#ifdef USE_LIB_DARKFONT
   FontRenderer m_FontRenderer;              // 2D truetype font renderer ( textured quad per glyph, tex atlas approach )
#endif
   PerfOverlay m_PerfOverlay;                // 2D TextOverlay
   Draw2DDemo m_Demo2D;
   UI m_GUIEnvironment;
   // Water3DRenderer m_Water3DRenderer;
   //CollisionManager m_CollisionManager;
   // LineRenderer m_LineRenderer;
   // StippleLineRenderer m_StippleLineRenderer;
   ISceneManager m_SceneManager;
   // GUIEnvironment m_GUIEnvironment;

// ###########################
// ###                     ###
// ###       Driver        ###
// ###                     ###
// ###########################
public:
   VideoDriverGLESv3();
   ~VideoDriverGLESv3() override;

   bool isDebug() const override;
   void setDebug( bool debug ) override;
   bool is_open() const override;
   void close() override;
   bool open( int w, int h, uint64_t winHandle = 0, SurfaceFormat const & fmt = SurfaceFormat(), bool debug = false ) override;
   void resize( int w, int h ) override;
   bool beginRender() override;
   void endRender() override;
   void makeCurrent() override;
   int32_t getScreenWidth() const override;
   int32_t getScreenHeight() const override;
   SurfaceFormat const & getScreenFormat() const override;
   int getVSync() const override;
   void setVSync( int vsync ) override;
   void setMSAA( int msaa ) override;
   int getMSAA() const override;
   bool createScreenShot( Image & img ) override;
   void postEvent( SEvent event ) override;
   double getTimer() override;
   int64_t getFrameCount() const override;
   float getFPS() const override;

   // typ = 0 -> MeshBuffers per Frame
   // typ = 1 -> Vertices per Frame
   // typ = 2 -> Indices per Frame
   // typ = 3 -> Primitives per Frame
   // typ = 4 -> Total rendered Frames
   // typ = 5 -> Total rendered MeshBuffers
   // typ = 6 -> Total rendered Vertices
   // typ = 7 -> Total rendered Indices
   // typ = 8 -> Total rendered Primitives
   uint64_t getRenderStat( int typ = 0 ) const override;
   // ###    CameraManager    ###
   ICamera* getActiveCamera() override;   // ###    CameraManager    ###
   void setActiveCamera( ICamera* camera ) override;   // ###    CameraManager    ###
   ISceneManager const* getSceneManager() const override;   // ###    SceneManager     ###
   ISceneManager* getSceneManager() override;   // ###    SceneManager     ###
   //setSceneManager( ISceneManager* scene ) { return &m_SceneManager; }
   UI const* getUI() const override;   // ###    GUIEnvironment   ###
   UI* getUI() override;   // ###    GUIEnvironment   ###

   int32_t getMouseX() const override; // ### Input for Collision  ###
   int32_t getMouseY() const override; // ### Input for Collision  ###
   int32_t getMouseMoveX() const override;
   int32_t getMouseMoveY() const override;
   //void setMousePos( int32_t x, int32_t y, bool isInit = false ) override; // ### Input for Collision  ###
// ###########################
// ###                     ###
// ###    StateManager     ###
// ###                     ###
// ###########################
   void setClearColor( glm::vec4 const & clearColor ) override; // ###    StateManager     ###
   State getState() const override;
   void setState( State const & state ) override;
//   void setCullTest( Culling const & culling );
//   void setDepthTest( Depth const & depth );
//   void setStencilTest( Stencil const & stencil );
//   void setBlendTest( Blend const & blend );
//   void setLineWidth( LineWidth const & neu );
//   void setPointSize( PointSize const & neu );
//   void setPolygonOffset( PolygonOffset const & neu );
//   void setRasterizerDiscard( RasterizerDiscard const & neu );
//   void drawPrimitiveList( PrimitiveType::EType primType,
//             void const* vertices, uint32_t vCount, FVF const & fvf,
//             void const* indices = nullptr, uint32_t iCount = 0,
//             IndexType::EType indexType = IndexType::U32 ) override;

// ###########################
// ###                     ###
// ###   TextureManager    ###
// ###                     ###
// ###########################
   TexManager* getTexManager() override;
   TexManager const * getTexManager() const override;

// ###########################
// ###                     ###
// ###     ModelMatrix     ###
// ###                     ###
// ###########################
   glm::dmat4 const& getModelMatrix() const override { return m_ModelMatrix; }
   glm::dmat4& getModelMatrix() override { return m_ModelMatrix; }
   void setModelMatrix( glm::dmat4 const & modelMat ) override
   {
      m_ModelMatrix = modelMat;
   }
   void resetModelMatrix() override { m_ModelMatrix = glm::dmat4( 1.0 ); }


   void
   draw2DDemo() override
   {
      m_Demo2D.setDriver( this );
      m_Demo2D.draw();
   }


// ###########################
// ###                     ###
// ###   FontRenderer5x8   ###
// ###                     ###
// ###########################
   glm::ivec2
   getTextSize5x8( std::string const & msg, Font5x8 const & font = Font5x8() ) override
   {
      return m_Font5x8Renderer.getTextSize( msg, font );
   }
   void
   draw2DText5x8( int x, int y, std::string const & msg, uint32_t color = 0xFF000000,
               Align::EAlign align = Align::Default, Font5x8 const & font = Font5x8() ) override
   {
      m_Font5x8Renderer.draw2DText( x, y, msg, color, align, font );
   }

#ifdef USE_LIB_DARKFONT
   // ###########################
   // ###                     ###
   // ###     FontRenderer    ###
   // ###                     ###
   // ###########################
   void draw2DText( int x, int y, std::wstring const & msg, uint32_t color = 0xFF000000,
               Align::EAlign align = Align::Default, Font const & font = Font() ) override
   {
      m_FontRenderer.draw2DText( x, y, msg, color, align, font );
   }
#endif
// ###########################
// ###                     ###
// ###     PerfOverlay     ###
// ###                     ###
// ###########################

   void draw2DPerfOverlay5x8() override { m_PerfOverlay.draw2DPerfOverlay5x8(); }
//   void draw2DPerfOverlay() override { m_PerfOverlay.draw2DPerfOverlay(); }


// ###########################
// ###                     ###
// ###    ShaderManager    ###
// ###                     ###
// ###########################
   int32_t getShaderVersionMajor() const override;
   int32_t getShaderVersionMinor() const override;
   std::string getShaderVersionHeader() const override;
   void clearShaders() override;
   IShader* getActiveShader() override;
   IShader* getShader( std::string const & name ) override;
   IShader* addShader( std::string const & name, std::string const & vs, std::string const & fs ) override;
   int32_t findShader( std::string const & name ) const override;
   bool setActiveShader( IShader* shader ) override;
   void addShader( IShader* shader ) override;

// ###############################################################
// ###                                                         ###
// ###  Geometry class independent 2D/3D material renderering  ###
// ###                                                         ###
// ###############################################################
   void
   drawPrimitiveList( IMeshBuffer & buf ) override
   {
      if ( !buf.isVisible() ) return;
      buf.draw();
   }

   void
   draw2D( IMeshBuffer & buf ) override
   {
      if ( !buf.isVisible() ) return;
      m_ScreenRenderer.setMaterial( buf.getMaterial() );
      buf.draw();
      m_ScreenRenderer.unsetMaterial( buf.getMaterial() );
   }

   void
   draw3D( IMeshBuffer & buf ) override
   {
      if ( !buf.isVisible() ) return;
      m_MaterialRenderer.setMaterial( buf.getMaterial() );
      buf.draw();
      m_MaterialRenderer.unsetMaterial( buf.getMaterial() );
   }

   void
   draw3D( IMeshBuffer & buf, std::vector< glm::mat4 > const & instances ) override
   {
      if ( !buf.isVisible() ) return;
      m_MaterialRenderer.setMaterial( buf.getMaterial() );
      buf.draw();
      m_MaterialRenderer.unsetMaterial( buf.getMaterial() );
   }

// ###########################
// ###                     ###
// ###   ScreenRenderer    ###
// ###                     ###
// ###########################
   void
   draw2DLine( float x1, float y1, float x2, float y2,
               uint32_t color, float z = 0.0f ) override
   {
      draw2DLine( x1, y1, x2, y2, color, color, z );
   }

   void
   draw2DLine( float x1, float y1, float x2, float y2,
               uint32_t colorA, uint32_t colorB, float z = 0.0f ) override
   {
      SMeshBuffer o( PrimitiveType::Lines );
      o.Name = "Driver::draw2DLine";
      o.setLighting( 0 );
      if ( RGBA_A( colorA ) == 255 && RGBA_A( colorB ) == 255 )
      {
         o.setBlend( Blend::disabled() );
      }
      else
      {
         o.setBlend( Blend::alphaBlend() );
      }
      o.Vertices.reserve( 2 );
      o.Vertices.push_back( S3DVertex( x1, y1, z, 0.f, 0.f, 0.f, colorA, 0,0 ) ); // A
      o.Vertices.push_back( S3DVertex( x2, y2, z, 0.f, 0.f, 0.f, colorB, 0,0 ) ); // B
      draw2D( o );
   }

   //
   //  B +-----+ C --> +x, +u
   //    |   / |
   //    | /   |     FrontQuad ABCD (cw) = ABC (cw) + ACD (cw)
   //  A +-----+ D
   //    |           Normal -z shows towards viewer
   //   +v = -y      Pos.z is always -1, so its at near plane.
   //

   void
   draw2DRect(
         Recti const & pos,
         uint32_t colorA, uint32_t colorB, uint32_t colorC, uint32_t colorD,
         TexRef const & ref = TexRef() ) override
   {
      if( pos.getWidth() < 1 ) { return; }
      if( pos.getHeight() < 1 ) { return; }
      SMeshBuffer o;
      smesh::S2DRect::add( o, pos, colorA, colorB, colorC, colorD, ref );
      o.Name = "Driver::draw2DRect";
      o.setCulling( false );
      draw2D( o );
   }

   void
   draw2DRect(
         Recti const & pos,
         uint32_t color = 0xFFFFFFFF,
         TexRef const & ref = TexRef() ) override
   {
      draw2DRect( pos, color, color, color, color, ref );
   }

   void
   draw2DRectOutline(
         Recti const & pos,
         uint32_t colorA, uint32_t colorB, uint32_t colorC, uint32_t colorD,
         TexRef const & ref = TexRef(), int borderWidth = 1 ) override
   {
      if( pos.getWidth() < 1 ) { return; }
      if( pos.getHeight() < 1 ) { return; }
      SMeshBuffer o;
      glm::vec2 A(pos.x,pos.y);
      glm::vec2 B(pos.x,pos.y);
      glm::vec2 C(pos.x,pos.y);
      glm::vec2 D(pos.x,pos.y);
      smesh::PolyLine::addExtrudedLine( o, A,B, colorA, colorB, borderWidth );
      smesh::PolyLine::addExtrudedLine( o, B,C, colorB, colorC, borderWidth );
      smesh::PolyLine::addExtrudedLine( o, C,D, colorC, colorD, borderWidth );
      smesh::PolyLine::addExtrudedLine( o, D,A, colorD, colorA, borderWidth );
      o.Name = "Driver::draw2DRectOutline";
      o.setCulling( false );
      if ( ref.tex ) o.setTexture( 0, ref );
      draw2D( o );
   }

   void
   draw2DRectOutline(
         Recti const & pos,
         uint32_t color = 0xFFFFFFFF,
         TexRef const & ref = TexRef(), int borderWidth = 1 ) override
   {
      if( pos.getWidth() < 1 ) { return; }
      if( pos.getHeight() < 1 ) { return; }
      SMeshBuffer o;
      smesh::S2DRectOutline::add( o, pos, color, ref, borderWidth );
      o.Name = "Driver::draw2DRectOutline";
      o.setCulling( false );
      draw2D( o );
   }

   void
   draw2DRoundRect(
         Recti const & pos, float rx, float ry,
         uint32_t color = 0xFFFFFFFF,
         TexRef const & tex = TexRef(), int tess = 13  ) override
   {
      int w = pos.getWidth();
      int h = pos.getHeight();
      if ( w < 3 ) { return; }
      if ( h < 3 ) { return; }
      SMeshBuffer o;
      smesh::S2DRoundRect::add( o, pos, glm::ivec2(rx,ry), color, tex, tess );
      o.Name = "Driver::draw2DRoundRect";
      o.setCulling( false );
      draw2D( o );
   }

   void
   draw2DRoundRectOutline(
      Recti const & pos, float rx, float ry,
      uint32_t color = 0xFFFFFFFF,
      TexRef const & tex = TexRef(), int borderWidth = 1, int tess = 13  ) override
   {
      int w = pos.getWidth();
      int h = pos.getHeight();
      if ( w < 3 ) { return; }
      if ( h < 3 ) { return; }
      SMeshBuffer o;
      smesh::S2DRoundRectOutline::add( o, pos, glm::ivec2(rx,ry), color, tex, tess );
      o.Name = "Driver::draw2DRoundRectOutline";
      o.setCulling( false );
      draw2D( o );
   }

   void
   draw2DCircle( Recti const & pos,
                 uint32_t color = 0xFFFFFFFF,
                 TexRef const & tex = TexRef(), int tess = 36 ) override
   {
      int w = pos.getWidth();
      int h = pos.getHeight();
      if ( w < 1 ) { return; }
      if ( h < 1 ) { return; }
      SMeshBuffer o;
      o.Name = "Driver::draw2DCircle";
      smesh::S2DCircle::add( o, pos, color, tex, tess );
      draw2D( o );
   }

   void
   draw2DCircleOutline( Recti const & pos, uint32_t color = 0xFFFFFFFF,
                        TexRef const & tex = TexRef(), int borderWidth = 1, int tess = 36 ) override
   {
      int w = pos.getWidth();
      int h = pos.getHeight();
      if ( w < 1 ) { return; }
      if ( h < 1 ) { return; }
      SMeshBuffer o;
      o.Name = "Driver::draw2DCircleOutline";
      smesh::S2DCircleOutline::add( o, pos, color, tex, borderWidth, tess );
      draw2D( o );
   }


// ###########################
// ###                     ###
// ### RenderTargetManager ###
// ###                     ###
// ###########################
   // Clear const & getClear() const { return m_RTManager.getClear(); }
   // void setClear( Clear clear ) { m_RTManager.setClear( clear ); }
   // void setClearColor( uint32_t color ) { m_RTManager.setClearColor( color ); }
   // void setClearColor( glm::vec4 const & color ) { m_RTManager.setClearColor( color ); }
   // Viewport const & getViewport() const { return m_RTManager.getViewport(); }
   // void setViewport( Viewport viewport ) { m_RTManager.setViewport( viewport ); }
   // void drawPrimitiveList( PrimitiveType::EType primType,
            // void const* vertices, uint32_t vCount, FVF const & fvf,
            // void const* indices = nullptr, uint32_t iCount = 0,
            // IndexType::EType indexType = IndexType::U32 )
   // {
      // GLES::drawPrimitiveList( __func__, primType,
               // vertices, vCount, fvf,  indices, iCount, indexType );
   // }
   // void clearRenderTargets();
   // void removeRenderTarget( IRenderTarget* tex );
   // void removeRenderTarget( std::string const & name );
   // IRenderTarget* addRenderWindow( std::string const & name, os::IWindow* win, SurfaceFormat const & fmt = SurfaceFormat() );
   // IRenderTarget* addRenderTexture( std::string const & name, int w, int h, SurfaceFormat const & fmt = SurfaceFormat() );
   //   bool setActiveTarget( IRenderTarget* rt )
   //   {
   //      if ( !rt )
   //      {
   //         DE_ERROR("No rt")
   //         return false;
   //      }

   //      if ( !m_RTManager.setActiveTarget( rt ) )
   //      {
   //         DE_ERROR("No active rt")
   //         return false;
   //      }

   //      int w = rt->getWidth();
   //      int h = rt->getHeight();
   //      m_SceneManager->resize( w, h );
   //      return true;
   //   }
   //   bool setActiveTarget( std::string const & name )
   //   {
   //      if ( !m_RTManager.setActiveTarget( name ) )
   //      {
   //         return false;
   //      }
   //      return true;
   //   }
   // IRenderTarget* getActiveTarget() { return m_RTManager.getActiveTarget(); }
   // IRenderTarget* getRenderTarget( std::string const & name ) { return m_RTManager.getRenderTarget( name ); }


   // // ### ISceneManager ###
   // ISceneManager const* getSceneManager() const { return m_SceneManager; }
   // ISceneManager* getSceneManager() { return m_SceneManager; }

   // void clearCameras() { m_SceneManager->clearCameras(); }
   // int32_t findCamera( ICamera* camera ) const { return m_SceneManager->findCamera( camera ); }
   // ICamera* getActiveCamera() { return m_SceneManager->getActiveCamera(); }
   // ICamera const* getActiveCamera() const { return m_SceneManager->getActiveCamera(); }
   // bool setActiveCamera( ICamera* camera ) { return m_SceneManager->setActiveCamera( camera ); }
   // ICamera* addCamera( std::string const & name, bool makeActive = true ) { return m_SceneManager->addCamera( name, makeActive ); }
   // void addCamera( ICamera* camera, bool makeActive = true ) { m_SceneManager->addCamera( camera, makeActive ); }

   // IGUIEnvironment* getGUIEnvironment() { return &m_GUIEnvironment; }
   // IGUIEnvironment const * getGUIEnvironment() const { return &m_GUIEnvironment; }

   // FontRenderer* getFontRenderer() { return &m_FontRenderer; }
   // LineRenderer* getLineRenderer() { return &m_LineRenderer; }
   // StippleLineRenderer* getStippleLineRenderer() { return &m_StippleLineRenderer; }

//   void
//   draw3DWater( float sx, float sz, float x = 0.0f, float y = 0.0f, float z = 0.0f );

};


} // end namespace gpu.
} // end namespace de.

#endif


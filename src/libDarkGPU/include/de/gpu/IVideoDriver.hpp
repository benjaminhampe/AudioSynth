#ifndef DE_GPU_IVIDEODRIVER_HPP
#define DE_GPU_IVIDEODRIVER_HPP

#include <de/gpu/TRS.hpp>
#include <de/gpu/FVF.hpp>
#include <de/gpu/State.hpp>
#include <de/gpu/Tex.hpp>
#include <de/gpu/TexManager.hpp>
#include <de/gpu/Shader.hpp>
#include <de/gpu/scene/ICamera.hpp>

#ifndef USE_SMESH_GEOMETRY
#define USE_SMESH_GEOMETRY
#endif

#ifdef USE_SMESH_GEOMETRY

#include <de/gpu/smesh/SMesh.hpp>
#include <de/gpu/smesh/SMeshIO.hpp>
#include <de/gpu/smesh/Box.hpp>
#include <de/gpu/smesh/Line.hpp>
#include <de/gpu/smesh/LineBox.hpp>
#include <de/gpu/smesh/LineCircle.hpp>
#include <de/gpu/smesh/LineSphere.hpp>
#include <de/gpu/smesh/Rect.hpp> // Quad
#include <de/gpu/smesh/RoundRect.hpp>
#include <de/gpu/smesh/Hexagon.hpp>
#include <de/gpu/smesh/Hillplane.hpp>
#include <de/gpu/smesh/Circle.hpp>
#include <de/gpu/smesh/Cone.hpp>
#include <de/gpu/smesh/Cylinder.hpp>
#include <de/gpu/smesh/PolyHull.hpp>
#include <de/gpu/smesh/Sphere.hpp>
#include <de/gpu/smesh/SphereLowerHalf.hpp>
#include <de/gpu/smesh/SphereUpperHalf.hpp>
#include <de/gpu/smesh/Ring.hpp>
#include <de/gpu/smesh/Debug.hpp>
#include <de/gpu/smesh/Geometry.hpp>
#include <de/gpu/smesh/S2DShapes.hpp>
#include <de/gpu/smesh/Tube.hpp>

#include <de/gpu/Font5x8Renderer.hpp>
#include <de/gpu/BillboardRenderer.hpp>
//#include <de/gpu/Generic3DSkyBox.hpp>
//#include <de/gpu/CollisionManager.hpp>
#include <de/gpu/scene/SceneManager.hpp>
#include <de/gpu/ui/UI.hpp>

#endif

namespace de {
namespace gpu {

//===========================================================
struct IVideoDriver
//===========================================================
{
//   typedef glm::vec2 V2f;
//   typedef glm::vec3 V3f;
//   typedef glm::vec4 V4f;
//   typedef glm::dvec2 V2d;
//   typedef glm::dvec3 V3d;
//   typedef glm::dvec4 V4d;
//   typedef glm::ivec2 V2i;
//   typedef glm::ivec3 V3i;
//   typedef glm::ivec4 V4i;
//   typedef glm::mat3 M3f;
//   typedef glm::mat4 M4f;
//   typedef glm::dmat3 M3d;
//   typedef glm::dmat4 M4d;

   // ###########################
   // ###                     ###
   // ###     DriverHeart     ###
   // ###                     ###
   // ###########################
   virtual ~IVideoDriver() {}

   virtual bool isDebug() const = 0;
   virtual void setDebug( bool debug ) = 0;
   // Close Gpu accelerated device, closes EGL if was used.
   // GLFW does EGL itself.
   virtual void close() = 0;
   // Open Gpu accelerated device, open EGL ourself if winHandle != 0.
   // GLFW does EGL itself and get 0 as window handle.
   // The driver has a rich standard material shader system
   // The driver creates one screen surface on the given window
   // The driver produces MSAA up to native max tex size
   // MSAA = 0 -> no Supersampling AA, just the screen as surface
   // MSAA = 1 -> blends offscreen tex with same size on screen surface
   // MSAA = 2 -> blends offscreen tex with 2x2 screen size on screen surface
   // MSAA = 3 -> blends offscreen tex with 3x3 screen size on screen surface
   // MSAA = 4 -> blends offscreen tex with 4x4 screen size on screen surface
   // if we have 1920x1080 screen and max 16*1024 tex then we support and vsync
   virtual bool open( int w, int h, uint64_t winHandle = 0, SurfaceFormat const & fmt = SurfaceFormat(), bool debug = false );
   virtual bool is_open() const = 0;

   virtual void setVSync( int vsync ) = 0;
   virtual int getVSync() const = 0;

   virtual void setMSAA( int msaa ) = 0;
   virtual int getMSAA() const = 0;
   virtual bool createScreenShot( Image & img ) = 0;

   virtual void resize( int w, int h ) = 0;
   virtual bool beginRender() = 0;
   virtual void endRender() = 0;
   virtual void makeCurrent() = 0;

   virtual void postEvent( SEvent event ) = 0;

   // ###########################
   // ###                     ###
   // ###       Screen        ###
   // ###                     ###
   // ###########################
   virtual int32_t getScreenWidth() const = 0;
   virtual int32_t getScreenHeight() const = 0;
   virtual SurfaceFormat const & getScreenFormat() const = 0;
   // ###########################
   // ###                     ###
   // ###     DriverHeart     ###
   // ###                     ###
   // ###########################
   virtual double getTimer() = 0;
   virtual int64_t getFrameCount() const = 0;
   virtual float getFPS() const = 0;
   // ###########################
   // ###                     ###
   // ###     DriverStats     ###
   // ###                     ###
   // ###########################
   // typ = 0 -> MeshBuffers per Frame
   // typ = 1 -> Vertices per Frame
   // typ = 2 -> Indices per Frame
   // typ = 3 -> Primitives per Frame
   // typ = 4 -> Total rendered Frames
   // typ = 5 -> Total rendered MeshBuffers
   // typ = 6 -> Total rendered Vertices
   // typ = 7 -> Total rendered Indices
   // typ = 8 -> Total rendered Primitives
   virtual uint64_t getRenderStat( int typ = 0 ) const;
   // ###########################
   // ###                     ###
   // ###      MouseInput     ###
   // ###                     ###
   // ###########################
   virtual int32_t getMouseX() const = 0;
   virtual int32_t getMouseY() const = 0;
   virtual int32_t getMouseMoveX() const = 0;
   virtual int32_t getMouseMoveY() const = 0;
   //virtual void setMousePos( int32_t x, int32_t y, bool isInit = false ) = 0;
   // ###########################
   // ###                     ###
   // ###    CameraManager    ###
   // ###                     ###
   // ###########################
   virtual ICamera* getActiveCamera() = 0;
   virtual void setActiveCamera( ICamera* camera ) = 0;
   // ###########################
   // ###                     ###
   // ###    ISceneManager    ###
   // ###                     ###
   // ###########################
   virtual ISceneManager const* getSceneManager() const = 0;
   virtual ISceneManager* getSceneManager() = 0;
   // ###########################
   // ###                     ###
   // ###    IGUIEnvironment  ###
   // ###                     ###
   // ###########################
   virtual UI const* getUI() const = 0;
   virtual UI* getUI() = 0;

//   virtual TexAtlas const* getTexAtlas() const = 0;
//   virtual TexAtlas* getTexAtlas() = 0;

   // ###########################
   // ###                     ###
   // ###     ModelMatrix     ###
   // ###                     ###
   // ###########################
   virtual glm::dmat4 const& getModelMatrix() const = 0;
   virtual glm::dmat4& getModelMatrix() = 0;
   virtual void setModelMatrix( glm::dmat4 const & modelMat ) = 0;
   virtual void resetModelMatrix() = 0;

   // ###########################
   // ###                     ###
   // ###    StateManager     ###
   // ###                     ###
   // ###########################
   virtual void setClearColor( glm::vec4 const & clearColor ) = 0;
   virtual void setClearColor( float r, float g, float b, float a = 1.0f )
   {
      setClearColor( glm::vec4{ r,g,b,a } );
   }
   virtual State getState() const = 0;
   virtual void setState( State const & state ) = 0;
//   virtual void setCullTest( Culling const & culling ) = 0;
//   virtual void setDepthTest( Depth const & depth ) = 0;
//   virtual void setStencilTest( Stencil const & stencil ) = 0;
//   virtual void setBlendTest( Blend const & blend ) = 0;
//   virtual void setLineWidth( LineWidth const & neu ) = 0;
//   virtual void setPointSize( PointSize const & neu ) = 0;
//   virtual void setPolygonOffset( PolygonOffset const & neu ) = 0;
//   virtual void setRasterizerDiscard( RasterizerDiscard const & neu ) = 0;

   // ###########################
   // ###                     ###
   // ###    ShaderManager    ###
   // ###                     ###
   // ###########################
   virtual int32_t getShaderVersionMajor() const = 0;
   virtual int32_t getShaderVersionMinor() const = 0;
   virtual std::string getShaderVersionHeader() const = 0;
   virtual void clearShaders() = 0;
   virtual IShader* getActiveShader() = 0;
   virtual bool setActiveShader( IShader* shader ) = 0;
   virtual int32_t findShader( std::string const & name ) const = 0;
   virtual IShader* getShader( std::string const & name ) = 0;
   virtual void addShader( IShader* shader ) = 0;
   virtual IShader* addShader( std::string const & name, std::string const & vs, std::string const & fs ) = 0;

//   // ###########################
//   // ###                     ###
//   // ###    BufferManager    ###
//   // ###                     ###
//   // ###########################
//   virtual GpuBufferManager* getBufferManager() = 0;
//   virtual GpuBufferManager const * getBufferManager() const = 0;

   // ###########################
   // ###                     ###
   // ###   TextureManager    ###
   // ###                     ###
   // ###########################
   virtual TexManager* getTexManager() = 0;
   virtual TexManager const * getTexManager() const = 0;

   // Driver offers all functionality and acts itself as tex manager.
   virtual void dumpTextures()
   { getTexManager()->dump(); }
   virtual void clearTextures()
   { getTexManager()->clearTextures(); }
   virtual void updateTextures()
   { getTexManager()->updateTextures(); }
   virtual void removeTexture( Tex* tex )
   { getTexManager()->removeTexture( tex ); }
   virtual void removeTexture( std::string const & name )
   { getTexManager()->removeTexture( name ); }
   virtual bool hasTexture( std::string const & name ) const
   { return getTexManager()->hasTex( name ); }
   virtual TexRef getTexture( std::string const & name )
   { return getTexManager()->getTex( name ); }
   virtual TexRef addTexture2D( std::string const & name, Image const & img,
                          bool keepImage = false, SO so = SO() )
   { return getTexManager()->add2D( name, img, keepImage, so ); }

   virtual TexRef addTexture2D( std::string const & name, std::string uri,
                           bool keepImage = false, SO so = SO() )
   { return getTexManager()->load2D( name, uri, keepImage, so ); }

   virtual TexRef addTexture2D( std::string const & uri,
                           bool keepImage = false, SO const & so = SO() )
   {
      return addTexture2D( uri, uri, keepImage, so );
   }

   virtual TexRef addTexture2D( std::string const & name, bool keepImage,
             int w, int h, uint32_t color = 0xFFFFFFFF, SO const & so = SO() )
   {
      return addTexture2D( name, de::Image( w,h,color,ColorFormat::RGBA8888 ), keepImage, so );
   }

   // ###########################
   // ###                     ###
   // ###   TexUnitManager    ###
   // ###                     ###
   // ###########################
   virtual bool bindTexture2D( int stage, uint32_t texId )
   { return getTexManager()->bindTexture2D( stage, texId ); }
   virtual bool unbindTexture2D( uint32_t texId )
   { return getTexManager()->unbindTexture2D( texId ); }
   virtual int bindTexture2D( uint32_t texId )
   { return getTexManager()->bindTexture2D( texId ); }
   virtual bool bindTexture( int stage, Tex* tex )
   { return getTexManager()->bindTexture( stage, tex ); }
   virtual bool unbindTexture( Tex* tex )
   { return getTexManager()->unbindTexture( tex ); }
   virtual int bindTexture( Tex* tex )
   { return getTexManager()->bindTexture( tex ); }

   //virtual bool activateTexture( int stage, std::string const & key ) = 0;
   virtual bool uploadTexture( Tex* tex, Image const & img )
   { return getTexManager()->upload( tex, img ); }

//   virtual bool uploadTexture2D( Tex* tex, Image const & img, SamplerOptions const & so = SamplerOptions() )
//   {
//      uploadTexture2D( tex, img, img.getRect(), so );
//   }

// ###########################
// ###                     ###
// ###      RTManager      ###
// ###                     ###
// ###########################
   // Clear const & getClear() const { return m_RTManager.getClear(); }
   // void setClear( Clear clear ) { m_RTManager.setClear( clear ); }
   // void setClearColor( uint32_t color ) { m_RTManager.setClearColor( color ); }
   // void setClearColor( glm::vec4 const & color ) { m_RTManager.setClearColor( color ); }
   // Viewport const & getViewport() const { return m_RTManager.getViewport(); }
   // void setViewport( Viewport viewport ) { m_RTManager.setViewport( viewport ); }

// ###########################
// ###                     ###
// ### RenderTargetManager ###
// ###                     ###
// ###########################
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

   virtual void
   drawPrimitiveList( IMeshBuffer & buf ) = 0;

   virtual void
   draw3D( IMeshBuffer & buf, std::vector< glm::mat4 > const & instances ) = 0;


   virtual void
   draw2D( IMeshBuffer & buf ) = 0;

   virtual void
   draw3D( IMeshBuffer & buf ) = 0;

   virtual void
   drawBKS( float len = 1000.0f )
   {
      draw3DLine( glm::vec3(0,0,0),glm::vec3(len,0,0),0xFF0000FF );
      draw3DLine( glm::vec3(0,0,0),glm::vec3(0,len,0),0xFF00FF00 );
      draw3DLine( glm::vec3(0,0,0),glm::vec3(0,0,len),0xFFFF0000 );
   }

   virtual void
   drawPrimitiveList( SMesh & mesh )
   {
      for ( SMeshBuffer & p : mesh.Buffers )
      {
         drawPrimitiveList( p );
      }
   }

   virtual void
   draw2D( SMesh & mesh )
   {
      for ( SMeshBuffer & p : mesh.Buffers )
      {
         draw2D( p );
      }
   }

   virtual void
   draw3D( SMesh & mesh )
   {
      for ( SMeshBuffer & p : mesh.Buffers )
      {
         draw3D( p );
      }
   }

   // ###########################
   // ###                     ###
   // ###   FontRenderer5x8   ###
   // ###                     ###
   // ###########################
   virtual glm::ivec2
   getTextSize5x8( std::string const & msg, Font5x8 const & font = Font5x8() ) = 0;

   virtual void
   draw2DText5x8( int x, int y, std::string const & msg, uint32_t color = 0xFF000000,
               Align::EAlign align = Align::Default, Font5x8 const & font = Font5x8() ) = 0;

#ifdef USE_LIB_DARKFONT

   // ###########################
   // ###                     ###
   // ###  FreeType2 Renderer ###
   // ###                     ###
   // ###########################
   virtual void
   draw2DText( int x, int y, std::wstring const & msg, uint32_t color = 0xFF000000,
               Align::EAlign align = Align::Default, Font const & font = Font() ) = 0;

#endif

#ifdef USE_SMESH_GEOMETRY

   virtual void
   draw2DDemo() = 0;



   // ###########################
   // ###                     ###
   // ###     PerfOverlay     ###
   // ###                     ###
   // ###########################
//   virtual void
//   draw2DPerfOverlay() = 0;

   virtual void
   draw2DPerfOverlay5x8() = 0;

//   virtual void draw3D( IMeshBuffer const & vao, glm::dmat4 const & mvp ) = 0;
//   virtual void draw3D( SMesh const & mesh, glm::dmat4 const & mvp ) = 0;
   virtual void draw3DDebug( SMeshBuffer & vao,
                     VisualDebugData const & dbgData = VisualDebugData::ALL )
   {
      smesh::Debug meshDebug;
      meshDebug.add( vao, dbgData );
      draw3D( meshDebug.debugMesh );
   }

   virtual void draw3DDebug( SMesh & mesh,
                     VisualDebugData const & dbgData = VisualDebugData::ALL )
   {
      smesh::Debug meshDebug;
      meshDebug.add( mesh, dbgData );

      //meshDebug.Name = "Driver::draw3DDebug";

      draw3D( meshDebug.debugMesh );
   }

   virtual void
   draw3DLine( glm::vec3 const & a, glm::vec3 const & b, uint32_t colorA, uint32_t colorB )
   {
      SMeshBuffer o( PrimitiveType::Lines );
      o.Name = "Driver::draw3DLine";
      o.setLighting( 0 );
      o.addVertex( S3DVertex( a.x, a.y, a.z, 0.f, 0.f, 0.f, colorA, 0,0 ) ); // A
      o.addVertex( S3DVertex( b.x, b.y, b.z, 0.f, 0.f, 0.f, colorB, 0,0 ) ); // B
      draw3D( o );
   }

   virtual void
   draw3DLine( glm::vec3 const & a, glm::vec3 const & b, uint32_t color = 0xFFFFFFFF )
   {
      draw3DLine( a, b, color, color );
   }

   virtual void
   draw3DLineBox( Box3f const & bb, uint32_t color = 0xFFFFFFFF )
   {
      SMeshBuffer o( PrimitiveType::Lines );
      o.Name = "Driver::draw3DLineBox";
      o.setLighting( 0 );
      smesh::LineBox::add( o, bb, color );
      draw3D( o );
   }

   virtual void
   draw3DLineCircle( glm::vec2 siz, TexRef const & tex = TexRef(),
                     uint32_t color = 0xFFFFFFFF,
                     int tessRadial = 33 )
   {
      SMeshBuffer o( PrimitiveType::Lines );
      o.Name = "Driver::draw3DLineCircle";
      o.setTexture( 0, tex );

      smesh::LineCircle::addXY( o, siz, tessRadial, color );
      draw3D( o );
   }

   virtual void
   draw3DLineSphere( glm::vec3 siz, TexRef const & tex = TexRef(),
                     uint32_t color = 0xFFFFFFFF,
                     int tessRadial = 33,
                     int tessRows = 33 )
   {
//      SMeshBuffer o( PrimitiveType::Lines );
//      o.Name = "Driver::draw3DLineSphere";
//      o.setTexture( 0, tex );
//      SLineSphere::add( o, false, siz, glm::vec3(0,0,0), color, tessRadial, tessRows );
//      draw3D( o );
   }

   virtual void
   draw3DRect( glm::vec2 siz, TexRef const & tex = TexRef(),
                            uint32_t color = 0xFFFFFFFF )
   {
      SMeshBuffer o( PrimitiveType::Triangles );
      o.Name = "Driver::draw3DRect";
      o.setTexture( 0, tex );
      smesh::Rect::addXY( o, siz, color );
      draw3D( o );
   }

   virtual void
   draw3DCircle( glm::vec2 siz, TexRef const & tex = TexRef(),
                     uint32_t innerColor = 0xFFFFFFFF,
                     uint32_t outerColor = 0xFFFFFFFF,
                     int tessRadial = 33 )
   {
      SMeshBuffer o( PrimitiveType::Triangles );
      o.Name = "Driver::draw3DCircle";
      o.setTexture( 0, tex );
      smesh::Circle::addXY( o, siz, innerColor, outerColor, tessRadial );
      draw3D( o );
   }

   virtual void
   draw3DSphere( glm::vec3 siz, TexRef const & tex = TexRef(),
                     uint32_t color = 0xFFFFFFFF,
                     int tessRadial = 33,
                     int tessRows = 33 )
   {
      SMeshBuffer o( PrimitiveType::Triangles );
      o.Name = "Driver::draw3DSphere";
      o.setTexture( 0, tex );
      smesh::Sphere::add( o, siz, color, tessRadial, tessRows );
      draw3D( o );
   }

// ###########################
// ###                     ###
// ###   ScreenRenderer    ###
// ###                     ###
// ###########################
   virtual void
   draw2DLine( float x1, float y1, float x2, float y2, uint32_t color, float z = 0.0f );

   virtual void
   draw2DLine( float x1, float y1, float x2, float y2, uint32_t colorA, uint32_t colorB, float z = 0.0f );

   virtual void
   draw2DRect( Recti const & pos, uint32_t color = 0xFFFFFFFF,
                            TexRef const & ref = TexRef() );

   virtual void
   draw2DRect( Recti const & pos, uint32_t colorA, uint32_t colorB, uint32_t colorC, uint32_t colorD,
                            TexRef const & ref = TexRef() );

   virtual void
   draw2DRectOutline( Recti const & pos, uint32_t color = 0xFFFFFFFF,
                      TexRef const & ref = TexRef(), int borderWidth = 1 );

   virtual void
   draw2DRectOutline( Recti const & pos, uint32_t colorA, uint32_t colorB, uint32_t colorC, uint32_t colorD,
                            TexRef const & ref = TexRef(), int borderWidth = 1 );

   virtual void
   draw2DRoundRect( Recti const & pos, float rx, float ry, uint32_t color = 0xFFFFFFFF,
                    TexRef const & ref = TexRef(), int tess = 13 );

   virtual void
   draw2DRoundRectOutline( Recti const & pos, float rx, float ry, uint32_t color = 0xFFFFFFFF,
                    TexRef const & ref = TexRef(), int borderWidth = 1, int tess = 13 );

   virtual void
   draw2DCircle( Recti const & pos, uint32_t color = 0xFFFFFFFF,
                    TexRef const & ref = TexRef(), int tess = 36 );

   virtual void
   draw2DCircleOutline( Recti const & pos, uint32_t color = 0xFFFFFFFF,
                        TexRef const & ref = TexRef(), int borderWidth = 1, int tess = 36 );
/*

   void draw2DTexture( TexRef const & ref, Recti const & pos ) override
   {
      if ( !ref.tex ) return;
      if ( pos.getWidth() < 1 ) return;
      if ( pos.getHeight() < 1 ) return;
      SMeshBuffer quad( PrimitiveType::Triangles );
      quad.setTexture( 0, ref );
      quad.setLighting( false );

      int x1 = pos.getX1();
      int y1 = pos.getY1();
      int x2 = pos.getX2()+1;
      int y2 = pos.getY2()+1;
      glm::vec3 A( x1,y2,0 );
      glm::vec3 B( x1,y1,0 );
      glm::vec3 C( x2,y1,0 );
      glm::vec3 D( x2,y2,0 );
      mesh::Quad::add( quad, A, B, C, D, 0xFFFFFFFF );

//      if ( preserveAspect )
//      {
//         int w = pos.getWidth();
//         int h = pos.getHeight();
//         glm::ivec2 fit = AspectRatio::fitAspectPreserving( ref.tex->getWidth(), ref.tex->getHeight(), w,h );
//         x1 = pos.getX1() + (w - fit.x)/2;
//         y1 = pos.getY1() + (h - fit.y)/2;
//         x2 = x1 + fit.x;
//         y2 = y1 + fit.y;
//      }

      draw2D( quad );
   }

   void draw2DImage( std::string const & uri, bool preserveAspect = false ) override
   {
      draw2DTexture( getTexture(uri), preserveAspect );
   }

   void draw2DImage( std::string const & uri, int x, int y ) override
   {
      draw2DTexture( getTexture(uri), x,y );
   }

   void draw2DImage( std::string const & uri, Recti const & pos, bool preserveAspect = false ) override
   {
      draw2DTexture( getTexture(uri), pos, preserveAspect );
   }

   void draw2DTexture( Tex* tex, bool preserveAspect = false ) override
   {
      int x = 0;
      int y = 0;
      int w = getScreenWidth();
      int h = getScreenHeight();
      draw2DTexture( tex, Recti(x,y,w,h), preserveAspect );
   }

   void draw2DTexture( Tex* tex, int x, int y ) override
   {
      int w = 0;
      int h = 0;
      if ( tex )
      {
         w = tex->getWidth();
         h = tex->getHeight();
      }
      draw2DTexture( tex, Recti(x,y,w,h), false );
   }

   void draw2DTexture( Tex* tex, Recti const & pos, bool preserveAspect = false ) override
   {
      if ( !tex )
      {
         DE_ERROR("No tex")
         return;
      }
      if ( pos.getWidth() < 1 )
      {
         DE_ERROR("No w")
         return;
      }

      if ( pos.getHeight() < 1 )
      {
         DE_ERROR("No h")
         return;
      }
      SMeshBuffer quad( PrimitiveType::Triangles );
      quad.setTexture( 0, tex );
      quad.setLighting( false );

      int x1 = pos.getX1();
      int y1 = pos.getY1();
      int x2 = pos.getX2()+1;
      int y2 = pos.getY2()+1;
      if ( preserveAspect )
      {
         int w = pos.getWidth();
         int h = pos.getHeight();
         glm::ivec2 fit = AspectRatio::fitAspectPreserving( tex->getWidth(), tex->getHeight(),w,h );
         x1 = pos.getX1() + (w - fit.x)/2;
         y1 = pos.getY1() + (h - fit.y)/2;
         x2 = x1 + fit.x;
         y2 = y1 + fit.y;
      }
      glm::vec3 A( x1,y2,0 );
      glm::vec3 B( x1,y1,0 );
      glm::vec3 C( x2,y1,0 );
      glm::vec3 D( x2,y2,0 );
      mesh::Quad::add( quad, A, B, C, D, 0xFFFFFFFF );
      draw2D( quad );
   }
*/

#endif

//   void
//   draw3DWater( float sx, float sz, float x = 0.0f, float y = 0.0f, float z = 0.0f );

   // Drawing 1: einen Punkt zeichnen.

   // void
   // draw2DDot( float x, float y,
               // uint32_t color = 0xffffffff, float point_size = 1.0f, float z = 0.9f );

   // // Drawing 2: eine Linie zeichnen.
   // void
   // draw2DLine( float x1, float y1, float x2, float y2,
               // uint32_t colorA = 0xffffffff, uint32_t colorB = 0xffffffff, float z = 0.9f );

   // // Drawing 2: eine Linie zeichnen.
   // void
   // draw2DRectBorder( float x, float y, float w, float h,
               // uint32_t colorA,
               // uint32_t colorB,
               // uint32_t colorC,
               // uint32_t colorD, float z = -0.9f );

   // void
   // draw2DRectBorder( float x, float y, float w, float h,
               // uint32_t color = 0xffffffff, float z = 0.0f )
   // {
      // draw2DRectBorder( x, y, w, h, color, color, color, color, z );
   // }

   // void
   // draw2DRectBorder( Recti const & intRect, uint32_t color = 0xffffffff, float z = 0.0f )
   // {
      // draw2DRectBorder( float( intRect.x ),
                        // float( intRect.y ),
                        // float( intRect.w ),
                        // float( intRect.h ), color, color, color, color, z );
   // }

   // //bool
   // //present2DImage( de::Image const & img, uint32_t color, float z );

   // void
   // draw2DImage( int x, int y, Tex* tex, uint32_t color = 0xFFFFFFFF );

   // void
   // draw2DImage( Tex* tex, bool preserveAspectRatio = true, uint32_t color = 0xFFFFFFFF );

   // void
   // draw2DRect( Recti const & pos, uint32_t color = 0xFFFFFFFF, Tex* tex = nullptr )
   // {
      // draw2DRect( pos, color, color, color, color, tex);
   // }

   // void
   // draw2DRect( Recti const & pos,
               // uint32_t colorA, uint32_t colorB,
               // uint32_t colorC, uint32_t colorD, Tex* tex = nullptr );

   // void
   // draw2DRoundRect( Recti pos, uint32_t color = 0xFFFFFFFF, Tex* tex = nullptr, Recti* tex_rect = nullptr );

   // void
   // draw2DText( int x, int y, std::wstring const & msg, uint32_t color = 0xFF000000,
               // Align::EAlign align = Align::Default, Font const & font = Font() );

   // void
   // draw3DCube( glm::vec3 size, Tex* tex = nullptr, uint32_t color = 0xFFFFFFFF );

   // void
   // draw3DSphere( glm::vec3 size, Tex* tex = nullptr, uint32_t color = 0xFFFFFFFF );

   // void
   // draw3DRectXY( float w = 10.0f, float h = 10.0f, Tex* tex = nullptr,
          // uint32_t color = 0xFFFFFFFF, float x = 0.0f, float y = 0.0f, float z = 0.0f );

   // void
   // draw3DRectXZ( float w = 10.0f, float h = 10.0f, Tex* tex = nullptr,
          // uint32_t color = 0xFFFFFFFF, float x = 0.0f, float y = 0.0f, float z = 0.0f );

   // void
   // draw2DPerfOverlay();



};

IVideoDriver*
createVideoDriver( int w, int h, uint64_t winHandle = 0, SurfaceFormat const & fmt = SurfaceFormat(), bool debug = false );


} // end namespace gpu.
} // end namespace de.

#endif


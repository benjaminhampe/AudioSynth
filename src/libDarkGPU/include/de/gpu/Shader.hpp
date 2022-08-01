/// ==========================================================================
/// @file Shader.hpp
/// @author 2021 Benjamin Hampe <benjaminhampe@gmx.de>
/// @copyright Free open source software
/// ==========================================================================

#ifndef DE_GPU_SHADER_HPP
#define DE_GPU_SHADER_HPP

#include <de/gpu/FVF.hpp>

namespace de {
namespace gpu {

// =======================================================================
struct Uniform
// =======================================================================
{
   DE_CREATE_LOGGER("de.gpu.Uniform")

   enum EType
   {
      FLOAT = 0, VEC2, VEC3, VEC4,
      INT, IVEC2, IVEC3, IVEC4,
      UINT, UVEC2, UVEC3, UVEC4,
      BVEC1, BVEC2, BVEC3, BVEC4,
      MAT2, MAT3, MAT4,
      SAMPLER_2D, SAMPLER_3D, SAMPLER_CUBE, SAMPLER_2D_ARRAY,
      SAMPLER_2D_SHADOW, SAMPLER_CUBE_SHADOW, SAMPLER_2D_ARRAY_SHADOW,
      ISAMPLER_2D, ISAMPLER_3D, ISAMPLER_CUBE, ISAMPLER_2D_ARRAY,
      USAMPLER_2D, USAMPLER_3D, USAMPLER_CUBE, USAMPLER_2D_ARRAY,
      ETypeCount
   };

   static EType
   parseString( std::string const & txt );

   static std::string
   getString( EType const uniformType );

   Uniform();

   std::string
   toString() const;

   std::string m_Name;
   EType m_Type;
   int32_t m_Location;
   std::vector< uint8_t > m_Ubo;
   uint32_t m_InstanceCount;
};

// =======================================================================
struct ShaderType
// =======================================================================
{
   enum EType { vs = 0, fs, gs };
   static std::string getString( EType const shaderType )
   {
      switch ( shaderType )
      {
         case vs: return "vs";
         case fs: return "fs";
         case gs: return "gs";
         default: assert( false ); return "";
      }
   }
};

// =======================================================================
struct IShader
// =======================================================================
{
   virtual ~IShader() = default;
   virtual uint32_t getProgramId() const = 0;
   virtual std::string const & getName() const = 0;
   virtual void setName( std::string const & name ) = 0;

   // Used for (MVP) matrices.
   virtual bool setUniformM4f( std::string const & name, glm::mat4 const & value ) = 0;
   // Used for (TextureUnit) indices and (light mode) enums.
   virtual bool setUniformi( std::string const & name, int32_t value ) = 0;
   // Used for (time) values in seconds and positions
   virtual bool setUniformf( std::string const & name, float32_t value ) = 0;
   // Used for positions, colors, normals and texcoords
   virtual bool setUniform2f( std::string const & name, glm::vec2 const & value ) = 0;
   // Used for positions, colors, normals and texcoords
   virtual bool setUniform3f( std::string const & name, glm::vec3 const & value ) = 0;
   // Used for positions, colors, normals and texcoords
   virtual bool setUniform4f( std::string const & name, glm::vec4 const & value ) = 0;
   // Converts RGBA32 to RGBA128 and uploads it as vec4.
   virtual bool setUniformColor( std::string const & name, uint32_t color ) = 0;

   // New:
   virtual uint32_t getUniformCount() const = 0;
   virtual Uniform & getUniform( uint32_t i ) = 0;
   virtual Uniform const & getUniform( uint32_t i ) const = 0;
   virtual bool hasUniform( std::string const & name ) const = 0;
   virtual int32_t findUniform( std::string const & name ) const = 0;
};


// =======================================================================
struct Shader : public IShader
// =======================================================================
{
   DE_CREATE_LOGGER("de.gpu.Shader")

   static Shader*
   create( std::string const & shaderName,
           std::string const & vs,
           std::string const & fs,
           bool debug = false );

   Shader( std::string name );
   ~Shader() override;

   uint32_t getProgramId() const override;

   std::string const & getName() const override;
   void setName( std::string const & name ) override;

   // Used for (MVP) matrices.
   bool setUniformM4f( std::string const & name, glm::mat4 const & value ) override;
   // Used for (TextureUnit) indices and (light mode) enums.
   bool setUniformi( std::string const & name, int32_t value ) override;
   // Used for (time) values in seconds and positions
   bool setUniformf( std::string const & name, float32_t value ) override;
   // Used for positions, colors, normals and texcoords
   bool setUniform2f( std::string const & name, glm::vec2 const & value ) override;
   // Used for positions, colors, normals and texcoords
   bool setUniform3f( std::string const & name, glm::vec3 const & value ) override;
   // Used for positions, colors, normals and texcoords
   bool setUniform4f( std::string const & name, glm::vec4 const & value ) override;
   // Converts RGBA32 to RGBA128 and uploads it as vec4.
   bool setUniformColor( std::string const & name, uint32_t color ) override;

   uint32_t getUniformCount() const override { return m_Uniforms.size(); }
   Uniform & getUniform( uint32_t i ) override { return m_Uniforms[ i ]; }
   Uniform const & getUniform( uint32_t i ) const override { return m_Uniforms[ i ]; }
   bool hasUniform( std::string const & name ) const override { return findUniform( name ) > -1; }
   int32_t findUniform( std::string const & name ) const override
   {
      if ( name.empty() ) { DE_ERROR("No name") return -1; }
      auto it = std::find_if( m_Uniforms.begin(), m_Uniforms.end(),
         [&] ( Uniform const & cached ) { return ( cached.m_Name == name ); } );
      return ( it == m_Uniforms.end() ) ? -1 : int( std::distance( m_Uniforms.begin(), it ) );
   }

public:
   uint32_t m_ProgramId;
   bool m_Bound;
   std::string m_Name;
   std::string m_VS;
   std::string m_GS;
   std::string m_FS;
   std::vector< Uniform > m_Uniforms;
};


// =======================================================================
struct ShaderData
// =======================================================================
{
   DE_CREATE_LOGGER("de.gpu.ShaderData")

   std::vector< Uniform > m_Uniforms;
   //std::vector< uint8_t > m_UboData;
   IShader* m_Shader;
   bool m_IsDebug;

   ShaderData( IShader* shader = nullptr, bool debug = true )
      : m_Shader( shader )
      , m_IsDebug( debug )
   {

   }
   ~ShaderData()
   {

   }

   bool
   is_open() const { return m_Shader != nullptr; }

   void
   init( IShader* shader, bool debug = true )
   {
      m_Shader = shader;
      m_IsDebug = debug;
   }

   bool checkData() const
   {
      if ( !m_Shader ) return false;
      uint32_t datCount = m_Uniforms.size();
      uint32_t shdCount = m_Shader->getUniformCount();
      if ( shdCount != datCount )
      {
         DE_WARN("shdCount(",shdCount,") != datCount(",datCount,")")
         return false;
      }

      uint32_t valid = 0;
      for ( size_t i = 0; i < m_Uniforms.size(); ++i )
      {
         Uniform const & u = m_Uniforms[ i ];
         int found = m_Shader->hasUniform( u.m_Name );
         if (found > -1)
         {
            Uniform const & shdUniform = m_Shader->getUniform( found );
            if ( shdUniform.m_Type == u.m_Type )
            {
               ++valid;
            }
         }
      }

      if ( shdCount != valid )
      {
         DE_WARN("shdCount(",shdCount,") != validCount(",valid,")")
         return false;
      }

      return true;
   }

   int
   findUniformByName( std::string const & name ) const
   {
      if ( name.empty() ) { DE_ERROR("No name") return -1; }
      auto it = std::find_if( m_Uniforms.begin(), m_Uniforms.end(),
         [&] ( Uniform const & cached ) { return ( cached.m_Name == name ); } );
      return ( it == m_Uniforms.end() ) ? -1 : int( std::distance( m_Uniforms.begin(), it ) );
   }

   bool
   setUniformEx( std::string const & name, Uniform::EType uniType,
                  uint8_t const * src, uint32_t byteCount )
   {
      int found = findUniformByName( name );
      if (found < 0)
      {
         DE_DEBUG("Create uniform ",name)
         m_Uniforms.emplace_back();
         Uniform & u = m_Uniforms.back();
         u.m_Name = name;
         u.m_Type = uniType;
         u.m_Ubo.resize( byteCount );
         ::memcpy( u.m_Ubo.data(), src, byteCount );
         return true;
      }
      else
      {
         DE_DEBUG("Update found[",found,"] uniform ",name)

         Uniform & u = m_Uniforms[ found ];

         if ( u.m_Ubo.size() != byteCount )
         {
            DE_ERROR( "Size mismatch, need to rebuild UniformTable" )
            return false;
         }

         if ( u.m_Type != uniType )
         {
            DE_ERROR( "Type mismatch, need to rebuild UniformTable" )
            return false;
         }

         ::memcpy( u.m_Ubo.data(), src, byteCount );
         return true;
      }

   }

   bool setUniformColor( std::string const & name, uint32_t color )
   {
      glm::vec4 const v = RGBAf( color );
      return setUniformEx( name, Uniform::VEC4, r_cast< uint8_t const * >( glm::value_ptr( v ) ), sizeof( v ) );
   }

   bool setUniform( std::string const & name, glm::mat4 const & v )
   {
      return setUniformEx( name, Uniform::MAT4, r_cast< uint8_t const * >( glm::value_ptr( v ) ), sizeof( v ) );
   }

   bool setUniform( std::string const & name, glm::vec2 const & v )
   {
      return setUniformEx( name, Uniform::VEC2, r_cast< uint8_t const * >( glm::value_ptr( v ) ), sizeof( v ) );
   }

   bool setUniform( std::string const & name, glm::vec3 const & v )
   {
      return setUniformEx( name, Uniform::VEC3, r_cast< uint8_t const * >( glm::value_ptr( v ) ), sizeof( v ) );
   }

   bool setUniform( std::string const & name, glm::vec4 const & v )
   {
      return setUniformEx( name, Uniform::VEC4, r_cast< uint8_t const * >( glm::value_ptr( v ) ), sizeof( v ) );
   }

   bool setSampler2D( std::string const & name, uint32_t v )
   {
      return setUniformEx( name, Uniform::SAMPLER_2D, r_cast< uint8_t const * >( &v ), sizeof( v ) );
   }

   bool setSamplerCube( std::string const & name, uint32_t v )
   {
      return setUniformEx( name, Uniform::SAMPLER_CUBE, r_cast< uint8_t const * >( &v ), sizeof( v ) );
   }

//   virtual Uniform &
//   operator[] ( std::string const & name ) = 0;

//   virtual bool setShader( IShader* shader ) = 0;
//   virtual bool setUniform( std::string const & name, glm::mat4 const & value ) = 0;
//   virtual bool setUniform( std::string const & name, float32_t value ) = 0;
//   virtual bool setUniform( std::string const & name, glm::vec2 const & value ) = 0;
//   virtual bool setUniform( std::string const & name, glm::vec3 const & value ) = 0;
//   virtual bool setUniform( std::string const & name, glm::vec4 const & value ) = 0;
//   virtual bool setSampler( std::string const & name, uint32_t value ) = 0;
//   virtual bool setUniformColor( std::string const & name, uint32_t color ) = 0;

};



} // end namespace gpu.
} // end namespace de.


inline std::ostream &
operator<< ( std::ostream & o, de::gpu::Uniform const & uni )
{
   o << uni.toString();
   return o;
}

inline std::ostream &
operator<< ( std::ostream & o, de::gpu::ShaderType::EType const & typ )
{
   o << de::gpu::ShaderType::getString( typ );
   return o;
}


#endif


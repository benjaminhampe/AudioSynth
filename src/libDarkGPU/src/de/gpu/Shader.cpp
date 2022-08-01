#include <de/gpu/Shader.hpp>
#include <de/gpu/GLESv3.hpp>

namespace de {
namespace gpu {

// ===========================================================================
Uniform::EType
Uniform::parseString( std::string const & txt )
{
        if ( txt == "float" ) return FLOAT;
   else if ( txt == "int" ) return INT;
   else if ( txt == "vec2" ) return VEC2;
   else if ( txt == "vec3" ) return VEC3;
   else if ( txt == "vec4" ) return VEC4;
   else if ( txt == "sampler2D" ) return SAMPLER_2D;
   else return ETypeCount;
}

std::string
Uniform::getString( EType const uniformType )
{
   switch ( uniformType )
   {
      case FLOAT: return "float";
      case VEC2:  return "vec2";
      case VEC3:  return "vec3";
      case VEC4:  return "vec4";

      case INT:   return "int";
      case IVEC2: return "ivec2";
      case IVEC3: return "ivec3";
      case IVEC4: return "ivec4";

      case UINT:  return "uint";
      case UVEC2: return "uvec2";
      case UVEC3: return "uvec3";
      case UVEC4: return "uvec4";

      case BVEC1: return "bool";
      case BVEC2: return "bvec2";
      case BVEC3: return "bvec3";
      case BVEC4: return "bvec4";

      case MAT2:  return "mat2";
      case MAT3:  return "mat3";
      case MAT4:  return "mat4";

      case SAMPLER_2D:  return "sampler2D";
      case SAMPLER_3D:  return "sampler3D";
      case SAMPLER_CUBE:  return "samplerCube";
      case SAMPLER_2D_ARRAY:  return "sampler2DArray";

      case SAMPLER_2D_SHADOW:  return "sampler2DShadow";
      case SAMPLER_CUBE_SHADOW:  return "samplerCubeShadow";
      case SAMPLER_2D_ARRAY_SHADOW:  return "sampler2DArrayShadow";

      case ISAMPLER_2D:  return "isampler2D";
      case ISAMPLER_3D:  return "isampler3D";
      case ISAMPLER_CUBE:  return "isamplerCube";
      case ISAMPLER_2D_ARRAY:  return "isampler2DArray";

      case USAMPLER_2D:  return "usampler2D";
      case USAMPLER_3D:  return "usampler3D";
      case USAMPLER_CUBE:  return "usamplerCube";
      case USAMPLER_2D_ARRAY:  return "usampler2DArray";

      default: DE_ERROR("No uniformType ", int( uniformType ) ) return "unknown";
   }
}

Uniform::Uniform()
   : m_Type( ETypeCount )
   , m_Location( -1 )
//      , m_ByteStart( 0 )
//      , m_ByteCount( 0 )
   , m_InstanceCount( 1 )
{

}

std::string
Uniform::toString() const
{
   std::stringstream s;
   s  << m_Name << "," << getString( m_Type )
      << ",loc:" << m_Location
      << ",siz:" << m_Ubo.size();
   return s.str();
}


// ===========================================================================
Shader::Shader( std::string name )
   : m_ProgramId( 0 )
   , m_Bound( false )
   , m_Name( name )
{
   //DE_DEBUG(getName())
}

Shader::~Shader()
{
   //DE_DEBUG(getName())
   GLES::deleteProgram( m_ProgramId );
}

Shader*
Shader::create( std::string const & name,
                std::string const & vs, std::string const & fs, bool debug )
{
   GLuint programId = GLES::linkProgram( name, vs, fs, debug );
   if ( !programId )
   {
      DE_ERROR( "Compile or linker error. Shader (", name, ")" )
      return nullptr;
   }

   Shader* p = new Shader( name );
   p->m_ProgramId = programId;
   p->m_VS = vs;
   p->m_GS = "";
   p->m_FS = fs;

   GLint activeUniforms = 0;
   GLint activeUniformsMaxLen = 0;
   GLint activeAttribs = 0;
   GLint activeAttribsMaxLen = 0;
   glGetProgramiv( programId, GL_ACTIVE_UNIFORMS, &activeUniforms ); GL_VALIDATE
   ::glGetProgramiv( programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &activeUniformsMaxLen ); GL_VALIDATE
   ::glGetProgramiv( programId, GL_ACTIVE_ATTRIBUTES, &activeAttribs ); GL_VALIDATE
   ::glGetProgramiv( programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &activeAttribsMaxLen ); GL_VALIDATE

   if ( debug )
   {
      DE_DEBUG( "Shader[",name,"] has ",activeUniforms," active Uniforms, maxLen(",activeUniformsMaxLen,")")
      DE_DEBUG( "Shader[",name,"] has ",activeAttribs," active Attributes, maxLen(",activeAttribsMaxLen,")")
      DE_DEBUG( "Shader[",name,"] VS: ", vs )
      DE_DEBUG( "Shader[",name,"] FS: ", fs )
   }

   p->m_Uniforms.clear();

   char* uniName = (char*)::malloc( activeUniformsMaxLen );
   for ( GLint i = 0; i < activeUniforms; ++i )
   {
      GLint uniSize;
      GLenum uniType;
      ::memset( uniName, 0, activeUniformsMaxLen );
      ::glGetActiveUniform( programId, i, activeUniformsMaxLen, NULL, &uniSize, &uniType, uniName );
      GLint uniLoc = ::glGetUniformLocation( programId, uniName );

      Uniform u;
      u.m_Type = GLES::toEUniformType( uniType );
      u.m_Location = uniLoc;
      u.m_Name = uniName;
      u.m_InstanceCount = uniSize;
      if ( debug )
      {
         DE_DEBUG("Uniform[",i,"] ", u.toString() )
      }
      p->m_Uniforms.emplace_back( std::move( u ) );
   }
   ::free( uniName );

   if ( debug )
   {
      char* attrName = (char*)::malloc( activeAttribsMaxLen );
      for ( GLint i = 0; i < activeAttribs; ++i )
      {
         GLint attrSize;
         GLenum attrType;
         ::memset( attrName, 0, activeAttribsMaxLen );
         ::glGetActiveAttrib( programId, i, activeAttribsMaxLen, NULL, &attrSize, &attrType, attrName );
         std::string attrTypeStr;
         switch ( attrType )
         {
            case GL_FLOAT:       attrTypeStr = "float"; break;
            case GL_FLOAT_VEC2:  attrTypeStr = "vec2"; break;
            case GL_FLOAT_VEC3:  attrTypeStr = "vec3"; break;
            case GL_FLOAT_VEC4:  attrTypeStr = "vec4"; break;
            case GL_INT:         attrTypeStr = "int"; break;
            default:             attrTypeStr = "unknown"; break;
         }

         DE_DEBUG("Attribute[",i,"] name:", attrName, ", type:",attrTypeStr,", size:",attrSize)
      }
      ::free( attrName );
   }
   return p;
}

void
Shader::setName( std::string const & name )
{
   m_Name = name;
}

std::string const &
Shader::getName() const
{
   return m_Name;
}

uint32_t
Shader::getProgramId() const
{
   return m_ProgramId;
}

bool
Shader::setUniformM4f( std::string const & name, glm::mat4 const & value )
{
   if ( !m_ProgramId )
   {
      DE_ERROR("No programId" )
      return false;
   }

   GLint loc = ::glGetUniformLocation( m_ProgramId, name.c_str() );
   GL_VALIDATE
   if ( loc < 0 ) { DE_ERROR("Uniform not exist (", name, ")" ) return false; }
   ::glUniformMatrix4fv( loc, 1, GL_FALSE, glm::value_ptr( value ) );
   bool ok = GL_VALIDATE;
   if ( !ok )
   {
      DE_ERROR("Error for uniform name(", name,"), value(", value, ")" )
   }
   return ok;
}

bool
Shader::setUniformf( std::string const & name, float32_t value )
{
   if ( !m_ProgramId )
   {
      DE_ERROR("No programId" )
      return false;
   }

   GLint loc = ::glGetUniformLocation( m_ProgramId, name.c_str() ); GL_VALIDATE
   if ( loc < 0 ) { DE_ERROR("Uniform not exist (", name, ")" ) return false; }
   ::glUniform1f( loc, value );
   bool ok = GL_VALIDATE;
   if ( !ok )
   {
      DE_ERROR("Error for uniform name(", name,"), value(", value, ")" )
   }
   return ok;
}

bool
Shader::setUniform2f( std::string const & name, glm::vec2 const & value )
{
   if ( !m_ProgramId )
   {
      DE_ERROR("No programId" )
      return false;
   }

   GLint loc = ::glGetUniformLocation( m_ProgramId, name.c_str() ); GL_VALIDATE
   if ( loc < 0 ) { DE_ERROR("Uniform not exist (", name, "), value(", value, ")" ) return false; }
   ::glUniform2fv( loc, 1, glm::value_ptr( value ) );
   bool ok = GL_VALIDATE;
   if ( !ok )
   {
      DE_ERROR("Error for uniform name(", name,"), value(", value, ")" )
   }
   return ok;
}

bool
Shader::setUniform3f( std::string const & name, glm::vec3 const & value )
{
   if ( !m_ProgramId )
   {
      DE_ERROR("No programId" )
      return false;
   }

   GLint loc = ::glGetUniformLocation( m_ProgramId, name.c_str() ); GL_VALIDATE
   if ( loc < 0 ) { DE_ERROR("Uniform not exist (", name, ")" ) return false; }
   ::glUniform3fv( loc, 1, glm::value_ptr( value ) );
   bool ok = GL_VALIDATE;
   if ( !ok )
   {
      DE_ERROR("Error for uniform name(", name,"), value(", value, ")" )
   }
   return ok;
}

bool
Shader::setUniform4f( std::string const & name, glm::vec4 const & value )
{
   if ( !m_ProgramId )
   {
      DE_ERROR("No programId" )
      return false;
   }
   GLint loc = ::glGetUniformLocation( m_ProgramId, name.c_str() ); GL_VALIDATE
   if ( loc < 0 ) { DE_ERROR("Uniform not exist (", name, ")" ) return false; }
   ::glUniform4fv( loc, 1, glm::value_ptr( value ) );
   bool ok = GL_VALIDATE;
   if ( !ok )
   {
      DE_ERROR("Error for uniform name(", name,"), value(", value, ")" )
   }
   return ok;
}

bool
Shader::setUniformi( std::string const & name, int32_t value )
{
   if ( !m_ProgramId )
   {
      DE_ERROR("No programId" )
      return false;
   }

   GLint loc = ::glGetUniformLocation( m_ProgramId, name.c_str() ); GL_VALIDATE
   if ( loc < 0 ) { DE_ERROR("Uniform not exist (", name, ")" ) return false; }
   ::glUniform1i( loc, value );
   bool ok = GL_VALIDATE;
   if ( !ok )
   {
      DE_ERROR("Error for uniform name(", name,"), value(", value, ")" )
   }
   return ok;
}

bool
Shader::setUniformColor( std::string const & name, uint32_t color )
{
   if ( !m_ProgramId )
   {
      DE_ERROR("No programId" )
      return false;
   }

   GLint loc = ::glGetUniformLocation( m_ProgramId, name.c_str() ); GL_VALIDATE
   if ( loc < 0 ) { DE_ERROR("Uniform not exist (", name, "), color(", color,")" ) return false; }
   glm::vec4 color128 = RGBAf( color );
   ::glUniform4fv( loc, 1, glm::value_ptr( color128 ) );
   bool ok = GL_VALIDATE;
   if ( !ok )
   {
      DE_ERROR("Error for uniform name(", name,"), color(", color, ")" )
   }
   return ok;
}





} // end namespace gpu.
} // end namespace de.


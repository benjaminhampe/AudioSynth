#ifndef DE_MEMORY_BLOB_HPP
#define DE_MEMORY_BLOB_HPP

#include <de/FileSystem.hpp>

namespace de {

// ============================================================================
struct Binary
// ============================================================================
{
   DE_CREATE_LOGGER("de.Binary")

   std::vector< uint8_t > m_Blob;
   uint8_t* m_Ptr;
   uint64_t m_Count;
   uint64_t m_Index;
   uint64_t m_Avail;

   std::string m_Uri;

   Binary() : m_Ptr( nullptr ), m_Count( 0 ), m_Index( 0 ), m_Avail( 0 ) {}

   Binary( std::string uri ) : Binary() { load( uri ); }

   //~Binary() { close(); }

   static Binary
   fromRAM( uint8_t const * ptr, uint64_t count, std::string uri )
   {
      Binary b;
      b.openMemory( ptr, count, uri );
      return b;
   }

   static Binary
   fromHDD( std::string uri )
   {
      Binary b;
      b.load( uri );
      return b;
   }

   uint64_t
   size() const { return m_Count; }

   uint8_t const *
   data() const { return m_Ptr; }

   uint8_t*
   data() { return m_Ptr; }

   std::string const &
   getUri() const { return m_Uri; }

   bool
   is_open() const { return m_Ptr != nullptr; }

   void
   close()
   {
      m_Ptr = nullptr;
      m_Blob.clear();
   }

   void
   clear()
   {
      m_Ptr = nullptr;
      m_Blob.clear();
   }

   bool
   save( std::string uri ) const
   {
      FILE* file = ::fopen( uri.c_str(), "wb" );
      if ( !file )
      {
         DE_ERROR("Cant open to write ", uri )
         return false;
      }

      if ( m_Ptr )
      {
         size_t bytesWritten = ::fwrite( m_Ptr, 1, m_Count, file );
         if ( bytesWritten < m_Blob.size() )
         {
            DE_ERROR("bytesWritten(",bytesWritten,") < m_Count(",m_Count,")")
         }
      }
      else
      {
         DE_ERROR("No data to write ", uri )
      }

      ::fclose( file );
      return true;
   }

   bool
   openMemory( uint8_t const * ptr, uint64_t count, std::string uri )
   {
      m_Blob.clear();
      m_Ptr = const_cast< uint8_t* >( ptr );
      m_Count = count;
      m_Avail = count;
      m_Index = 0;
      m_Uri = uri;
      return true;
   }

   bool
   load( std::string uri )
   {
      m_Uri = uri;
      //DE_DEBUG("Load binary file ",uri)

      FILE* file = ::fopen( uri.c_str(), "rb" );
      if ( !file )
      {
         DE_ERROR("Cant open ", uri )
         return false;
      }

      ::fseeko64( file, 0, SEEK_END );
      size_t byteCount = size_t( ::ftello64( file ) );
      ::fseeko64( file, 0, SEEK_SET );

      if ( byteCount < 1 )
      {
         DE_ERROR("Empty file content ", uri )
         return false;
      }

      m_Blob.clear();
      try
      {
         m_Blob.resize( byteCount, 0x00 );
         size_t bytesRead = ::fread( m_Blob.data(), 1, m_Blob.size(), file );
         if ( bytesRead < byteCount )
         {
            DE_ERROR("bytesRead(",bytesRead,") < byteCount(",byteCount,")")
            return false;
         }
      }
      catch(...)
      {
         DE_ERROR("OOM")
         return false;
      }

      ::fclose( file );
      m_Ptr = m_Blob.data();
      m_Count = m_Blob.size();
      m_Avail = m_Blob.size();
      m_Index = 0;
      return true;
   }

   uint64_t
   read( void* dst, uint64_t byteCount )
   {
      if ( !m_Ptr || m_Avail == 0 )
      {
         DE_DEBUG("EOF")
         return 0;
      }

      if ( byteCount > m_Avail )
      {
         //DE_DEBUG("LAST CHUNK byteCount(",byteCount,") > m_Avail(",m_Avail,")")
         auto retVal = m_Avail;
         m_Index = m_Count - 1 - m_Avail;
         ::memcpy( dst, m_Ptr + m_Index, m_Avail );
         m_Avail = 0;
         m_Index = 0;
         return retVal;
      }

      ::memcpy( dst, m_Ptr + m_Index, byteCount );
      m_Index += byteCount;
      m_Avail -= byteCount;
      //DE_DEBUG("CHUNK(",byteCount,"), m_Index(",m_Index,"), m_Avail(",m_Avail,")")
      return byteCount;
   }

   uint64_t
   tell() const
   {
      return m_Index;
   }

   bool
   seek( uint64_t byteOffset, bool relativeMovement = false, int dir = SEEK_SET )
   {
      if ( !m_Ptr || m_Avail == 0 )
      {
         DE_DEBUG("EOF")
         return 0;
      }

      // Skip
      if ( relativeMovement )
      {
         if ( dir == SEEK_SET )
         {
            if ( byteOffset > m_Avail )
            {
               DE_DEBUG("Invalid rel offset")
               return false;
            }
            else
            {
               m_Index += byteOffset;
               m_Avail -= byteOffset;
               return true;
            }

         }
         else if ( dir == SEEK_END )
         {
            if ( byteOffset > m_Count )
            {
               DE_DEBUG("Invalid end offset")
               return false;
            }
            else
            {
               m_Index -= byteOffset;
               m_Avail += byteOffset;
            }
         }

      }
      // Seek
      else
      {
         if ( m_Count < byteOffset )
         {
            DE_DEBUG("Invalid offset")
            return false;
         }

         if ( dir == SEEK_SET )
         {
            m_Index = byteOffset;
            m_Avail = m_Count - m_Index;
         }
         else if ( dir == SEEK_END )
         {
            m_Index = m_Count - byteOffset;
            m_Avail = byteOffset;
         }

      }

      return true;
   }

};

} // end namespace de.

#endif // DE_FILE_STREAM_HPP

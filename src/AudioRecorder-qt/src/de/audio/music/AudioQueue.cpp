#include <de/audio/music/AudioQueue.hpp>

namespace de {
namespace audio {

/*
std::string
AudioQueue::toString() const
{
   std::stringstream s;
   s << ", chunkCount = " << getChunkCount();
   s << ", chunkIndex = " << getChunkIndex();
   s << ", chunkOffset = " << getChunkOffset();
   s << ", frameIndex = " << getFrameCount();
   s << ", frameCount = " << getFrameIndex();
   // s << ", dur:" << dbSecondsToString( totalDuration() );
   // s << ", bytes:" << dbStrByteCount( totalBytes() );
   return s.str();
}

void
AudioQueue::dump()
{
   for ( size_t i = 0; i < getChunkCount(); ++i )
   {
      if ( !m_Queue[ i ] )
      {
         DE_ERROR("Got nullptr Queue[",i,"]:" )
      }
      else {
         m_Queue[ i ]->dump();
      }

   }
   DE_DEBUG("chunkCount = ", getChunkCount() )
   DE_DEBUG("chunkIndex = ", getChunkIndex() )
   DE_DEBUG("chunkOffset = ", getChunkOffset() )
   DE_DEBUG("frameIndex = ", getFrameCount() )
   DE_DEBUG("frameCount = ", getFrameIndex() )

   // s << ", dur:" << dbSecondsToString( totalDuration() );
   // s << ", bytes:" << dbStrByteCount( totalBytes() );
}

uint64_t
AudioQueue::pop_front( Buffer & dst, double pts )
{
   uint64_t dstFrames = dst.getFrameCount();
   if ( dstFrames < 1 )
   {
      DE_ERROR("Got empty dst buffer")
      return 0;
   }

   std::lock_guard< std::mutex > guard( m_Mutex );

   if ( m_FrameIndex + dstFrames > m_FrameCount )
   {
      DE_ERROR("Not enough data dstFrames(",dstFrames,"), fc(",m_FrameCount,"), fi(",m_FrameIndex,")")
      return 0;
   }

// 1st :: WRITE CHUNKS UNTIL queue is empty OR dstFrames == readFrames
   //ESampleType dstType = dst.getSampleType(); // Not needed, write() already uses dst.getSampleType().
   uint64_t desiredFrames = dst.getFrameCount(); // used as count-down, makes life easier.
   uint64_t totalFrames = 0;
   //size_t finishedFrames = 0; // count-up does not really help.
   //size_t readChunks = 0;
   //size_t chunkIndex = 0;
   uint64_t i = 0;
#ifdef DE_DEBUG_BUFFER_QUEUE
   DE_DEBUG("pop_front(",dst.getFrameCount(),"), "
            "off(",m_FrameOffset,"), "
            "c(",m_FrameCount,"), "
            "i(",m_FrameIndex,"), "
            "pts(",pts,")" )
#endif

   while ( desiredFrames > 0 )
   {
      if ( m_Queue.empty() )  // moved check into while body for debug message
      {
         DE_ERROR("Queue is empty")
         break;
      }

      Buffer::Ptr & chunk = m_Queue.front();
      if ( !chunk )
      {
         DE_ERROR("Got nullptr, unexpected")
         m_Queue.pop_front();
         i++;
         continue;
      }

      uint64_t availFrames = chunk->getFrameCount() - m_FrameOffset;
#ifdef DE_DEBUG_BUFFER_QUEUE
      DE_DEBUG("it[", i, "] availFrames = ", availFrames )
//         DE_DEBUG("it[", i, "] chunk->getFrameCount() = ", chunk->getFrameCount() )
//         DE_DEBUG("it[", i, "] m_FrameOffset = ", m_FrameOffset )
//         DE_DEBUG("it[", i, "] m_FrameIndex = ", m_FrameIndex )
//         DE_DEBUG("it[", i, "] m_FrameCount = ", m_FrameCount )
#endif

      if ( availFrames < 1 )
      {
         DE_ERROR("No frames avail in this chunk")
         m_FrameIndex += chunk->getFrameCount();
         m_FrameOffset = 0;
         m_ChunkIndex++;
         m_Queue.pop_front();
         //m_FrameIndex = 0;
         //m_FrameIndex = 0;
         i++;
         continue;
      }

      // [1-A] Need to eat chunk entirely.
      if ( desiredFrames >= availFrames )
      {
         // Read rest of chunk
         if ( m_FrameOffset > 0 )
         {
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("REST OF CHUNK" )
            DE_DEBUG("it[", i, "] m_FrameOffset = ", m_FrameOffset )
#endif
            chunk->sub( m_WorkBuffer, m_FrameOffset, availFrames ); // chunkFrames is already adapted ( see -= ).
            uint64_t writtenFrames = dst.write( totalFrames, m_WorkBuffer );
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("Wrote REST ", writtenFrames )
            if ( writtenFrames != m_WorkBuffer.getFrameCount() )
            {
               DE_ERROR("Wrote ",writtenFrames," frames of ", m_WorkBuffer.getFrameCount())
            }
#endif
            desiredFrames -= writtenFrames;
            totalFrames += writtenFrames;
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("it[", i, "] m_WorkBuffer = ", m_WorkBuffer.toString() )
            DE_DEBUG("it[", i, "] writtenFrames = ", writtenFrames )
            DE_DEBUG("it[", i, "] desiredFrames = ", desiredFrames )
            DE_DEBUG("it[", i, "] totalFrames = ", totalFrames )
#endif
         }
         // Read total chunk
         else
         {
            uint64_t writtenFrames = dst.write( totalFrames, *chunk.get() );
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("Wrote FULL ", writtenFrames )
            if ( writtenFrames != chunk->getFrameCount() )
            {
               DE_ERROR("Wrote ",writtenFrames," frames of ", chunk->getFrameCount())
            }
#endif
            desiredFrames -= writtenFrames;
            totalFrames += writtenFrames;
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("FULL CHUNK:" )
            DE_DEBUG("it[", i, "] writtenFrames = ", writtenFrames )
            DE_DEBUG("it[", i, "] desiredFrames = ", desiredFrames )
            DE_DEBUG("it[", i, "] totalFrames = ", totalFrames )
#endif
         }
         m_FrameIndex += chunk->getFrameCount();
         m_FrameOffset = 0;
         m_ChunkIndex++;
         m_Queue.pop_front();
      }
      // [1-B] Need to process only a part of this chunk -> we reached the last chunk to process.
      else // if ( desiredFrames < availFrames )
      {
#ifdef DE_DEBUG_BUFFER_QUEUE
         DE_DEBUG( "desiredFrames(", desiredFrames,") < availFrames(",availFrames,")" )
         // Read rest of chunk
         if ( m_FrameOffset > 0 )
         {
            DE_ERROR( "Unexpected m_FrameOffset(",m_FrameOffset,") > 0 for last chunk" )
            //break;
         }
#endif
         // Read last needed chunk ( can be in between bigger queue chunk )
         if ( desiredFrames > 0 )
         {
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("END CHUNK desiredFrames(",desiredFrames,")" )
#endif
            chunk->sub( m_WorkBuffer, 0, desiredFrames );
            if ( m_WorkBuffer.getFrameCount() != desiredFrames )
            {
               DE_ERROR("Differ desiredFrames(",desiredFrames,") != ", m_WorkBuffer.getFrameCount())
            }
            uint64_t writtenFrames = dst.write( totalFrames, m_WorkBuffer );
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("END CHUNK writtenFrames(", writtenFrames, "), desiredFrames(",desiredFrames,")" )
            if ( writtenFrames != m_WorkBuffer.getFrameCount() )
            {
               DE_ERROR("Wrote ",writtenFrames," frames of ", m_WorkBuffer.getFrameCount())
            }
#endif
            desiredFrames -= writtenFrames;
            totalFrames += writtenFrames;
            m_FrameOffset += writtenFrames;
#ifdef DE_DEBUG_BUFFER_QUEUE
            DE_DEBUG("it[", i, "] m_WorkBuffer = ", m_WorkBuffer.toString() )
            DE_DEBUG("it[", i, "] writtenFrames = ", writtenFrames )
            DE_DEBUG("it[", i, "] desiredFrames = ", desiredFrames )
            DE_DEBUG("it[", i, "] totalFrames = ", totalFrames )
#endif
            i++;
         }

      }
      i++;
   }

#ifdef DE_DEBUG_BUFFER_QUEUE
   DE_DEBUG("totalFrames = ", totalFrames )
#endif
   return totalFrames;
}

*/

} // end namespace audio
} // end namespace de










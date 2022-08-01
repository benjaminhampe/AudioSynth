// HtmlWriter.hpp
// Copyright (C) 2002-2012 Benjamin Hampe
// allowed for free use

#ifndef DE_HTMLWRITER_HPP
#define DE_HTMLWRITER_HPP

#include <de/Image.hpp>
#include <iostream>
#include <fstream>

namespace de {

typedef std::vector< std::pair< std::string, std::string > > HtmlAttributes;

class HtmlWriter // : public IEventReceiver //, ILogger
{
   DE_CREATE_LOGGER("de.HtmlWriter")
public:
   HtmlWriter( std::string const & saveUri, std::string const & title, std::string const & mediaDir = "" );
   ~HtmlWriter();

   void save();

   void beginHtml( std::string const & title, std::string const & mediaDir = "" );
   void endHtml();

   void writeTag( std::string const & tag,
                  HtmlAttributes const & attribs = mkAttrib("",""),
                  std::string const & data = "" );

   void endTag();
   void endTags();

   void beginTag( std::string const & tag,
                  HtmlAttributes const & attribs = mkAttrib("","") );

   void beginDiv( HtmlAttributes const & attribs = mkAttrib("id","0") ); // display:block;
   void beginTable( HtmlAttributes const & attribs = mkAttrib("width","100%") );
   void beginTH( HtmlAttributes const & attribs = mkAttrib("valign","middle") );
   void beginTR( HtmlAttributes const & attribs = mkAttrib("valign","middle") );
   void beginTD( HtmlAttributes const & attribs = mkAttrib("align","center") );

   void write( std::string const & data );
   void writeText( std::string const & msg );
   void writeComment( std::string const & comment );
   void writeImage( Image const & img, std::string const & uri );

   void writeBR();
   void writeHR();
   void writeH1( std::string const & msg );
   void writeH2( std::string const & msg );
   void writeH3( std::string const & msg );
   void writeH4( std::string const & msg );
   void writeH5( std::string const & msg );
   void writeH6( std::string const & msg );
   void writeCode( std::string const & msg);
   void writeQuote( std::string const & msg );
   void writeURL( std::string const & msg );

   static std::string
   mkLineString( size_t n );

//   static std::string
//   mkAttrib( std::string const & key, std::string const & value );

   static HtmlAttributes
   mkAttrib( std::string const & key, std::string const & value )
   {
      HtmlAttributes a;
      addAttrib( a, key, value );
      return a;
   }

   static void
   addAttrib( HtmlAttributes & attribs, std::string const & key, std::string const & value )
   {
      if ( key.empty() ) return;
      if ( value.empty() ) return;
      attribs.emplace_back( key, value );
   }

   static std::string
   toString( HtmlAttributes const & attribs )
   {
      std::stringstream s;
      for ( auto const & a : attribs )
      {
         s << " " << a.first << "=\"" << a.second << "\"";
      }
      return s.str();
   }

protected:

private:
   //	uint32_t Time;
   uint32_t m_lineCounter;
   std::ostringstream m_html;
   std::string m_saveUri;
   std::string m_mediaDir;

   std::string m_title;
   std::vector< std::string > m_openTags;
};

} // end namespace de.

#endif

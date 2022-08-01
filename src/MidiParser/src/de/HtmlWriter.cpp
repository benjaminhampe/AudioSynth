// HtmlWriter.cpp
// Copyright (C) 2002-2012 Benjamin Hampe
// allowed for free use
#include <de/HtmlWriter.hpp>
#include <de/FileSystem.hpp>


#include <iostream>
#include <ios>
#include <fstream>
// #include <fs.h>

namespace de {

// static
//std::string
//HtmlWriter::mkAttrib( std::string const & key, std::string const & value )
//{
//   std::stringstream s;
//   s << " " << key << "=\"" << value << "\"";
//   return s.str();
//}

// static
// TIME to STRING
std::string
HtmlWriter::mkLineString( size_t n )
{
   std::stringstream s;
//	if ( n < 1000000000) s << "0";
//	if ( n < 100000000) s << "0";
   if ( n < 10000000) s << "0";
   if ( n < 1000000) s << "0";
   if ( n < 100000) s << "0";
   if ( n < 10000) s << "0";
   if ( n < 1000) s << "0";
   if ( n < 100) s << "0";
   if ( n < 10) s << "0";
   s << n;
   return s.str();
}

// constructor
HtmlWriter::HtmlWriter( std::string const & saveUri, std::string const & title, std::string const & mediaDir )
   : m_lineCounter( 0 )
   , m_saveUri( saveUri )
{
   beginHtml( title, mediaDir );
}

// DESTRUCTOR
HtmlWriter::~HtmlWriter()
{
   endHtml();
   save();
}

void
HtmlWriter::save()
{
   std::ofstream file( m_saveUri.c_str() );
   if ( file.is_open() )
   {
      std::string html = m_html.str();
      file << html;
      file.close();

      DE_DEBUG("Saved HTML ", m_saveUri, " with ", html.size(), " bytes.")
   }


}

void
HtmlWriter::beginHtml( std::string const & title, std::string const & mediaDir )
{
   m_title = title;
   m_mediaDir = dbCWD();

   if ( !mediaDir.empty() )
   {
      m_mediaDir = dbMakeAbsolute( mediaDir );
   }

   if ( !dbExistDirectory( m_mediaDir ) )
   {
      dbMakeDirectory( m_mediaDir );
   }

//		<< "<!xml version=\"1.1\">\n"
//		<< "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" "
//		<< "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
//		<< "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n"
//		<< "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />\n

   m_html << "<html>\n"
   << "<title>Logfile: " << m_saveUri << "</title>\n"
   << "<style type=\"text/css\">\n"
   << "*    { margin:0; border:0; padding:0; }\n"
   << "body { width:100%; height:100%; color:rgb(0,0,0); background-color:rgb(255,255,255); font-family:monospace; font-size:16px; }\n"
   << "header { background-color:black; color:white; position:fixed; top:0px; left:0px; width:100%; height:50px; z-index:2;}\n"
   << "caption { font-weight:bold; font-size:24px; line-height:50px; vertical-align:middle;}\n"
   << "content { background-color:white; color:black; position:absolute; top:50px; padding-top:20px; white-space:nowrap; z-index:1;}\n"
   << "footer { background-color:black; color:white; position:fixed; bottom:0px; left:0px; width:100%; height:50px; line-height:50px; z-index:3;}\n"
//		<< "hr { color:black; background-color:white; width:100%; height:3px; padding-top:10px; padding-bottom:10px;}\n"
//		<< "h1 { color:black; font-size:44px; padding:20px;}\n"
//		<< "h2 { color:black; font-size:31px; padding:20px;}\n"
//		<< "h3 { color:black; font-size:23px; padding:20px;}\n"
//		<< "h4 { color:black; font-size:16px; padding:20px;}\n"
//		<< "h5 { color:black; font-size:12px; padding:20px;}\n"
//		<< "h6 { color:black; font-size:10px; padding:20px;}\n"
   << "img { border:5px; border-color:black; padding:5px; margin:5px;}\n"
   << "div128 { width:100%; height:128px; line-height:128px; display-style:inline; }\n"
   << "div64 { width:100%; height:64px; line-height:64px; display-style:inline; }\n"
   << "div32 { width:100%; height:32px; line-height:32px; display-style:inline; }\n"
   << "div16 { width:100%; height:16px; line-height:16px; display-style:inline; }\n"
   << "div12 { width:100%; height:12px; line-height:12px; display-style:inline; }\n"
   << "h1 { color:white; background-color:rgb(0   ,0   ,0  ); font-size:64px; vertical-align:middle; font-weight:900; }\n"
   << "h2 { color:white; background-color:rgb(127 ,192 ,255); font-size:32px; vertical-align:middle; font-weight:750; }\n"
   << "h3 { color:white; background-color:rgb(192 ,200 ,255); font-size:24px; vertical-align:middle; font-weight:600; }\n"
   << "h4 { color:white; background-color:rgb(200 ,200 ,255); font-size:24px; vertical-align:middle; font-weight:600; }\n"
   << "h5 { color:black; background-color:rgb(255 ,255 ,255); font-size:16px; vertical-align:middle; font-weight:400; }\n"
   << "h6 { color:white; background-color:rgb(64  ,64  ,192); font-size:12px; vertical-align:middle; font-weight:400; }\n"
   << "hr { color:black; background-color:rgb(255 ,255 ,0); height:10px; line-height:5px; vertical-align:middle; }\n"
   << "date { color:#FF00FF; font-weight:bold; margin-left:20px;}\n"
   << "line { color:#207F20; font-weight:bold; margin-left:20px;}\n"
   << "time { color:#4040DF; font-weight:bold; margin-left:20px;}\n"
   << "trace {color:white; background-color:#00AF00; margin-left:20px;}\n"
   << "debug {color:white; background-color:#FF00FF; margin-left:20px;}\n"
   << "info { margin-left:20px; }\n"
   << "warn {color:black; background-color:yellow; margin-left:20px;}\n"
   << "error {color:white; background-color:red; margin-left:20px;}\n"
   << "</style>\n</head>\n"
   << "<body>\n"
   << "<header>\n"
   << "<table width=\"100%\" height=\"100%\">\n"
   << "<tr valign=\"middle\">"
   << "<td align=\"center\"><caption>" << m_title << "</caption></td>\n"
   << "</tr>\n"
   << "</table>\n</header>\n<content>\n"
//		<< "<irrlicht><img src=\"irrlicht_mini.png\" /></irrlicht>\n"
   << "<line><b>Line #&nbsp;&nbsp;</b></line>"
   << "<time><b>Time in ms</b></time>"
   << "<information><b>Message</b></information><br/><br/>\n";
}

void
HtmlWriter::endHtml()
{
   endTags();

   m_html << "<br /><br /><br /><br /><br /></content>\n"
   << "<footer>\n"
   << "<table width=\"100%\" height=\"100%\">\n"
   << "<tr valign=\"middle\"><td align=\"center\"><h3>log-file produced by class HtmlWriter</h3></td></tr>\n"
   << "<tr valign=\"middle\"><td align=\"center\"><h6>(c) 2022 by BenjaminHampe@gmx.de</h6></td></tr>\n"
   << "</table>\n"
   << "</footer>\n"
   << "</body>\n</html>\n";
}

void HtmlWriter::writeTag( std::string const & tag, HtmlAttributes const & attribs, std::string const & data )
{
    beginTag( tag, attribs );
    m_html << data;
    endTag();
}

void HtmlWriter::beginTag( std::string const & tag, HtmlAttributes const & attribs )
{
   m_openTags.push_back(tag);
   //m_html << "<line>" << mkLineString( m_lineCounter ) << "</line>";
   m_html << "<" << tag << toString( attribs ) << " >\n";
}

void HtmlWriter::endTag()
{
   if ( m_openTags.size() > 0 )
   {
      m_html << "</" << m_openTags.back() << ">\n";
      m_openTags.erase( m_openTags.end() - 1 );
   }
}

void HtmlWriter::endTags()
{
   while ( m_openTags.size() > 0 )
   {
      m_html << "</" << m_openTags.back() << ">\n";
      m_openTags.erase( m_openTags.end() - 1 );
   }
}

void HtmlWriter::beginDiv( HtmlAttributes const & attribs )
{
   beginTag("div", attribs);
}
void HtmlWriter::beginTable( HtmlAttributes const & attribs )
{
   beginTag("table", attribs );
}
void HtmlWriter::beginTH( HtmlAttributes const & attribs )
{
   beginTag( "th", attribs );
}
void HtmlWriter::beginTR( HtmlAttributes const & attribs )
{
   beginTag( "tr", attribs );
}
void HtmlWriter::beginTD( HtmlAttributes const & attribs )
{
   beginTag( "td", attribs );
}
void HtmlWriter::writeText( std::string const & txt)
{
   //m_html << "<line>" << mkLineString( m_lineCounter ) << "</line>"
   m_html << "<p>\n" << txt << "</p>\n<br />";
}

void HtmlWriter::writeComment( std::string const & comment )
{
   m_html << "<!-- " << comment<< " -->\n";
}
void HtmlWriter::writeCode( std::string const & msg )
{
   writeTag("code", mkAttrib("",""), msg );
}
void HtmlWriter::writeQuote( std::string const & msg )
{
   writeTag("quote", mkAttrib("",""), msg );
}
void HtmlWriter::writeURL( std::string const & msg )
{
   writeTag("url", mkAttrib("",""), msg );
}

//void HtmlWriter::write( std::string const & data )
//{
//	m_html <<data<<flush;
//}
void HtmlWriter::write( std::string const & data )
{
   m_html << data;
}
void HtmlWriter::writeBR()
{
   m_html << "<br />\n";
}
void HtmlWriter::writeHR()
{
   m_html << "\n<hr />\n";
}
void HtmlWriter::writeH1( std::string const & msg )
{
   m_html << "<h1>" << msg << "</h1>\n";
}
void HtmlWriter::writeH2( std::string const & msg )
{
   m_html << "<h2>" << msg << "</h2>\n";
}
void HtmlWriter::writeH3( std::string const & msg )
{
   m_html << "<h3>" << msg << "</h3>\n";
}
void HtmlWriter::writeH4( std::string const & msg )
{
   m_html << "<h4>" << msg << "</h4>\n";
}
void HtmlWriter::writeH5( std::string const & msg )
{
   m_html << "<h5>" << msg << "</h5>\n";
}
void HtmlWriter::writeH6( std::string const & msg )
{
   m_html << "<h6>" << msg << "</h6>\n";
}
void HtmlWriter::writeImage( Image const & img, std::string const & uri )
{
   if ( img.empty() ) { return; }

   std::string fullName = FileSystem::makeAbsolute( uri );

   m_html << "<img src=\""
            << fullName
            << "\" width=\""
            << img.getWidth()
            << "\" height=\""
            << img.getHeight()
            << "\" alt=\""
               << fullName
               << ", width(" << img.getWidth() << ")"
               << ", height(" << img.getWidth() << ")"
               << ", bpp(" << img.getBitsPerPixel() << ")"
            << "\" />\n<br />\n"
            << std::flush;

   m_lineCounter++;
}

/*
void HtmlWriter::log( std::string const & msg, LogLevel ll )
{
   m_html << "<line>" << mkLineString() << "</line>";
   //	m_html << "<time>" << getTimeString() << "</time>";
   // replace '\n' with <br />\n

   std::vector< std::string > lines = dbStrSplit( msg, '\n' );

   // std::vector< std::string > lines;
   // int32_t pos = tmp.findFirst('\n');
   // if ( pos != -1 )
   // {
      // lines = StringUtils::split( tmp, '\n' );
   // }
   // else
   // {
      // lines.push_back( tmp );
   // }

   std::string tagType = "";
   std::string tagColor = "";

   switch (ll)
   {
      case LogLevel::Trace: tagType = "trace"; tagColor = "\033[40;35m"; break; // purple
      case LogLevel::Debug: tagType = "debug"; tagColor = "\033[40;32m"; break;// green
      case LogLevel::Info: tagType = "info"; tagColor = "\033[40;37m"; break;// white
      case LogLevel::Warn: tagType = "warn"; tagColor = "\033[40;33m"; break;// yellow
      case LogLevel::Error: tagType = "debug"; tagColor = "\033[40;31m"; break;// red
      default:	break;
   }

   m_html << "<" << tagType << ">";

   for (uint32_t i=0; i<lines.size(); i++)
   {
      m_html << lines[i] << "<br />\n";
   }

   m_html << "</" << tagType << "><br />\n" << std::flush;

   printf("%s%s\n", tagColor, msg );
   //std::cout<<msg<<std::endl;

   m_lineCounter++;
}
*/


} // END NAMSPACE de

#ifndef DE_ALPHASONIC_CREATE_DEVICE_HPP
#define DE_ALPHASONIC_CREATE_DEVICE_HPP

#include <irrExt/StringUtils.hpp>
#include <irrXML.h>

irr::IrrlichtDevice*
createDevice( uint32_t w, uint32_t h, irr::SIrrlichtCreationParameters & cfg );

inline std::string
toINIString( irr::SIrrlichtCreationParameters const & cfg )
{
    std::stringstream s;
    XML_writeHeader( s );
    s << "[device]\n" << (int)cfg.DeviceType << '\n';
    s << "[driver]\n" << (int)cfg.DriverType << '\n';
    s << "[loglevel]\n" << (int)cfg.LoggingLevel << '\n';
    s << "[width]\n" << (int)cfg.WindowSize.Width << '\n';
    s << "[height]\n" << (int)cfg.WindowSize.Height << '\n';
    s << "[bits]\n" << (int)cfg.Bits << '\n';
    s << "[antialias]\n" << (int)cfg.AntiAlias << '\n';
    s << "[doublebuffer]\n" << (int)cfg.Doublebuffer << '\n';
    s << "[vsync]\n" << (int)cfg.Vsync << '\n';
    s << "[fullscreen]\n" << (int)cfg.Fullscreen << '\n';
    s << "[zbufferbits]\n" << (int)cfg.ZBufferBits << '\n';
    s << "[stencilbuffer]\n" << (int)cfg.Stencilbuffer << '\n';
    return s.str();
}

inline std::string
toXMLElement( irr::SIrrlichtCreationParameters const & cfg )
{
    std::stringstream s;
    s << "<creation-params>\n";
    s << "\t<device>" << (int)cfg.DeviceType << "</device>\n";
    s << "\t<driver>" << (int)cfg.DriverType << "</driver>\n";
    s << "\t<loglevel>" << (int)cfg.LoggingLevel << "</loglevel>\n";
    s << "\t<width>" << (int)cfg.WindowSize.Width << "</width>\n";
    s << "\t<height>" << (int)cfg.WindowSize.Height << "</height>\n";
    s << "\t<bits>" << (int)cfg.Bits << "</bits>\n";
    s << "\t<antialias>" << (int)cfg.AntiAlias << "</antialias>\n";
    s << "\t<doublebuffer>" << (int)cfg.Doublebuffer << "</doublebuffer>\n";
    s << "\t<vsync>" << (int)cfg.Vsync << "</vsync>\n";
    s << "\t<fullscreen>" << (int)cfg.Fullscreen << "</fullscreen>\n";
    s << "\t<zbufferbits>" << (int)cfg.ZBufferBits << "</zbufferbits>\n";
    s << "\t<stencilbuffer>" << (int)cfg.Stencilbuffer << "</stencilbuffer>\n";
    s << "</creation-params>\n";
    return s.str();
}

inline bool
fromXMLFile( std::string fileName, irr::SIrrlichtCreationParameters & cfg )
{
    // create the xml parser from a filename
    irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader( fileName.c_str() );

    // strings for storing the data we want to get out of the file
    std::string modelFile;
    std::string messageText;
    std::string caption;

    // parse the file until end reached
    while(xml && xml->read())
    {
        switch(xml->getNodeType())
        {
        case irr::io::EXN_TEXT:
            // in this xml file, the only text which occurs is the messageText
            messageText = xml->getNodeData();
            break;
        case irr::io::EXN_ELEMENT:
            {
                if (!strcmp("model", xml->getNodeName()))
                    modelFile = xml->getAttributeValue("file");
                else
                if (!strcmp("messageText", xml->getNodeName()))
                    caption = xml->getAttributeValue("caption");
            }
            break;
        default:
            break;
        }
    }

    // delete the xml parser after usage
    delete xml;
    return true;
}

#endif // DEVICE_HPP

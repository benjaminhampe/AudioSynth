All libs can be compiled shared ( dll ) or staticly be linked in ( into exe from .o object file )
Enable/Disable commented out cmake stuff to force dll since default they are private, ergo no public dll.
Private libs are compiled in, public libs are offered to user like glm and tinyxml2, freetype, fontawesome

Private is mostly for image decoder libs and decompressors like libz, jpeg is really never used but through 
high level DarkImage API like dbLoadImage().

Most users ( like me ) never want to know how to decompress jpg they just want to do it.
Also DarkImage uses 32bit R8G8B8A8 Format by default for most convenience.
But 8bit and 32bit float geo tiff are supported for heightmaps just because i also want to do it.

There are plenty algos for drawing in ImagePainter with uses Text rendering and Bresenham drawing algos.
Use ImagePainter to do drawing on images ( Circle/Ellipse, Line, Dot, Rect, RoundRect, Blend, Scale, Rotate, TTFont Text and Filter like Smooth and Edge detect ).

Ring, but its just a mask of two circles:

int ringOuterRadius = 128;
int ringInnerRadius = 100;
de::Image ringMask( 256,256 ); // Default is R8G8B8A8 32 bit format with alpha channel ( 2^24 RGB color + 2^8 A opacity )
img.fill( 0x00000000 ); // Fill transparent R=0,G=0,B=0,A=0;
int center_x = 128;
int center_y = 128;
de::ImagePainter::drawCircle( ringMask, center_x,center_y, ringOuterRadius, 0xFFFFFFFF );
de::ImagePainter::drawCircle( ringMask, center_x,center_y, ringInnerRadius, 0x00000000 ); // draw inner circle in transparent to cut out ring shape of bigger white circle.

// Now you have a white ring on transparent background image in ca. 3 lines.

// Load Image

de::Image img; // Default is empty image in format R8G8B8A8
std::string uri = "../../media/irrlicht.png";
if ( !dbLoadImage( img, uri ) )
{
   std::cout << "Cant load image uri " << uri << std::endl;
}
else
{
   
   de::ImagePainter::



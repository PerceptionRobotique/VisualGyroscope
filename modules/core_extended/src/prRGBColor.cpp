#include <per/prRGBColor.h>

prRGBColor::prRGBColor(const unsigned char R, const unsigned char G, const unsigned char B)
{
    p.resize(3);
    
    set(R, G, B);
}

prRGBColor::~prRGBColor()
{
    
}

void prRGBColor::set(const unsigned char & R, const unsigned char & G, const unsigned char & B)
{
    p[0] = R;
    p[1] = G;
    p[2] = B;
}
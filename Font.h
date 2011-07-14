/* 
 * File:   Font.h
 * Author: brandon
 *
 * Created on May 7, 2011, 10:25 PM
 */

#ifndef FONT_H
#define	FONT_H
#include "GL/gl.h"

class Font {
public:
    Font();
    Font(const Font& orig);
    virtual ~Font();
private:
    GLuint* font;

};

#endif	/* FONT_H */


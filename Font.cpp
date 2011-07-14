/* 
 * File:   Font.cpp
 * Author: brandon
 * 
 * Created on May 7, 2011, 10:25 PM
 */

#include "Font.h"

Font::Font() {
    font = new GLuint[36];
}

Font::Font(const Font& orig) {
}

Font::~Font() {
}


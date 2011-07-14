/* 
 * File:   Sprite.h
 * Author: brandon
 *
 * Created on May 7, 2011, 1:07 PM
 */

#ifndef SPRITE_H
#define	SPRITE_H
#include "vmath.h"
#include <QtOpenGL/qgl.h>
#include <QtGui/QImage>
#include <string>
#include <cstdlib>
#include <cassert>

//#define PNG_SIG_BYTES 8
//#define HEADER_LENGTH 8

#define EASE_CONST 10

class Sprite {
public:
    Sprite();
    Sprite(std::string filename);
    Sprite(const Sprite& orig);
    virtual ~Sprite();
    GLuint loadTexture(std::string filename);

    void Draw();
    bool Update();

    void setSize (Vector2f dimension);
    void setPosition(Vector3f position);

    void setTint(Vector3f tint);

    void easeTo(Vector3f location);
    void addState(std::string fileName);
    void changeState(int state);


private:
    std::string file;
    int size;
    int state;
    Vector2f corner1;
    Vector2f corner2;
    
    GLuint* texture;
    Vector3f color;

    Vector3f position;
    bool easing;
    Vector3f target;
};

#endif	/* SPRITE_H */


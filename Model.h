/* 
 * File:   modelLoad.h
 * Author: brandon
 *
 * Created on January 10, 2011, 9:11 AM
 */

#ifndef MODELLOAD_H
#define	MODELLOAD_H
#include <string>
#include <cstdlib>
#include <QtOpenGL/QtOpenGL>
#include <vector>
#include "vmath.h"


#define PNG_SIG_BYTES 8
#define HEADER_LENGTH 8

typedef struct face {
    short vertex [3]; // valid vertices
    Vector3f** normals;
} triFace;

typedef struct Frame {
    Vector3f* vertices;
} frame;

typedef struct Action {
    std::string name;
    int numFrames;
    frame** frames;
} action;

typedef struct Animation {
    int numActions;
    int numFaces;
    int vertsPerFrame;
    int currentAction;
    int currentFrame;
    action** actions;
    triFace** faces;
} animation;

//typedef Vector2f UVTextureVertex;

typedef face* mesh;

class Model {
public:
    Model();
    Model(std::string modelFileName);
    //Model(std::string modelFileName, std::string textureFileName);
    ~Model();
    void addFrame(std::string modelFileName, int frame);
    void loadRawModel(std::string fileName);
    void loadMDLModel(std::string fileName);
    void swapTexture(std::string fileName);
    void Update();
    void Draw();

    void glGenerateDisplayList(Vector3f offset);
    void glGenerateDisplayList(Vector3f offset, Matrix3f modification);

    void shift3f(Vector3f shift);
    void scalef(float scale);

    bool isPlaying();
    void play();
    void pause();


    void changeAction(int to);
    int getAction();

    void changeColor(Vector3f color);

private:
    void jumpToFrame(int frame);
    void loadAnimation();
    void advanceFrame();

    bool playing;
    animation anim;
    // UVTextureVertex* textureVerticies;
    GLuint texture;

    Vector3f color;

    char* load_png(std::string name, int *width, int *height);
    GLuint loadTexture(std::string filename);
    void calculateSurfaceNormals();
    void calculateSurfaceNormals(Vector3f sum);
};

#endif	/* MODELLOAD_H */


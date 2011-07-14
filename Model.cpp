/**
 File:  Model.cpp
 */
#include "Model.h"
#include <fstream>
#include <queue>

//Model::Model(){}

Model::Model(std::string modelFileName) {
    playing = false;
    //loadAnimation();
    texture = -1;
    loadMDLModel(modelFileName);
    color = Vector3f(0.9, 0.2, 0.2);
}

//Model::Model(std::string modelFileName, std::string textureFileName) {
//    color = Vector3f(1.0, 0.7, 0.7);
//    //texture = loadTexture(textureFileName);
//    loadMDLModel(modelFileName);
//}

Model::~Model() {
    for (int i = 0; i < anim.numActions; i++) {
        for (int j = 0; j < anim.numFaces; j++) {
            for (int k = 0; k < anim.actions[i]->numFrames; k++) {
                //delete anim.faces[i]->normals[j][k];
                if (j == 0) {
                    delete[] anim.actions[i]->frames;
                }
            }
            delete [] anim.faces[j]->normals[i];
        }
        delete [] anim.faces[i]->normals;
        delete anim.actions[i];
    }
    delete [] anim.faces;
    delete [] anim.actions;
}

void Model::changeColor(Vector3f color) {
    this->color = color;
}

/*
void Model::addFrame(std::string modelFileName, int frame) {
    anim.currentFrame = frame;
    loadRawModel(modelFileName);
}*/

void Model::loadAnimation() {/*
    anim.numFrames = 2;
    anim.currentFrame = 0;
    anim.frames = new frame* [2];
    for (int i = 0; i < 2; i++) {
        anim.frames[i] = new frame;
    }*/
}

/**
 * FIX THIS RAW MODEL LOADING, mdl file loading borked it
 * @param fileName
 */

/*
void Model::loadRawModel(std::string fileName) {
    anim.frames[anim.currentFrame] = new frame;
    std::queue <triFace*> facequeue; // queue of faces to load to the model
    fileName = "Resources/" + fileName;
    std::ifstream modelFile(fileName.c_str());
    char* component = new char (10);
    while (modelFile.good()) {
        triFace *temp = new triFace;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                modelFile >> component;
             temp->vertex[i][j] = atof(component);
            }
        }
        // calculate surface normal
        calculateSurfaceNormal(temp);

        facequeue.push(temp); // load all faces to a temporary queue
        modelFile.ignore();
    }
    anim.faces = new triFace* [facequeue.size()]; // create mesh of queue size
    anim.numFaces = facequeue.size();
    for (int i = 0; i < anim.numFaces; i++) {
        anim.faces[i] = facequeue.front(); // unload the queue to the permemenent mesh
        facequeue.pop();
    }
    modelFile.close();
}*/

void Model::loadMDLModel(std::string fileName) {
    fileName = "Resources/" + fileName;
    std::ifstream modelFile(fileName.c_str());
    char* buffer = new char[50];
    modelFile.get(buffer, 4);
    assert(strcmp(buffer, std::string("MODL").c_str())); // make sure it has the MODL magic value
    modelFile.ignore();
    modelFile >> buffer; // next characters should be number of actions
    anim.numActions = atoi(buffer);
    anim.currentFrame = 0;
    anim.currentAction = 0;

    modelFile >> buffer; // next characters, number of faces
    anim.numFaces = atoi(buffer);

    modelFile >> buffer; // last of the header, number of vertices
    int num_verts = atoi(buffer);
    anim.vertsPerFrame = num_verts;

    anim.faces = new triFace* [anim.numFaces];

    for (int i = 0; i < anim.numFaces; i++) { // load all the face references
        anim.faces[i] = new triFace(); //[3] // make new face reference
        for (int j = 0; j < 3; j++) { // for each vertex of the face
            modelFile >> buffer;
            anim.faces[i]->vertex[j] = atoi(buffer); // load vertices that face is referencing
        }
        modelFile.ignore(3); // ignore space and null bytes
    }

    if (anim.numActions > 0) {
        anim.actions = new Action* [anim.numActions];
        modelFile.ignore();
        for (int i = 0; i < anim.numActions; i++) {
            modelFile.ignore(4);
            anim.actions[i] = new Action();
            modelFile >> anim.actions[i]->name;

            modelFile >> buffer;

            anim.actions[i]->numFrames = atoi(buffer);

            anim.actions[i]->frames = new frame*[anim.actions[i]->numFrames];

            for (int j = 0; j < anim.actions[i]->numFrames; j++) { // load all the frames
                if (j > 0) {
                    modelFile.ignore(4); // ignore the FRME magic value; for some reason this is only needed if frame >0
                }
                anim.actions[i]->frames[j] = new frame();
                anim.actions[i]->frames[j]->vertices = new Vector3f [num_verts];
                for (int k = 0; k < num_verts; k++) { // load all the vertices
                    anim.actions[i]->frames[j]->vertices[k] = Vector3f();
                    for (int m = 0; m < 3; m++) { // load xyz of verts
                        modelFile >> buffer;
                        anim.actions[i]->frames[j]->vertices[k][m] = atof(buffer); // load each xyz of every vert of every frame
                    }
                    std::cout << anim.actions[i]->frames[j]->vertices[k] << std::endl; //[k][m] << ", ";
                    modelFile.ignore(2);
                }
            }
        }
    } else {
        anim.numActions = 1;
        anim.actions = new Action*[1];
        anim.actions[0] = new Action();
        anim.actions[0]->name = "Idle";
        anim.actions[0]->numFrames = 1;
        anim.actions[0]->frames = new frame*[1];
        anim.actions[0]->frames[0] = new frame();
        anim.actions[0]->frames[0]->vertices = new Vector3f[num_verts];

        modelFile.ignore(4); // ignore MESH value. I know ignoring is a bad habit...

        for (int i = 0; i < num_verts; i++) { // load all the vertices
            anim.actions[0]->frames[0]->vertices[i] = Vector3f();
            for (int j = 0; j < 3; j++) { // load xyz of verts
                modelFile >> buffer;
                anim.actions[0]->frames[0]->vertices[i][j] = atof(buffer); // load each xyz of every vert of every frame
            }
            //std::cout << anim.actions[0]->frames[0]->vertices[i] << std::endl; //[k][m] << ", ";
            modelFile.ignore(2);
        }
        modelFile.close();
        delete buffer;
    }
    /*
    std::cout << "seg? " << anim.actions[2]; //->frames[0]->vertices[0] << std::endl; //[k][m] << ", ";

    for (int i = 0; i < anim.numActions; i++) {
        std::cout << "action " << i << ": " << anim.actions[i]->name << ", " << anim.actions[i]->numFrames << std::endl;
        for (int j = 0; j < anim.actions[i]->numFrames; j++) {
            std::cout << "frame " << j << std::endl;
            for (int k = 0; k < num_verts; k++) {
                for (int m = 0; m < 3; m++) {
                    std::cout << anim.actions[i]->frames[j]->vertices; //[k][m] << ", ";
                }
                std::cout << " :: " << k << std::endl;
            }
        }
    }*/
    calculateSurfaceNormals();
}

void Model::calculateSurfaceNormals() {
    for (int i = 0; i < anim.numFaces; i++) {
        anim.faces[i]->normals = new Vector3f*[anim.numActions];
        for (int j = 0; j < anim.numActions; j++) {
            anim.faces[i]->normals[j] = new Vector3f[anim.actions[j]->numFrames];
            for (int k = 0; k < anim.actions[j]->numFrames; k++) {
                Vector3f side1 = anim.actions[j]->frames[k]->vertices[anim.faces[i]->vertex[1]] - anim.actions[j]->frames[k]->vertices[anim.faces[i]->vertex[2]];
                Vector3f side2 = anim.actions[j]->frames[k]->vertices[anim.faces[i]->vertex[2]] - anim.actions[j]->frames[k]->vertices[anim.faces[i]->vertex[0]];
                anim.faces[i]->normals[j][k] = side1.crossProduct(side2);
                anim.faces[i]->normals[j][k].normalize();
            }
        }
    }
}

void Model::swapTexture(std::string fileName) {
    texture = 0;//loadTexture(fileName);
}

void Model::shift3f(Vector3f shift) {
    for (int i = 0; i < anim.numActions; i++)
        for (int j = 0; j < anim.actions[i]->numFrames; j++)
            for (int k = 0; k < anim.vertsPerFrame; k++)
                anim.actions[i]->frames[j]->vertices[k] += shift;
}

void Model::scalef(float scale) {
    for (int i = 0; i < anim.numActions; i++)
        for (int j = 0; j < anim.actions[i]->numFrames; j++)
            for (int k = 0; k < anim.vertsPerFrame; k++)
                anim.actions[i]->frames[j]->vertices[k] *= scale;
}

void Model::jumpToFrame(int frame) {
    if (anim.actions[anim.currentAction]->numFrames == 0) {
        std::cout << "jumptoError: numframes = 0; current action: " << anim.currentAction << std::endl;
        anim.currentFrame = 0;
    }
    anim.currentFrame = frame % anim.actions[anim.currentAction]->numFrames;
}

void Model::advanceFrame() {
    anim.currentFrame = (anim.currentFrame + 1) % anim.actions[anim.currentAction]->numFrames;
}

void Model::Update() {
    if (playing) {
        advanceFrame();
    }
}

bool Model::isPlaying() {
    return playing;
}

void Model::play() {
    playing = true;
}

void Model::pause() {
    playing = false;
    jumpToFrame(0);
}

void Model::changeAction(int to) {
    if (anim.currentAction != to) {
        anim.currentAction = to;
        anim.currentFrame = 0;
    }
}

int Model::getAction() {
    return anim.currentAction;
}

void Model::Draw() {

    if ((signed int) texture != -1) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glEnable(GL_TEXTURE_2D);
    } else {
        glDisable(GL_TEXTURE_2D);
    }
    
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < anim.numFaces; i++) {
        glNormal3fv(anim.faces[i]->normals[anim.currentAction][anim.currentFrame]);
        glTexCoord2d(0, 0);
        glVertex3fv(anim.actions[anim.currentAction]->frames[anim.currentFrame]->vertices[anim.faces[i]->vertex[0]]);
        glTexCoord2d(1, 0);
        glVertex3fv(anim.actions[anim.currentAction]->frames[anim.currentFrame]->vertices[anim.faces[i]->vertex[1]]);
        glTexCoord2d(1, 1);
        glVertex3fv(anim.actions[anim.currentAction]->frames[anim.currentFrame]->vertices[anim.faces[i]->vertex[2]]);
    }
    glEnd();
    /*
   glPointSize(3);
   glBegin(GL_POINTS);
   for (int i = 0; i < 252; i++) {
       glVertex3fv(anim.frames[0]->vertices[i]);
   }
   glEnd();*/
    glDisable(GL_TEXTURE_2D);
}

void Model::glGenerateDisplayList(Vector3f offset) {
    //glColor3f(color.x, color.y, color.z);
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture);

    //glBegin(GL_TRIANGLES);
    glDisable(GL_TEXTURE_2D);
    for (int i = 0; i < anim.numFaces; i++) {
        glNormal3fv(anim.faces[i]->normals[anim.currentAction][anim.currentFrame]);
        glTexCoord2d(0, 0);
        glVertex3fv(anim.actions[anim.currentAction]->frames[anim.currentFrame]->vertices[anim.faces[i]->vertex[0]] + offset);
        glTexCoord2d(1, 0);
        glVertex3fv(anim.actions[anim.currentAction]->frames[anim.currentFrame]->vertices[anim.faces[i]->vertex[1]] + offset);
        glTexCoord2d(1, 1);
        glVertex3fv(anim.actions[anim.currentAction]->frames[anim.currentFrame]->vertices[anim.faces[i]->vertex[2]] + offset);
        //glDisable(GL_TEXTURE_2D);
    }
}

void Model::glGenerateDisplayList(Vector3f offset, Matrix3f modification) {
    glDisable(GL_TEXTURE_2D);
    glColor3f(color.x, color.y, color.z);
    Vector3f modedVert;
    for (int i = 0; i < anim.numFaces; i++) {
        modedVert = modification * (anim.faces[i]->normals[anim.currentAction][anim.currentFrame]);
        glNormal3fv(modedVert);
        glTexCoord2d(0, 0);
        modedVert = modification * (anim.actions[anim.currentAction]->frames[anim.currentFrame]->vertices[anim.faces[i]->vertex[0]]) + offset;
        glVertex3fv(modedVert);
        glTexCoord2d(1, 0);
        modedVert = modification * (anim.actions[anim.currentAction]->frames[anim.currentFrame]->vertices[anim.faces[i]->vertex[1]]) + offset;
        glVertex3fv(modedVert);
        glTexCoord2d(1, 1);
        modedVert = modification * (anim.actions[anim.currentAction]->frames[anim.currentFrame]->vertices[anim.faces[i]->vertex[2]]) + offset;
        glVertex3fv(modedVert);
    }
}

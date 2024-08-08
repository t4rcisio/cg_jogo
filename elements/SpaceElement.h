#ifndef SPACE_ELEMENT_H
#define SPACE_ELEMENT_H

#include <GL/glut.h>
#include "SOIL/SOIL.h"
#include <cstdio>
#include <ctime>
#include <experimental/random>

class SpaceElement {
public:
    GLuint elementID;
    int posX;
    int posY;
    const char *elementName;
    const char *elementPath;
    int *subIds;
    int subIdsMax;
    SpaceElement* SUB_IDS;

    SpaceElement();
    SpaceElement(const char* path, const char* name);
    void setSubIds(int *list, int vmax);
    int genRandomCoord(int min, int max);
    void setSubItens(int *subIds, int vMax);

    int generate();

private:
    GLuint loadTexture(const char* path);
};

#endif // SPACE_ELEMENT_H

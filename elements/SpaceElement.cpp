#include "SpaceElement.h"
#include <random>



SpaceElement::SpaceElement()
    : elementID(0), posX(0), posY(0), elementName(nullptr), elementPath(nullptr)  {}

SpaceElement::SpaceElement(const char* path, const char* name)
    : elementPath(path), elementName(name) {}


void SpaceElement::setSubItens(int *subIds, int vMax) {

    SpaceElement::SUB_IDS = new SpaceElement[vMax];

    for (int index=0; index++; index<=vMax){

        SpaceElement::SUB_IDS[index] = SpaceElement(elementPath, elementName);
        SpaceElement::SUB_IDS[index].elementID = subIds[index];
        SpaceElement::SUB_IDS[index].posX = genRandomCoord(0,500);
        SpaceElement::SUB_IDS[index].posY = genRandomCoord(0,500);
    }

}

void SpaceElement::setSubIds(int *subIds, int vMax) {

    SpaceElement::subIds = subIds;
    SpaceElement::subIdsMax = vMax;

    setSubItens(subIds, vMax);
    
}

int SpaceElement::generate() {
    elementID = loadTexture(elementPath);
    return elementID;
}

int SpaceElement::genRandomCoord(int min, int max){

    std::srand(time(nullptr));
    int valor = std::experimental::randint(min,max);

    return valor;

}


GLuint SpaceElement::loadTexture(const char* path) {
    GLuint idTexture = SOIL_load_OGL_texture(
        path,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (idTexture == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTexture;
}

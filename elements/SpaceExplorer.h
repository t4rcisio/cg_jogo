#ifndef SPACE_EXPLORER_H
#define SPACE_EXPLORER_H

#include <GL/freeglut.h>
#include "SOIL/SOIL.h"
#include "SpaceElement.h"

class SpaceExplorer {
public:
    int currentLevel;
    int levelDuration; // in milliseconds
    int levelStartTime;
    int world_length;
    int world_height;
    const char* window_title;
    int counter;
    float posY;
    float posX;
    int speed;
    char* speedText;
    int img_index;
    int key_control;
    int BUFF_SIZ;
    bool drawCircle;
    bool drawSquare;
    float mouseX, mouseY;
    SpaceElement* OBJ_IDS;
    SpaceElement* MOVE_IDS;
    char currentState;
    float planetSpeed;
    
    SpaceExplorer(int max_length, int max_height, const char* title);
    ~SpaceExplorer();

    void drawCircleAt(float x, float y, float radius);
    void drawSquareAt(float x, float y, float size);
    void drawObject(GLuint elementID, int length, int height);
    void drawObject_Coords(GLuint elementID, int length, int height, float coords[8]);
    void redimensionada(int width, int height);
    void displayText(void* font, char* s, float x, float y);
    void mouseTracker(int x, int y);
    void mouseEvent(int button, int state, int x, int y);
    void keyboardUp(unsigned char key, int x, int y);
    void keyboard(unsigned char key, int x, int y);
    void xyControl(int IMG_SIZE);
    float* calculateCoords(int index, int tsize);
    void desenha();
    void drawMenu();
    void idle();
    void updateBackground(int id, SpaceElement *PARENT);
    void checkColision();
    void startLevelTimer();
    void updateLevel();
    


    static void backgroudClock(int value);
    static void desenhaWrapper();
    static void redimensionadaWrapper(int width, int height);
    static void keyboardWrapper(unsigned char key, int x, int y);
    static void keyboardUpWrapper(unsigned char key, int x, int y);
    static void mouseEventWrapper(int button, int state, int x, int y);
    static void idleWrapper();
    static void backgroudWrapper();
    void inicializa();
    bool checkCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
    void handleCollision();
    void drawGameOver();
    int show(int argc, char** argv);

private:
    static SpaceExplorer* instance;
    GLuint loadTexture(const char* path);
};

#endif // SPACE_EXPLORER_H


#include "SpaceExplorer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SpaceElement.h"

enum GameState { MENU, INFO, GAME, GAME_OVER };
SpaceExplorer* SpaceExplorer::instance = nullptr;
SpaceElement *OBJ_IDS;
GameState currentState;


SpaceExplorer::SpaceExplorer(int max_length, int max_height, const char* title) {
    
    currentLevel = 1;
    levelDuration = 60000; // 60 seconds in milliseconds
    levelStartTime = 0;
    planetSpeed = 1.0f;

    world_length = max_length;
    world_height = max_height;
    window_title = title;
    counter = 0;

    BUFF_SIZ = 30;

    posY = world_length / 2.0f;
    posX = world_height / 2.0f;
    speed = 1;
    speedText = (char*)malloc(BUFF_SIZ * sizeof(char));
    img_index = 12;
    key_control = 0;
    drawCircle = false;
    drawSquare = false;

    OBJ_IDS = new SpaceElement[100];
    MOVE_IDS = new SpaceElement[100];
    currentState = MENU;
}

SpaceExplorer::~SpaceExplorer() {
    free(speedText);
    delete[] OBJ_IDS;
}

void SpaceExplorer::startLevelTimer() {
    levelStartTime = glutGet(GLUT_ELAPSED_TIME);
}

void SpaceExplorer::drawObject(GLuint elementID, int length, int height) {
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, elementID);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0); glVertex3f(length, 0, 0);
    glTexCoord2f(1, 1); glVertex3f(length, height, 0);
    glTexCoord2f(0, 1); glVertex3f(0, height, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void SpaceExplorer::drawObject_Coords(GLuint elementID, int length, int height, float coords[8]) {
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, elementID);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(coords[0], coords[1]); glVertex3f(0, 0, 0);
    glTexCoord2f(coords[2], coords[3]); glVertex3f(length, 0, 0);
    glTexCoord2f(coords[4], coords[5]); glVertex3f(length, height, 0);
    glTexCoord2f(coords[6], coords[7]); glVertex3f(0, height, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
bool SpaceExplorer::checkCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void SpaceExplorer::redimensionada(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, world_length, 0, world_length, -1, 1);

    float hViewport;
    float wViewport;
    float yViewport;
    float xViewport;

    float xyChange ;

    float razaoAspectoJanela = ((float)width) / height;
    float razaoAspectoMundo = ((float)world_length) / world_height;
    if (razaoAspectoJanela < razaoAspectoMundo) {
        hViewport = width / razaoAspectoMundo;
        yViewport = (height - hViewport) / 2;
        glViewport(0, yViewport, width, hViewport);
    } else if (razaoAspectoJanela > razaoAspectoMundo) {
        wViewport = ((float)height) * razaoAspectoMundo;
        xViewport = (width - wViewport) / 2;
        glViewport(xViewport, 0, wViewport, height);
    } else {
        glViewport(0, 0, width, height);
        xyChange = 0;
        
    }
    
    printf("Viewport: %f %f\n", xViewport, yViewport);
    printf("Bar: %f %f\n", wViewport, hViewport);
    printf("size: %d %d\n", width, height);

    //world_length =  width + xViewport;
    //world_height =  height + yViewport;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SpaceExplorer::displayText(void* font, char* s, float x, float y) {
    int i;
    glRasterPos3f(x, y, 0);

    for (i = 0; s[i] != '\0'; i++) {
        glutBitmapCharacter(font, s[i]);
    }
}

void SpaceExplorer::mouseTracker(int x, int y) {
    mouseX = x;
    mouseY = y;
}

void SpaceExplorer::mouseEvent(int button, int state, int x, int y) {

    if (button == 0 && state == 0){

        if (x >= world_length-60 && x <= world_length-10){
            
            if (y >= 10 && y <= 60){

                speed -=1;

                if (speed <=0){
                    speed = 1;
                }
            }
        }

        if (x >= world_length-120 && x <= world_length-60){
            
            if (y >= 10 && y <= 60){

                speed +=1;

                if (speed > 10){
                    speed = 10;
                }
            }

        }

        char *temp = "SPEED";
        speedText[0] = '\0';

        char newInfo[30]; 
        snprintf(newInfo, sizeof(newInfo), "%s: %d\0", temp, speed);

        strcpy(speedText, newInfo);

    }
}

void SpaceExplorer::keyboardUp(unsigned char key, int x, int y) {
        img_index = 12;
        key_control = false;

}

void SpaceExplorer::keyboard(unsigned char key, int x, int y) {
    char mKey = (char)key;

     if (currentState == MENU) {
        if (key == 's' || key == 'S') {
                currentState = GAME;
                currentLevel = 1;
                planetSpeed = 1.0f;
                OBJ_IDS[4].subIdsMax = 3; // Reset to initial number of planets
                // Reset planet positions
                for (int i = 0; i < OBJ_IDS[4].subIdsMax; i++) {
                    OBJ_IDS[4].SUB_IDS[i].posX = OBJ_IDS[4].SUB_IDS[i].genRandomCoord(0, world_length);
                    OBJ_IDS[4].SUB_IDS[i].posY = OBJ_IDS[4].SUB_IDS[i].genRandomCoord(world_height, world_height * 2);
                }
                startLevelTimer();
        } else if (key == 'i' || key == 'I') {
            currentState = INFO;
        }
    } else if (currentState == INFO) {
        if (key == 27) { // ESC key
            currentState = MENU;
        }
    } else if (currentState == GAME) {
        // Existing game controls
        if (key == 27) { // ESC key
            currentState = MENU;
        } else { 
            if (key == 119 || key == 87){ // W
                //printf("\nPARA CIMA");
                key_control = true;
                posY +=1*speed;
                img_index = 22;
                glutPostRedisplay();
                
            }
            if (key == 115 || key == 83){ // S
                //printf("\nPARA BAIXO");
                key_control = true;
                posY -=1*speed;
                img_index = 2;
                glutPostRedisplay();
                
            }
            if (key == 100 || key == 68){ // D
                //printf("\nPARA DIREITA");
                key_control = true;
                posX +=1*speed;
                img_index = 14;
                glutPostRedisplay();
                
            }
            if (key == 97 || key == 65){ // A
                //printf("\nPARA ESQUERDA");
                key_control = true;
                posX -=1*speed;
                img_index = 10;
                glutPostRedisplay();
                
            }
            if(key == 32){

                printf("FIRE");

            }

            if (key == ' ') {
                drawSquare = true;
            }

            if (key == 27) {
                exit(0);
            }
        }
    }
    else if (currentState == GAME_OVER){
        if (key == 27) { // ESC key
            currentState = MENU;
            posX = world_length / 2.0f;
            posY = world_height / 2.0f;
        }
    }
    glutPostRedisplay();
}

void SpaceExplorer::xyControl(int IMG_SIZE) {
    if (posY >= world_height - IMG_SIZE){
                posY = world_height-1 - IMG_SIZE;
            }

            if (posY <= 0 ){
                posY = 1 ;
            }

            if (posX >= world_length - IMG_SIZE){
                posX = world_length-1 - IMG_SIZE;
            }

            if (posX <= 0){
                posX = 1;
            }

}

float* SpaceExplorer::calculateCoords(int index, int tsize) {
   static float Coords[8];
            int row = index / tsize;
            int col = index % tsize;
            float size = 1.0f / tsize; 

            Coords[0] = col * size;
            Coords[1] = row * size;
            Coords[2] = (col + 1) * size;
            Coords[3] = row * size;
            Coords[4] = (col + 1) * size;
            Coords[5] = (row + 1) * size;
            Coords[6] = col * size;
            Coords[7] = (row + 1) * size;

            return Coords;
}

void SpaceExplorer::desenha() {
    xyControl(80);

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    // DESENHA O FUNDO
    drawObject(OBJ_IDS[0].elementID, world_length, world_height);

    // DESENHA A NAVE
    glPushMatrix();
    glTranslatef(posX, posY, 0);
    //drawObject_Coords(OBJ_IDS[1], 80, 80);
    float* Coords = calculateCoords(img_index, 5);
    drawObject_Coords(OBJ_IDS[1].elementID, 80, 80, Coords);
    glPopMatrix();

    displayText(GLUT_BITMAP_8_BY_13, "TARCISIO B. PRATES", world_length-450, world_height-20);

    displayText(GLUT_BITMAP_8_BY_13, speedText, world_length-200, world_height-40);

    char planetText[30];
    //sprintf(planetText, "Planets: %d", OBJ_IDS[4].subIdsMax);
    displayText(GLUT_BITMAP_8_BY_13, planetText, 15, world_height-60);


    // Display current level
    char levelText[20];
    sprintf(levelText, "Level: %d", currentLevel);
    displayText(GLUT_BITMAP_8_BY_13, levelText, 10, world_height-80);

    // Display time remaining
    int timeRemaining = (levelDuration - (glutGet(GLUT_ELAPSED_TIME) - levelStartTime)) / 1000;
    char timeText[20];
    sprintf(timeText, "Time: %d", timeRemaining);
    displayText(GLUT_BITMAP_8_BY_13, timeText, 10, world_height - 100);

    
    glPushMatrix();
    glTranslatef(world_length-120, world_height-60, 0);
    drawObject(OBJ_IDS[2].elementID, 50, 50);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(world_length-60, world_height-60, 0);
    drawObject(OBJ_IDS[3].elementID, 50, 50);
    glPopMatrix();
    //drawObject(OBJ_IDS[3], 80, 80);

    // DESENHA A PLANETA

    for (int index = 0; index <OBJ_IDS[4].subIdsMax; index++){

        //updateBackground(index, &OBJ_IDS[4]);
        glPushMatrix();
        glTranslatef(OBJ_IDS[4].SUB_IDS[index].posX, OBJ_IDS[4].SUB_IDS[index].posY, 0);
        //drawObject_Coords(OBJ_IDS[1], 80, 80);
        Coords = calculateCoords(OBJ_IDS[4].subIds[index], 4);
        drawObject_Coords(OBJ_IDS[4].elementID, 80, 80, Coords);
        glPopMatrix();

        if (checkCollision(posX, posY, 80, 80, 
                           OBJ_IDS[4].SUB_IDS[index].posX, OBJ_IDS[4].SUB_IDS[index].posY, 80, 80)) {
            // Collision detected!
            handleCollision();
        }

    }


    glutSwapBuffers();

}

void SpaceExplorer::updateBackground(int id, SpaceElement *PARENT) {
    PARENT->SUB_IDS[id].posY = PARENT->SUB_IDS[id].posY - (1 * planetSpeed);

    if (PARENT->SUB_IDS[id].posY <= 0) {
        PARENT->SUB_IDS[id].posX = PARENT->SUB_IDS[id].genRandomCoord(0, world_length);
        PARENT->SUB_IDS[id].posY = PARENT->SUB_IDS[id].genRandomCoord(world_height, world_height * 2);
    }
}

void SpaceExplorer::handleCollision() {
    currentState = GAME_OVER;
    currentLevel = 1;
    planetSpeed = 1.0f;
    OBJ_IDS[4].subIdsMax = 3; // Reset to initial number of planets
    // Reset planet positions
    for (int i = 0; i < OBJ_IDS[4].subIdsMax; i++) {
        OBJ_IDS[4].SUB_IDS[i].posX = OBJ_IDS[4].SUB_IDS[i].genRandomCoord(0, world_length);
        OBJ_IDS[4].SUB_IDS[i].posY = OBJ_IDS[4].SUB_IDS[i].genRandomCoord(world_height, world_height * 2);
    }
}

void  SpaceExplorer::checkColision(){

    for (int index = 0; index <OBJ_IDS[4].subIdsMax; index++){

        updateBackground(index, &OBJ_IDS[4]);
    }
}


void SpaceExplorer::backgroudClock(int value) {

    //instance->desenha();

    for (int index = 0; index < instance->OBJ_IDS[4].subIdsMax; index++){

        instance->updateBackground(index, &instance->OBJ_IDS[4]);
    }

    //instance->counter = instance->counter + 1;
    glutTimerFunc(100, backgroudClock, 0);

    
}

void SpaceExplorer::drawGameOver() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw the Game Over texture
    drawObject(OBJ_IDS[8].elementID, world_length, world_height);
    
    // You can add additional text if you want
    displayText(GLUT_BITMAP_8_BY_13, "Press ESC to return to menu", world_length/2 - 100, 50);

    glutSwapBuffers();
}

// void SpaceExplorer::idle() {

//     glutPostRedisplay();
// }

void SpaceExplorer::desenhaWrapper() {
    if (instance) {
        switch(instance->currentState) {
            case MENU:
            case INFO:
                instance->drawMenu();
                break;
            case GAME:
                instance->desenha();
                break;
            case GAME_OVER:
                instance->drawGameOver();
                break;
        }
    }
}

void SpaceExplorer::redimensionadaWrapper(int width, int height) {
    if (instance) {
        instance->redimensionada(width, height);
    }
}

void SpaceExplorer::keyboardWrapper(unsigned char key, int x, int y) {
    if (instance) {
        instance->keyboard(key, x, y);
    }
}

void SpaceExplorer::keyboardUpWrapper(unsigned char key, int x, int y) {
    if (instance) {
        instance->keyboardUp(key, x, y);
    }
}

void SpaceExplorer::mouseEventWrapper(int button, int state, int x, int y) {
    if (instance) {
        instance->mouseEvent(button, state, x, y);
    }
}

void SpaceExplorer::idleWrapper() {
    if (instance) {
        instance->idle();
    }
}

void SpaceExplorer::inicializa() {
    printf("inicializa\n");
            glClearColor(0, 0, 0, 0);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // CERREGA OS ELEMENTOS

            OBJ_IDS[0] = SpaceElement("./img/space.png", "SPACE");
            OBJ_IDS[0].elementID = OBJ_IDS[0].generate();

            SpaceElement x_wing("./img/avioes.png", "X-WING");
            x_wing.elementID = x_wing.generate();
            OBJ_IDS[1] = x_wing;

            SpaceElement up_button("./img/up.png", "X-WING"); 
            up_button.elementID = up_button.generate();
            OBJ_IDS[2] = up_button;

            SpaceElement down_button("./img/down.png", "X-WING");
            down_button.elementID = down_button.generate();
            OBJ_IDS[3] = down_button;

            SpaceElement planets("./img/planets.png", "planets"); 
            planets.elementID = planets.generate();
            OBJ_IDS[4] = planets;
            
            SpaceElement shot("./img/shot.png", "shot"); 
            shot.elementID = shot.generate();
            OBJ_IDS[5] = shot;

            SpaceElement menuBackground("./img/menu.png", "MENU");
            menuBackground.elementID = menuBackground.generate();
            OBJ_IDS[6] = menuBackground;

            SpaceElement infoBackground("./img/info.png", "INFO");
            infoBackground.elementID = infoBackground.generate();
            OBJ_IDS[7] = infoBackground;

            SpaceElement gameOverScreen("./img/gameover.png", "GAME_OVER");
            gameOverScreen.elementID = gameOverScreen.generate();
            OBJ_IDS[8] = gameOverScreen;

            int v[16] = {0, 1, 2, 3, 5, 6, 7, 8 ,9 ,10,11,12,13,14,15};
            OBJ_IDS[4].setSubIds(v, 16);

            int s[1] = {0};
            OBJ_IDS[5].setSubIds(s, 1);

            // strcpy(speedText,"SPEED: 1");
}

void SpaceExplorer::idle() {
    if (currentState == GAME) {
        updateLevel();
    }
    glutPostRedisplay();
}

void SpaceExplorer::updateLevel() {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if (currentTime - levelStartTime >= levelDuration) {
        currentLevel++;
        startLevelTimer(); // Reset the timer for the new level

        // Increase planet speed
        planetSpeed += 1.5f;
        speed+=2;

        // Add a new planet
        if (OBJ_IDS[4].subIdsMax < 16) { 
            int newPlanetIndex = OBJ_IDS[4].subIdsMax;
            OBJ_IDS[4].SUB_IDS[newPlanetIndex].posX = OBJ_IDS[4].SUB_IDS[newPlanetIndex].genRandomCoord(0, world_length);
            OBJ_IDS[4].SUB_IDS[newPlanetIndex].posY = OBJ_IDS[4].SUB_IDS[newPlanetIndex].genRandomCoord(world_height, world_height * 2);
            //OBJ_IDS[4].subIds[newPlanetIndex] = rand() % 16; 
            OBJ_IDS[4].subIdsMax++;
        }

        //printf("Level %d: %d planets, speed %.1f\n", currentLevel, OBJ_IDS[4].subIdsMax, planetSpeed);
    }
}

void SpaceExplorer::drawMenu() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (currentState == MENU) {
        drawObject(OBJ_IDS[6].elementID, world_length, world_height);

    } else if (currentState == INFO) {
        drawObject(OBJ_IDS[7].elementID, world_length, world_height);
    }

    glutSwapBuffers();
}

int SpaceExplorer::show(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(world_length, world_height);
    glutCreateWindow(window_title);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    instance = this;

    inicializa();
    currentState = MENU;
    
    startLevelTimer();

    glutDisplayFunc(desenhaWrapper);
    glutReshapeFunc(redimensionadaWrapper);
    glutKeyboardFunc(keyboardWrapper);
    glutKeyboardUpFunc(keyboardUpWrapper);
    glutMouseFunc(mouseEventWrapper);
    glutIdleFunc(idleWrapper);
    glutTimerFunc(1000, backgroudClock, 0);

    glutMainLoop();

    return 0;
}

#include "SpaceExplorer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SpaceElement.h"


SpaceExplorer* SpaceExplorer::instance = nullptr;
SpaceElement *OBJ_IDS;


SpaceExplorer::SpaceExplorer(int max_length, int max_height, const char* title) {
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
}

SpaceExplorer::~SpaceExplorer() {
    free(speedText);
    delete[] OBJ_IDS;
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

    displayText(GLUT_BITMAP_8_BY_13, "TARCISIO B. PRATES", world_length-450, world_height-40);

    displayText(GLUT_BITMAP_8_BY_13, speedText, world_length-200, world_height-40);

    
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
    }


    glutSwapBuffers();

}

void SpaceExplorer::updateBackground(int id, SpaceElement *PARENT){
    

    PARENT->SUB_IDS[id].posY = PARENT->SUB_IDS[id].posY - (1*speed);

    if (PARENT->SUB_IDS[id].posY <= 0){

        PARENT->SUB_IDS[id].posX = PARENT->SUB_IDS[id].genRandomCoord(0, world_length);
        PARENT->SUB_IDS[id].posY = PARENT->SUB_IDS[id].genRandomCoord(world_length, world_length*2);
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

void SpaceExplorer::idle() {

    glutPostRedisplay();
}

void SpaceExplorer::desenhaWrapper() {
    if (instance) {
        instance->desenha();
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
            
            int v[3] = {0, 1, 5};
            OBJ_IDS[4].setSubIds(v, 3);

            SpaceElement shot("./img/shot.png", "shot"); 
            shot.elementID = shot.generate();
            OBJ_IDS[5] = shot;

            int s[1] = {0};
            OBJ_IDS[5].setSubIds(s, 1);

            strcpy(speedText,"SPEED: 1");
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

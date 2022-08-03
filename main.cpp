#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include "game.h"
#include <stdlib.h>
#include <string>

#define PLAYER_SPEED 0.015


using namespace std;
// Global variables

char title[] = "Full-Screen & Windowed Mode";  // Windowed mode's title
int windowWidth  = 640;     // Windowed mode's width
int windowHeight = 480;     // Windowed mode's height
int windowPosX   = 50;      // Windowed mode's top-left corner x
int windowPosY   = 50;      // Windowed mode's top-left corner y

int level = 0;
extern int deaths;

extern float extraSpeed; // for difficulty adjustment
extern float obstacleSpeed; // base speed

extern float RIGHT_WALL; // play area borders
extern float LEFT_WALL;


int keyStates[] = {0,0,0,0}; //left,right,up,down

extern GLfloat xPos;     // Player's speed in x and y directions
extern GLfloat yPos;
int refreshMillis = 16;      // Refresh period in milliseconds

// Projection clipping area
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;

bool fullScreenMode = true; // Full-screen or windowed mode?

/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
}



void movePlayer()
{
    if(xPos< LEFT_WALL)
        xPos = LEFT_WALL;
    if(xPos>RIGHT_WALL - 0.1)
        xPos= RIGHT_WALL - 0.1;
    if(yPos<-1.0f)
        yPos=-1.0f;
    if(yPos>0.9f)
        yPos=0.9f;

    if(keyStates[0] && xPos > LEFT_WALL )           //LEFT ARROW
        xPos -= PLAYER_SPEED;
    if(keyStates[1] && xPos < RIGHT_WALL - 0.1)     //RIGHT ARROW
        xPos += PLAYER_SPEED;
    if(keyStates[2] && yPos < 0.9f)                 //UP ARROW
        yPos += PLAYER_SPEED;
    if(keyStates[3] && yPos > -1.0f)                //DOWN ARROW
        yPos -= PLAYER_SPEED;

    checkAllCollisions();
}

  void
  output(GLfloat x, GLfloat y, const char *text,float div = 1000.38  )  // TEXT RENDERING
  {
    const char *p;
    glColor3f(1.0,1.0,1.0);
    glLineWidth(3);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(1/div, 1/div, 1/100.38);
    for (p = text; *p; p++)
      glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
  }

void drawEndGameText()
{
        string res2 = "Deaths: " + to_string(deaths);
        output(-0.3,0.8,res2.c_str());
        output(-0.8,0.0,"You Made It",500.38);
        if(deaths > 0)
        {
            output(-0.1,-0.1,"(Barely)",2000.38);
        }
        output(-0.35,-0.2,"Press Esc to Exit",1500.38);
}

void drawGameText()
{
    string res2 = "Deaths: " + to_string(deaths);
    string res = "Level: " + to_string(level);

    output(-0.9,0.8,res.c_str());
    output(0.2,0.8,res2.c_str());
    output(0.3,-0.8,"Speed Control > +/-",3000.38);
    output(0.3,-0.85,"Level > Page Up/Down",3000.38);
    output(0.3,-0.9,"Reset > r",3000.38);
    output(0.3,-0.95,"FullScreen / Windowed > F1",3000.38);
}

float angle = 0;
void drawStartScreen()
{
    output(-0.9,  0.4,"Best Game",500.38);
    output(-0.9,  0.2,"You Ever",500.38);
    output(-0.9,  0.0,"Gonna Play",500.38);
    output( 0.55, -0.07,"(Period)",2500.38);
    output( -0.5, -0.5,"Press SPACE to START",1500.38);
    glPushMatrix();
    glTranslatef(0.65, 0.07,0.0);
    glRotatef(-angle * 2, 0.0, 0.0, 1);
    glBegin(GL_QUADS);
        glVertex2f(-0.05, -0.05);
        glVertex2f( 0.05, -0.05);
        glVertex2f( 0.05,  0.05);
        glVertex2f(-0.05,  0.05);
    glEnd();
    glPopMatrix();

    angle += 1;
    if(angle >= 360)
    {
        angle = 0;
    }
}
/* Callback handler for window re-paint event */
void display() {
   glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer

    drawBorder();

    if(level >= 1){

        movePlayer();
        drawPlayer();
        drawObstacles(level);

        if(level == 11)
        {
            drawEndGameText();
        }
        else if(level <= 10)
        {
            drawGameText();
        }
    }else if( level == 0)
    {
        drawStartScreen();
    }



   glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)

}

/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height) {
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;

   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);

   // Set the aspect ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset the projection matrix
   if (width >= height) {
      clipAreaXLeft   = -1.0 * aspect;
      clipAreaXRight  = 1.0 * aspect;
      clipAreaYBottom = -1.0;
      clipAreaYTop    = 1.0;
   } else {
      clipAreaXLeft   = -1.0;
      clipAreaXRight  = 1.0;
      clipAreaYBottom = -1.0 / aspect;
      clipAreaYTop    = 1.0 / aspect;
   }
   gluOrtho2D(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
}

/* Called back when the timer expired */
void Timer(int value) {
   glutPostRedisplay();    // Post a paint request to activate display()
   glutTimerFunc(refreshMillis, Timer, 0); // subsequent timer call at milliseconds
}

/* Callback handler for normal-key event */
void keyboard(unsigned char key, int x, int y) {
   switch (key) {
      case 27:     // ESC key - EXIT
         exit(0);
         break;
      case '+':     // MAKE THE OBSTACLES FASTER
        extraSpeed += 0.01;
        break;
      case '-':     // SLOW DOWN THE OBSTACLES
        if(extraSpeed + obstacleSpeed >= 0.01)
            extraSpeed -= 0.01;
        break;
      case 'r':         // RESET GAME
        level = 0;
        deaths = 0;
        break;
      case 32:    // SPACE KEY - START THE GAME
        if(level == 0)
            level += 1;
   }
}


/* Callback handler for special-key event */
void specialKeys(int key, int x, int y) {
   switch (key) {
      case GLUT_KEY_F1:                            // F1: Toggle between full-screen and windowed mode
         fullScreenMode = !fullScreenMode;         // Toggle state
         if (fullScreenMode) {                     // Full-screen mode
            windowPosX   = glutGet(GLUT_WINDOW_X); // Save parameters for restoring later
            windowPosY   = glutGet(GLUT_WINDOW_Y);
            windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
            windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();                      // Switch into full screen
         } else {                                         // Windowed mode
            glutReshapeWindow(windowWidth, windowHeight); // Switch into windowed mode
            glutPositionWindow(windowPosX, windowPosX);   // Position top-left corner
         }
         break;
      case GLUT_KEY_LEFT:     // GO LEFT
         keyStates[0] = 1; break;
      case GLUT_KEY_RIGHT:    // GO RIGHT
         keyStates[1] = 1; break;
      case GLUT_KEY_UP:       // GO UP
         keyStates[2] = 1; break;
      case GLUT_KEY_DOWN:     // GO DOWN
         keyStates[3] = 1; break;
      case GLUT_KEY_PAGE_UP:     // NEXT LEVEL
          if(level < 11)
            level += 1;
          break;
      case GLUT_KEY_PAGE_DOWN:     // PREVIOUS LEVEL
         if(level >= 1)
            level -= 1;
         break;
   }
}

void specialUpKeys(int key, int x, int y) {
   switch (key) {
      case GLUT_KEY_LEFT:     // CHECKS IF KEY IS UP THEN CHANGES KEY STATES ACCORDINGLY
         keyStates[0] = 0; break;
      case GLUT_KEY_RIGHT:
         keyStates[1] = 0; break;
      case GLUT_KEY_UP:
         keyStates[2] = 0; break;
      case GLUT_KEY_DOWN:
         keyStates[3] = 0; break;
   }
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutInitWindowPosition(windowPosX, windowPosY); // Initial window top-left corner (x, y)
   glutCreateWindow(title);      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);     // Register callback handler for window re-shape
   glutTimerFunc(0, Timer, 0);   // First timer call immediately

   glutIgnoreKeyRepeat(1);       // THIS IS NEEDED FOR KEYSTATES TO WORK
   glutSpecialFunc(specialKeys); // Register callback handler for special-key event
   glutSpecialUpFunc(specialUpKeys);
   glutKeyboardFunc(keyboard);   // Register callback handler for special-key event




   glutFullScreen();             // Put into full screen
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 0;
}

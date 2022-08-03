#include <GL/glut.h>
#include <GL/gl.h>
#include <time.h>
#include "game.h"
#include <iostream>

#define OBSTACLE_COUNT 5
#define LEVEL_COUNT 10


using namespace std;

GLfloat xPos = -0.1f;        // PLAYER POS X
GLfloat yPos = -1.0f;        // PLAYER POS Y

extern int level;
float obstacleSpeed = 0.05;  // BASE SPEED
float extraSpeed = 0.0;      // DIFFICULTY ADJUSTMENT

float RIGHT_WALL = 1.0;      //
float LEFT_WALL = -1.0;      // BORDERS

int deaths = 0;

class Obstacle
{
public:
    float r, g, b, speedMultiplier;
    int direction[LEVEL_COUNT];
    float xPos[LEVEL_COUNT], yPos[LEVEL_COUNT], width[LEVEL_COUNT], height[LEVEL_COUNT];
    Obstacle(const float &x, const float &y, const float &w, const float &h, const int &d, float rr = 1.0, float gg = 1.0, float bb = 1.0, float speed = 1.0)
    {
        for (int i = 0; i < LEVEL_COUNT; i++)
        {
            xPos[i] = *(&x + i);
            yPos[i] = *(&y + i);
            width[i] = *(&w + i);
            height[i] = *(&h + i);
            direction[i] = *(&d + i);
        }
        r = rr;
        g = gg;
        b = bb;
        speedMultiplier = speed;
    }
};
        //LEVEL        1    2     3    4    5     6      7     8     9    10
float obs1_X[10] =   {-1.0,-1.0,  0.0, 0.0, 0.5, -1.0,  -1.0, -1.0, -1.0,-1.0};
float obs1_Y[10] =   {0.0,  0.0,  0.2, 0.1, 0.5, -0.5,  -0.3, -0.4, -0.6,-0.6};
float obs1_W[10] =   {0.4,  0.4,  0.7, 0.4, 0.4,  0.8,   0.4,  0.4,  0.4, 0.4};
float obs1_H[10] =   {0.1,  0.1,  0.1, 0.1, 0.1,  0.1,   0.1,  0.1,  0.1, 0.1};
int obs1_D[10] =     {1,    0,    1  , 0  , 1  ,  1  ,   1  ,  1  ,  1  , 1  };

float obs2_X[10] =   {0.6,  0.0, -1.0, 1.0, 0.0, -0.4 ,  1.0,  1.0,  1.0, 1.0};
float obs2_Y[10] =   {0.2,  0.2, -0.2,-0.3,-0.5, -0.2 ,  0.3,  0.4,  0.6, 0.6};
float obs2_W[10] =   {0.4,  0.4,  0.7, 0.4, 0.4,  0.5 ,  0.4,  0.4,  0.4, 0.4};
float obs2_H[10] =   {0.1,  0.1,  0.1, 0.1, 0.1,  0.1 ,  0.1,  0.1,  0.1, 0.1};
int obs2_D[10] =     {0,    1,    0  , 0  , 1  ,  1   ,  1  ,  1  ,  1  , 1  };

float obs3_X[10] =   {0.0,  0.0, -0.2,-0.4, 0.0 , 0.0 ,  0.0,  0.0,  0.0, 0.0};
float obs3_Y[10] =   {0.0,  0.0,  0.4,-0.1,-0.3 , 0.2 ,  0.0,  0.0,  0.2, 0.2};
float obs3_W[10] =   {0,    0,    0  , 0.7, 0.4 , 0.4 ,  0.0,  0.4,  0.4, 0.4};
float obs3_H[10] =   {0,    0,    0  , 0.1, 0.1 , 0.1 ,  0.0,  0.1,  0.1, 0.1};
int obs3_D[10] =     {1,    1,    1  , 0,   1   , 0   ,  1,    1  ,  1  , 1  };

float obs4_X[10] =   {0.0,  0.0,  0.0,-0.8, 0.1 , 0.6 ,  0.0,  0.0,  0.5, 0.5};
float obs4_Y[10] =   {0.0,  0.0,  0.0, 0.8, 0.2 , 0.5 ,  0.0,  0.0, -0.2,-0.2};
float obs4_W[10] =   {0.0,  0.0,  0.0, 0.4, 0.4 , 0.55,  0.0,  0.0,  0.4, 0.4};
float obs4_H[10] =   {0.0,  0.0,  0.0, 0.1, 0.1 , 0.15,  0.0,  0.0,  0.1, 0.1};
int obs4_D[10] =     {0,    0,    0,   0,   1   , 1 ,    1  ,  1  ,  1  , 1  };

float levelUp_X[10] ={-0.1,-0.1, -0.1,-0.1,-0.1, -0.1,  -0.1 ,-0.1, -0.1,-0.1};
float levelUp_Y[10] ={ 0.8, 0.8,  0.8, 0.8, 0.8,  0.8,   0.8 , 0.8,  0.8, 0.8};
float levelUp_W[10] ={ 0.1, 0.1,  0.1, 0.1, 0.1,  0.1,   0.1 , 0.1,  0.1, 0.1};
float levelUp_H[10] ={ 0.1, 0.1,  0.1, 0.1, 0.1,  0.1,   0.1 , 0.1,  0.1, 0.1};
int levelUp_D[10] =  {-1  ,-1  , -1  ,-1  ,-1  , -1  ,  -1   ,-1  , -1  ,-1  };

Obstacle obs1(*obs1_X, *obs1_Y, *obs1_W, *obs1_H, *obs1_D, 1.0, 1.0, 1.0); // x, y, width , height, direction , r , g , b, speed
Obstacle obs2(*obs2_X, *obs2_Y, *obs2_W, *obs2_H, *obs2_D, 1.0, 1.0, 1.0); // x, y, width , height, direction , r , g , b, speed
Obstacle obs3(*obs3_X, *obs3_Y, *obs3_W, *obs3_H, *obs3_D, 1.0, 1.0, 1.0, 1.25); // x, y, width , height, direction , r , g , b, speed
Obstacle obs4(*obs4_X, *obs4_Y, *obs4_W, *obs4_H, *obs4_D, 1.0, 1.0, 1.0, 1.5); // x, y, width , height, direction , r , g , b, speed
Obstacle levelUp(*levelUp_X, *levelUp_Y, *levelUp_W, *levelUp_H, *levelUp_D, 1.0, 1.0, 0.0);

Obstacle allObstacles[] = {levelUp, obs1, obs2 , obs3 , obs4};

void drawPlayer()
{
    glLineWidth(1);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xPos, yPos);
    glVertex2f(xPos + 0.1, yPos);
    glVertex2f(xPos + 0.1, yPos + 0.1);
    glVertex2f(xPos, yPos + 0.1);
    glEnd();
}

void drawBorder()
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(LEFT_WALL, -1.0f);
    glVertex2f(LEFT_WALL, 1.0f);
    glVertex2f(-2.0f, 1.0f);
    glVertex2f(-2.0f, -1.0f);

    glVertex2f(RIGHT_WALL, -1.0f);
    glVertex2f(RIGHT_WALL, 1.0f);
    glVertex2f(2.0f, 1.0f);
    glVertex2f(2.0f, -1.0f);

    glEnd();
}

int frame= 0;
void obstacleMovement(Obstacle &obs, float obs_speed)  // MOVEMENT ALGO'S FOR OBSTACLES... PRETTYBAD RIGHT NOW
{
    int index = level - 1;

    if (obs.xPos[index] + obs.width[index] >= RIGHT_WALL)
        obs.direction[index] = 0;
    if (obs.xPos[index] <= LEFT_WALL)
        obs.direction[index] = 1;

    if (obs.direction[index] == 1) // RIGHT
    {
        obs.xPos[index] += obs_speed * obs.speedMultiplier;
    }
    else // LEFT
    {
        obs.xPos[index] -= obs_speed * obs.speedMultiplier;
    }

    if(index == 5)  //LEVEL 6
    {
        frame += 1;
        if(frame > 180)
            obs.yPos[index] -=  0.003;
        if(frame <= 180)
            obs.yPos[index] +=  0.003;
        if(frame > 360)
            frame = 0;
    }

    if(index == 6 || index == 7 || index == 8)  //LEVEL 7 8 9
    {
        frame += 1;
        if(frame > 30)
            obs.yPos[index] +=  0.0300;
        else
            obs.yPos[index] -=  0.0300;
        if(frame > 60)
        {
            obs.yPos[index] -=  0.0300;
            frame = 0;
        }
    }

    if(index == 9)          // FINAL LEVEL
    {
        frame += 1;
        if(frame > 180)
        {
            obs.yPos[index] +=  0.0030;
            LEFT_WALL  -=  0.003;
            RIGHT_WALL +=  0.003;
        }
        else
        {
            obs.yPos[index] -=  0.0030;
            LEFT_WALL  +=  0.003;
            RIGHT_WALL -=  0.003;
        }
        if(frame > 360)
        {
            obs.yPos[index] -=  0.0030;
            LEFT_WALL  +=  0.003;
            RIGHT_WALL -=  0.003;
            frame = 0;
        }
    }

}

bool checkCollision(Obstacle &obs)
{
    //    cout << obs.direction << "dir \n";    // AABB COLLISION DETECTION FROM LEARNOPENGL.COM
    int index = level - 1;
    bool collisionX = obs.xPos[index] + obs.width[index] >= xPos && xPos + 0.1f >= obs.xPos[index];

    bool collisionY = obs.yPos[index] + obs.height[index] >= yPos && yPos + 0.1f >= obs.yPos[index];

    return collisionX && collisionY;
}

void checkAllCollisions()
{
    int index = level - 1;
    int length = 3;

    switch(index)
    {
    case 0:
        length = 2; break;
    case 1:
    case 2:
        length = 3; break;
      case 3:
        length = 4; break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        length = 5; break;
    }
    for (int i = 0; i < length; i++)
    {
        bool res = checkCollision(allObstacles[i]);
        if (res)
        {
            xPos = -0.1f;
            yPos = -1.0f;
            deaths += 1;
            if (allObstacles[i].direction[index] == -1)
                {
                level += 1;
                deaths -= 1;
                }

            break;
        }
    }
}
void drawObstacle(int index, int obsCount)
{
    for (int i = 0; i < obsCount; i++)
    {

        glColor3f(allObstacles[i].r, allObstacles[i].g, allObstacles[i].b);
        glBegin(GL_QUADS);
        glVertex2f(allObstacles[i].xPos[index], allObstacles[i].yPos[index]);
        glVertex2f(allObstacles[i].xPos[index] + allObstacles[i].width[index], allObstacles[i].yPos[index]);
        glVertex2f(allObstacles[i].xPos[index] + allObstacles[i].width[index], allObstacles[i].yPos[index] + allObstacles[i].height[index]);
        glVertex2f(allObstacles[i].xPos[index], allObstacles[i].yPos[index] + allObstacles[i].height[index]);
        glEnd();

        if (allObstacles[i].direction[index] != -1)  // IF NOT LEVEL UP TOKEN DO THE MOVEMENTS
            obstacleMovement(allObstacles[i], (obstacleSpeed + extraSpeed) );
    }
}
void drawObstacles(int level)
{
    int index = level - 1;
    if(level != 10)   // STOP WALLS FROM MOVING
    {
        RIGHT_WALL = 1.0;
        LEFT_WALL = -1.0;
    }

    switch (level)
    {
    case 1: // LEVEL 1
    {
        drawObstacle(index,2);
        break;
    }
    case 2: // LEVEL 2
    {
        drawObstacle(index,3);
        break;
    }
    case 3: // LEVEL 3
    {
        obstacleSpeed = 0.08;
        drawObstacle(index,3);
        break;
    }
    case 4: // LEVEL 4
    {
        obstacleSpeed = 0.05;
        drawObstacle(index,4);
        break;
    }
    case 5: // LEVEL 5
    {
        drawObstacle(index,5);
        break;
    }
    case 6: // LEVEL 6
    {
        drawObstacle(index,5);
        break;
    }
    case 7: // LEVEL 7
    {
        drawObstacle(index,3);
        break;
    }
    case 8: // LEVEL 8
    {
        drawObstacle(index,4);
        break;
    }
    case 9: // LEVEL 9
    {
        drawObstacle(index,5);
        break;
    }
    case 10: // LEVEL 10
    {
        obstacleSpeed = 0.08;
        drawObstacle(index,5);
        break;
    }


    }
}

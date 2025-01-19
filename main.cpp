

#include <windows.h> // for MS Windows
#include<cstdio>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include<math.h>>
# define PI           3.14159265358979323846
using namespace std;


// Initial positions and speeds
GLfloat posSf1x, posSf1y, posSf2x, posSf2y, posf1x, posf1y, posf2x, posf2y, posVx, posVy; // Position for fish1 2
GLfloat speedF1 = 0.007f;
GLfloat speedF2 = 0.02f;
GLfloat speedSF1 = 0.03f;
GLfloat speedSF2 = -0.025f;
GLfloat speedv = -0.03f;
GLfloat speedVx = 0.02f;
GLfloat speedVy = 0.02f;


// Direction control for Object 1 and Object 2
bool direction1 = true; // true means moving from (1, 1) to (-1, -1), false means moving from (-1, -1) to (1, 1)
bool direction2 = true; // true means moving from (-1, 1) to (1, -1), false means moving from (1, -1) to (-1, 1)


void renderBitmapString(float x, float y, float z, void *font, char *string)
{
    char *c;
    glRasterPos3f(x, y,z);
    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}


void sea ()
{
    glBegin(GL_POLYGON);
    glColor3ub(5, 167, 247); // sky blue
    glVertex2f(0.0f, 1.0f);
    glColor3ub(44, 52, 255); // royal blue
    glVertex2f(1.0f, 1.0f);
    glColor3ub(0, 0, 0); // black
    glVertex2f(1.0f, -1.0f);
    glColor3ub(20, 22, 61); // navy blue
    glVertex2f(-1.0f, -1.0f);
    glColor3ub(44, 52, 255); // royal blue
    glVertex2f(-1.0f, 1.0f);
    glColor3ub(5, 167, 247); // sky blue
    glVertex2f(0.0f, 1.0f);
    glEnd();
}


void drawLine (float x, float y, float a, float b)
{
    //glLineWidth(2);
    glBegin (GL_LINES);
    glVertex2f (x, y);
    glVertex2f (a, b);
    glEnd();
}


void drawFiledEllipse(float cx, float cy, float rx, float ry, int num_segments)
{
    float theta = 2 * PI / float(num_segments);
    float c = cosf(theta);//precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = 1;//we start at angle = 0
    float y = 0;

    glBegin(GL_POLYGON);
    for(int ii = 0; ii < num_segments; ii++)
    {
        //apply radius and offset
        glVertex2f(x * rx + cx, y * ry + cy);//output vertex

        //apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();
}


void drawEmptyEllipse(float cx, float cy, float rx, float ry, int num_segments)
{
    float theta = 2 * PI / float(num_segments);
    float c = cosf(theta);//precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = 1;//we start at angle = 0
    float y = 0;

    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii < num_segments; ii++)
    {
        //apply radius and offset
        glVertex2f(x * rx + cx, y * ry + cy);//output vertex

        //apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();
}


void grass (float x, float y)
{
    glBegin (GL_POLYGON);
    //right 1
    glVertex2f (x+0.02f, y+0.0f);
    glVertex2f (x+0.085f, y+0.15f);
    glVertex2f (x-0.0f, y-0.01f);

    glVertex2f (x, y);
    glVertex2f (x+0.07f, y+0.20f);
    glVertex2f (x+0.01f, y);

    //right 2
    glVertex2f (x+0.01f, y+0.0f);
    glVertex2f (x+0.09f, y+0.29f);
    glVertex2f (x-0.0f, y-0.01f);

    //middle right:
    glVertex2f (x, y);
    glVertex2f (x+0.047f, y+0.22f);
    glVertex2f (x+0.045f, y+0.35f);
    glVertex2f (x+0.046f, y+0.22f);
    glVertex2f (x, y);

    glVertex2f (x, y);
    glVertex2f (x+0.035f, y+0.37f);
    glVertex2f (x, y);

    //middle 0
    glVertex2f (x-0.001f, y+0.0f);
    glVertex2f (x-0.0f, y+0.39f);
    glVertex2f (x+0.001f, y-0.0f);

    //middle left:
    glVertex2f (x, y);
    glVertex2f (x-0.045f, y+0.35f);
    glVertex2f (x, y);

    //left 2
    glVertex2f (x, y+0.01f);
    glVertex2f (x-0.09f, y+0.29f);
    glVertex2f (x-0.01f, y+0.0f);

    //left 1
    glVertex2f (x+0.0f, y-0.01f);
    glVertex2f (x-0.085f, y+0.15f);
    glVertex2f (x-0.02f, y+0.0f);

    glVertex2f (x+0.0f, y-0.01f);
    glVertex2f (x-0.095f, y+0.1f);
    glVertex2f (x-0.02f, y+0.0f);

    glEnd();
}


void grassAll ()
{
    //up 1st layer
    glColor3f(0.16f, 0.37f, 0.16f);
    for (float i=-0.9f; i<=1.0; i=i+0.15f)
    {
        grass (i, -0.55f);
    }

    //grass
    //up 2nd layer
    glColor3f(0.17f, 0.34f, 0.16f);
    for (float i=-1.0f; i<=1.0; i=i+0.15f)
    {
        grass (i, -0.7f);
    }

    //grass
    //up 3rd layer
    glColor3f(0.18f, 0.31f, 0.16f);
    for (float i=-0.9f; i<=1.0; i=i+0.15f)
    {
        grass (i, -0.85f);
    }

    //grass
    //up 4th layer
    glColor3f(0.19f, 0.28f, 0.16f);
    for (float i=-1.0f; i<=1.0; i=i+0.15f)
    {
        grass (i, -1.0f);
    }
}


void stoneAll ()
{
    for (float j=-1.0f; j<=-0.39f; j=j+0.3f)
    {
        for (float i=-1.0f; i<=1.1; i=i+0.2f)
        {
            //light
            glColor3f(0.25f, 0.24f, 0.30f);
            drawFiledEllipse(i, j, 0.06, 0.1, 15);
        }

        for (float i=-1.1f; i<=1.1; i=i+0.2f)
        {
            //dark brown
            glColor3ub (47, 42, 56);
            drawFiledEllipse(i, j, 0.06, 0.1, 15);
        }
    }

    for (float j=-1.15f; j<=-0.39f; j=j+0.3f)
    {
        for (float i=-1.0f; i<=1.1; i=i+0.2f)
        {
            //light brown
            glColor3f(0.1f, 0.17f, 0.15f);
            drawFiledEllipse(i, j, 0.06, 0.1, 15);
        }

        for (float i=-1.1f; i<=1.1; i=i+0.2f)
        {
            //deep brown
            glColor3ub (43, 34, 38);
            drawFiledEllipse(i, j, 0.06, 0.1, 15);
        }
    }
}


void updateV(int value)
{
    //for vehicle left to right
    /*
     posVy -= speedSF2;
     if (posVy >= 1.1f)
     {
         posVy = -1.65f;
     }
     */

    posVx -= speedv;
    if (posVx >= 1.1f)
    {
        posVx = -1.65f;
    }
    glutPostRedisplay(); // Redraw the screen
    glutTimerFunc(120, updateV, 0); // Continue updating at 100ms intervals
}


void OmniRoute (float x, float y)
{
    glColor3f(0.1f, 0.1f, 0.35f);
    drawFiledEllipse(x, y, 0.2f, 0.18f, 1000);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawEmptyEllipse(x, y, 0.2f, 0.18f, 1000);
    glColor3f(0.1f, 0.1f, 0.3f);
    drawFiledEllipse(x, y, 0.18f, 0.17f, 1000);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawEmptyEllipse(x, y, 0.18f, 0.17f, 1000);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawLine (x, y+0.18f, x, y-0.18f);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawLine (x+0.18f, y, x-0.18f, y);

    //BACK WINDOW:
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin (GL_POLYGON);
    glVertex2f (x-0.01f, y+0.01f);
    glVertex2f (x-0.01f, y+0.17f);
    glVertex2f (x-0.18f, y+0.01f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin (GL_LINES);
    glVertex2f (x-0.01f, y+0.01f);
    glVertex2f (x-0.01f, y+0.17f);
    glVertex2f (x-0.18f, y+0.01f);
    glEnd();

    //FRONT WINDOW:
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin (GL_POLYGON);
    glVertex2f (x+0.01f, y+0.01f);
    glVertex2f (x+0.01f, y+0.17f);
    glVertex2f (x+0.18f, y+0.01f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin (GL_LINES);
    glVertex2f (x+0.01f, y+0.01f);
    glVertex2f (x+0.01f, y+0.17f);
    glVertex2f (x+0.18f, y+0.01f);
    glEnd();

    //glLineWidth (3);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin (GL_LINES);
    //radder center
    glVertex2f (x, y+0.18f);
    glVertex2f (x, y+0.31f);
    //radder right
    glVertex2f (x+0.01f, y+0.18f);
    glVertex2f (x+0.01f, y+0.29f);
    //radder left
    glVertex2f (x-0.01f, y+0.18f);
    glVertex2f (x-0.01f, y+0.29f);
    glEnd();


    //radder center
    //signal center
    glColor3ub (52, 216, 235);
    for (float i=0.33f; i<=0.6f; i=i+0.025f)
    {
        drawFiledEllipse (x, y+i, 0.005f, 0.005f, 10);
    }
    //signal right
    glColor3ub (105, 239, 255);
    for (float i=0.30f; i<=0.58f; i=i+0.025f)
    {
        drawFiledEllipse (x+0.015f, y+i, 0.005f, 0.005f, 10);
    }
    //signal left
    glColor3ub (105, 239, 255);
    for (float i=0.30f; i<=0.58f; i=i+0.025f)
    {
        drawFiledEllipse (x-0.015f, y+i, 0.005f, 0.005f, 10);
    }


    //radder right
    //signal center
    glColor3ub (135, 233, 245);
    for (float i=0.33f, j=0.01f; i<=0.6f; i=i+0.025f)
    {
        drawFiledEllipse (x+j, y+i, 0.005f, 0.005f, 10);
    }
    //signal right
    glColor3ub (135, 233, 245);
    for (float i=0.30f, j=0.01f; i<=0.58f; i=i+0.025f)
    {
        drawFiledEllipse (x+0.015f+j, y+i, 0.005f, 0.005f, 10);
    }
    //signal left
    glColor3ub (135, 233, 245);
    for (float i=0.30f, j=0.01f; i<=0.58f; i=i+0.025f)
    {
        drawFiledEllipse (x-0.015f+j, y+i, 0.005f, 0.005f, 10);
    }


    //radder left
    //signal center
    glColor3ub (175, 242, 250);
    for (float i=0.33f, j=-0.01f; i<=0.6f; i=i+0.025f)
    {
        drawFiledEllipse (x+j, y+i, 0.005f, 0.005f, 10);
    }
    //signal right
    glColor3ub (175, 242, 250);
    for (float i=0.30f, j=-0.01f; i<=0.58f; i=i+0.025f)
    {
        drawFiledEllipse (x+0.015f+j, y+i, 0.005f, 0.005f, 10);
    }
    //signal left
    glColor3ub (175, 242, 250);
    for (float i=0.30f, j=-0.01f; i<=0.58f; i=i+0.025f)
    {
        drawFiledEllipse (x-0.015f+j, y+i, 0.005f, 0.005f, 10);
    }
}



void star (float x, float y)
{
    glBegin (GL_POLYGON);
    glColor3ub (212, 104, 74);
    glVertex2f (x+0.031f, y+0.0225f);
    glColor3ub (163, 80, 57);
    glVertex2f (x+0.10f, y+0.0f);
    glColor3ub (212, 104, 74);
    glVertex2f (x+0.031f, y-0.0225f);
    glColor3ub (163, 80, 57);
    glVertex2f (x+0.031f, y-0.095f);
    glColor3ub (212, 104, 74);
    glVertex2f (x-0.012f, y-0.0362f);
    glColor3ub (163, 80, 57);
    glVertex2f (x-0.081f, y-0.059f);
    glColor3ub (212, 104, 74);
    glVertex2f (x-0.038f, y+0.0f);
    glColor3ub (163, 80, 57);
    glVertex2f (x-0.081f, y+0.059f);
    glColor3ub (212, 104, 74);
    glVertex2f (x-0.012f, y+0.036f);
    glColor3ub (163, 80, 57);
    glVertex2f (x+0.031f, y+0.095f);
    glEnd();
}


void starFish (float x, float y)
{
    star (x, y);
    glColor3f(1.0f, 1.0f, 0.8f);
    drawFiledEllipse(x-0.02, y, 0.01f, 0.01f, 5);
    drawFiledEllipse(x+0.01, y, 0.01f, 0.01f, 5);

    glColor3f(0.0f, 0.0f, 0.0f);
    drawEmptyEllipse(x-0.02, y, 0.01f, 0.01f, 5);
    drawEmptyEllipse(x+0.01, y, 0.01f, 0.01f, 5);
}



void updateStarFish (int value)
{
    // updateV Object 1's position
    if (direction1)
    {
        // Move from (1, 1) to (-1, -1)
        posSf1x -= speedSF1;
        posSf1y -= speedSF1;
        if (posSf1x <= -1.5f && posSf1y <= -1.5f)
        {
            direction1 = false;  // Change direction to move back
        }
    }
    else
    {
        // Move from (-1, -1) to (1, 1)
        posSf1x += speedSF1;
        posSf1y += speedSF1;
        if (posSf1x >= 1.5f && posSf1y >= 1.5f)
        {
            direction1 = true;  // Change direction to move forward
        }
    }

    // updateV StarFish position
    if (direction2)
    {
        // Move from (-1, 1) to (1, -1)
        posSf2x -= speedSF2;
        posSf2y += speedSF2;
        if (posSf2x >= 1.5f || posSf2y <= -1.5f)
        {
            direction2 = false;
        }
    }
    else
    {
        // Move from (1, -1) to (-1, 1)
        posSf2x += speedSF2;
        posSf2y -= speedSF2;
        if (posSf2x <= -1.5f || posSf2y >= 1.5f)
        {
            direction2 = true;
        }
    }

    glutPostRedisplay(); // Redraw the screen
    glutTimerFunc(110, updateStarFish, 0); // Continue updating at 100ms intervals
}



void starFishAll ()
{
    glPushMatrix();
    glTranslatef(posSf2x, posSf2y, 0.0f);
    for (float i=-2.0f; i<=2.0f; i=i+1.0f)
    {
        for (float j=-2.0f; j<=2.0f; j=j+1.0f)
        {
            starFish (i, j);
        }
    }
    glPopMatrix();


    glPushMatrix();
    glTranslatef(posSf1x, posSf1y, 0.0f);
    for (float i=-2.5f; i<=2.5f; i=i+1.0f)
    {
        for (float j=-2.5f; j<=2.5f; j=j+1.0f)
        {
            starFish (i, j);
        }
    }
    glPopMatrix();
}


void fish1 (float x, float y)
{
    float s = 0.07f;

    //tail
    glBegin (GL_POLYGON);
    glColor3ub(94, 54, 29); //dark
    glVertex2f (x+s-(0.2*s), y);
    glColor3ub(176, 109, 67); //light
    glVertex2f (x+s+(0.7*s), y+(0.6*s));
    glColor3ub(120, 73, 44); //medium
    glVertex2f (x+s+(0.4f*s), y);
    glEnd();

    glBegin (GL_POLYGON);
    glColor3ub(94, 54, 29); //dark
    glVertex2f (x+s-(0.2*s), y);
    glColor3ub(176, 109, 67); //light
    glVertex2f (x+s+(0.7*s), y-(0.6*s));
    glColor3ub(120, 73, 44); //medium
    glVertex2f (x+s+(0.4f*s), y);
    glEnd();

    //body
    glColor3ub(120, 73, 44);
    drawFiledEllipse(x, y, s, s-0.03f, 1000);

    //pankha up
    glBegin (GL_POLYGON);
    glColor3ub(94, 54, 29); //dark
    glVertex2f (x-(0.25*s), y+(s-0.03));
    glColor3ub(176, 109, 67); //light
    glVertex2f (x-(0.25*s)+(0.7*s), y+(s-0.03)+(0.6*s));
    glColor3ub(120, 73, 44); //medium
    glVertex2f (x-(0.25*s)+(0.4f*s), y+(s-0.03));
    glEnd();

    //pankha down
    glBegin (GL_POLYGON);
    glColor3ub(94, 54, 29); //dark
    glVertex2f (x-(0.25*s), y-(s-0.03));
    glColor3ub(176, 109, 67); //light
    glVertex2f (x-(0.25*s)+(0.7*s), y-(s-0.03)-(0.6*s));
    glColor3ub(120, 73, 44); //medium
    glVertex2f (x-(0.25*s)+(0.4f*s), y-(s-0.03));
    glEnd();

    //eye:
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFiledEllipse (x-s+(s*0.30), y+(s*0.15), 0.004f, 0.005f, 100);

    //grid line verticle
    glColor3ub (102, 63, 40);
    //back
    drawLine (x+0.070f, y-0.006f, x+0.070f, y+0.006f);
    drawLine (x+0.056f, y-0.020f, x+0.056f, y+0.020f);
    drawLine (x+0.042f, y-0.026f, x+0.042f, y+0.026f);
    drawLine (x+0.028f, y-0.032f, x+0.028f, y+0.032f);
    drawLine (x+0.014f, y-0.034f, x+0.014f, y+0.034f);
    //middle
    drawLine (x, y-0.04f, x, y+0.04f);
    //front
    drawLine (x-0.017f, y-0.034f, x-0.017f, y+0.034f);
    drawLine (x-0.028f, y-0.032f, x-0.028f, y+0.032f);
    drawLine (x-0.040f, y-0.026f, x-0.040f, y+0.026f);

    //grid line horizontal
    drawLine (x+s, y, x-s+0.02f, y-0.005f);
}


void updateFish1(int value)
{
    //straight movement
    //fish1 right to left
    posf1x -= speedF1;
    if (posf1x <= -1.0f)
    {
        posf1x = 1.0f;
    }


    //sine wave movement
    // Fish1 movement pattern: sine wave
    static float angle = 0.0f; // Angle for sine wave calculation

    // Update x position (move left)
    posf1x -= speedF1;

    // Update y position based on sine wave
    posf1y = 0.2f * sin(angle);

    // Increment the angle for sine wave progression
    angle += 0.15f;
    if (angle > 2 * PI) // Keep angle within a full circle range
    {
        angle -= 2 * PI;
    }

    // Reset x position when fish exits the screen
    if (posf1x <= -2.0f)
    {
        posf1x = 1.5f;
    }


    glutPostRedisplay(); // Redraw the screen
    glutTimerFunc(100, updateFish1, 0); // Continue updating at 100ms intervals
}


void fish1All ()
{
    glPushMatrix();
    glTranslatef(posf1x, posf1y, 0.0f);

    for (float i=-2.5f; i<=2.5f; i=i+1.2f)
    {
        for (float j=-2.0f; j<=2.0f; j=j+1.2f)
        {
            fish1 (i, j);
        }
    }

    for (float i=-2.0f; i<=2.0f; i=i+1.2f)
    {
        for (float j=-2.5f; j<=2.5f; j=j+1.2f)
        {
            fish1 (i, j);
        }
    }

    glPopMatrix();
}


void fish2 (float x, float y)
{
    //body
    glBegin (GL_POLYGON);
    glColor3ub(221, 127, 38); //medium
    glVertex2f (x+0.05f, y);
    glColor3ub(245, 148, 56); //light
    glVertex2f (x+0.001f, y+0.045f);
    glColor3ub(204, 105, 12); //dark
    glVertex2f (x-0.05f, y);
    glColor3ub(204, 105, 12); //dark
    glVertex2f (x+0.001f, y-0.045f);
    glEnd();

    //tail upper
    glBegin (GL_POLYGON);
    glColor3ub(204, 105, 12); //dark
    glVertex2f (x-0.045f, y);
    glColor3ub(245, 148, 56); //light
    glVertex2f (x-0.08f, y+0.03f);
    glColor3ub(204, 105, 12); //dark
    glVertex2f (x-0.065f, y+0.001f);
    glEnd();

    //tail lower
    glBegin (GL_POLYGON);
    glColor3ub(204, 105, 12); //dark
    glVertex2f (x-0.045f, y);
    glColor3ub(245, 148, 56); //light
    glVertex2f (x-0.08f, y-0.03f);
    glColor3ub(204, 105, 12); //dark
    glVertex2f (x-0.065f, y-0.001f);
    glEnd();

    //eye
    glColor3ub (71, 54, 36);
    drawFiledEllipse (x+0.03f, y+0.003f, 0.002f, 0.003f, 100);

    //grid line verticle
    glColor3ub (181, 120, 56);
    //front
    drawLine (x+0.017f, y-0.025f, x+0.017f, y+0.025f);
    //middle
    drawLine (x, y-0.04f, x, y+0.04f);
    //back
    drawLine (x-0.017f, y-0.025f, x-0.017f, y+0.025f);
    drawLine (x-0.029f, y-0.015f, x-0.029f, y+0.015f);

    //grid line horizontal
    drawLine (x+0.025f, y, x-0.05f, y);
}


void updateFish2(int value)
{
    /*
    //straight movement
    //fish2 left to right
    posf2x += speedF2;
    if (posf2x >= 2.0f)
    {
        posf2x = -2.0f;
    }
    */

    //sine wave movement
    // Fish1 movement pattern: sine wave
    static float angleF2 = 0.0f; // Angle for sine wave calculation

    //Update x position (move right)
    posf2x += speedF2; // Change '-' to '+' to move right

    //Update y position based on sine wave
    posf2y = 0.2f * sin(angleF2);

    //Increment the angle for sine wave progression
    angleF2 += 0.15f;
    if (angleF2 > 2 * PI) // Keep angle within a full circle range
    {
        angleF2 -= 2 * PI;
    }

    //Reset x position when fish exits the screen
    if (posf2x >= 2.0f) // Change the condition for right side exit
    {
        posf2x = -1.5f; // Reset to the left side
    }


    glutPostRedisplay(); // Redraw the screen
    glutTimerFunc(110, updateFish2, 0); // Continue updating at 100ms intervals
}


void fish2All ()
{
    glPushMatrix();
    glTranslatef(posf2x, posf2y, 0.0f);

    for (float i=-2.5f; i<=2.5f; i=i+1.2f)
    {
        for (float j=-2.5f; j<=2.5f; j=j+1.2f)
        {
            fish2 (i, j);
        }
    }

    for (float i=-2.0f; i<=2.0f; i=i+1.2f)
    {
        for (float j=-2.0f; j<=2.0f; j=j+1.2f)
        {
            fish2 (i, j);
        }
    }

    glPopMatrix();
}


void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer (background)
    sea();

    /*
    //glLineWidth(2);
    glBegin(GL_LINES);
    // blue4
    glColor3f(0.04f, 0.141f, 0.3f);
    for (float i=-1.0f; i<=1.0; i=i+0.1f)
    {
        glVertex2f (-1.0f, i);
        glVertex2f (1.0f, i);
    }
    glEnd();

    //glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 0.0f);
    for (float i=-1.0f; i<=1.0; i=i+0.1f)
    {
        glVertex2f (i, -1.0f);
        glVertex2f (i, 1.0f);
    }
    glEnd();
    */


    glPushMatrix();
    glTranslatef(posVx, posVy, 0.0f);
    OmniRoute(0.0f, 0.0f);
    glPopMatrix();

    stoneAll ();
    grassAll ();

    starFishAll ();
    fish1All ();
    fish2All ();

    glFlush();
}


void sound()
{
    PlaySound("music.wav", NULL,SND_ASYNC|SND_FILENAME|SND_LOOP);
}


void handleKeypress(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'J': // Move right
    case 'j': // Move right
        posVx += speedVx;
        break;

    case 'F': // Move left
    case 'f': // Move left
        posVx -= speedVx;
        break;

    case 'H': // Move up
    case 'h': // Move left
        posVy += speedVy;
        break;

    case 'G': // Move down
    case 'g': // Move left
        posVy -= speedVy;
        break;
    }
    glutPostRedisplay();
}


void SpecialInput(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_RIGHT: // Move right
        posVx += speedVx;
        break;

    case GLUT_KEY_LEFT: // Move left
        posVx -= speedVx;
        break;

    case GLUT_KEY_UP: // Move up
        posVy += speedVy;
        break;

    case GLUT_KEY_DOWN: // Move down
        posVy -= speedVy;
        break;
    }
    glutPostRedisplay();
}
/*
void handleMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		glutDisplayFunc(display_up);
	}
	if (button == GLUT_RIGHT_BUTTON)
	{

		glutDisplayFunc(display_down);
	}
	glutPostRedisplay();
}
*/

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(1400, 730);
    glutInitWindowPosition(30, 20);
    glutCreateWindow("UnderWater View");
    glutDisplayFunc(display);
    glutTimerFunc(100, updateV, 0); // Start the update function to animate both objects
    glutTimerFunc(110, updateFish1, 0); // Start the update function to animate both objects
    glutTimerFunc(110, updateFish2, 0); // Start the update function to animate both objects
    glutTimerFunc(100, updateStarFish, 0); // Start the update function to animate both objects
    sound();
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(SpecialInput);
    //glutMouseFunc(handleMouse);
    glutMainLoop();
    return 0;
}

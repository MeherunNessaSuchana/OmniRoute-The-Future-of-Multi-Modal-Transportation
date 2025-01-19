
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <cstdio>
#include <cmath>
#include <GL/gl.h>
#include <string>
#include <mmsystem.h>

#define PI 3.141592653589
void drawBird(float x, float y, float scale);
void drawHuman(float x, float y, float scale);
void drawCircle1(float cx, float cy, float radius, int segments = 50);
void drawFilledEllipse(float cx, float cy, float rx, float ry, int num_segments);
void drawEmptyEllipse(float cx, float cy, float rx, float ry, int num_segments);
void OmniRoute(float x, float y);
void updateMovement();
void updateCar(int value);
void updateBirdPosition();
void update(int value);
void display();
void keyboard(unsigned char key, int x, int y);
void keyboardHandler(int key, int x, int y);
void sun(double x, double y, double r);
//void moon(double x, double y, double r);
void cloud(double x, double y, double r);

bool isRedLightDeep = false;//traffic light
// Global variables for the scene
float birdPositionX = -0.99f, birdPositionY = 0.45f, birdScale = 1.0f;
GLfloat sunX = -0.09f, sunY = 0.36f, sunRadius = 0.2f;
GLfloat moonX = -0.09f, moonY =  0.34f, moonRadius = 0.01f;
GLfloat cloudPosition = 0.0f;



GLfloat moonStartX = -0.09f;
GLfloat moonStartY = 0.34f;
GLfloat moonEndX = -0.114f;
GLfloat moonEndY = 0.99f;
GLfloat moonSpeedX = (moonEndX - moonStartX) / 5000; // Adjust speed as needed
GLfloat moonSpeedY = (moonEndY - moonStartY) / 5000; // Adjust speed as needed
GLfloat moonMinRadius = 0.01f; // Minimum size of the moon
GLfloat moonMaxRadius = 0.1f;   // Maximum size of the moon

// CAR movement and size variables
float carStartX = -0.079f;  // Starting X position
float carStartY = -0.54f;   // Starting Y position
float carEndX = -0.085f;    // Ending X position
float carEndY = 0.28f;      // Ending Y position
float carPositionX = carStartX;  // Current X position
float carPositionY = carStartY;  // Current Y position
float carScaleStart = 1.0f;  // Initial scale of the car
float carScaleEnd = 0.2f;    // Final scale of the car
float carScale = carScaleStart; // Current scale of the car
float carSpeed = 0.002f; // Speed of the car
bool carMoving = false;  // Flag to track if the car is moving

// Human movement and size variables
bool isPaused = false;
float posX = -0.98f, posY = -0.71f;  // Starting position on left path
float scale = 1.0f;                  // Initial scale
float step = 0.001f;                 // Movement step size (constant speed)
float startX1 = -0.98f, startY1 = -0.71f, endX1 = -0.145f, endY1 = 0.296f;  // Left path
float startX2 = 0.97f, startY2 = -0.72f, endX2 = -0.031f, endY2 = 0.30f;    // Right path
bool onLeftPath = true;              // Track which path the person is on
bool movingForward = true;           // Direction flag for moving forward or backward

// Bird movement and size variables
float birdStartX = -0.99f;  // Starting X position of the bird
float birdStartY = 0.45f;   // Starting Y position of the bird
float birdEndX = -0.15f;    // Ending X position of the bird
float birdEndY = 0.97f;     // Ending Y position of the bird
float birdScaleStart = 1.0f; // Starting size of the bird
float birdScaleEnd = 0.3f;  // Ending size of the bird
//float birdPositionX = birdStartX;
//float birdPositionY = birdStartY;
//float birdScale = birdScaleStart;

// Bird movement speed variables
float birdSpeedX = 0.001f;  // Speed of the bird's horizontal movement
float birdSpeedY = 0.001f;  // Speed of the bird's vertical movement

// Sun movement variables
GLfloat sunVerticalSpeed = 0.0002f;   // Speed of vertical movement
GLfloat sunShrinkRate = 0.00005f;     // Rate of size reduction
// Moon movement variables
GLfloat moonVerticalSpeed = 0.0001f; // Speed of moon's vertical movement
GLfloat moonShrinkRate = 0.00005f;  // Rate at which the moon shrinks (if needed)
// Cloud movement variables
GLfloat cloudSpeed = 0.0005f;   // Speed of the cloud movement

// Global variable to track the display mode (day, night, or rain)
enum DisplayMode { DAY, NIGHT, RAIN };
DisplayMode currentDisplayMode = DAY;
// Function to draw a circle
void drawCircle(float cx, float cy, float radius) {
    int numSegments = 100; // Number of segments to approximate the circle
    GLfloat angle;

    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numSegments; i++) {
        angle = 2.0f * PI * i / numSegments;
        glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
    }
    glEnd();
}

// Function to draw a half-moon
void drawHalfMoon(float x, float y, float radius) {
    // Draw the full moon
    glColor3f(1.0f, 1.0f, 0.8f); // Light yellow color
    drawCircle(x, y, radius);

    // Draw the shadow to create the crescent shape
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for shadow
    drawCircle(x + radius / 3.0f, y, radius * 0.9f); // Offset shadow circle
}

void moon1() {
    glPushMatrix();
    glTranslatef(moonX, moonY, 0.0f); // Translate the moon's position
    drawHalfMoon(0.0f, 0.0f, moonRadius); // Draw the moon at the translated position
    glPopMatrix();
}


void renderBitmapString(float x, float y, float z, void *font, const char *string)
{
    const char *c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void Traffic() {
    // Traffic light body (rectangle)
    glBegin(GL_POLYGON); // Traffic signal
    glColor3f(0.921f, 0.086f, 0.027f);
    glVertex2f(0.18f, 0.06f);
    glVertex2f(0.18f, 0.24f);
    glVertex2f(0.186f, 0.24f);
    glVertex2f(0.186f, 0.06f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.1f, 0.1f, 0.1f); // Dark gray for the body
    glVertex2f(0.18 - 0.03f, 0.3 - 0.1f); // Bottom-left corner
    glVertex2f(0.18 + 0.03f, 0.3 - 0.1f); // Bottom-right corner
    glVertex2f(0.18 + 0.03f, 0.3 + 0.1f); // Top-right corner
    glVertex2f(0.18 - 0.03f, 0.3 + 0.1f); // Top-left corner
    glEnd();

    // Red light
    if (isRedLightDeep) {
        glColor3f(1.0f, 0.0f, 0.0f); // Deep red color
    } else {
        glColor3f(0.70f, 0.0f, 0.0f); // Normal red color
    }
    drawCircle(0.18, 0.3 + 0.06f, 0.02f);

    // Yellow light
    glColor3f(0.70f, 0.70f, 0.0f); // Yellow color
    drawCircle(0.18, 0.3, 0.02f);

    // Green light
    glColor3f(0.0f, 0.70f, 0.0f); // Green color
    drawCircle(0.18, 0.3 - 0.06f, 0.02f); // Traffic light End
}


void lamppost()
{

    glBegin(GL_POLYGON);// lamppost 3
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(0.25f, 0.02f);
    glVertex2f(0.256f, 0.02f);
    glVertex2f(0.256f, 0.27f);
    glVertex2f(0.25f, 0.27f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.149f, 0.149f, 0.145f); // Same gray color
    glVertex2f(0.247f, 0.27f);
    glVertex2f(0.259f, 0.27f);
    glVertex2f(0.259f, 0.29f);
    glVertex2f(0.247f, 0.29f);
    glEnd();

    // Circular light at the top
    glColor3f(1.0f, 1.0f, 0.8f); // Yellowish light
    drawCircle(0.253f, 0.31f, 0.02f);

    glBegin(GL_POLYGON);// lamppost 2
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(0.45f, -0.2f);
    glVertex2f(0.456f, -0.2f);
    glVertex2f(0.456f, 0.1f);
    glVertex2f(0.45f, 0.1f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.149f, 0.149f, 0.145f); // Same gray color
    glVertex2f(0.447f, 0.1f);
    glVertex2f(0.459f, 0.1f);
    glVertex2f(0.459f, 0.12f);
    glVertex2f(0.447f, 0.12f);
    glEnd();

    // Circular light at the top
    glColor3f(1.0f, 1.0f, 0.8f); // Yellowish light
    drawCircle(0.453f, 0.13f, 0.02f);

    //

    glBegin(GL_POLYGON);// lamppost 1
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(0.68f, -0.54f);
    glVertex2f(0.686f, -0.54f);
    glVertex2f(0.686f, -0.2f);
    glVertex2f(0.68f, -0.2);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.149f, 0.149f, 0.145f); // Same gray color
    glVertex2f(0.677f, -0.2f);
    glVertex2f(0.689f, -0.2f);
    glVertex2f(0.689f, -0.18f);
    glVertex2f(0.677f, -0.18f);
    glEnd();

    // Circular light at the top
    glColor3f(1.0f, 1.0f, 0.8f); // Yellowish light
    drawCircle(0.683f, -0.175f, 0.02f);


    // from this

    glBegin(GL_POLYGON);
    glColor3f(0.1f, 0.1f, 0.1f); // Dark gray
    glVertex2f(0.05f, 0.18f);
    glVertex2f(0.075f, 0.18f);
    glVertex2f(0.075f, 0.2f);
    glVertex2f(0.05f, 0.2f);
    glEnd();

    // Lamppost pole
    glBegin(GL_POLYGON);
    glColor3f(0.149f, 0.149f, 0.145f); // Slightly lighter gray
    glVertex2f(0.06f, 0.2f);
    glVertex2f(0.066f, 0.2f);
    glVertex2f(0.066f, 0.35f);
    glVertex2f(0.06f, 0.35f);
    glEnd();

    // Top extension of the lamppost
    glBegin(GL_POLYGON);
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(0.057f, 0.35f);
    glVertex2f(0.07f, 0.35f);
    glVertex2f(0.07f, 0.36f);
    glVertex2f(0.057f, 0.36f);
    glEnd();

    // Circular light at the top
    glColor3f(1.0f, 1.0f, 0.8f); // Yellowish light
    drawCircle(0.0635f, 0.365f, 0.015f);



    //For left side lamppost
    glBegin(GL_POLYGON);// lamppost 1
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(-0.86f, -0.7f);
    glVertex2f(-0.86f, -0.4f);
    glVertex2f(-0.854f, -0.4f);
    glVertex2f(-0.854f, -0.7);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.8f); // Yellowish light
    drawCircle(-0.858f, -0.38f, 0.02f);



    glBegin(GL_POLYGON);// lamppost 2
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(-0.68f, -0.44f);
    glVertex2f(-0.68f, -0.16f);
    glVertex2f(-0.674f, -0.16f);
    glVertex2f(-0.674f, -0.44f);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.8f); // Yellowish light
    drawCircle(-0.678f, -0.14f, 0.02f);


    glBegin(GL_POLYGON);// lamppost 3
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(-0.5f, -0.2f);
    glVertex2f(-0.5f, 0.0f);
    glVertex2f(-0.494f, 0.0f);
    glVertex2f(-0.494f, -0.2);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.8f); // Yellowish light
    drawCircle(-0.498f, 0.02f, 0.02f);


    glBegin(GL_POLYGON);// lamppost 4
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(-0.32f, 0.06f);
    glVertex2f(-0.32f, 0.25f);
    glVertex2f(-0.314f, 0.25f);
    glVertex2f(-0.314f, 0.06f);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.8f); // Yellowish light
    drawCircle(-0.318f, 0.27f, 0.02f);


    glBegin(GL_POLYGON);// lamppost 5
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(-0.19f, 0.22f);
    glVertex2f(-0.19f, 0.32f);
    glVertex2f(-0.185f, 0.32f);
    glVertex2f(-0.185f, 0.22f);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.8f); // Yellowish light
    drawCircle(-0.189f, 0.34f, 0.02f);

}

void lamppost1()
{

    glBegin(GL_POLYGON);// lamppost 3
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(0.25f, 0.02f);
    glVertex2f(0.256f, 0.02f);
    glVertex2f(0.256f, 0.27f);
    glVertex2f(0.25f, 0.27f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.149f, 0.149f, 0.145f); // Same gray color
    glVertex2f(0.247f, 0.27f);
    glVertex2f(0.259f, 0.27f);
    glVertex2f(0.259f, 0.29f);
    glVertex2f(0.247f, 0.29f);
    glEnd();

    // Circular light at the top
    glColor3f(1.0f, 0.843f, 0.0f); // Yellowish light
    drawCircle(0.253f, 0.31f, 0.02f);

    glBegin(GL_POLYGON);// lamppost 2
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(0.45f, -0.2f);
    glVertex2f(0.456f, -0.2f);
    glVertex2f(0.456f, 0.1f);
    glVertex2f(0.45f, 0.1f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.149f, 0.149f, 0.145f); // Same gray color
    glVertex2f(0.447f, 0.1f);
    glVertex2f(0.459f, 0.1f);
    glVertex2f(0.459f, 0.12f);
    glVertex2f(0.447f, 0.12f);
    glEnd();

    // Circular light at the top
    glColor3f(1.0f, 0.843f, 0.0f); // Yellowish light
    drawCircle(0.453f, 0.13f, 0.02f);

    //

    glBegin(GL_POLYGON);// lamppost 1
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(0.68f, -0.54f);
    glVertex2f(0.686f, -0.54f);
    glVertex2f(0.686f, -0.2f);
    glVertex2f(0.68f, -0.2);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.149f, 0.149f, 0.145f); // Same gray color
    glVertex2f(0.677f, -0.2f);
    glVertex2f(0.689f, -0.2f);
    glVertex2f(0.689f, -0.18f);
    glVertex2f(0.677f, -0.18f);
    glEnd();

    // Circular light at the top
    glColor3f(1.0f, 0.843f, 0.0f); // Yellowish light
    drawCircle(0.683f, -0.175f, 0.02f);


    // from this

    glBegin(GL_POLYGON);
    glColor3f(0.1f, 0.1f, 0.1f); // Dark gray
    glVertex2f(0.05f, 0.18f);
    glVertex2f(0.075f, 0.18f);
    glVertex2f(0.075f, 0.2f);
    glVertex2f(0.05f, 0.2f);
    glEnd();

    // Lamppost pole
    glBegin(GL_POLYGON);
    glColor3f(0.149f, 0.149f, 0.145f); // Slightly lighter gray
    glVertex2f(0.06f, 0.2f);
    glVertex2f(0.066f, 0.2f);
    glVertex2f(0.066f, 0.35f);
    glVertex2f(0.06f, 0.35f);
    glEnd();

    // Top extension of the lamppost
    glBegin(GL_POLYGON);
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(0.057f, 0.35f);
    glVertex2f(0.07f, 0.35f);
    glVertex2f(0.07f, 0.36f);
    glVertex2f(0.057f, 0.36f);
    glEnd();

    // Circular light at the top
    glColor3f(1.0f, 0.843f, 0.0f); // Yellowish light
    drawCircle(0.0635f, 0.365f, 0.015f);



    //For left side lamppost
    glBegin(GL_POLYGON);// lamppost 1
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(-0.86f, -0.7f);
    glVertex2f(-0.86f, -0.4f);
    glVertex2f(-0.854f, -0.4f);
    glVertex2f(-0.854f, -0.7);
    glEnd();

    glColor3f(1.0f, 0.843f, 0.0f); // Yellowish light
    drawCircle(-0.858f, -0.38f, 0.02f);



    glBegin(GL_POLYGON);// lamppost 2
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(-0.68f, -0.44f);
    glVertex2f(-0.68f, -0.16f);
    glVertex2f(-0.674f, -0.16f);
    glVertex2f(-0.674f, -0.44f);
    glEnd();

    glColor3f(1.0f, 0.843f, 0.0f); // Yellowish light
    drawCircle(-0.678f, -0.14f, 0.02f);


    glBegin(GL_POLYGON);// lamppost 3
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(-0.5f, -0.2f);
    glVertex2f(-0.5f, 0.0f);
    glVertex2f(-0.494f, 0.0f);
    glVertex2f(-0.494f, -0.2);
    glEnd();

    glColor3f(1.0f, 0.843f, 0.0f); // Yellowish light
    drawCircle(-0.498f, 0.02f, 0.02f);


    glBegin(GL_POLYGON);// lamppost 4
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(-0.32f, 0.06f);
    glVertex2f(-0.32f, 0.25f);
    glVertex2f(-0.314f, 0.25f);
    glVertex2f(-0.314f, 0.06f);
    glEnd();

    glColor3f(1.0f, 0.843f, 0.0f); // Yellowish light
    drawCircle(-0.318f, 0.27f, 0.02f);


    glBegin(GL_POLYGON);// lamppost 5
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(-0.19f, 0.22f);
    glVertex2f(-0.19f, 0.32f);
    glVertex2f(-0.185f, 0.32f);
    glVertex2f(-0.185f, 0.22f);
    glEnd();

    glColor3f(1.0f, 0.843f, 0.0f); // Yellowish light
    drawCircle(-0.189f, 0.34f, 0.02f);

}


void city ()
{
     glBegin(GL_POLYGON); // for RoadLine left
    glColor3f(0.9882f, 0.823f, 0.643f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(-1.0f, -0.56f);
    glVertex2f(-0.14f, 0.32f);
    glVertex2f(-0.12f, 0.32f);
    glVertex2f(-0.9f, -1.0f);
    glEnd();

    glBegin(GL_POLYGON); // for RoadLine Right
    glColor3f(0.9882f, 0.823f, 0.643f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.52f);
    glVertex2f(-0.04f, 0.32f);
    glVertex2f(-0.06f, 0.32f);
    glVertex2f(0.9f, -1.0f);
    glEnd();


    glBegin(GL_POLYGON); // for Road
    glColor3f(0.345f, 0.337f, 0.349f);
    glVertex2f(-0.9f, -1.0f);
    glVertex2f(-0.12f, 0.32f);
    glVertex2f(-0.056f, 0.32f);
    glVertex2f(0.243f, -0.017f);
    glVertex2f(0.32f, -0.04f);
    glVertex2f(0.32f, -0.1f);
    glVertex2f(0.93f, -1.0f);
    glEnd();

    glBegin(GL_POLYGON);// for road line 1 right
    glColor3f(0.96f, 0.976f, 1.0f);
    glVertex2f(0.33f, -0.92f);
    glVertex2f(0.13f, -0.284f);
    glVertex2f(0.14f, -0.28f);
    glVertex2f(0.35f, -0.92f);
    glEnd();

    glBegin(GL_POLYGON);// for road line 2 right
    glColor3f(0.96f, 0.976f, 1.0f);
    glVertex2f(0.125f, -0.26f);
    glVertex2f(0.1f, -0.18f);
    glVertex2f(0.11f, -0.18f);
    glVertex2f(0.135f, -0.26f);
    glEnd();

    glBegin(GL_POLYGON);// for road line 3 right
    glColor3f(0.96f, 0.976f, 1.0f);
    glVertex2f(0.09f, -0.157f);
    glVertex2f(0.04f, -0.02f);
    glVertex2f(0.052f, -0.02f);
    glVertex2f(0.1f, -0.15f);
    glEnd();

    glBegin(GL_POLYGON);// for road line 4 right
    glColor3f(0.96f, 0.976f, 1.0f);
    glVertex2f(0.03f, 0.01f);
    glVertex2f(-0.037, 0.21f);
    glVertex2f(-0.02f, 0.18f);
    glVertex2f(0.044, 0.01f);
    glEnd();
// for road line 1 left
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(9.0f);
    // Draw a line with the specified floating-point coordinates
    glBegin(GL_LINES);
    glVertex2f(-0.289f, -0.929f); // First point
    glVertex2f(-0.20f, -0.3f); // Second point
    glEnd();

    glBegin(GL_POLYGON);// for road line 2 left
    glColor3f(0.96f, 0.976f, 1.0f);
    glVertex2f(-0.2f, -0.26f);
    glVertex2f(-0.172f, -0.057f);
    glVertex2f(-0.161f, -0.06f);
    glVertex2f(-0.189f, -0.26f);
    glEnd();

     glBegin(GL_POLYGON);// for road line 3 left
    glColor3f(0.96f, 0.976f, 1.0f);
    glVertex2f(-0.174f, -0.021f);
    glVertex2f(-0.16f, 0.07f);
    glVertex2f(-0.152f, 0.07f);
    glVertex2f(-0.16f, -0.02f);
    glEnd();

    glBegin(GL_POLYGON);// for road line 4 left
    glColor3f(0.96f, 0.976f, 1.0f);
    glVertex2f(-0.16f, 0.09f);
    glVertex2f(-0.14f, 0.22f);
    glVertex2f(-0.135f, 0.22f);
    glVertex2f(-0.15f, 0.09f);
    glEnd();

    glBegin(GL_POLYGON); // for Stright Road Line 1 left
    glColor3f(0.96f, 0.976f, 1.0f);
    glVertex2f(-0.795, -0.98f);
    glVertex2f(-0.21f, 0.16f);
    glVertex2f(-0.204f, 0.16f);
    glVertex2f(-0.768f, -0.98f);
    glEnd();

    glBegin(GL_POLYGON); // for Stright Road Line 1 right
    glColor3f(0.96f, 0.976f, 1.0f);
    glVertex2f(0.82, -0.98f);
    glVertex2f(0.059f, 0.18f);
    glVertex2f(0.052f, 0.18f);
    glVertex2f(0.8f, -0.98f);
    glEnd();





  glFlush();

    glBegin(GL_POLYGON); // for 1st building Right
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(1.0f, -0.52f);
    glVertex2f(0.33f, 0.02f);
    glVertex2f(0.33f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glEnd();


    glBegin(GL_POLYGON); // for 1st and 2nd middle building Right
    glColor3f(0.019f, 0.470f, 0.651f);
    glVertex2f(0.33f, 0.018f);
    glVertex2f(0.2f, 0.133f);
    glVertex2f(0.2f, 1.0f);
    glVertex2f(0.33f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON); // for 1st and 2nd middle building Right side portion
    glColor3f(0.004f, 0.114f, 0.878f);
    glVertex2f(0.28f, 0.06f);
    glVertex2f(0.2f, 0.13f);
    glVertex2f(0.2f, 1.0f);
    glVertex2f(0.28f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 1 floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.33f, 0.16f);
    glVertex2f(0.28f, 0.2f);
    glVertex2f(0.28f, 0.22f);
    glVertex2f(0.33f, 0.18f);
    glEnd();


    glBegin(GL_POLYGON); // for miss building 2nd floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.33f, 0.28f);
    glVertex2f(0.28f, 0.32f);
    glVertex2f(0.28f, 0.34f);
    glVertex2f(0.33f, 0.3f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 3rd floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.33f, 0.4f);
    glVertex2f(0.28f, 0.44f);
    glVertex2f(0.28f, 0.46f);
    glVertex2f(0.33f, 0.42f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 4th floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.33f, 0.52f);
    glVertex2f(0.28f, 0.56f);
    glVertex2f(0.28f, 0.58f);
    glVertex2f(0.33f, 0.54f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 5th floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.33f, 0.64f);
    glVertex2f(0.28f, 0.68f);
    glVertex2f(0.28f, 0.7f);
    glVertex2f(0.33f, 0.66f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 6th floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.33f, 0.76f);
    glVertex2f(0.28f, 0.8f);
    glVertex2f(0.28f, 0.82f);
    glVertex2f(0.33f, 0.78f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 7th floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.33f, 0.88f);
    glVertex2f(0.28f, 0.92f);
    glVertex2f(0.28f, 0.94f);
    glVertex2f(0.33f, 0.9f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building side portion 1st floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.28f, 0.2f);
    glVertex2f(0.2f, 0.2f);
    glVertex2f(0.2f, 0.22f);
    glVertex2f(0.28f, 0.22f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 1st floor 1st window
    glColor3f(0.96f, 0.976f, 1.0f);//white
    glVertex2f(0.275f, 0.23f);
    glVertex2f(0.241f, 0.229f);
    glVertex2f(0.241f, 0.29f);
    glVertex2f(0.275f, 0.3f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 1st floor 2nd window
    glColor3f(0.96f, 0.976f, 1.0f);//white
    glVertex2f(0.233f, 0.23f);
    glVertex2f(0.205f, 0.229f);
    glVertex2f(0.205f, 0.28f);
    glVertex2f(0.233f, 0.29f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building side portion 2nd floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.28f, 0.32f);
    glVertex2f(0.2f, 0.3f);
    glVertex2f(0.2f, 0.32f);
    glVertex2f(0.28f, 0.34f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 2nd floor 1st window
    glColor3f(0.96f, 0.976f, 1.0f);//white
    glVertex2f(0.275f, 0.35f);
    glVertex2f(0.241f, 0.343f);
    glVertex2f(0.241f, 0.419f);
    glVertex2f(0.275f, 0.429f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 2nd floor 2nd window
    glColor3f(0.96f, 0.976f, 1.0f);//white
    glVertex2f(0.233f, 0.34f);
    glVertex2f(0.205f, 0.339f);
    glVertex2f(0.205f, 0.409f);
    glVertex2f(0.233f, 0.41f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building side portion 3rd floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.28f, 0.44f);
    glVertex2f(0.2f, 0.42f);
    glVertex2f(0.2f, 0.44f);
    glVertex2f(0.28f, 0.46f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building side portion 4th floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.28f, 0.56f);
    glVertex2f(0.2f, 0.54f);
    glVertex2f(0.2f, 0.56f);
    glVertex2f(0.28f, 0.58f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 4th floor 1st window
    glColor3f(0.96f, 0.976f, 1.0f);//white
    glVertex2f(0.275f, 0.59f);
    glVertex2f(0.241f, 0.59f);
    glVertex2f(0.241f, 0.66f);
    glVertex2f(0.275f, 0.66f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 4th floor 2nd window
    glColor3f(0.96f, 0.976f, 1.0f);//white
    glVertex2f(0.233f, 0.585f);
    glVertex2f(0.205f, 0.575f);
    glVertex2f(0.205f, 0.65f);
    glVertex2f(0.233f, 0.655f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building side portion 5th floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.28f, 0.68f);
    glVertex2f(0.2f, 0.66f);
    glVertex2f(0.2f, 0.68f);
    glVertex2f(0.28f, 0.7f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 5th floor 1st window
    glColor3f(0.96f, 0.976f, 1.0f);//white
    glVertex2f(0.275f, 0.71f);
    glVertex2f(0.241f, 0.71f);
    glVertex2f(0.241f, 0.78f);
    glVertex2f(0.275f, 0.78f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 5th floor 2nd window
    glColor3f(0.96f, 0.976f, 1.0f);//white
    glVertex2f(0.233f, 0.705f);
    glVertex2f(0.205f, 0.70f);
    glVertex2f(0.205f, 0.767f);
    glVertex2f(0.233f, 0.77f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building side portion 6th floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.28f, 0.8f);
    glVertex2f(0.2f, 0.78f);
    glVertex2f(0.2f, 0.8f);
    glVertex2f(0.28f, 0.82f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 6th floor 1st window
    glColor3f(0.96f, 0.976f, 1.0f);//white
    glVertex2f(0.275f, 0.83f);
    glVertex2f(0.241f, 0.83f);
    glVertex2f(0.241f, 0.9f);
    glVertex2f(0.275f, 0.91f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 6th floor 2nd window
    glColor3f(0.96f, 0.976f, 1.0f);//white
    glVertex2f(0.233f, 0.83f);
    glVertex2f(0.205f, 0.83f);
    glVertex2f(0.205f, 0.89f);
    glVertex2f(0.233f, 0.89f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building side portion 7th floor line
    glColor3f(0.878f, 0.0473f, 0.753f);
    glVertex2f(0.28f, 0.92f);
    glVertex2f(0.2f, 0.9f);
    glVertex2f(0.2f, 0.92f);
    glVertex2f(0.28f, 0.94f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 7th floor 1st window
    glColor3f(0.96f, 0.976f, 1.0f);//white
    glVertex2f(0.275f, 0.95f);
    glVertex2f(0.241f, 0.95f);
    glVertex2f(0.241f, 1.0f);
    glVertex2f(0.275f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON); // for miss building 7th floor 2nd window
    glColor3f(0.96f, 0.976f, 1.0f);//white
    glVertex2f(0.233f, 0.95f);
    glVertex2f(0.205f, 0.95f);
    glVertex2f(0.205f, 1.0f);
    glVertex2f(0.233f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON);// 1st floor 1st window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(1.0f, -0.46f);
    glVertex2f(0.756f, -0.28f);
    glVertex2f(0.785f, -0.107f);
    glVertex2f(1.0f, -0.22f);
    glEnd();

    glBegin(GL_POLYGON);// 1st floor 2nd window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.72f, -0.25f);
    glVertex2f(0.538f, -0.117);
    glVertex2f(0.56f, 0.016f);
    glVertex2f(0.75f, -0.09f);
    glEnd();

    glBegin(GL_POLYGON);// 1st floor 3rd window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.485f, -0.07f);
    glVertex2f(0.384f, 0.0f);
    glVertex2f(0.4f, 0.103f);
    glVertex2f(0.507f, 0.05);
    glEnd();

    glBegin(GL_POLYGON);// 1st building 1st white line
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(1.0f, 0.156f);
    glVertex2f(0.33f, 0.265f);
    glVertex2f(0.33f, 0.28f);
    glVertex2f(1.0f, 0.18);
    glEnd();

    glBegin(GL_POLYGON);// 1st building 2nd white line
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(1.0f, 0.58f);
    glVertex2f(0.33f, 0.41f);
    glVertex2f(0.33f, 0.43f);
    glVertex2f(1.0f, 0.63f);
    glEnd();

    glBegin(GL_POLYGON);// 1st building 3rd white line
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(0.93f, 1.0f);
    glVertex2f(0.33f, 0.58f);
    glVertex2f(0.33f, 0.61f);
    glVertex2f(0.88f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON);// 1st building 4th floor 1st window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.85f, 1.0f);
    glVertex2f(0.79f, 0.96f);
    glVertex2f(0.79f, 1.0f);
    glVertex2f(0.85f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON);// 1st building 4th floor 2nd window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.77f, 0.95f);
    glVertex2f(0.68f, 0.9f);
    glVertex2f(0.68f, 1.0f);
    glVertex2f(0.77f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON);// 1st building 4th floor 3rd window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.66f, 0.88f);
    glVertex2f(0.57f, 0.81f);
    glVertex2f(0.57f, 1.0f);
    glVertex2f(0.66f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON);// 1st building 4th floor 4th window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.55f, 0.8f);
    glVertex2f(0.48f, 0.74f);
    glVertex2f(0.48f, 0.9f);
    glVertex2f(0.55f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON);// 1st building 4th floor 5th window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.46f, 0.73f);
    glVertex2f(0.41f, 0.69f);
    glVertex2f(0.41f, 0.81f);
    glVertex2f(0.46f, 0.88f);
    glEnd();

     glBegin(GL_POLYGON);// 1st building 4th floor 6th window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.39f, 0.69f);
    glVertex2f(0.35f, 0.64f);
    glVertex2f(0.35f, 0.74f);
    glVertex2f(0.39f, 0.8f);
    glEnd();

    glBegin(GL_POLYGON);// 1st building 4th white line
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(0.53f, 1.0f);
    glVertex2f(0.33f, 0.74f);
    glVertex2f(0.33f, 0.77f);
    glVertex2f(0.5f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON);// 1st building 5th floor 1st window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.48f, 1.0f);
    glVertex2f(0.41f, 0.91f);
    glVertex2f(0.41f, 1.0f);
    glVertex2f(0.46f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON);// 1st building 5th floor 2nd window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.39f, 0.89f);
    glVertex2f(0.35f, 0.83f);
    glVertex2f(0.35f, 1.0f);
    glVertex2f(0.39f, 1.0f);
    glEnd();

     glBegin(GL_POLYGON);// 1st building side white line
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(0.34f, 0.01f);
    glVertex2f(0.33f, 0.028f);
    glVertex2f(0.33f, 1.0f);
    glVertex2f(0.34f, 1.0f);
    glEnd();

    //message space for right side: / for message space

    glColor3f(0.96f, 0.976f, 1.0f);
  drawCircle(0.75f, 0.09f, 0.09f);
  glColor3f(0.117f, 0.7412f, 0.969f);
  drawCircle(0.75f, 0.09f, 0.07f);

    glBegin(GL_POLYGON);
    glColor3f(0.96f, 0.976f, 1.0f);
    glVertex2f(0.606f, 0.15f);
    glVertex2f(0.894f, 0.09f);
    glVertex2f(0.894f, -0.1f);
    glVertex2f(0.606f, -0.00f);
    glEnd();


    glBegin(GL_POLYGON);
    glColor3f(0.117f, 0.7412f, 0.969f);
    glVertex2f(0.62f, 0.13f);
    glVertex2f(0.88f, 0.07f);
    glVertex2f(0.88f, -0.08f);
    glVertex2f(0.62f, 0.019f);
    glEnd();




    glBegin(GL_POLYGON);// 3rd floor 1st window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(1.0f, 0.66f);
    glVertex2f(0.9f, 0.63f);
    glVertex2f(0.9f, 0.94f);
    glVertex2f(1.0f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON);// 3rd floor 2nd window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.88f, 0.62f);
    glVertex2f(0.78f, 0.59f);
    glVertex2f(0.78f, 0.87f);
    glVertex2f(0.88f, 0.93f);
    glEnd();

    glBegin(GL_POLYGON);// 3rd floor 3rd window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.76f, 0.59f);
    glVertex2f(0.68f, 0.57f);
    glVertex2f(0.68f, 0.8f);
    glVertex2f(0.76f, 0.85f);
    glEnd();

    glBegin(GL_POLYGON);// 3rd floor 4th window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.66f, 0.56f);
    glVertex2f(0.58f, 0.53f);
    glVertex2f(0.58f, 0.74f);
    glVertex2f(0.66f, 0.79f);
    glEnd();

    glBegin(GL_POLYGON);// 3rd floor 5th window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.56f, 0.53f);
    glVertex2f(0.49f, 0.5f);
    glVertex2f(0.49f, 0.67f);
    glVertex2f(0.56f, 0.72f);
    glEnd();

    glBegin(GL_POLYGON);// 3rd floor 6th window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.47f, 0.5f);
    glVertex2f(0.42f, 0.48f);
    glVertex2f(0.42f, 0.62f);
    glVertex2f(0.47f, 0.65f);
    glEnd();

    glBegin(GL_POLYGON);// 3rd floor 7th window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.4f, 0.48f);
    glVertex2f(0.35f, 0.46f);
    glVertex2f(0.35f, 0.58f);
    glVertex2f(0.4f, 0.61f);
    glEnd();

    glBegin(GL_POLYGON);// 2nd floor 1st window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(1.0f, 0.237f);
    glVertex2f(0.88f, 0.24f);
    glVertex2f(0.9f, 0.49f);
    glVertex2f(1.0f, 0.505f);
    glEnd();

    glBegin(GL_POLYGON);// 2nd floor 2nd window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.858f, 0.242f );
    glVertex2f(0.76f, 0.245f);
    glVertex2f(0.78f, 0.48f);
    glVertex2f(0.883f, 0.49);
    glEnd();

    glBegin(GL_POLYGON);// 2nd floor 3rd window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.74f, 0.256f);
    glVertex2f(0.65f, 0.26f);
    glVertex2f(0.67f, 0.463f);
    glVertex2f(0.757f, 0.47f);
    glEnd();

    glBegin(GL_POLYGON);// 2nd floor 4th window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.63f, 0.27f);
    glVertex2f(0.545f,0.265f);
    glVertex2f(0.565f, 0.44f);
    glVertex2f(0.65f, 0.46);
    glEnd();

    glBegin(GL_POLYGON);// 2nd floor 5th window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.525f, 0.27f);
    glVertex2f(0.45f,0.28f);
    glVertex2f(0.465f, 0.42f);
    glVertex2f(0.545f, 0.44f);
    glEnd();

    glBegin(GL_POLYGON);// 2nd floor 6th window
    glColor3f(0.980f, 0.494f, 0.145f);
    glVertex2f(0.43f, 0.28f);
    glVertex2f(0.36f,0.29f);
    glVertex2f(0.37f, 0.4f);
    glVertex2f(0.45f, 0.42f);
    glEnd();

    glBegin(GL_POLYGON); // for 2nd building Right
    glColor3f(0.067f, 0.349f, 0.012f);
    glVertex2f(0.2f, 0.133f);
    glVertex2f(0.12f, 0.1977f);
    glVertex2f(0.12f, 1.0f);
    glVertex2f(0.2f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON); // for 3rd building Right
    glColor3f(0.34f, 0.4117f, 0.439f);
    glVertex2f(0.12f, 0.2f);
    glVertex2f(0.12f, 1.0f);
    glVertex2f(0.04f, 0.66f);
    glVertex2f(0.04f, 0.26f);
    glEnd();

    glBegin(GL_POLYGON);// White line 1st for 2nd and 3rd building 1st
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(0.2f, 0.34f);
    glVertex2f(0.12f, 0.38f);
    glVertex2f(0.12f, 0.40f);
    glVertex2f(0.2f, 0.36f);
    glEnd();

    glBegin(GL_POLYGON);// White line 1st for 2nd and 3rd building 1st
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(0.12f, 0.38f);
    glVertex2f(0.04f, 0.37f);
    glVertex2f(0.04f, 0.38);
    glVertex2f(0.12f, 0.4f);
    glEnd();


    glBegin(GL_POLYGON);// White line 1st for 2nd and 3rd building 2nd
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(0.2f, 0.46f);
    glVertex2f(0.12f, 0.5f);
    glVertex2f(0.12f, 0.52f);
    glVertex2f(0.2f,0.48f);
    glEnd();

    glBegin(GL_POLYGON);// White line 1st for 2nd and 3rd building 2nd
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(0.12f, 0.5f);
    glVertex2f(0.04f, 0.47f);
    glVertex2f(0.04f, 0.48f);
    glVertex2f(0.12f, 0.52f);
    glEnd();

    glBegin(GL_POLYGON);// White line 1st for 2nd and 3rd building 3nd
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(0.2f,0.58f);
    glVertex2f(0.12f,0.62f);
    glVertex2f(0.12f,0.64f);
    glVertex2f(0.2f,0.6f);
    glEnd();

    glBegin(GL_POLYGON);// White line 1st for 2nd and 3rd building 3nd
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(0.12f, 0.62f);
    glVertex2f(0.04f, 0.57f);
    glVertex2f(0.04f, 0.58f);
    glVertex2f(0.12f, 0.64f);
    glEnd();

    glBegin(GL_POLYGON);// White line 1st for 2nd and 3rd building 4th
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(0.2f,0.7f);
    glVertex2f(0.12f,0.74f);
    glVertex2f(0.12f,0.76f);
    glVertex2f(0.2f,0.72f);
    glEnd();

    glBegin(GL_POLYGON);// White line 1st for 2nd and 3rd building 4th
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(0.12f, 0.74f);
    glVertex2f(0.04f, 0.65f);
    glVertex2f(0.04f, 0.66f);
    glVertex2f(0.12f, 0.76f);
    glEnd();

    glBegin(GL_POLYGON);// White line 1st for 2nd and 3rd building 5th
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(0.2f,0.83);
    glVertex2f(0.12f,0.87f);
    glVertex2f(0.12f,0.89f);
    glVertex2f(0.2f,0.85f);
    glEnd();

    glBegin(GL_POLYGON);// White line 1st for 2nd and 3rd building 5th
    glColor3f(0.988f, 0.984f, 0.980f);
    glVertex2f(0.12f, 0.87f);
    glVertex2f(0.072f, 0.80f);
    glVertex2f(0.072f, 0.81f);
    glVertex2f(0.12f, 0.89f);
    glEnd();


    glBegin(GL_POLYGON); // for 4th building Right
    glColor3f(0.502f, 0.043f, 0.0118f);
    glVertex2f(0.04f, 0.26f);
    glVertex2f(0.04f, 0.62f);
    glVertex2f(-0.01f, 0.56f);
    glVertex2f(-0.01f, 0.302f);
    glEnd();

    glBegin(GL_POLYGON); // for 4th  side portion building Right
    glColor3f(0.961f, 0.796f, 0.259f);
    glVertex2f(0.02f, 0.27f);
    glVertex2f(0.02f, 0.6f);
    glVertex2f(-0.01f, 0.56f);
    glVertex2f(-0.01f, 0.298f);
    glEnd();

    glBegin(GL_POLYGON); // for 4th  side portion building Right 1st black line
    glColor3f(0.09f, 0.09f, 0.086f);
    glVertex2f(0.04f, 0.35f);
    glVertex2f(0.04f, 0.36f);
    glVertex2f(0.02f, 0.37f);
    glVertex2f(0.02f, 0.36f);
    glEnd();

    glBegin(GL_POLYGON); // for 4th  side portion building Right 1st black line side
    glColor3f(0.09f, 0.09f, 0.086f);
    glVertex2f(0.02f, 0.36f);
    glVertex2f(0.02f, 0.37f);
    glVertex2f(-0.01f, 0.36f);
    glVertex2f(-0.01f, 0.35f);
    glEnd();


    glBegin(GL_POLYGON); // for 4th  side portion building Right 2nd black line
    glColor3f(0.09f, 0.09f, 0.086f);
    glVertex2f(0.04f, 0.45f);
    glVertex2f(0.04f, 0.46f);
    glVertex2f(0.02f, 0.47f);
    glVertex2f(0.02f, 0.46f);
    glEnd();

    glBegin(GL_POLYGON); // for 4th  side portion building Right 2nd black line side
    glColor3f(0.09f, 0.09f, 0.086f);
    glVertex2f(0.02f, 0.46f);
    glVertex2f(0.02f, 0.47f);
    glVertex2f(-0.01f, 0.45f);
    glVertex2f(-0.01f, 0.44f);
    glEnd();

    glBegin(GL_POLYGON); // for 4th building 1st window 2nd floor
    glColor3f(0.45f, 0.059f, 0.64f);
    glVertex2f(0.015f, 0.38f);
    glVertex2f(0.015f, 0.443f);
    glVertex2f(-0.005f, 0.439f);
    glVertex2f(-0.005f, 0.378f);
    glEnd();

     glBegin(GL_POLYGON); // for 4th building 1st window 3rd floor
    glColor3f(0.45f, 0.059f, 0.64f);
    glVertex2f(0.015f, 0.48f);
    glVertex2f(0.015f, 0.541f);
    glVertex2f(-0.005f, 0.53f);
    glVertex2f(-0.005f, 0.471f);
    glEnd();

    glBegin(GL_POLYGON); // for 4th  side portion building Right 3rd black line
    glColor3f(0.09f, 0.09f, 0.086f);
    glVertex2f(0.04f, 0.55f);
    glVertex2f(0.04f, 0.56f);
    glVertex2f(0.02f, 0.57f);
    glVertex2f(0.02f, 0.56f);
    glEnd();

    glBegin(GL_POLYGON); // for 4th  side portion building Right 3rd black line side
    glColor3f(0.09f, 0.09f, 0.086f);
    glVertex2f(0.02f, 0.56f);
    glVertex2f(0.02f, 0.57f);
    glVertex2f(-0.01f, 0.54f);
    glVertex2f(-0.01f, 0.53f);
    glEnd();

    glBegin(GL_POLYGON); // for 5th building Right
    glColor3f(0.149f, 0.149f, 0.145f);
    glVertex2f(-0.01f, 0.301f);
    glVertex2f(-0.01f, 0.49f);
    glVertex2f(-0.035f,0.413f);
    glVertex2f(-0.035f, 0.31f);
    glEnd();

    glBegin(GL_POLYGON); // for 5th building Right 1st white line
    glColor3f(0.98f, 0.988f, 0.98f);
    glVertex2f(-0.01f, 0.34f);
    glVertex2f(-0.01f, 0.35f);
    glVertex2f(-0.02f,0.36f);
    glVertex2f(-0.02f, 0.35f);
    glEnd();

     glBegin(GL_POLYGON); // for 5th building Right 2nd white line
    glColor3f(0.98f, 0.988f, 0.98f);
    glVertex2f(-0.01f, 0.41f);
    glVertex2f(-0.01f, 0.42f);
    glVertex2f(-0.02f,0.43f);
    glVertex2f(-0.02f, 0.42f);
    glEnd();

    glBegin(GL_POLYGON); // for 5th building Right side
    glColor3f(0.08f, 0.44f, 0.255f);
    glVertex2f(-0.02f, 0.302f);
    glVertex2f(-0.02f, 0.468f);
    glVertex2f(-0.035f,0.413f);
    glVertex2f(-0.035f, 0.31f);
    glEnd();

    glBegin(GL_POLYGON); // for 5th building Right side white line 1st
    glColor3f(0.98f, 0.988f, 0.98f);
    glVertex2f(-0.02f, 0.35f);
    glVertex2f(-0.02f, 0.36f);
    glVertex2f(-0.035f,0.35f);
    glVertex2f(-0.035f, 0.34f);
    glEnd();

    glBegin(GL_POLYGON); // for 5th building Right side white line 2nd
    glColor3f(0.98f, 0.988f, 0.98f);
    glVertex2f(-0.02f, 0.42f);
    glVertex2f(-0.02f, 0.43f);
    glVertex2f(-0.035f,0.41f);
    glVertex2f(-0.035f, 0.4f);
    glEnd();

    glBegin(GL_POLYGON); // for 5th building Right side window
    glColor3f(0.83f, 1.086f, 0.0f);
    glVertex2f(-0.023f, 0.367f);
    glVertex2f(-0.023f, 0.41f);
    glVertex2f(-0.031f,0.4f);
    glVertex2f(-0.031f, 0.369f);
    glEnd();

    // for  Left side  building
    glBegin(GL_POLYGON); //1st building
  glColor3f(0.5f, 0.5f, 0.5f); // Set color to gray
    glVertex2f(-1.0f, -0.56f);//b
    glVertex2f(-1.0f, 1.0f);//k
    glVertex2f(-0.8f, 1.0f);//t
    glVertex2f(-.8f, -0.358f);//o3
    glEnd();

glLineWidth(7.0f); // Set line thickness to 0.5

glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white
    glVertex2f(-0.80f, 1.0f);//t
    glVertex2f(-0.8f,-0.36f);//o3
glEnd();
//window1
glBegin(GL_POLYGON); // Drawing a rectangle
    glColor3f(0.678f, 0.847f, 0.902f);  //  Sky Blue
    glVertex2f(-1.0f, -0.52f);  // G5
    glVertex2f(-0.82f, -0.35f); // I5
    glVertex2f(-0.82f, -0.042f); // H5
    glVertex2f(-1.0f, -0.27f);  // F5
glEnd();
//window2
glBegin(GL_POLYGON); // Drawing a rectangle
   glColor3f(0.678f, 0.847f, 0.902f);  //  Sky Blue
    glVertex2f(-1.0f, -0.0175f);  // L4
    glVertex2f(-1.0f, -0.18f);    // I4
    glVertex2f(-0.82f, 0.05f);    // M4
    glVertex2f(-0.82f, 0.224f);   // H4
glEnd();
//window3
glBegin(GL_POLYGON); // Drawing a rectangle
    glColor3f(0.678f, 0.847f, 0.902f);  //  Sky Blue
    glVertex2f(-1.0f, 0.08f);    // V5
    glVertex2f(-0.82f, 0.3f);    // U5
    glVertex2f(-0.82f, 0.472f);  // T5
    glVertex2f(-1.0f, 0.25f);    // S5
glEnd();
//window4
glBegin(GL_POLYGON); // Drawing a rectangle
     glColor3f(0.678f, 0.847f, 0.902f);  //  Sky Blue
    glVertex2f(-1.0f, 0.32f);    // R5
    glVertex2f(-0.82f, 0.55f);   // Q5
    glVertex2f(-0.82f, 0.744f);  // P5
    glVertex2f(-1.0f, 0.55f);    // O5
glEnd();
//window5
glBegin(GL_POLYGON); // Drawing a rectangle
     glColor3f(0.678f, 0.847f, 0.902f);  //  Sky Blue
    glVertex2f(-1.0f, 0.64f);    // N5
    glVertex2f(-0.82f, 0.82f);   // M5
    glVertex2f(-0.82f, 1.0f);   // L5
    glVertex2f(-1.0f, 1.0f);     // K5
glEnd();


glLineWidth(7.0f); // Set line thickness to 0.5

glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white
    glVertex2f(-1.0f, -0.23f);//t3
    glVertex2f(-0.8f, 0.0314f);//s
glEnd();


glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.03f);//h3
    glVertex2f(-0.8f, 0.28f);//i2
glEnd();

glBegin(GL_LINES);
    glVertex2f(-1.0f, .28f);//g3
    glVertex2f(-0.8f, 0.534f);//j3
glEnd();

glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.6f);
    glVertex2f(-0.8f, .80f);
glEnd();
/*


*/
 // for1st building part 2 left
   glBegin(GL_POLYGON);
    glColor3f(0.5f, 0.5f, 0.5f); // Set color to gray
     glVertex2f(-0.8f, 1.0f);//t
    glVertex2f(-.8f, -0.358f);//o3
    glVertex2f(-0.45f, 0.0f);  // V
    glVertex2f(-0.45f, 1.0f);   // B1
glEnd();
glBegin(GL_POLYGON); //1st building others
    glColor3f(0.9f, 0.1f, 0.145f);
    glVertex2f(-0.75f, -0.0f);//T_1
    glVertex2f(-0.75f, -0.129f);//S_1
    glVertex2f(-0.5f, 0.05f);//f_2
    glVertex2f(-0.5f, 0.16f);//B2
    glEnd();




//Window 1
glBegin(GL_QUADS);
    glColor3f(0.678f, 0.847f, 0.902f);  //  Sky Blue (RGB)
    glVertex2f(-0.78f, 0.09f);  // W3
    glVertex2f(-0.47f, 0.25f);  // Z3
    glVertex2f(-0.469f, 0.38f); // A4
    glVertex2f(-0.78f, 0.26f);   // V3
glEnd();


//Window 2
glBegin(GL_POLYGON); // Drawing a quadrilateral
    glColor3f(0.678f, 0.847f, 0.902f); // Set color (e.g., light green or cyan)
    glVertex2f(-0.78f, 0.34f);  // S3
    glVertex2f(-0.47f, 0.45f); // B4
    glVertex2f(-0.47f, 0.59f); // C4
    glVertex2f(-0.78f, 0.51f);  // K3
glEnd();

//Window 3

glBegin(GL_POLYGON); // Drawing a quadrilateral
    glColor3f(0.678f, 0.847f, 0.902f);
    glVertex2f(-0.78f, 0.578f);  // E4
    glVertex2f(-0.469f, 0.66f);   // F4
    glVertex2f(-0.469f, 0.81f);  // G4
    glVertex2f(-0.78f, 0.765f);  // D4
glEnd();

//Window 4

glBegin(GL_POLYGON); // Drawing a quadrilateral
   glColor3f(0.678f, 0.847f, 0.902f);
    glVertex2f(-0.78f, 1.0f);    // N4
    glVertex2f(-0.78f, 0.837f);   // O4
    glVertex2f(-0.47f, 0.87f);   // P4
    glVertex2f(-0.47f, 1.0f);    // Q4
glEnd();



//window line
glLineWidth(7.5f); // Set line thickness to 0.5

glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white
    glVertex2f(-0.71f, 0.087f);  // W5
    glVertex2f(-0.71f, 1.0f);    // V5


glBegin(GL_LINES);
    glVertex2f(-0.62f, 0.14f);   // C5
    glVertex2f(-0.62f, 1.0f);    // B5
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.54f, 0.19f);   // D5
    glVertex2f(-0.54f, 1.0f);    // Z4
glEnd();

//
glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white

glBegin(GL_LINES);
   glVertex2f(-0.8f, 0.0314f);//s
    glVertex2f(-0.449f, 0.23f);   // M3
glEnd();


glBegin(GL_LINES);
       glVertex2f(-0.8f, 0.28f);//i2
    glVertex2f(-0.449f, 0.418f);    // N3
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.8f, 0.534f);  // j3
    glVertex2f(-0.449f, 0.617f);     // M
glEnd();

glBegin(GL_LINES);
    glVertex2f(-0.8f, 0.8f);  // s3
    glVertex2f(-0.45f, 0.84f);     // u3
glEnd();

// for2nd building part 1 left
glBegin(GL_POLYGON);
    glColor3f(0.119f, 0.9f, 0.105f);
    glVertex2f(-0.45f, 0.8f);   // W
    glVertex2f(-0.45f, 0.0f);   // v
    glVertex2f(-0.384f, 0.068f);  // Z
    glVertex2f(-0.383f, 0.792f); // A1
glEnd();

//
glLineWidth(6.0f); // Set line thickness to 0.5


glBegin(GL_LINES);
        glColor3f(0.60f, 0.60f, 0.60f);
    glVertex2f(-0.45f, 0.176f);  // A5
    glVertex2f(-0.38f, 0.29f);   // W5
glEnd();

glBegin(GL_LINES);
    glVertex2f(-0.45f, 0.299f);  // D5
    glVertex2f(-0.38f, 0.40f);   // B5
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.45f, 0.42f);   // G5
    glVertex2f(-0.38f, 0.52f);   // F5
glEnd();

glBegin(GL_LINES);
    glVertex2f(-0.45f, 0.54f);   // S5
    glVertex2f(-0.38f, 0.62f);   // Q5
glEnd();

glBegin(GL_LINES);
    glVertex2f(-0.45f, 0.67f);   // V5
    glVertex2f(-0.38f, 0.73f);   // U5
glEnd();


// for2nd building part 2 left
glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.9f, 0.105f);

    glVertex2f(-0.384f, 0.068f);  // Z
    glVertex2f(-0.383f, 0.792f); // A1
     glVertex2f(-0.287f, 0.717f) ;  // L1
    glVertex2f(-0.287f, 0.16f); // K1

glEnd();


// Rectangle 1
glBegin(GL_POLYGON);
    glColor3f(0.2f, 0.6f, 0.8f); // Set color (blue)
    glVertex2f(-0.378f, 0.716f);  // B8
    glVertex2f(-0.378f, 0.632f);  // C8
    glVertex2f(-0.294f, 0.632f);  // D8
    glVertex2f(-0.294f, 0.708f);  // E8
glEnd();

// Rectangle 2
glBegin(GL_POLYGON);
    glColor3f(0.4f, 0.8f, 0.4f); // Set color (green)
    glVertex2f(-0.378f, 0.524f);  // W7
    glVertex2f(-0.294f, 0.525f);  // Z7
    glVertex2f(-0.294f, 0.611f);  // A8
    glVertex2f(-0.378f, 0.612f);  // V7
glEnd();

// Rectangle 3
glBegin(GL_POLYGON);
    glColor3f(0.8f, 0.4f, 0.2f); // Set color (orange)
    glVertex2f(-0.378f, 0.412f);  // s7
    glVertex2f(-0.294f, 0.408f);  // T7
    glVertex2f(-0.294f, 0.505f);  // U7
    glVertex2f(-0.378f, 0.504f);  // R7
glEnd();

// Rectangle 4
glBegin(GL_POLYGON);
    glColor3f(0.6f, 0.2f, 0.8f); // Set color (purple)
    glVertex2f(-0.379f, 0.302f);  // O7
    glVertex2f(-0.294f, 0.304f);  // P7
    glVertex2f(-0.294f, 0.391f);  // Q7
    glVertex2f(-0.378f, 0.390f);  // N7
glEnd();

glLineWidth(6.0f); // Set line thickness to 0.5

glBegin(GL_LINES);
  glColor3f(0.9f, 0.9f, 0.105f);
    glVertex2f(-0.337f, 0.735f);  // G8
    glVertex2f(-0.337f, 0.269f);  // F8
glEnd();
glLineWidth(4.0f); // Set line thickness to 0.5


glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white
    glVertex2f(-0.384f, 0.29f);   // W5
    glVertex2f(-0.285f, 0.29f);   // Z5
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.382f, 0.40f);   // B6
    glVertex2f(-0.285f, 0.40f);   // C6
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.382f, 0.52f);   // F6
    glVertex2f(-0.28f, 0.51f);   // E6
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.382f, 0.62f);   // Q6
    glVertex2f(-0.285f, 0.62f);   // R6
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.3825f, 0.73f);   // U6
    glVertex2f(-0.29f, 0.72f);   // T6
glEnd();



// for3rd building part 1 left
glBegin(GL_POLYGON);
   glColor3f(0.9f, 0.9f, 0.9f);
     glVertex2f(-0.287f, 0.638f); // C1
     glVertex2f(-0.287f, 0.168f); // K1
      glVertex2f(-0.25, 0.203);  // v1

   glVertex2f(-0.25f, 0.633f);   //w1
glEnd();

// for3rd building part 2 left
glBegin(GL_POLYGON);

   glColor3f(0.3f, 0.4f, 0.9f);
  glVertex2f(-0.25, 0.203);  // v1
   glVertex2f(-0.25f, 0.633f);   //w1
     glVertex2f(-0.2f, 0.577f); //z1
  glVertex2f(-0.199f, 0.258f); // E2

glEnd();

glColor3f(0.0f, 1.0f, 1.0f); // Set color to Cyan for all rectangles

// Group 1 Rectangle
glBegin(GL_POLYGON);
    glVertex2f(-0.246f, 0.335f); // I8
    glVertex2f(-0.205f, 0.335f); // J8
    glVertex2f(-0.205f, 0.387f); // K8
    glVertex2f(-0.246f, 0.386f); // H8
glEnd();

// Group 2 Rectangle
glBegin(GL_POLYGON);
    glVertex2f(-0.246f, 0.415f); // N8
    glVertex2f(-0.207f, 0.415f); // O8
    glVertex2f(-0.206f, 0.472f); // P8
    glVertex2f(-0.246f, 0.473f); // M8
glEnd();

// Group 3 Rectangle
glBegin(GL_POLYGON);
    glVertex2f(-0.245f, 0.500f); // S8
    glVertex2f(-0.206f, 0.501f); // T8
    glVertex2f(-0.206f, 0.552f); // U8
    glVertex2f(-0.245f, 0.551f); // R8
glEnd();

glLineWidth(3.5f); // Set line thickness to 0.5
 glColor3f(0.3f, 0.4f, 0.9f); // Set color to white

glBegin(GL_LINES);
    glVertex2f(-0.224f, 0.574f); // F9
    glVertex2f(-0.225f, 0.313f); // G9
glEnd();

glLineWidth(4.0f); // Set line thickness to 0.5
glColor3f(1.0f, 1.0f, 1.0f); // Set color to white

// Group 1
glBegin(GL_LINES);
    glVertex2f(-0.286f, 0.525f); // W6
    glVertex2f(-0.251f, 0.567f); // Z6
    glVertex2f(-0.251f, 0.567f); // Z6
    glVertex2f(-0.199f, 0.567f); // A7
glEnd();

// Group 2
glBegin(GL_LINES);
    glVertex2f(-0.287f, 0.439f); // P6
    glVertex2f(-0.253f, 0.487f); // O6
    glVertex2f(-0.253f, 0.487f); // O6
    glVertex2f(-0.199f, 0.485f); // N6
glEnd();

// Group 3
glBegin(GL_LINES);
    glVertex2f(-0.287f, 0.348f); // M6
    glVertex2f(-0.252f, 0.401f); // L6
    glVertex2f(-0.252f, 0.401f); // L6
    glVertex2f(-0.198f, 0.400f); // K6
glEnd();

// Group 4
glBegin(GL_LINES);
    glVertex2f(-0.287f, 0.271f); // J6
    glVertex2f(-0.252f, 0.320f); // I6
    glVertex2f(-0.252f, 0.320f); // I6
    glVertex2f(-0.199f, 0.320f); // H6
glEnd();





// for4th building part 1 left
glBegin(GL_POLYGON);


   glColor3f(0.8f, 0.7f, 0.9f);
  glVertex2f(-0.2f, 0.52);   //c2
   glVertex2f(-0.199f, 0.258f); // E2
  glVertex2f(-0.18f, 0.277f); // G2
  glVertex2f(-0.18f, 0.52f); //a2
glEnd();

// for4th building part 2 left
glBegin(GL_POLYGON);


   glColor3f(0.45f, 0.07f, 0.9f);
    glVertex2f(-0.18f, 0.52f); //a2
     glVertex2f(-0.18f, 0.277f); // G2
    glVertex2f(-0.15f, 0.31f); // p
  glVertex2f(-0.15f, 0.49f);   //o2


glEnd();

glLineWidth(2.0f); // Set line thickness to 0.5
glColor3f(1.0f, 1.0f, 1.0f); // Set color to white

// Draw lines between the specified points
glBegin(GL_LINES);
    // B7 to C7
    glVertex2f(-0.199f, 0.335f);
    glVertex2f(-0.180f, 0.360f);

    // C7 to D7
    glVertex2f(-0.180f, 0.360f);
    glVertex2f(-0.150f, 0.360f);

    // E7to F7
    glVertex2f(-0.199f, 0.376f);
    glVertex2f(-0.181f, 0.400f);

    // F7 to G7
    glVertex2f(-0.181f, 0.400f);
    glVertex2f(-0.150f, 0.400f);

    // H7 to I7
    glVertex2f(-0.199f, 0.413f);
    glVertex2f(-0.181f, 0.440f);

    // I7to J7
    glVertex2f(-0.181f, 0.440f);
    glVertex2f(-0.150f, 0.440f);

    // K7 to L7
    glVertex2f(-0.199f, 0.461f);
    glVertex2f(-0.181f, 0.480f);

    // L7 to M7
    glVertex2f(-0.181f, 0.480f);
    glVertex2f(-0.149f, 0.480f);
glEnd();


glColor3f(0.0f, 1.0f, 1.0f); // Set color to Cyan for all rectangles


glBegin(GL_POLYGON);
    glVertex2f(-0.175f, 0.368f); // I9
    glVertex2f(-0.155f, 0.368f); // J9
    glVertex2f(-0.155f, 0.392f); // K9
    glVertex2f(-0.175f, 0.392f); // H9
glEnd();

glBegin(GL_POLYGON);
    glVertex2f(-0.175f, 0.407f); // C9
    glVertex2f(-0.155f, 0.407f); // D9
    glVertex2f(-0.155f, 0.432f); // E9
    glVertex2f(-0.175f, 0.432f); // B9
glEnd();

glBegin(GL_POLYGON);
    glVertex2f(-0.175f, 0.448f); // W8
    glVertex2f(-0.154f, 0.448f); // Z8
    glVertex2f(-0.154f, 0.472f); // A9
    glVertex2f(-0.175f, 0.472f); // V8
glEnd();


// for5th building part 1 left
glBegin(GL_POLYGON);


   glColor3f(0.6f, 0.0997f, 0.9f);
    glVertex2f(-0.15f, 0.46f); //M2
    glVertex2f(-0.15f, 0.31f); // p1
     glVertex2f(-0.141f, 0.317f); // c

  glVertex2f(-0.14f, 0.45f);   //Q2

glColor3f(0.0f, 1.0f, 1.0f); // Set color to Cyan for all rectangles


glBegin(GL_POLYGON);
    glVertex2f(-0.148f, 0.339f); // Z9
    glVertex2f(-0.144f, 0.339f); // W9
    glVertex2f(-0.143f, 0.383f); // V9
    glVertex2f(-0.148f, 0.383f); // Q9
glEnd();

glBegin(GL_POLYGON);
    glVertex2f(-0.148f, 0.406f); // P9
    glVertex2f(-0.143f, 0.406f); // O9
    glVertex2f(-0.143f, 0.438f); // N9
    glVertex2f(-0.148f, 0.438f); // M9
glEnd();





   //left side building end

    glBegin(GL_POLYGON);// 2ND building 1st floor 1st window
    glColor3f(0.7098f, 0.98f, 0.706f);
    glVertex2f(0.11f, 0.42f);
    glVertex2f(0.083f, 0.41f);
    glVertex2f(0.083f, 0.47f);
    glVertex2f(0.11f, 0.48f);
    glEnd();

    glBegin(GL_POLYGON);// 2ND building 1st floor 2nd window
    glColor3f(0.7098f, 0.98f, 0.706f);
    glVertex2f(0.11f, 0.527f);
    glVertex2f(0.083f, 0.51f);
    glVertex2f(0.083f, 0.575f);
    glVertex2f(0.11f, 0.59f);
    glEnd();

    glBegin(GL_POLYGON);// 2ND building 3rd floor 2nd window
    glColor3f(0.7098f, 0.98f, 0.706f);
    glVertex2f(0.073f, 0.516f);
    glVertex2f(0.046f, 0.503f);
    glVertex2f(0.045f, 0.556f);
    glVertex2f(0.073f, 0.57f);
    glEnd();

    glBegin(GL_POLYGON);// 2ND building 2nd floor 2nd window
    glColor3f(0.7098f, 0.98f, 0.706f);
    glVertex2f(0.073f, 0.41f);
    glVertex2f(0.0465f, 0.399f);
    glVertex2f(0.045f, 0.456f);
    glVertex2f(0.073f, 0.47f);
    glEnd();

    glBegin(GL_POLYGON);// 2ND building 4th floor 2nd window
    glColor3f(0.7098f, 0.98f, 0.706f);
    glVertex2f(0.073f, 0.62f);
    glVertex2f(0.0465f, 0.6f);
    glVertex2f(0.045f, 0.64f);
    glVertex2f(0.073f, 0.67f);
    glEnd();

    glBegin(GL_POLYGON);// 2ND building 4th floor 1st window
    glColor3f(0.7098f, 0.98f, 0.706f);
    glVertex2f(0.11f, 0.65f);
    glVertex2f(0.08f, 0.63f);
    glVertex2f(0.08f, 0.68f);
    glVertex2f(0.11f, 0.71f);
    glEnd();

    glBegin(GL_POLYGON);// 2ND building 5th floor 1st window
    glColor3f(0.7098f, 0.98f, 0.706f);
    glVertex2f(0.11f, 0.77f);
    glVertex2f(0.08f, 0.73f);
    glVertex2f(0.08f, 0.8f);
    glVertex2f(0.11f, 0.84f);
    glEnd();




// zebra Crossing
glLineWidth(7.0f); // Set line thickness to 0.5

glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
glBegin(GL_LINES);
    glVertex2f(-0.185f, 0.181f);
    glVertex2f(-0.166f, 0.238f);
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.165f, 0.183f); // G
    glVertex2f(-0.154f, 0.239f); // F
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.146f, 0.182f); // S
    glVertex2f(-0.137f, 0.239f); // H
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.128f, 0.182f); // U
    glVertex2f(-0.119f, 0.238f); // T


glBegin(GL_LINES);
    glVertex2f(-0.107f, 0.182f); // B
    glVertex2f(-0.100f, 0.240f); // A
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.086f, 0.183f); // D
    glVertex2f(-0.080f, 0.240f); // Unnamed point
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.054f, 0.181f); // F
    glVertex2f(-0.061f, 0.241f); // E
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.033f, 0.181f); // H
    glVertex2f(-0.040f, 0.240f); // G
glEnd();


glBegin(GL_LINES);
    glVertex2f(-0.011f, 0.181f); // J
    glVertex2f(-0.020f, 0.240f); // I
glEnd();


glBegin(GL_LINES);
    glVertex2f(0.007f, 0.181f); // N
    glVertex2f(-0.006f, 0.238f); // M
glEnd();


}
void tree()
{
    glBegin(GL_POLYGON);
    glColor3f(0.4f, 0.2f, 0.0f); // Brown color for the trunk
    glVertex2f(0.83 - 0.02f, -0.6 - 0.2f);
    glVertex2f(0.83 + 0.02f, -0.6 - 0.2f);
    glVertex2f(0.83 + 0.02f, -0.6 + 0.1f);
    glVertex2f(0.83 - 0.02f, -0.6 + 0.1f);
    glEnd();

    // Draw the tree canopy (multiple circles)
    glColor3f(0.0f, 0.5f, 0.0f); // Green color for the canopy
    drawCircle(0.83, -0.6 + 0.15f, 0.08f);
    drawCircle(0.83 - 0.06f, -0.6 + 0.1f, 0.06f);
    drawCircle(0.83 + 0.06f, -0.6 + 0.1f, 0.06f);
    drawCircle(0.83 - 0.04f, -0.6 + 0.2f, 0.06f);
    drawCircle(0.83 + 0.04f, -0.6 + 0.2f, 0.06f);



    // for tree 2
    // Draw the trunk (rectangle)
    glBegin(GL_POLYGON);
    glColor3f(0.4f, 0.2f, 0.0f); // Brown color for the trunk
    glVertex2f(-0.44 - 0.02f, 0.02 - 0.2f);
    glVertex2f(-0.44+ 0.02f, 0.02- 0.2f);
    glVertex2f(-0.44+ 0.02f, 0.02 + 0.1f);
    glVertex2f(-0.424- 0.02f, 0.02 + 0.1f);
    glEnd();

    // Draw the tree canopy (multiple circles)
    glColor3f(0.0f, 0.5f, 0.0f); // Green color for the canopy
    drawCircle(-0.44, -0.02 + 0.15f, 0.08f);
    drawCircle(-0.44- 0.06f, 0.02 + 0.1f, 0.06f);
    drawCircle(-0.44+ 0.06f, 0.02 + 0.1f, 0.06f);
    drawCircle(-0.44- 0.04f, 0.02 + 0.2f, 0.06f);
    drawCircle(-0.44+ 0.04f, 0.02 + 0.2f, 0.06f);
}

void bird()
{
    glPushMatrix();

    // Draw multiple birds with different positions and sizes, adjusting distance and size
    drawBird(birdPositionX, birdPositionY, birdScale);                // Bird 1
    drawBird(birdPositionX + 0.10f * birdScale, birdPositionY, birdScale - 0.15f);   // Bird 2
    drawBird(birdPositionX - 0.10f * birdScale, birdPositionY, birdScale - 0.15f);   // Bird 3

    drawBird(birdPositionX, birdPositionY - 0.05f * birdScale, birdScale);           // Bird 4
    drawBird(birdPositionX + 0.08f * birdScale, birdPositionY - 0.05f * birdScale, birdScale); // Bird 5
    drawBird(birdPositionX - 0.08f * birdScale, birdPositionY - 0.05f * birdScale, birdScale); // Bird 6

    drawBird(birdPositionX + 0.09f * birdScale, birdPositionY - 0.12f * birdScale, birdScale); // Bird 7
    drawBird(birdPositionX - 0.05f * birdScale, birdPositionY - 0.12f * birdScale, birdScale); // Bird 8

    drawBird(birdPositionX, birdPositionY + 0.07f * birdScale, birdScale);           // Bird 9
    drawBird(birdPositionX, birdPositionY - 0.17f * birdScale, birdScale);           // Bird 10

    glPopMatrix();
}
void human()
{
    updateMovement();
glPushMatrix();
 // drawHuman(-0.9f,  -0.80f,0.40f);
drawHuman(posX, posY, scale);
    glPopMatrix();
}





void car1()
{
    glPushMatrix();
    glTranslatef(carPositionX, carPositionY, 0.0f); // Translate to car position
    glScalef(carScale, carScale, 1.0f);            // Scale the car

    OmniRoute(0.0f, 0.0f);
    glPopMatrix();


    glPopMatrix();

}

void sun1()
{
     glPushMatrix();
    glTranslatef(sunX, sunY, 0.0f); // Translate the sun's position
        sun(0.0f, 0.0f, sunRadius);      // Draw the sun at the translated position
    glPopMatrix();

}


void cloud1()
{
    glPushMatrix();
    glTranslatef(cloudPosition, 0.0f, 0.0f);
    // Draw clouds at the specified positions
    cloud(-0.11, 0.59, 0.05); // Cloud m
    cloud(-0.11, 0.66, 0.05); // Cloud i
    cloud(-0.18, 0.634, 0.05); // Cloud n
    cloud(-0.053, 0.64, 0.05); // Cloud j
    glEnd();

glPopMatrix();

}
void drawStar(float x, float y, float size, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);  // Move the star to the correct position
    glColor3f(r, g, b);  // Set the color for the star

    glBegin(GL_POLYGON);  // Drawing the star
    glVertex2f(-0.5f * size, -0.3f * size);
    glVertex2f(-0.36f * size, -0.73f * size);
    glVertex2f(-0.55f * size, -0.53f * size);

    glVertex2f(-0.61f * size, -0.73f * size);
    glVertex2f(-0.36f * size, -0.47f * size);
    glVertex2f(-0.53f * size, -0.47f * size);

    glVertex2f(-0.61f * size, -0.47f * size);
    glVertex2f(-0.55f * size, -0.53f * size);

    glEnd();
    glPopMatrix();
}
void drawSceneStars() {
    // Define the size and color for the stars
    float size = 0.03f; // Uniform size for all stars
    float r = 1.0f, g = 1.0f, b = 1.0f; // White color

    // Render stars at the specified positions
    drawStar(-0.39f, 0.91f, size, r, g, b);
    drawStar(-0.33f, 0.97f, size, r, g, b);
    drawStar(-0.33f, 0.83f, size, r, g, b);
    drawStar(-0.27f, 0.88f, size, r, g, b);
    drawStar(-0.04f, 0.93f, size, r, g, b);
    drawStar(0.05f, 0.84f, size, r, g, b);
    drawStar(0.00f, 0.86f, size, r, g, b);
    drawStar(0.07f, 0.94f, size, r, g, b);
    drawStar(-0.17f, 0.94f, size, r, g, b);
}




// Function to render text dynamically
void renderBitmapString(float x, float y, void *font, const char *string) {
    glRasterPos2f(x, y);
    for (const char *ch = string; *ch != '\0'; ch++) {
        glutBitmapCharacter(font, *ch);
    }
}

// Function to calculate the width of the text for alignment
float getTextWidth(void *font, const char *string) {
    float width = 0.0f;
    for (const char *ch = string; *ch != '\0'; ch++) {
        width += glutBitmapWidth(font, *ch);
    }
    return width;
}
void interpol(float startX, float startY, float endX, float endY, float t, float &resultX, float &resultY) {
    resultX = startX + t * (endX - startX);
    resultY = startY + t * (endY - startY);
}

// Custom min and max functions without std
float customMin(float a, float b) {
    return (a < b) ? a : b;
}

float customMax(float a, float b) {
    return (a > b) ? a : b;
}

// Linear interpolation function renamed to interpolate1
float interpolate1(float start, float end, float progress) {
    return start + (end - start) * progress;
}


// Calculate the Euclidean distance between two points
float calculateDistance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Function to update position and size of the human figure based on speed and distance
// Update movement function for human
void updateMovement() {
    if (isPaused) return; // Do nothing if paused

    float distance, progress;
    if (onLeftPath) {
        distance = calculateDistance(posX, posY, (movingForward ? endX1 : startX1), (movingForward ? endY1 : startY1));
        if (distance > step) {
            posX = interpolate1(posX, (movingForward ? endX1 : startX1), step / distance);
            posY = interpolate1(posY, (movingForward ? endY1 : startY1), step / distance);
        }

        // Gradually adjust scale
        progress = (posY - startY1) / (endY1 - startY1);
        scale = interpolate1(.50f, 0.1f, progress);
    } else {
        distance = calculateDistance(posX, posY, (movingForward ? endX2 : startX2), (movingForward ? endY2 : startY2));
        if (distance > step) {
            posX = interpolate1(posX, (movingForward ? endX2 : startX2), step / distance);
            posY = interpolate1(posY, (movingForward ? endY2 : startY2), step / distance);
        }

        // Gradually adjust scale
        progress = (posY - startY2) / (endY2 - startY2);
        scale = interpolate1(.50f, 0.1f, progress);
    }
}

    void Hands(float cx, float cy, float radius, int segments = 50) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * PI * i / segments;
        glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
    }
    glEnd();

}void drawRectangle(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y - height);
    glVertex2f(x, y - height);
    glEnd();
}
// Function to draw a human figure
void drawHuman(float x, float y, float scale) {
    // Head
    glColor3f(.5f, 0.8f, 0.6f); // Skin color
    Hands(x, y + 0.6f * scale, 0.1f * scale);

    // Neck
    drawRectangle(x - 0.025f * scale, y + 0.5f * scale, 0.05f * scale, 0.1f * scale);

    // Torso
    glColor3f(0.0f, 0.0f, 1.0f); // Blue shirt
    drawRectangle(x - 0.1f * scale, y + 0.5f * scale, 0.2f * scale, 0.3f * scale);

    // Arms
    glColor3f(1.0f, 0.8f, 0.1f); // Skin color
    drawRectangle(x - 0.15f * scale, y + 0.45f * scale, 0.05f * scale, 0.25f * scale); // Left arm
    drawRectangle(x + 0.1f * scale, y + 0.45f * scale, 0.05f * scale, 0.25f * scale);  // Right arm

    // Hands
    Hands(x - 0.15f * scale, y + 0.2f * scale, 0.03f * scale); // Left hand
    Hands(x + 0.15f * scale, y + 0.2f * scale, 0.03f * scale);  // Right hand

    // Legs
    glColor3f(0.0f, 0.0f, 0.0f); // Black pants
    drawRectangle(x - 0.08f * scale, y + 0.2f * scale, 0.06f * scale, 0.3f * scale); // Left leg
    drawRectangle(x + 0.02f * scale, y + 0.2f * scale, 0.06f * scale, 0.3f * scale);  // Right leg

    // Feet
    glColor3f(0.4f, 0.2f, 0.0f); // Brown shoes
    drawRectangle(x - 0.1f * scale, y - 0.1f * scale, 0.08f * scale, 0.05f * scale); // Left shoe
    drawRectangle(x + 0.02f * scale, y - 0.1f * scale, 0.08f * scale, 0.05f * scale); // Right shoe
}
// Linear interpolation function



// Helper function to interpolate between two values
float interpolate(float start, float end, float factor) {
    return start + factor * (end - start);
}



void updateCar(int value) {
    if (carMoving) {
        // Increment Y position
        carPositionY += carSpeed;

        // Calculate progress after updating Y position
        float progress = (carPositionY - carStartY) / (carEndY - carStartY);
        progress = fminf(fmaxf(progress, 0.0f), 1.0f); // Clamp progress

        // Interpolate X position and scale based on progress
        carPositionX = interpolate(carStartX, carEndX, progress);
        carScale = interpolate(carScaleStart, carScaleEnd, progress);

        // Reset if the car reaches the end
        if (carPositionY >= carEndY) {
            carPositionX = carStartX;
            carPositionY = carStartY;
            carScale = carScaleStart;
        }

        glutPostRedisplay(); // Redraw the scene
    }

    glutTimerFunc(16, updateCar, 0); // Schedule next update (~60 FPS)
}


void Circle(double x, double y, double r, GLfloat red, GLfloat green, GLfloat blue) {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(red, green, blue); // Set the circle color
    glVertex2d(x, y);            // Center of the circle
    for (int i = 0; i <= 360; i++) {
        double angle = (i * M_PI) / 180.0; // Convert degrees to radians
        double dx = x + cos(angle) * r;
        double dy = y + sin(angle) * r;
        glVertex2d(dx, dy);
    }
    glEnd();
}


void sun(double x, double y, double r) {
    Circle(x, y, r, 1.0f, 0.843f, 0.0f); // Golden yellow color for the sun
}
void cloud(double x, double y, double r) {
    Circle(x, y, r, 0.988f, 0.98f, 0.961f); // Light white color for clouds
}

// Function to calculate size based on Y position
float calculateSize(float yPosition) {
    return 0.5f + (yPosition * 0.5f); // Size varies with Y position
}



// Function to draw a bird at a given position with a specific size
void drawBird(float x, float y, float scale) {
    // Head of the bird (circle)
    Circle(x, y, 0.011f * scale, 0.149f, 0.149f, 0.145f); // Dark gray color

    // Left wing (quad)
    glBegin(GL_QUADS);
    glColor3f(0.149f, 0.149f, 0.145f); // Dark gray color
    glVertex2f(x - 0.01f * scale, y);
    glVertex2f(x - 0.04f * scale, y + 0.04f * scale);
    glVertex2f(x - 0.08f * scale, y - 0.03f * scale);
    glVertex2f(x - 0.04f * scale, y + 0.02f * scale);
    glEnd();

    // Right wing (quad)
    glBegin(GL_QUADS);
    glColor3f(0.149f, 0.149f, 0.145f); // Dark gray color
    glVertex2f(x + 0.01f * scale, y);
    glVertex2f(x + 0.04f * scale, y + 0.04f * scale);
    glVertex2f(x + 0.08f * scale, y - 0.03f * scale);
    glVertex2f(x + 0.04f * scale, y + 0.02f * scale);
    glEnd();
}

// Function to update the bird's position and size
void updateBirdPosition() {
    // Interpolate the position of the bird from start to end
    birdPositionX += (birdEndX - birdStartX) * birdSpeedX;
    birdPositionY += (birdEndY - birdStartY) * birdSpeedY;

    // Interpolate the size of the bird based on its distance from the start position
    float distanceTraveled = sqrt(pow(birdPositionX - birdStartX, 2) + pow(birdPositionY - birdStartY, 2));
    float totalDistance = sqrt(pow(birdEndX - birdStartX, 2) + pow(birdEndY - birdStartY, 2));
    birdScale = birdScaleStart - (distanceTraveled / totalDistance) * (birdScaleStart - birdScaleEnd);

    // Reverse direction if the bird reaches the end position
    if (birdPositionX > birdEndX || birdPositionY > birdEndY) {
        birdPositionX = birdStartX;
        birdPositionY = birdStartY;
    }
}

void update(int value) {
    updateBirdPosition();  // Update the bird's position
    sunY += sunVerticalSpeed;   // Move sun vertically upward
    sunRadius -= sunShrinkRate; // Shrink the sun

    // Ensure the sun keeps rising and doesn't reset prematurely
    if (sunY > 1.0f || sunRadius <= 0.05f) { // Stop moving when the sun is too high or small
        sunY = 0.36f;   // Reset vertical position
        sunRadius = 0.2f; // Reset size
    }

    // Update moon position
    moonX += moonSpeedX; // Move moon horizontally
    moonY += moonSpeedY; // Move moon vertically

    // Ensure the moon keeps moving and doesn't reset prematurely
    if (moonY > moonEndY) { // Stop moving when the moon reaches the end position
        moonX = moonStartX;   // Reset horizontal position
        moonY = moonStartY;   // Reset vertical position
    }

    // Calculate the size of the moon based on its vertical position
    float progress = (moonY - moonStartY) / (moonEndY - moonStartY);
    moonRadius = moonMinRadius + progress * (moonMaxRadius - moonMinRadius); // Interpolate size

    // Ensure the moon does not go below the minimum size
    if (moonRadius >moonMinRadius) {
        moonRadius = moonMinRadius;
    }

    // Ensure the moon does not exceed the maximum size
    if (moonRadius < moonMaxRadius) {
        moonRadius = moonMaxRadius;
    }

    cloudPosition += cloudSpeed; // Move clouds to the right
    if (cloudPosition > .25f) {  // Reset position when off-screen
        cloudPosition = -.5f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Roughly 60 FPS
}

// Function to draw a filled ellipse
void drawFilledEllipse(float cx, float cy, float rx, float ry, int num_segments) {
    float theta = 2 * PI / float(num_segments);
    float c = cosf(theta); // precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = 1; // we start at angle = 0
    float y = 0;

    glBegin(GL_POLYGON);
    for (int ii = 0; ii < num_segments; ii++) {
        glVertex2f(x * rx + cx, y * ry + cy); // output vertex

        // apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();
}
// Function to draw an empty ellipse
void drawEmptyEllipse(float cx, float cy, float rx, float ry, int num_segments) {
    float theta = 2 * PI / float(num_segments);
    float c = cosf(theta); // precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = 1; // we start at angle = 0
    float y = 0;

    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++) {
        glVertex2f(x * rx + cx, y * ry + cy); // output vertex

        // apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();
}

// Function to draw a line
void drawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void OmniRoute(float x, float y) {
    float scale = 0.6f;  // Scale factor to reduce the car's size

    // Body of the car
    glColor3ub(9, 18, 84);
    drawFilledEllipse(x, y, 0.15f * scale, 0.6f * scale, 100); // Main body

    // Outline of the car
    glColor3f(0.0f, 0.0f, 0.0f);
    drawEmptyEllipse(x, y, 0.15f * scale, 0.45f * scale, 100);

    // New Roof Design (Sloped roof with more details)
    glColor3ub(47, 58, 137); // Roof color
    // Draw the main roof
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.14f * scale, y + 0.15f * scale); // Left edge of the roof
    glVertex2f(x - 0.07f * scale, y + 0.25f * scale); // Left top of the sloped roof
    glVertex2f(x + 0.07f * scale, y + 0.25f * scale); // Right top of the sloped roof
    glVertex2f(x + 0.14f * scale, y + 0.15f * scale); // Right edge of the roof
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f); // Outline color
    drawEmptyEllipse(x, y + 0.15f * scale, 0.14f * scale, 0.1f * scale, 100); // Draw outline for roof

    // Windows
    glColor3f(0.25f, 0.25f, 0.25f);

    // Wheels
    glColor3ub(0, 0, 0); // Black color for wheels
    // Top-left wheel
    drawFilledEllipse(x - 0.132f * scale, y + 0.30f * scale, 0.03f * scale, 0.13f * scale, 100);
    // Top-right wheel
    drawFilledEllipse(x +0.132f * scale, y + 0.30f * scale, 0.03f * scale, 0.13f * scale, 100);
    // Bottom-left wheel
    drawFilledEllipse(x -0.132f* scale, y - 0.30f * scale, 0.03f * scale, 0.13f * scale, 100);
    // Bottom-right wheel
    drawFilledEllipse(x + 0.132f* scale, y - 0.30f * scale, 0.03f * scale, 0.13f * scale, 100);
    //drawFilledEllipse(x + 0.15f * scale, y - 0.35f * scale, 0.03f * scale, 0.15f * scale, 100);


glColor3ub(255, 255, 102); // Yellow for headlights
glBegin(GL_POLYGON);
glVertex2f(x - 0.05f * scale, y + 0.55f * scale); // Bottom-left
glVertex2f(x - 0.02f * scale, y + 0.55f * scale); // Bottom-right
glVertex2f(x - 0.02f * scale, y + 0.58f * scale); // Top-right
glVertex2f(x - 0.05f * scale, y + 0.58f * scale); // Top-left
glEnd();

glBegin(GL_POLYGON);
glVertex2f(x + 0.02f * scale, y + 0.55f * scale); // Bottom-left
glVertex2f(x + 0.05f * scale, y + 0.55f * scale); // Bottom-right
glVertex2f(x + 0.05f * scale, y + 0.58f * scale); // Top-right
glVertex2f(x + 0.02f * scale, y + 0.58f * scale); // Top-left
glEnd();

// Taillights (rectangular design)
glColor3ub(255, 0, 0); // Red for taillights
glBegin(GL_POLYGON);
glVertex2f(x - 0.05f * scale, y - 0.58f * scale); // Bottom-left
glVertex2f(x - 0.02f * scale, y - 0.58f * scale); // Bottom-right
glVertex2f(x - 0.02f * scale, y - 0.55f * scale); // Top-right
glVertex2f(x - 0.05f * scale, y - 0.55f * scale); // Top-left
glEnd();

glBegin(GL_POLYGON);
glVertex2f(x + 0.02f * scale, y - 0.58f * scale); // Bottom-left
glVertex2f(x + 0.05f * scale, y - 0.58f * scale); // Bottom-right
glVertex2f(x + 0.05f * scale, y - 0.55f * scale); // Top-right
glVertex2f(x + 0.02f * scale, y - 0.55f * scale); // Top-left
glEnd();

}

void displayDay() {
    glClearColor(0.117f, 0.7412f, 0.969f, 1.0f); // Set background color for day
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



   sun1();
       cloud1();
    city();

    bird();
    human();
  car1();
    tree();
lamppost();
Traffic();

  glFlush();
}

void displayNight() {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // Set background color for night
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
moon1();
   cloud1();
    city();
    tree();
   lamppost1();
    car1();
   Traffic();
drawSceneStars();

 glFlush();
}

void displayRain() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set background color for rain
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //sun1();
    cloud1();
    city();
    tree();
    //bird();
    //human();
    car1();
 Traffic();

    // Draw rain
    glColor3f(.80f, .80f, 1.0f); // Blue color for rain
    for (int i = 0; i < 100; i++) {
        float x = (rand() % 200 - 100) / 100.0f; // Random x position
        float y = (rand() % 200 - 100) / 100.0f; // Random y position
        glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x, y - 0.1f); // Draw a line for rain
        glEnd();
    }

glFlush();
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);



    switch (currentDisplayMode) {
        case DAY:
            displayDay();
            break;
        case NIGHT:
            displayNight();
            break;
        case RAIN:
            displayRain();
            break;
    }

     glColor3f(1.0, 1.0, 1.0); // Set text color to red

    // Render each character at the specified coordinates
    renderBitmapString(-0.71f, -0.046f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "C");
    renderBitmapString(-0.69f, -0.036f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "a");
    renderBitmapString(-0.67f, -0.025f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "f");
    renderBitmapString(-0.66f, -0.02f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "e");

    renderBitmapString(-0.63f, 0.011f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "S");
    renderBitmapString(-0.61f, 0.02f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "h");
    renderBitmapString(-0.59f, 0.026f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "o");
    renderBitmapString(-0.575f, 0.0299f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "p");

     renderBitmapString(0.64f, 0.067f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "H");
    renderBitmapString(0.665f, 0.06f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "o");
    renderBitmapString(0.68f, 0.057f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "t");
    renderBitmapString(0.69f, 0.05f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "e");
    renderBitmapString(0.71f, 0.0447f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "l");
    renderBitmapString(0.77f, 0.032f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "I");
    renderBitmapString(0.78f, 0.026f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "n");
    renderBitmapString(0.80f, 0.022f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "t");
    renderBitmapString(0.82f, 0.015f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, ".");


    glFlush();


}



   void timer(int value) {
    // Update bird position (called periodically)
    updateBirdPosition();

    glutPostRedisplay(); // Request a redraw
    glutTimerFunc(16, timer, 0); // Call timer again after 16ms
}
void keyboardHandler(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            birdSpeedX = -fabs(birdSpeedX); // Move the bird to the left
            break;
        case GLUT_KEY_RIGHT:
            birdSpeedX = fabs(birdSpeedX); // Move the bird to the right
            break;
        case GLUT_KEY_UP:
            birdSpeedY = fabs(birdSpeedY); // Move the bird up
            break;
        case GLUT_KEY_DOWN:
            birdSpeedY = -fabs(birdSpeedY); // Move the bird down
            break;
    }
    glutPostRedisplay(); // Request a redraw

}
// Utility to calculate minimum
float min(float a, float b) {
    return (a < b) ? a : b;
}

// Utility to calculate maximum
float max(float a, float b) {
    return (a > b) ? a : b;
}

void keyboard(unsigned char key, int x, int y) {
    // Handle car speed controls
    switch (key) {
        case 'x': // Increase speed
            carSpeed += 0.001f;
            break;
        case 'y': // Decrease speed
            carSpeed = fmaxf(carSpeed - 0.001f, 0.001f); // Prevent negative speed
            break;
        case '1': // Switch to day mode
            currentDisplayMode = DAY;
            break;
        case '2': // Switch to night mode
            currentDisplayMode = NIGHT;
            break;
        case '3': // Switch to rain mode
            currentDisplayMode = RAIN;
            break;
        default:
            break;
    }

    // Handle human movement controls
    if (key == 'w' || key == 'W') {  // Pause movement
        isPaused = !isPaused; // Toggle pause state
    } else if (key == 's' || key == 'S') {  // Move forward
        movingForward = true;
        isPaused = false; // Ensure movement resumes
    } else if (key == 'b' || key == 'B') {  // Move backward
        movingForward = false;
        isPaused = false; // Ensure movement resumes
    } else if (key == 'r' || key == 'R') {  // Switch to right path
        if (onLeftPath) {
            float progress = (posY - startY1) / (endY1 - startY1);
            posX = interpolate1(startX2, endX2, progress);
            posY = interpolate1(startY2, endY2, progress);
            scale = interpolate1(1.0f, 0.5f, progress);
            onLeftPath = false;
        }
    } else if (key == 'l' || key == 'L') {  // Switch to left path
        if (!onLeftPath) {
            float progress = (posY - startY2) / (endY2 - startY2);
            posX = interpolate1(startX1, endX1, progress);
            posY = interpolate1(startY1, endY1, progress);
            scale = interpolate1(1.0f, 0.5f, progress);
            onLeftPath = true;
        }
    }
if (key == 'a') {
        sndPlaySound("birdSound.wav", SND_ASYNC);}

        else if (key =='r')
        {
           sndPlaySound("rainSound.wav", SND_ASYNC);
        }
      else if (key =='c')
        {
           sndPlaySound("carSound.wav", SND_ASYNC);
        }
        else if (key =='h')
        {
           sndPlaySound("car-horn.wav", SND_ASYNC);
        }
    glutPostRedisplay(); // Redraw screen
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Toggle car's movement state on left mouse click
        carMoving = !carMoving;
        isRedLightDeep = !isRedLightDeep; // Toggle the red light color
    }



    glutPostRedisplay(); // Redraw screen
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1400, 730); // Window size
    glutInitWindowPosition(30, 20); // Window position
    glutCreateWindow("City View "); // Window title

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Coordinate system

    glutDisplayFunc(display);          // Display callback
    glutKeyboardFunc(keyboard);        // Register the combined keyboard handler
    glutSpecialFunc(keyboardHandler);  // Special key handler
    glutMouseFunc(mouse);              // Register mouse callback
    glutTimerFunc(33, updateCar, 0);   // Update the car position
    glutTimerFunc(33, update, 0);      // General update function


    sndPlaySound("rainSound.wav", SND_ASYNC);
    sndPlaySound("carSound.wav", SND_ASYNC);
    sndPlaySound("car-horn.wav", SND_ASYNC);
     sndPlaySound("birdSound.wav", SND_ASYNC);


    glutMainLoop(); // Start the main event loop

    return 0;
}



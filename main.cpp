#include <windows.h> // For MS Windows
#include <GL/glut.h> // GLUT, include glu.h and gl.h

/* Function to render text on the screen */
void renderBitmapString(float x, float y, float z, void *font, const char *string)
{
    const char *c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

/* Display function to render the cover page */
void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // Set background color to white
    glClear(GL_COLOR_BUFFER_BIT);        // Clear the color buffer (background)

    glColor3f(0.0, 0.0, 0.0); // Set text color to black

    // Institution Name (at the top center)
    renderBitmapString(-0.4f, 0.8f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "American International University of Bangladesh");
    //Course name
    renderBitmapString(-0.35f, 0.6f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Course Name: Computer Graphiics");
    // Title (slightly below the institution name)
    renderBitmapString(-0.35f, 0.5f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Project Title: OmniRoute: The Future of Multi-Modal Transportation");
    //group number
    renderBitmapString(-0.35f, 0.4f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Group: 01");

    // Section
    renderBitmapString(-0.35f, 0.3f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Section: H");
    // date
    renderBitmapString(-0.35f, 0.2f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Date: 19th January, 2025");


    // Members (centered)
    renderBitmapString(-0.45f, 0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Group Members: ");
    renderBitmapString(-0.45f, -0.1f, 0.0f, GLUT_BITMAP_HELVETICA_18, "1. Name: MD.Sohanur Rohaman Riddho         ID: 22-46800-1     (Serial No: 06)");
    renderBitmapString(-0.45f, -0.2f, 0.0f, GLUT_BITMAP_HELVETICA_18, "2. Name: Monim Tazwar                                   ID: 22-49399-3     (Serial No: 33)");
    renderBitmapString(-0.45f, -0.3f, 0.0f, GLUT_BITMAP_HELVETICA_18, "3. Name: Meherun Nessa Suchana                  ID: 22-49405-3     (Serial No: 34)");
    renderBitmapString(-0.45f, -0.4f, 0.0f, GLUT_BITMAP_HELVETICA_18, "4. Name: Aeysha Akter Purnia                          ID: 22-49538-3     (Serial No: 35)");
    renderBitmapString(-0.45f, -0.5f, 0.0f, GLUT_BITMAP_HELVETICA_18, "5. Name: Ahmed Jubayer                                 ID: 22-49862-3     (Serial No: 41)");

    // Supervised By Section (centered)
    renderBitmapString(-0.25f, -0.7f, 0.0f, GLUT_BITMAP_HELVETICA_18, "Supervised By: MAHFUJUR RAHMAN");

    glFlush(); // Render now
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(1400, 730);
    glutInitWindowPosition(30, 20);
    glutCreateWindow("Cover Page");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

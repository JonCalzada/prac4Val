#include <GL/glut.h>
#include <cmath>

// Función que dibuja la circunferencia utilizando el algoritmo de Bresenham
void drawCircle(int x0, int y0, int r) {
    int x = 0;
    int y = r;
    int d = 3 - 2*r;

    glBegin(GL_POINTS);
    while (x <= y) {
        glVertex2i(x0 + x, y0 + y);
        glVertex2i(x0 + y, y0 + x);
        glVertex2i(x0 - y, y0 + x);
        glVertex2i(x0 - x, y0 + y);
        glVertex2i(x0 - x, y0 - y);
        glVertex2i(x0 - y, y0 - x);
        glVertex2i(x0 + y, y0 - x);
        glVertex2i(x0 + x, y0 - y);

        if (d < 0) {
            d = d + 4*x + 6;
        } else {
            d = d + 4*(x-y) + 10;
            y--;
        }
        x++;
    }
    glEnd();
}

// Función que se encarga de renderizar la escena
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0); // Establecemos el color de la circunferencia a blanco
    drawCircle(200, 200, 100); // Dibujamos la circunferencia centrada en (200, 200) con radio 100

    glFlush();
}

// Función principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("Circunferencia de Bresenham");
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

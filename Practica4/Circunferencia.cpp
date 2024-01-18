/*---------------------------------------------------------*/
/*-----------------    2023-2   ---------------------------*/
/*------------- Alumno: Del Valle Aragón Alexis Rafael          ---------------*/
/*------------- Circunferencia de Bresenham*/

#include <glew.h>
#include <glfw3.h>
#include <glut.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow* window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor* monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void getResolution(void);

/*Ingresamos nuestros valores de entrada
centro (X,Y) y radio de la circunferencia*/
float   x = 45.0f,
        y = 69.0f,
        radius = 300.87f;

void getResolution()
{
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    SCR_WIDTH = mode->width;
    SCR_HEIGHT = (mode->height) - 80;
}


void drawCircle(float x, float y, float radius)
{
    glBegin(GL_POINTS);
    int d = 3 - 2 * radius;
    int xPos = 0;
    int yPos = radius;
    if ( radius > 0)    //Validamos que el radio sea  positivo
    {
        while (xPos <= yPos)
        {
            // Simétrico alrededor de los ejes cartesianos
            glVertex2f(x + xPos, y + yPos);
            glVertex2f(x - xPos, y + yPos);
            glVertex2f(x + xPos, y - yPos);
            glVertex2f(x - xPos, y - yPos);
            glVertex2f(x + yPos, y + xPos);
            glVertex2f(x - yPos, y + xPos);
            glVertex2f(x + yPos, y - xPos);
            glVertex2f(x - yPos, y - xPos);

            if (d < 0)
            {
                d += 4 * xPos + 6;
            }
            else
            {
                d += 4 * (xPos - yPos) + 10;
                yPos--;
            }
            xPos++;
        }

        glEnd();
    }
}

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//Color de Lienzo
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);    //Color de circunfrencia
    glPointSize(2.0f);      //Grosor de circunferencia
    //centro y radio de la cirunferencia(X,Y,r);
    drawCircle(x, y, radius); 
    glFlush();

    //Plano cartesiano centrad en el origen (0,0)
    // Eje x  
    glBegin(GL_LINES);
    glVertex2f(-1000, 0);
    glVertex2f(1000, 0);
    glEnd();

    // Eje y
    glBegin(GL_LINES);
    glVertex2f(0, -1000);
    glVertex2f(0, 1000);
    glEnd();

    // Mostrar el resultado
    glutSwapBuffers();
}

int main(int argc, char** argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    //Pantalla 3 veces el radio en horizontal y vertical
    //Es recomendable obtener un tamaño cuadratico para observar la circunferencia con mejor detalle.
    glutInitWindowSize(3*radius, 3*radius); 
    glutCreateWindow("Circunferencia de Bresenham");
    //Posicion de pantalla desplazada 2 veces el radio del centro en X y Y
    //Para obtener un tamaño medio.
    gluOrtho2D(x-2*radius, x+ 2 * radius, y- 2 * radius, y+ 2 * radius);
    glutDisplayFunc(display);
    glutMainLoop();
    glEnable(GL_DEPTH_TEST);
    return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    //2023-1
    glViewport(0, 0, width, height);
}
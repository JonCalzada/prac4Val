/*---------------------------------------------------------*/
/* ----------------   Práctica 4 --------------------------*/
/*-----------------    2023-2   ---------------------------*/
/*------------- Alumno: Del Valle Aragón Alexis Rafael                      ---------------*/
/*------------- No. de cuenta:  314088808            ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void getResolution(void);

//For Keyboard
float	movX = -6.0f,
		movY = 0.0f,
		movZ = -25.0f;

float	angX = 0.0f,
		angY = 0.0f,
		angZ = 0.0f;

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	GLfloat verticesCubo[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		//V0 - Frontal
		0.5f, -0.5f, 0.5f,		//V1
		0.5f, 0.5f, 0.5f,		//V5
		-0.5f, -0.5f, 0.5f,		//V0
		-0.5f, 0.5f, 0.5f,		//V4
		0.5f, 0.5f, 0.5f,		//V5

		0.5f, -0.5f, -0.5f,		//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, 0.5f, -0.5f,		//V7
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, 0.5f, -0.5f,		//V6
		-0.5f, 0.5f, -0.5f,		//V7

		-0.5f, 0.5f, 0.5f,		//V4 - Izq
		-0.5f, 0.5f, -0.5f,		//V7
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, 0.5f, 0.5f,		//V4
		-0.5f, -0.5f, 0.5f,		//V0

		0.5f, 0.5f, 0.5f,		//V5 - Der
		0.5f, -0.5f, 0.5f,		//V1
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, 0.5f, 0.5f,		//V5
		0.5f, 0.5f, -0.5f,		//V6
		0.5f, -0.5f, -0.5f,		//V2

		-0.5f, 0.5f, 0.5f,		//V4 - Sup
		0.5f, 0.5f, 0.5f,		//V5
		0.5f, 0.5f, -0.5f,		//V6
		-0.5f, 0.5f, 0.5f,		//V4
		-0.5f, 0.5f, -0.5f,		//V7
		0.5f, 0.5f, -0.5f,		//V6

		-0.5f, -0.5f, 0.5f,		//V0 - Inf
		-0.5f, -0.5f, -0.5f,	//V3
		0.5f, -0.5f, -0.5f,		//V2
		-0.5f, -0.5f, 0.5f,		//V0
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, -0.5f, 0.5f,		//V1
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 4 2023-2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

	Shader myShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");
	myShader.use();

	glm::mat4 viewOp = glm::mat4(1.0f);			//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection

	glm::mat4 cuelloOp = glm::mat4(1.0f); //Crea otra matriz ""CuelloOp" y la limpiamos
	glm::mat4 HomIzqOp = glm::mat4(1.0f);
	glm::mat4 HomDerOp = glm::mat4(1.0f);
	glm::mat4 BraDerOp = glm::mat4(1.0f);
	glm::mat4 CaderaOp = glm::mat4(1.0f);
	glm::mat4 PieIzqOp = glm::mat4(1.0f);
	glm::mat4 PieDerOp = glm::mat4(1.0f);

	glm::mat4 TorMinrOp = glm::mat4(1.0f);
	glm::mat4 OjoIzqOp = glm::mat4(1.0f);
	glm::mat4 OjoDerOp = glm::mat4(1.0f);

	//Use "projection" in order to change how we see the information
	projectionOp = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Background color
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi bloque de dibujo
		/*******************************************/
		glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
		//Use "view" in order to affect all models
		viewOp = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));
		viewOp = glm::rotate(viewOp, glm::radians(angX), glm::vec3(1.0f, 0.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(angY), glm::vec3(0.0f, 1.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(angZ), glm::vec3(0.0f, 0.0f, 1.0f));
		// pass them to the shaders
		myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		myShader.setMat4("projection", projectionOp);

		//Modelo
		glBindVertexArray(VAO);

			//Pecho
			modelOp = glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 5.0f, 1.0f)); //Aplicar escala referida al origen (Factores de escala con formula Vf/Vi)
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//Cuello  CuelloOp guarda la matriz sin incluir la escala
			cuelloOp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.75f, 0.0f)); 
			modelOp = glm::scale(modelOp, glm::vec3(0.5f, 0.5f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//Cabeza
			//modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.25f, 0.0f));
			modelOp = glm::translate(cuelloOp, glm::vec3(0.0f, 1.5f, 0.0f));//Crea a partir del cuello sin añadir la escala
			modelOp = glm::scale(modelOp, glm::vec3(1.5f, 2.5f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//Hombro izquierdo
			HomIzqOp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-2.75f, 2.25f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(1.5f, 0.5f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//Brazo izquierdo
			modelOp = glm::translate(HomIzqOp, glm::vec3(-0.25f, -2.0f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(1.0f, 3.5f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//Hombro derecho
			HomDerOp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.75f, 2.25f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(1.5f, 0.5f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//Brazo derecho
			BraDerOp = modelOp = glm::translate(HomDerOp, glm::vec3(0.25f, -2.0f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(1.0f, 3.5f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//Espada
			modelOp = glm::translate(BraDerOp, glm::vec3(0.0f, -1.9f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(1.0f, 0.3f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.502f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			modelOp = glm::translate(BraDerOp, glm::vec3(0.65f, -1.9f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(0.3f, 0.6f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.502f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			modelOp = glm::translate(BraDerOp, glm::vec3(2.4f, -1.9f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(3.5f, 0.3f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(0.6902f, 0.7686f, 1.0f));
			glDrawArrays(GL_TRIANGLES, 0, 36);


			//Cadera
			CaderaOp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(4.0f, 1.0f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(0.9569f, 0.6431f, 0.3765f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//Pierna izquierda
			PieIzqOp =modelOp = glm::translate(CaderaOp, glm::vec3(-1.25f, -2.25f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(1.5f, 3.5f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(0.9569f, 0.6431f, 0.3765f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//Pierna derecha
			PieDerOp = modelOp = glm::translate(CaderaOp, glm::vec3(1.25f, -2.25f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(1.5f, 3.5f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(0.9569f, 0.6431f, 0.3765f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//Pie izquierdo
			modelOp = glm::translate(PieIzqOp, glm::vec3(-0.5f, -2.125f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(2.5f, 0.75f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(0.7373f, 0.5608f, 0.5608f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//Pie derecho
			modelOp = glm::translate(PieDerOp, glm::vec3(0.5f, -2.125f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(2.5f, 0.75f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(0.7373f, 0.5608f, 0.5608f));
			glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glBindVertexArray(VAO);
		//Figura Minecraft
		//Torso
		TorMinrOp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, 2.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(4.0f, 4.0f, 3.0f)); //Aplicar escala referida al origen (Factores de escala con formula Vf/Vi)
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuello
		modelOp = glm::translate(TorMinrOp, glm::vec3(0.0f, 2.25f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 0.5f, 0.75f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cabeza
		modelOp = glm::translate(TorMinrOp, glm::vec3(0.0f, 4.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(3.0f, 3.0f, 3.5f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Brazo derecho
		modelOp = glm::translate(TorMinrOp, glm::vec3(2.075f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.15f, 0.5f, 0.5f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(TorMinrOp, glm::vec3(2.45f, -2.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 8.0f, 0.75f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Brazo Izquierdo
		modelOp = glm::translate(TorMinrOp, glm::vec3(-2.075f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.15f, 0.5f, 0.5f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(TorMinrOp, glm::vec3(-2.45f, -2.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 8.0f, 0.75f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pierna Izquierda
		modelOp = glm::translate(TorMinrOp, glm::vec3(-1.325f, -7.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 10.0f, 0.75f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pierna derecho
		modelOp = glm::translate(TorMinrOp, glm::vec3(1.325f, -7.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 10.0f, 0.75f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ojo izquierdo
		OjoIzqOp = modelOp = glm::translate(TorMinrOp, glm::vec3(-0.75f, 4.5f, 1.8f));
		modelOp = glm::scale(modelOp, glm::vec3(0.8f, 0.35f, 0.1f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(OjoIzqOp, glm::vec3(0.0f, 0.0f, 0.05f));
		modelOp = glm::scale(modelOp, glm::vec3(0.55f, 0.35f, 0.1f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.7294f, 0.3333f, 0.8275f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ojo derecho
		OjoDerOp = modelOp = glm::translate(TorMinrOp, glm::vec3(0.75f, 4.5f, 1.8f));
		modelOp = glm::scale(modelOp, glm::vec3(0.8f, 0.35f, 0.1f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(OjoDerOp, glm::vec3(0.0f, 0.0f, 0.05f));
		modelOp = glm::scale(modelOp, glm::vec3(0.55f, 0.35f, 0.1f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.7294f, 0.3333f, 0.8275f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 1.0f;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		angX -= 2.5f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		angX += 2.5f;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		angY -= 2.5f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		angY += 2.5f;

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		angZ -= 2.5f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		angZ += 2.5f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}
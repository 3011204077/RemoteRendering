#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc\matrix_inverse.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Geometry.h"
#include "Camera.h"
#include "Util.h"

#include <sstream>

int programID;
Geometry* earth;
int modelLocation;
glm::mat4x4 modelMatrix;
glm::vec3 moveDir;
bool bContinue;
Camera* cam;
bool culling;
bool wireframe;
int n;
int viewProjLoc;

void printMat(glm::mat4x4 m, const char* info)
{
	printf(info);
	printf("\n");
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			printf("%f ", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");

}
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);

	glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(cam->getView()));

	glm::mat4 viewProj = cam->getProjection() * cam->getView();

	glUniformMatrix4fv(viewProjLoc, 1, false, glm::value_ptr(viewProj));

	earth->draw();
	glutSwapBuffers();

	//glCheckError(glGetError(), "After drawing");
}

void timer(int v)
{
	glutPostRedisplay();
	glutTimerFunc(16, timer, v);
}

void keyboard(unsigned char key, int x, int y)
{
	if(key == 'w')
	{
		cam->move(1,0,0);
	}

	if(key == 's')
	{
		cam->move(-1,0,0);
	}
}

GLuint createTexture(const char* fileName)
{
	GLuint texID;
	int width, height, imgFormat, internalFormat;
	float* imgData = getImage(fileName, &height, &width, &imgFormat);
	switch (imgFormat)
	{
	case GL_RED: internalFormat = GL_R8; break;
	case GL_RG: internalFormat = GL_RG8; break;
	case GL_RGB: internalFormat = GL_RGB8; break;
	case GL_RGBA: internalFormat = GL_RGBA8; break;
	default: fprintf(stderr, "\n Cannot get ImgType \n"); break;
	}
	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imgFormat, GL_FLOAT, (void*) imgData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	glUniform1i(glGetUniformLocation(programID, "colorTex"), 0);
	earth->draw();
	glCheckError(glGetError(), "After drawing");
	//glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

void initialize(int argc, char** argv)
{
	modelMatrix = glm::mat4x4();
	moveDir = glm::vec3(0.0f, 0.0f, 0.0f);
	bContinue = true;
	cam = new Camera();
	culling = true;
	wireframe = true;
	n = 2;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Decoder!");
	glewInit();	

	glutDisplayFunc(render);
	glutTimerFunc(16, timer, 0);
	glutKeyboardFunc(keyboard);

	glViewport(0, 0, 800, 600);
	glPrimitiveRestartIndex(-1);
	glEnable(GL_PRIMITIVE_RESTART);
}

void main(int argc, char** argv)
{
	initialize(argc, argv);
	glCheckError(glGetError(), "After init");
	programID = createShaderProgram("shader/Main_VS.glsl", "shader/VertexColor_FS.glsl");
	glCheckError(glGetError(), "After creating Program");
	modelLocation = glGetUniformLocation(programID, "model");
	glCheckError(glGetError(), "After model");
	viewProjLoc = glGetUniformLocation(programID, "viewProj");
	glCheckError(glGetError(), "After ViewProjLoc");
	cam->move(-5.0f, 0.0f, 0.0f);
	earth = createSphere(1, 64, 32, "textures/earth.jpg");
	createTexture("textures/earth.jpg");

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glCheckError(glGetError(), "Before main Loop");

	glutMainLoop();
	std::cin.get();
}

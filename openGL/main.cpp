#include <iostream>
#include<gl/glut.h>
#include<gl/glu.h>
#include<gl/gl.h>
#include "errorCheck.h"





using namespace std;

//test1
void init(void) {
	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}
void linesegment(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	glBegin(GL_LINES);
	//glBegin(GL_POINTS);
	glVertex2f(180,15);
	glVertex2f(10,145);
	glEnd();
	glFlush();
}
void test1(void) {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(400, 300);
	glutCreateWindow("OpenGL - First window demo");
	init();
	glutDisplayFunc(linesegment);
	glutMainLoop();
}

//test2
void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, 0.0, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glEnd();
    glutSwapBuffers();
}
void test2(void){
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(320, 320);
    glutCreateWindow("Hello OpenGL");
    glutDisplayFunc(renderScene);
}


int main1(int argc ,char* argv[]) {
	glutInit(&argc, (char**)argv);
	test1();
	//test2();
	glutMainLoop();
	return 0;
}


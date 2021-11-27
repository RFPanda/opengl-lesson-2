#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include "glut.h"  
#include <Windows.h>
#include <vector>
using namespace std;
//
struct Star
{
	float x, y, z;
};
vector<Star> starlist;
enum { RANGE = 80 };
//

float mat1_dif[] = {1,1,1 }; // вектор diffuse определяет цвет поверхности при рассеянном освещении. Рассеянный цвет (так же, как и фоновое освещение) устанавливается на желаемый цвет поверхности;
float mat1_amb[] = { 0.9f, 0.9f, 0.9f }; //  вектор ambient определяет, какой цвет отражает поверхность при фоновом освещении (обычно он совпадает с цветом поверхности);
float mat1_spec[] = { 0.1f, 0.1f, 0.1f }; // вектор specular задает цвет зеркального блика на поверхности (или, возможно, даже отражает специфический для поверхности цвет);
float mat1_shininess = 0.02; //переменная shininess влияет на рассеивание/радиус зеркального блика.

bool one = false;
static float tx = 0.0;
static float rotx = 0.0; // скорость вращения
static float worldX = 15.0;
static float worldY = 0.0;
static bool animate = true; // запуск анимации сразу
static float first = 1;
static float second = 1;
float a=0;
int globalW, globalH;

using namespace std;


void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	a += 0.0001;
	//
	for (vector<Star>::iterator i = starlist.begin(); i != starlist.end(); ++i)
	{
		const float L = 70 / abs(i->z - (70 + 50));
		glPointSize(1.0f + 1.0f - (50 - i->z) / 550.0f);
		glColor3f(L, L, L);
		glBegin(GL_POINTS);
		glVertex3f(i->x, i->y, i->z);
		glEnd();
	}//

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat1_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat1_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat1_spec);
	glMaterialf(GL_FRONT, GL_SHININESS, mat1_shininess);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glTranslatef(0, 0, -100);

	if (!animate) {
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glColor3f(1, 1, 1);
		glTranslatef(-8, 50, 0.0);
		glPopMatrix();
	}

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLUquadricObj* quadric;
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluDeleteQuadric(quadric);

	glScalef(0.4, 0.4, 0.4);

	glRotatef(worldX, 1, 0, 0);
	glRotatef(worldY, 0, 1, 0);

	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0.4);
	glVertex2f(-500, -500);
	glVertex2f(-500, 500);
	glColor3f(0.1, 0.1, 1);
	glVertex2f(500, 500);
	glVertex2f(500, -500);
	glEnd();


	glDisable(GL_LIGHTING);

	glPushMatrix();
	glColor3f(0.8, 0.498039, 0.196078);
	gluSphere(quadric, 15, 36, 18);
	glColor3f(1, 1, 0);
	glEnable(GL_BLEND);
	glBlendFunc(first, second);
	gluSphere(quadric, 20, 36, 18);
	glDisable(GL_BLEND);
	glPopMatrix();


	glEnable(GL_LIGHTING);
	GLfloat light_position[] = { 0.0, 0.0, 0.0,1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	GLfloat light_diff[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
	GLfloat light_amb[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);



	glPushMatrix(); //1 planet
	glRotatef(rotx, 0, 1, 0);
	glTranslatef(40, 0, -10.0);
	glRotatef(rotx, 1, 0, 0);
	glColor3f(1, 0, 0);
	gluSphere(quadric, 5, 36, 18);
	glPopMatrix();

	glPushMatrix();// 2 planet
	glRotatef(rotx, 0, 1, 0);
	glTranslatef(-20, 0.0, -70);
	glRotatef(rotx, 0, 0, 1);
	glColor3f(0, 1, 0);
	gluSphere(quadric, 10, 36, 18);
	glPopMatrix();

	glPushMatrix();// 3 planet
	glRotatef(rotx, 0, 1, 0);
	glTranslatef(-60, 0, 0.0);
	glRotatef(rotx, 1, 0, 0);
	glColor3f(0.55217, 0.3525, 1);
	gluSphere(quadric, 17, 36, 18);
	glPopMatrix();


	glPushMatrix();// 4 planet
	glRotatef(rotx, 0, 1, 0);
	glTranslatef(20, 0, 44.0);
	glRotatef(rotx, 1, 0, 0);
	glColor3f(0.3, 0.3, 0.3);
	gluSphere(quadric, 11, 36, 18);
	glPopMatrix();

	glutSwapBuffers();  
}


void Reshape(int w, int h)
{

	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	globalH = h;
	globalW = w;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 1000.0); // fovy(1) – угол обзора; aspect(2) – отношение ширины окна картинной плоскости к его высоте; near(3) – расстояние от центра проецирования до ближней отсекающей плоскости; far(3) – расстояние от центра проецирования до задней плоскости отсечения.

}

void timer(int = 0)
{
	for (vector<Star>::iterator i = starlist.begin(); i != starlist.end(); ++i)
	{
		i->z += 0.05;
		if (i->z > 50)
		{
			i->x = 1.5f * (rand() % (2 * RANGE) - RANGE);
			i->y = 1.5f * (rand() % (2 * RANGE) - RANGE);
			i->z = -500;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(1, timer, 0);
}
void Idle()
{
	if (animate)
		rotx += 0.05;
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		MessageBox(NULL, L"Application was closed!", L"Exit app", MB_OK | MB_ICONEXCLAMATION);
		exit(0);
		break;
	case 'w': if (animate) worldX -= 1.0f;
		break;
	case 's': if (animate) worldX += 1.0f;
		break;
	case 'a': if (animate) worldY -= 1.0f;
		break;
	case 'd': if (animate)  worldY += 1.0f;
		break;
	case 'v': animate = !animate;;
		break;
	default: break;
	}
	glutPostRedisplay();

}

void init()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLoadIdentity();
	//glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(int argc, char* argv[])
{
	for (int i = 0; i < 1000; ++i)
	{
		Star star = { -(1.5f * (rand() % (2 * RANGE) - RANGE)),
				   1.5f * (rand() % (2 * RANGE) - RANGE),
				   1.0f * (rand() % 500 - 500) };
		starlist.push_back(star);
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 60);

	glutCreateWindow("Khasanov_9_Lab");
	init();
	glutDisplayFunc(draw);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Keyboard);
	timer();
	glutMainLoop();
	return 0;
}
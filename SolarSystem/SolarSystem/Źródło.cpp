#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
# define M_PI           3.14159265358979323846  /* pi */

using namespace std;

class Planet {
public:
	float radius, distance, orbit, orbitSpeed, axisTilt, axisAni;
	Planet(float _radius, float _distance, float _orbit, float _orbitSpeed, float _axisTilt, float _axisAni) {
		radius = _radius;
		distance = _distance;
		orbit = _orbit;
		orbitSpeed = _orbitSpeed;
		axisTilt = _axisTilt;
		axisAni = _axisAni;
	}
};

//Sun and Planets
Planet* sun = new Planet(5.0, 0, 0, 0, 0, 0);
Planet* mercury = new Planet(1.0, 7, 0, 4.74, 02.11, 0);
Planet* venus = new Planet(1.5, 11, 0, 3.50, 177.0, 0);
Planet* earth = new Planet(2.0, 16, 0, 2.98, 23.44, 0);
Planet* mars = new Planet(1.2, 21, 0, 2.41, 25.00, 0);
Planet* jupiter = new Planet(3.5, 28, 0, 1.31, 03.13, 0);
Planet* saturn = new Planet(3.0, 37, 0, 0.97, 26.70, 0);
Planet* uranus = new Planet(2.5, 45.5, 0, 0.68, 97.77, 0);
Planet* neptune = new Planet(2.3, 53.6, 0, 0.54, 28.32, 0);
Planet* pluto = new Planet(0.3, 59, 0, 0.47, 119.6, 0);

bool animating = false;
bool bigOrbitActive = true;
int changeCamera = 0;

static GLfloat viewer[] = { 30.0, 20.0, 20.0 };
static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;

static int delta_x = 0;        // r�nica pomi�dzy pozycj� bie��c� i poprzedni� kursora myszy
static int delta_y = 0;
static GLint status = 0;
static GLfloat theta = 0.50;
static GLfloat phi = 0.25;
static GLfloat delta_theta = 0.0;
static GLfloat delta_phi = 0.0;
static GLfloat pix2angle = 0.0;
static GLfloat radius = 50.0;

float lightPos[] = { 0.0, 0.0, -75.0, 1.0 }; // Spotlight position.
static float spotAngle = 40; // Spotlight cone half-angle.
float spotDirection[] = { 1.0, 0.0, 0.0 }; // Spotlight direction.
static float spotExponent = 1.0; // Spotlight exponent = attenuation factor.


void setup(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	//LIGHTING SETUP
	glEnable(GL_LIGHTING);
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float globAmb[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
}

void orbitalTrails(void) {
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.001, mercury->distance, 100.0, 100.0);
	glutWireTorus(0.001, venus->distance, 100.0, 100.0);
	glutWireTorus(0.001, earth->distance, 100.0, 100.0);
	glutWireTorus(0.001, mars->distance, 100.0, 100.0);
	glutWireTorus(0.001, jupiter->distance, 100.0, 100.0);
	glutWireTorus(0.001, saturn->distance, 100.0, 100.0);
	glutWireTorus(0.001, uranus->distance, 100.0, 100.0);
	glutWireTorus(0.001, neptune->distance, 100.0, 100.0);
	glutWireTorus(0.001, pluto->distance, 100.0, 100.0);
	glPopMatrix();
}

void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (status == 1) {
		delta_theta = delta_x * pix2angle;
		delta_phi = delta_y * pix2angle;

		theta += delta_theta * 0.01;
		phi += delta_phi * 0.01;
	}
	else if (status == 2) {
		radius += delta_y / 10.0;
		if (radius > 75.0) radius = 75.0;
		if (radius < 20.0) radius = 20.0;
	}

	if (theta > 2.0 * M_PI) theta -= 2.0 * M_PI;
	if (theta < 0.0) theta += 2.0 * M_PI;
	if (phi > 2.0 * M_PI) phi -= 2.0 * M_PI;
	if (phi < 0.0) phi += 2.0 * M_PI;

	viewer[0] = radius * cos(theta) * cos(phi);
	viewer[1] = radius * sin(phi);
	viewer[2] = radius * sin(theta) * cos(phi);
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	if (bigOrbitActive == 1) orbitalTrails();

	GLUquadric* quadric;
	quadric = gluNewQuadric();

	//Sun
	glPushMatrix();
	glRotatef(sun->orbit, 0.0, 1.0, 0.0);
	glTranslatef(sun->distance, 0.0, 0.0);
	glPushMatrix();
	glRotatef(sun->axisTilt, 1.0, 0.0, 0.0);
	glRotatef(sun->axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);*/
	gluSphere(quadric, sun->radius, 20.0, 20.0);
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Mercury
	glPushMatrix();
	glRotatef(mercury->orbit, 0.0, 1.0, 0.0);
	glTranslatef(mercury->distance, 0.0, 0.0);
	glPushMatrix();
	glRotatef(mercury->axisTilt, 1.0, 0.0, 0.0);
	glRotatef(mercury->axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, merTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);*/
	gluSphere(quadric, mercury->radius, 20.0, 20.0);
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Venus
	glPushMatrix();
	glRotatef(venus->orbit, 0.0, 1.0, 0.0);
	glTranslatef(venus->distance, 0.0, 0.0);
	glPushMatrix();
	glRotatef(venus->axisTilt, 1.0, 0.0, 0.0);
	glRotatef(venus->axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, venTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);*/
	gluSphere(quadric, venus->radius, 20.0, 20.0);
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Earth
	glPushMatrix();
	glRotatef(earth->orbit, 0.0, 1.0, 0.0);
	glTranslatef(earth->distance, 0.0, 0.0);
	glPushMatrix();
	glRotatef(earth->axisTilt, 1.0, 0.0, 0.0);
	glRotatef(earth->axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, earTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);*/
	gluSphere(quadric, earth->radius, 20.0, 20.0);
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Mars
	glPushMatrix();
	glRotatef(mars->orbit, 0.0, 1.0, 0.0);
	glTranslatef(mars->distance, 0.0, 0.0);
	glPushMatrix();
	glRotatef(mars->axisTilt, 1.0, 0.0, 0.0);
	glRotatef(mars->axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, marTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);*/
	gluSphere(quadric, mars->radius, 20.0, 20.0);
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Jupiter
	glPushMatrix();
	glRotatef(jupiter->orbit, 0.0, 1.0, 0.0);
	glTranslatef(jupiter->distance, 0.0, 0.0);
	glPushMatrix();
	glRotatef(jupiter->axisTilt, 1.0, 0.0, 0.0);
	glRotatef(jupiter->axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, jupTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);*/
	gluSphere(quadric, jupiter->radius, 20.0, 20.0);
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Saturn
	glPushMatrix();
	glRotatef(saturn->orbit, 0.0, 1.0, 0.0);
	glTranslatef(saturn->distance, 0.0, 0.0);
	glPushMatrix();
	glRotatef(saturn->axisTilt, 1.0, 0.0, 0.0);
	glRotatef(saturn->axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, satTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);*/
	gluSphere(quadric, saturn->radius, 20.0, 20.0);
	glPopMatrix();
	//glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor3ub(158, 145, 137);
	glRotatef(-63.0, 1.0, 0.0, 0.0);
	//glutWireTorus(0.2, 6.0, 30.0, 30.0);
	glutWireTorus(0.4, 5.0, 30.0, 30.0);
	glPopMatrix();
	glPopMatrix();

	glColor3ub(255, 255, 255);		//FIXES SHADING ISSUE

	//Uranus
	glPushMatrix();
	glRotatef(uranus->orbit, 0.0, 1.0, 0.0);
	glTranslatef(uranus->distance, 0.0, 0.0);
	glPushMatrix();
	glRotatef(uranus->axisTilt, 1.0, 0.0, 0.0);
	glRotatef(uranus->axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, uraTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);*/
	gluSphere(quadric, uranus->radius, 20.0, 20.0);
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Neptune
	glPushMatrix();
	glRotatef(neptune->orbit, 0.0, 1.0, 0.0);
	glTranslatef(neptune->distance, 0.0, 0.0);
	glPushMatrix();
	glRotatef(neptune->axisTilt, 1.0, 0.0, 0.0);
	glRotatef(neptune->axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, nepTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);*/
	gluSphere(quadric, neptune->radius, 20.0, 20.0);
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Pluto
	glPushMatrix();
	glRotatef(pluto->orbit, 0.0, 1.0, 0.0);
	glTranslatef(pluto->distance, 0.0, 0.0);
	glPushMatrix();
	glRotatef(pluto->axisTilt, 1.0, 0.0, 0.0);
	glRotatef(pluto->axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pluTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);*/
	gluSphere(quadric, pluto->radius, 20.0, 20.0);
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	/*glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, staTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_POLYGON);
	glTexCoord2f(-1.0, 0.0); glVertex3f(-200, -200, -100);
	glTexCoord2f(2.0, 0.0); glVertex3f(200, -200, -100);
	glTexCoord2f(2.0, 2.0); glVertex3f(200, 200, -100);
	glTexCoord2f(-1.0, 2.0); glVertex3f(-200, 200, -100);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, staTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-200, -83, 200);
	glTexCoord2f(8.0, 0.0); glVertex3f(200, -83, 200);
	glTexCoord2f(8.0, 8.0); glVertex3f(200, -83, -200);
	glTexCoord2f(0.0, 8.0); glVertex3f(-200, -83, -200);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();*/

	glutSwapBuffers();
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie

	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie��cej

	gluPerspective(80, 1.0, 1.0, 130.0);
	// Ustawienie parametr�w dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej
}

void animate(int n) {
	if (animating) {
		mercury->orbit += mercury->orbitSpeed;
		venus->orbit += venus->orbitSpeed;
		earth->orbit += earth->orbitSpeed;
		mars->orbit += mars->orbitSpeed;
		jupiter->orbit += jupiter->orbitSpeed;
		saturn->orbit += saturn->orbitSpeed;
		uranus->orbit += uranus->orbitSpeed;
		neptune->orbit += neptune->orbitSpeed;
		pluto->orbit += pluto->orbitSpeed;

		if (mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluto->orbit > 360.0) {
			mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluto->orbit -= 360.0;
		}
		mercury->axisAni += 10.0;
		venus->axisAni += 10.0;
		earth->axisAni += 10.0;
		mars->axisAni += 10.0;
		jupiter->axisAni += 10.0;
		saturn->axisAni += 10.0;
		uranus->axisAni += 10.0;
		neptune->axisAni += 10.0;
		pluto->axisAni += 10.0;
		if (mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluto->axisAni > 360.0) {
			mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluto->axisAni -= 360.0;
		}
		glutPostRedisplay();
		glutTimerFunc(30, animate, 1);
	}
}

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;
		y_pos_old = y;
		status = 1;
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		x_pos_old = x;
		y_pos_old = y;
		status = 2;
	}
	else
		status = 0;
}

void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
	delta_y = y - y_pos_old;
	x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie
	y_pos_old = y;
	glutPostRedisplay();     // przerysowanie obrazu sceny
}

void keyInput(unsigned char key, int x, int y) {
	switch (key) {
	case 27: exit(0); break;
	case 'a':
		animating = !animating;
		if (animating) animate(1);
		break;
	case 'o': bigOrbitActive = !bigOrbitActive; break;
	case '1': changeCamera = 0; break;
	case '2': changeCamera = 1; break;
	case '3': changeCamera = 2; break;
	}
	glutPostRedisplay();
}

void intructions(void) {
	cout << "a to play/pause the simulation." << endl;
	cout << "ESC to exit the simulation." << endl;
	cout << "o to show/hide Big Orbital Trails." << endl;
	cout << "1, 2 and 3 to change camera angles." << endl;
}

int main() {
	intructions();

	/*glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);*/

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Solar System");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(keyInput);

	glEnable(GL_DEPTH_TEST);
	setup();
	glutMainLoop();

	return 0;
}
//This is a recovered version of a school project I wrote in 2020
//Awful code with barely any understanding of C++ and opengl at the time but somehow I got it working
//Wanted to upload this just to have a record of things I had gotten done in the past.
//-Andrew Tischenko, 2022-05-07
//Going to make some alterations so the code actually works
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include<cmath>
using namespace std;


const int gridres = 40; //the number of rows and columns
double scale = 0.02; //Scale of grid cells in viewer, and meters (temporary)
double world_scale = 1; //unimplemented

double vert_max = 0.5; //Vertical scaling

int vert_rot = 0;
int horiz_rot = 0;
float radius = 1;
long double ep[gridres*gridres][gridres*gridres] = {0};
long double minimum = 0;
long double maximum= 0;
float znear = 0.01f;
float zfar = 1000000;
//Define point charge locations:
struct charge {
	int x;
	int y;
	float charge; //(in coulombs)
};

bool gridon = true;
bool axison = true;

const int points = 3;
charge arr[points];

void show_text(double x, double y, double z, string text){
	//void * font = GLUT_BITMAP_9_BY_15;
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(0.0003,0.0003,0.0003);
	glColor3f(1, 1, 1);
	//cout<<text<<endl;
	for(string::iterator i = text.begin(); i != text.end(); ++i)
	{
	    char c = *i;
	    //glutBitmapCharacter(font, c);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
	}
	glPopMatrix();
}
void cube(double x, double y, double z, double size)
{
	//creates a cube at the given position
	//cout<<x<<" "<<y<<" "<<z<<endl;
	glTranslatef(x, y, z);
	glutSolidCube(size);
	glTranslatef(-x, -y, -z);

}
void keyboard(unsigned char key, int x, int y)
{
	switch(key) {
		case 'a':{
			horiz_rot -= 1;
			y = sin(horiz_rot);
			x = cos(horiz_rot);
			break;
		}
		case 'd':{
			horiz_rot += 1;
			y = sin(horiz_rot);
			x = cos(horiz_rot);
			break;
		}

		case 'w':{
			vert_rot -= 1;
			y = sin(vert_rot);
			x = cos(vert_rot);
			break;
		}

		case 's':{
			vert_rot += 1;
			y = sin(vert_rot);
			x = cos(vert_rot);
			break;
		}


		case 'r':{
			radius += 0.05;
			x = cos(vert_rot);
			y = sin(vert_rot);
			break;
		}

		case 'f':{
			radius -= 0.05;
			x = cos(vert_rot);
			y = sin(vert_rot);
			break;
		}

		case 'g':{
			gridon = !gridon;
			break;
		}

		case 't':{
			axison = !axison;
			break;
		}
		default:{
			break;
		}
	}
	//cout<<radius<<endl;

	glutPostRedisplay();

}
void grid()
{
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	for (GLfloat i = -gridres*scale; i <= gridres*scale; i += scale) {
		glVertex3f(i, 0, gridres*scale); glVertex3f(i, 0, -gridres*scale);
		glVertex3f(gridres*scale, 0, i); glVertex3f(-gridres*scale, 0, i);
	}
	glEnd();
}
void axis()
{
	//Axis:
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3f(0, -vert_max, 0); glVertex3f(0, vert_max, 0);
	glVertex3f(0, 0, gridres*scale); glVertex3f(0, 0, -gridres*scale);
	glVertex3f(gridres*scale, 0, 0); glVertex3f(-gridres*scale, 0, 0);
	glEnd();
	show_text(0,0, gridres*scale, to_string(gridres*scale)+"m");
	show_text(gridres*scale,0, 0, to_string(gridres*scale)+"m");
}
void menu(int id)
{

}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//glTranslatef(0, 0, 0);
	glRotatef(vert_rot, 1,0,0);
	glRotatef(horiz_rot, 0, 1, 0);
	glScalef(radius, radius, radius);
	if (gridon == true) {
		grid();
	}
	if (axison == true) {
		axis();
	}

	for (int i = 0; i < points; i++) {
		glColor3f(0,0,0);
		if(arr[i].charge > 0){
			glColor3f(1,0,0);
		}
		if(arr[i].charge < 0){
			glColor3f(0,0,1);
		}
		cube(arr[i].x*scale, 0, arr[i].y*scale, 0.03);
	}

	for (int x = -gridres; x < gridres; x++){
		for (int z = -gridres; z < gridres; z++){
			double ep_val = ep[x+gridres][z+gridres];
			double col_ratio = ((ep_val - minimum) / (maximum - minimum));
			double ep_val_scaled = (vert_max+vert_max)*((ep_val-minimum)/(maximum-minimum)-vert_max);
			glColor3f(col_ratio, 0, 1-col_ratio);
			cube(x*scale, ep_val_scaled, z*scale, 0.01);
		}
	}

	show_text(0,vert_max,0,"Max: "+to_string(maximum)+" V");
	show_text(0,0,0, "0 V");
	show_text(0,-vert_max,0,"Min: "+to_string(minimum)+" V");

	// glutCreateMenu(menu);
	// glutAddMenuEntry("Option 1",1);
	// glutAddMenuEntry("Option 2",2);
	// glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutSwapBuffers();
}

void init()
{
	//glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35, 1.0f, znear, zfar);
	//glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DITHER);
	glClearColor(0, 0, 0, 1);

}


int main(int argc, char* argv[]) {
	double inp;
	//Set charges
	arr[0].x = 0;
	arr[0].y = 0;
	arr[0].charge = -0.005;

	arr[1].x = 10;
	arr[1].y = 10;
	arr[1].charge = -0.005;

	arr[2].x = -1;
	arr[2].y = -3;
	arr[2].charge = 0.005;

	//long double K = (8.99e9);
	for (int i = 0; i < points; i++) {
		for (int x = -gridres; x < gridres; x++) {
			for (int y = -gridres; y < gridres; y++) {
				long double distx = abs(x - arr[i].x) * scale;
				long double disty = abs(y - arr[i].y) * scale;
				long double hyp = distx*distx + disty*disty;
				hyp = sqrt(hyp);
				if(hyp!=0){
					ep[x+gridres][y+gridres] = ep[x+gridres][y+gridres] + ((arr[i].charge) / hyp);
				}
			}
		}
	}
	//minimum = ep[0][0];
	//maximum= ep[0][0];
	for (int x = -gridres; x < gridres; x++) {
		for (int y = -gridres; y < gridres; y++) {
			if (ep[x+gridres][y+gridres] < minimum) { minimum = ep[x+gridres][y+gridres]; }
			if (ep[x+gridres][y+gridres] > maximum) { maximum = ep[x+gridres][y+gridres]; }
			//Calculate min and max
			//cout<<minimum<<endl;
			//cout<<maximum<<endl;
		}
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Electric Potential Visualizer/Calculator: Made by Andrew Tischenko 2020");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

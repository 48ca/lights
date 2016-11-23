#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

void keyfunc(unsigned char key, int xscr, int yscr);
void displayfunc();
void reshapefunc(int wscr,int hscr);

unsigned int win_width = 1200;
unsigned int win_height = 900;
double aspect = 12/9;

double* coeffs;
double* powers;
double* thetas;
int* colors;

#define COLOR_CHOICES 1

#define NUM_LIGHTS 40

int num_lights = NUM_LIGHTS;

double theta_offset = 0;
double turn_increment = M_PI/360;

void generateFunctions() {
    register int i;

    register double appension;
    register double temp_theta = 0;
    double theta_inc = M_PI * 2 / num_lights;

    coeffs = (double*)malloc(sizeof(double) * num_lights);
    powers = (double*)malloc(sizeof(double) * num_lights);
    thetas = (double*)malloc(sizeof(double) * num_lights);
    colors = (int*)malloc(sizeof(int) * num_lights);

    for(i=0;i<num_lights;++i) {
        coeffs[i] = -1 + (double)(rand())/RAND_MAX * (-3);
        powers[i] = 2 + (double)(rand())/RAND_MAX * (2);
        thetas[i] = theta_inc * i;
        colors[i] = rand() % COLOR_CHOICES;
    }
};

void drawLights() {
    register int i;

    register int x;
    register int y;

    register int xi;
    for(i=0;i<num_lights;++i) {
        for(xi=0;xi<win_width;++xi) {
            x = win_width/2.0 + xi/2.0 *  cos(thetas[i] + theta_offset);
            y = win_height/2.0 * coeffs[i] * pow((double)(xi - win_width/4)/win_width, powers[i]) + win_height/4;
            if(y >= 0 && y < win_height) {
                glBegin(GL_POINTS);
                glColor3f(255, 255, 255); // black
                glVertex2f(x,y);
                glEnd();
            }
        }
    }
}

void keyfunc(unsigned char key, int xscr, int yscr) {
    switch(key) {
        case 'q':
            printf("Exiting\n");
            exit(0);
            break;
    }
}

void displayfunc() {
    glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_POLYGON);
    glColor3f(0,0,0);
    glVertex2f(0,0);
    glVertex2f(win_width, 0);
    glVertex2f(0, win_height);
    glVertex2f(win_width, win_height);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(win_width, win_height);
    glVertex2f(0, win_height);
    glVertex2f(win_width, 0);
    glVertex2f(0,0);
    glEnd();

    drawLights();

    theta_offset += turn_increment;

    glutSwapBuffers();

    glutPostRedisplay();
};

void reshapefunc(int wscr,int hscr)
{
    win_width = wscr;
    win_height = hscr;
    aspect = (double)(win_width) / win_height;
    glViewport(0,0,(GLsizei)win_width,(GLsizei)win_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,1.0*win_width,0.0,1.0*win_height);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(win_width,win_height);
    glutInitWindowPosition(100,100);
    glutCreateWindow("");
    glClearColor(1.0,1.0,1.0,0.0);
    glShadeModel(GL_SMOOTH);

    srand(time(NULL));

    generateFunctions();

    glutReshapeFunc(reshapefunc);
    glutDisplayFunc(displayfunc);
    glutKeyboardFunc(keyfunc);
    glutMainLoop();

    return 0;
}

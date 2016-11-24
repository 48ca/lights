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


#define LIGHT_SIZE 20

#define COLOR_CHOICES 3

double color_presets[][3] = {
    {255, 100, 234},
    {0, 255, 100},
    {0, 100, 255}
};

#define NUM_LIGHTS 100

int num_lights = NUM_LIGHTS;

double theta_offset = 0;
double turn_increment = M_PI/720;

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
        coeffs[i] = -20 + (double)(rand())/RAND_MAX * (-4);
        powers[i] = 4 + (double)(rand())/RAND_MAX * (.5);
        thetas[i] = theta_inc * i;
        colors[i] = rand() % COLOR_CHOICES;
    }
};

void drawLights() {
    register int i;

    register int x;
    register int y;

    double grey;

    register int xi;

    int max_xi = win_width;
    double xi_scalar = 1.0 / (2 * win_width);

    glBegin(GL_POINTS);
    for(i=0;i<num_lights;++i) {
        for(xi=0;xi<max_xi;++xi) {
            x = win_width/2.0 + win_width * xi_scalar * xi * cos(thetas[i] + theta_offset);
            y = win_height/2.0 * coeffs[i] * pow((double)(xi)*xi_scalar, powers[i]) + win_height;
            if(y < 0 || y >= win_height || x < 0 || x >= win_width) continue;
            if(xi > max_xi - LIGHT_SIZE) {
                glColor3f(
                    color_presets[colors[i]][0]/255.0,
                    color_presets[colors[i]][1]/255.0,
                    color_presets[colors[i]][2]/255.0
                );
            } else {
                grey = (150 + 50 * sin(thetas[i] + theta_offset))/255.0;
                glColor3f(grey, grey, grey);
            }
            glVertex2f(x, y);
        }
    }
    glEnd();
}

void keyfunc(unsigned char key, int xscr, int yscr) {
    switch(key) {
        case 'q':
            exit(0);
            break;
    }
}

void displayfunc() {
    glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT);

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
    glutCreateWindow("lights");
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_SMOOTH);

    srand(time(NULL));

    generateFunctions();

    glutReshapeFunc(reshapefunc);
    glutDisplayFunc(displayfunc);
    glutKeyboardFunc(keyfunc);
    glutMainLoop();

    return 0;
}

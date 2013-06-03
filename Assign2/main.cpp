#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//#include <GLUT/glut.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

using namespace std;

GLfloat vertices[][3] = {{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
    {1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0,1.0}, 
    {1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0}};


GLfloat colors[][3] = {{0.0,0.0,0.0},{1.0,0.0,0.0},
    {1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}, 
    {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}};

static GLfloat theta1 = 0.0;  //stores two angles
GLfloat y_viewpoint=10;

void polygon(int a, int b, int c , int d) {
    /* draw a polygon via list of vertices */
    
    glBegin(GL_POLYGON);
    glColor3fv(colors[a]);
    glVertex3fv(vertices[a]);
    glColor3fv(colors[b]);
    glVertex3fv(vertices[b]);
    glColor3fv(colors[c]);
    glVertex3fv(vertices[c]);
    glColor3fv(colors[d]);
    glVertex3fv(vertices[d]);
    glEnd();
}

void colorcube(void) {
    /* map vertices to faces */
    polygon(0,3,2,1);
    polygon(2,3,7,6);
    polygon(0,4,7,3);
    polygon(1,2,6,5);
    polygon(4,5,6,7);
    polygon(0,1,5,4);
}

void drawroof(void) {
    glBegin(GL_POLYGON);
    glVertex3f(0,7,0);  //this vertex is on the y axis at height 7
    glVertex3f(0,4,9);  //this is on the positive z axis at a radius of 9 and height of 4
    glVertex3f(7.794, 4, 4.5);  // this is where the next postis, height of 4
    //note that 7.794 is 9.0 * sin (60.0)
    glEnd();
}

void drawfloor(void) {
    glBegin(GL_POLYGON);
    glVertex3f(0,-4,0);  //this vertex is on the y axis at height -4
    glVertex3f(0,-4,9);  //this is on the positive z axis at a radius of 9 and height of -4
    glVertex3f(7.794, -4, 4.5);  // this is where the next postis, height of -4
    //note that 7.794 is 9.0 * sin (60.0)
    glEnd();    
}

void myReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
	//Uncomment the desired projection function
    gluPerspective(80.0, 1.0,10.0,40.0);
    //glOrtho(-4,4,-4,4,-4,4);
	//glFrustum(-1,1,-1,1,1,30);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color buffer and z buffer
    glMatrixMode(GL_MODELVIEW);
    
    //gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz);
    glLoadIdentity();  // start with an identity in the MODELVIEW transformation
    gluLookAt(0.0,y_viewpoint,20.0, 0.0,0.0,0.0,0,1,0);  //multiply viewing component of transformation
    
    for (int i=0; i<6; i++) {
        
        //Floors
        glPushMatrix();
        glRotated(theta1+i*60,0,1,0);
        //glRotated(90,0,0,1);
        glColor3f(.36,.2,.09);
        //glColor3f(1,1,1);
        drawfloor();
        glPopMatrix();
        
        //Cube Draw
        glPushMatrix();
        glRotated(theta1+i*60,0,1,0);
        glTranslatef(0,0,8);
        glScalef(.5,4,.5);
        colorcube();
        glPopMatrix();
         
        //Roof Draw.
        glPushMatrix();
        glRotated(theta1+i*60,0,1,0);

        if((i%2)==0)
            glColor3f(0,0,1);
        else
            glColor3f(1,1,0);
        drawroof();
        glPopMatrix();
    }
    
    //Drawing Flag Post.
    glPushMatrix();
    glTranslatef(0,9,0);
    glScalef(.15,2,.15);
    colorcube();
    glPopMatrix();
    
    //Drawing Flag.
    
    glPushMatrix();
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    glVertex3f(0,9,0);  //Since the post is translated by 9, its mid-point is at y=9.
    glVertex3f(0,11,0); //Since the post is scaled by 2(2*initial height), y=11 represents top of post.
    glVertex3f(2,10,0);  // Mid point of Flag edge on post.
    glEnd();    
    glPopMatrix();
    
    glFlush();
    glutSwapBuffers();
}

void spinCube()
{
    
    //idle callback.  Angles are incremented by varying amounts each time it is called.
    theta1 += 0.05; // slow rotation
    if( theta1 > 360.0 ) theta1 -= 360.0;
    glutPostRedisplay();  // generate a display event
}

void main_menu_callback(int selection) {
	if (selection == 0) y_viewpoint=1;  // exit the program if exit is selected
    else if (selection == 1) y_viewpoint=10; //scale the size of global size multiplier
    else if (selection==2) exit(0);
    glutPostRedisplay();  // generate a display event
}

void create_menu() {
    //create the main menu
	glutCreateMenu(main_menu_callback);
	//attach the color and shape menus to the main menu as submenus
    glutAddMenuEntry("Front View",0);  //this is not a submenu -> calls callback
	glutAddMenuEntry("Top View", 1);
    glutAddMenuEntry("Exit", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);  //attach menu to right button
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    
    /* need both double buffering and z buffer */
    
    glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowSize(700, 700);
    glutCreateWindow("My Rotating Cube Example");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutIdleFunc(spinCube);
    create_menu();
    
    //Note sure what is this used for.
    //glEnable(GL_DEPTH_TEST); // Enable hidden surface removal
    glutMainLoop();
    return 0;
}
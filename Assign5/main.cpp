#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <cmath>

static GLfloat theta1 = 0.0;  //stores two angles
GLfloat theta1_step=1;
//GLfloat view_point_theta=0;


using namespace std;

GLfloat y_cube_scale=4;
GLint no_of_cycles = 2;
GLint no_of_carousel_pairs = 4;

GLfloat divisor = 1;
GLfloat move=0;

#define PI 3.14
GLfloat theta_win1 = 0.0, phi_win1 = 0.0;  //stores two angles
GLfloat theta_win2 = 0.0, phi_win2 = 0.0;  //stores two angles
GLfloat radius = 1.0;
GLfloat xo_win1 = 0.0;
GLfloat yo_win1 = 0.0;
//GLfloat zo = 10.0; // initial viewer position
GLfloat zo_win1=0;
GLfloat x_win1,y_win1,z_win1; //point on viewing sphere

GLfloat xo_win2 = 10.0;
GLfloat yo_win2 = 0.0;
GLfloat zo_win2=-20;
GLfloat x_win2,y_win2,z_win2; //point on viewing sphere

GLdouble point[3];
GLdouble Znear=3;
GLdouble Zfar=200;
GLdouble YGround = -2.5;
GLdouble sun_theta=0;
GLdouble sun_radius=25;
GLfloat x_sun,y_sun,z_sun=-20;

int sun_light = 1;
int spot_light = 1;
int car_spin =1;
int fog_choice=1;
int jump_car=0;
int jump_horse=0;

int window1, window2;  // identifiers for windows

void colorcube(void) {
    // map vertices to faces //
    
    GLfloat vertices[][3]={
        {1,1,1},{-1,1,1},{-1,-1,1},{1,-1,1}, //FRONT
        {1,1,-1},{-1,1,-1},{-1,-1,-1},{1,-1,-1}, //BACK
        {1,1,1},{1,1,-1},{-1,1,-1},{-1,1,1}, //UP
        {1,-1,1},{-1,-1,1},{-1,-1,-1},{1,-1,-1}, //DOWN
        {1,-1,1},{1,-1,-1},{1,1,-1},{1,1,1}, //RIGHT
        {-1,1,1},{-1,1,-1},{-1,-1,-1},{-1,-1,1} //LEFT
    };
    
    GLfloat normals[][3]={{0,0,1},{0,0,1},{0,0,1},{0,0,1}, //FRONT
        {0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1}, //BACK
        {0,1,0},{0,1,1},{0,1,0},{0,1,0}, //UP
        {0,-1,0},{0,-1,0},{0,-1,0},{0,-1,0}, //DOWN
        {1,0,0},{1,0,0},{1,0,0},{1,0,0}, //RIGHT
        {-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0} //LEFT
    };
    
    glNormalPointer(GL_FLOAT,0, normals);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_QUADS, 0, 24);
}

void drawRealHorse(void) {
    
    GLfloat horse_vertices[][3]={{0,1,0},{2.5,2.75,0},{3,3,0},{3,2.5,0},{3.5,2,0},{5.5,1,0},{5,0,0},
        {3,1,0},{2,-1,0},{2,-4,0},{1.6,-4,0},{1.6,-1,0},{-1.6,-1,0},{-1.6,-4,0},{-2,-4,0},{-2,-1,0},
        {-2.5,-1.5,0},{-2.75,-1.5,0},{-2.75,-2,0},{-3,-2,0},{-2.8,1.5,0},{-2.5,1.5,0}};
    
    GLfloat horse_normal[][3] = {{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1}};
    
    glNormalPointer(GL_FLOAT,0, horse_normal);
    glVertexPointer(3, GL_FLOAT, 0, horse_vertices);
    glDrawArrays(GL_POLYGON, 0, 22);
    
}

void drawroof(void) {
    GLfloat roof_vertices[][3]={{0,7,0},{0,4,9},{7.794,4,4.5}};
    GLfloat roof_normal[][3] = {{0.34,0.89,0.298},{0.34,0.9,0.298},{0.34,0.89,0.298}};
    
    glNormalPointer(GL_FLOAT,0, roof_normal);
    glVertexPointer(3, GL_FLOAT, 0, roof_vertices);
    glDrawArrays(GL_POLYGON, 0, 3);
}

void drawfloor(void) {
    
    glBindTexture(GL_TEXTURE_2D, 2); //TM
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);  // HM added this
    
    GLfloat floor_vertices[][3]={{0,-4,0},{0,-4,9},{7.794,-4,4.5}};
    GLfloat floor_normal[][3] = {{0,1,0},{0,1,0},{0,1,0}};
    static const GLfloat floor_texture_coord[][2] = {{0,0},{0,1},{1,1}};
    
    glNormalPointer(GL_FLOAT,0, floor_normal);
    glVertexPointer(3, GL_FLOAT, 0, floor_vertices);
    
    glTexCoordPointer(2, GL_FLOAT, 0, floor_texture_coord);
    glDrawArrays(GL_POLYGON, 0, 3);
    glBindTexture(GL_TEXTURE_2D, 0); //TM
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);  //HM added this
    
}

void window1_myReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
	//Uncomment the desired projection function
    gluPerspective(50.0, 2.0,Znear,Zfar);
    glMatrixMode(GL_MODELVIEW);
}

void window2_myReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
	//Uncomment the desired projection function
    gluPerspective(50.0, 2.0,Znear,Zfar);
    glMatrixMode(GL_MODELVIEW);
}

void drawCarausel(void) {
    for (int i=0; i<6; i++) {
        //Cube Draw
        glPushMatrix();
        glRotated(theta1+i*60,0,1,0);
        glTranslatef(0,0,8);
        glScalef(.5,y_cube_scale,.5);
        GLfloat cube_mat_specular[]={0.5, 0.5, 0.5, 1.0};
        GLfloat cube_mat_diffuse[]={0.5, 0.5, 0.5, 1.0};
        GLfloat cube_mat_ambient[]={0.0, 0.0, 0.0, 0.0};
        GLfloat cube_mat_shininess={50.0};
        
        glMaterialfv(GL_FRONT, GL_SPECULAR, cube_mat_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, cube_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_mat_diffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, cube_mat_shininess);
        colorcube();
        glPopMatrix();
        
        
        //Real Horse
        glPushMatrix();
        glRotated(theta1+i*60,0,1,0);
        //cout<<"Horse Rotation = "<<theta1<<endl;
        glTranslatef(0,0,8);
        glTranslated(0, move,0); 
        GLfloat horse_mat_specular[]={1.0, 1.0, 1.0, 1.0};
        GLfloat horse_mat_diffuse[]={1.0, 0.0, 0.0, 1.0};
        GLfloat horse_mat_ambient[]={0.0, 0.0, 0.0, 0.0};
        GLfloat horse_mat_shininess={90.0};
        
        glMaterialfv(GL_FRONT, GL_SPECULAR, horse_mat_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, horse_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, horse_mat_diffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, horse_mat_shininess);
        drawRealHorse();
        glPopMatrix();
        
        //Roof Draw.
        glPushMatrix();
        glRotated(theta1+i*60,0,1,0);
        if((i%2)==0) {
            GLfloat roof_mat_specular[]={0.0, 0.0, 1.0, 1.0};
            GLfloat roof_mat_diffuse[]={0.0, 0.0, 1.0, 1.0};
            GLfloat roof_mat_ambient[]={0.0, 0.0, 0.0, 0.0};
            GLfloat roof_mat_shininess={0.0};
            
            glMaterialfv(GL_FRONT, GL_SPECULAR, roof_mat_specular);
            glMaterialfv(GL_FRONT, GL_AMBIENT, roof_mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, roof_mat_diffuse);
            glMaterialf(GL_FRONT, GL_SHININESS, roof_mat_shininess);
            
        }
        else {
            GLfloat roof_mat_specular[]={1.0,1.0, 0.0, 1.0};
            GLfloat roof_mat_diffuse[]={1.0,1.0, 0.0, 1.0};
            GLfloat roof_mat_ambient[]={0.0,0.0, 0.0, 0.0};
            GLfloat roof_mat_shininess={0.0};
            
            glMaterialfv(GL_FRONT, GL_SPECULAR, roof_mat_specular);
            glMaterialfv(GL_FRONT, GL_AMBIENT, roof_mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, roof_mat_diffuse);
            glMaterialf(GL_FRONT, GL_SHININESS, roof_mat_shininess);
            
        }
        drawroof();
        glPopMatrix();
        
        
        //Floors
        glPushMatrix();
        glRotated(theta1+i*60,0,1,0);
        GLfloat floor_mat_specular[]={1.0, 1.0, 1.0, 1.0};
        GLfloat floor_mat_diffuse[]={1.0, 1.0, 1.0, 1.0};
        GLfloat floor_mat_ambient[]={0.0, 0.0, 0.0, 0.0};
        GLfloat floor_mat_shininess={0.0};
        
        glMaterialfv(GL_FRONT, GL_SPECULAR, floor_mat_specular);
        glMaterialfv(GL_FRONT, GL_AMBIENT, floor_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, floor_mat_diffuse);
        glMaterialf(GL_FRONT, GL_SHININESS, floor_mat_shininess);
        
        drawfloor();
        glPopMatrix();
        
    }
    
    //Drawing Flag Post.
    glPushMatrix();
    GLfloat cube_mat_specular[]={0.5, 0.5, 0.5, 1.0};
    GLfloat cube_mat_diffuse[]={0.5, 0.5, 0.5, 1.0};
    GLfloat cube_mat_ambient[]={0.0, 0.0, 0.0, 0.0};
    GLfloat cube_mat_shininess={50.0};
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, cube_mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, cube_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, cube_mat_shininess);
    glTranslatef(0,9,0);
    glScalef(.15,2,.15);
    colorcube();
    glPopMatrix();
    
    //Drawing Flag.
    glPushMatrix();
    GLfloat roof_mat_specular[]={1.0,0.0, 0.0, 1.0};
    GLfloat roof_mat_diffuse[]={1.0,0.0, 0.0, 1.0};
    GLfloat roof_mat_ambient[]={0.0,0.0, 0.0, 0.0};
    GLfloat roof_mat_shininess={0.0};
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, roof_mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, roof_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, roof_mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, roof_mat_shininess);
    
    
    
    GLfloat flag_vertices[][3]={{0,9,0},{0,11,0},{2,10,0}};
    GLfloat flag_normal[][3] = {{0,1,1},{0,1,1},{0,1,1}};
    
    glNormalPointer(GL_FLOAT,0,flag_normal);
    glVertexPointer(3, GL_FLOAT, 0, flag_vertices);
    glDrawArrays(GL_POLYGON, 0, 3);
    glPopMatrix();
    
}

void drawGround(void) {
    for (GLdouble z = 0;z>=-100;z=z-2) {
        for(GLdouble x=-60;x<=60;x=x+2) {
            glPushMatrix();
            
            GLfloat ground_mat_specular[]={1.0, 1.0, 1.0, 1.0};
            GLfloat ground_mat_diffuse[]={1.0, 1.0, 1.0, 1.0};
            GLfloat ground_mat_ambient[]={0.0, 0.0, 0.0, 0.0};
            GLfloat ground_mat_shininess={0.0};
            
            glMaterialfv(GL_FRONT, GL_SPECULAR, ground_mat_specular);
            glMaterialfv(GL_FRONT, GL_AMBIENT, ground_mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, ground_mat_diffuse);
            glMaterialf(GL_FRONT, GL_SHININESS, ground_mat_shininess);
            
            
            glTranslated(x, YGround, z);
            
            glBindTexture(GL_TEXTURE_2D, 1); //TM
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);  // HM added this
            GLfloat ground_vertices[][3]={{1,YGround,-1},{1,YGround,1},{-1,YGround,1},{-1,YGround,-1}};
            GLfloat ground_normal[][3] = {{0,1,0},{0,1,0},{0,1,0},{0,1,0}};
            static const GLfloat ground_texture_coord[][2] = {{0,0},{0,1},{1,1},{1,0}};
            
            glNormalPointer(GL_FLOAT,0, ground_normal);
            glVertexPointer(3, GL_FLOAT, 0, ground_vertices);
            
            glTexCoordPointer(2, GL_FLOAT, 0, ground_texture_coord);
            glDrawArrays(GL_POLYGON, 0, 4);
            glBindTexture(GL_TEXTURE_2D, 0); //TM
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);  //HM added this
            glPopMatrix();
            //
        }
        
    }
    glBindTexture(GL_TEXTURE_2D, 0); //TM
}

void drawSun(void) {
    x_sun=sun_radius*cos(sun_theta/180);
    y_sun=sun_radius*sin(sun_theta/180);
    
    glTranslatef(x_sun, y_sun, z_sun);
    GLfloat roof_mat_specular[]={1.0, 1.0, 1.0, 1.0};
    GLfloat roof_mat_diffuse[]={1.0, 0.0, 0.0, 1.0};
    GLfloat roof_mat_ambient[]={1.0, 0.55, 0.0, 0.0};
    GLfloat roof_mat_shininess={90.0};
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, roof_mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, roof_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, roof_mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, roof_mat_shininess);
    glNormal3f(0, 0, 1);
    glBegin(GL_POLYGON);
    glVertex3f(1, 1, 0);
    glVertex3f(1, -1, 0);
    glVertex3f(-1, -1, 0);
    glVertex3f(-1, 1, 0);
    glEnd();
}

void create_scene(void) {
    
    glPushMatrix();
    //glTranslated(10, 0, -20);
    if(jump_horse==1) glTranslated(0, move,0);
    glTranslated(xo_win1, yo_win1, zo_win1);
    glutWireSphere(1,10,10);
    glPopMatrix();

    glPushMatrix();
    glTranslated(xo_win2,0, zo_win2);
    glutSolidSphere(1,10,10);
    glPopMatrix();
    

    
    //Draw Caraousel
    for(int j=1;j<=no_of_carousel_pairs;j++) 
    {
        glPushMatrix();
        glTranslated(20, 0, j*-20);
        drawCarausel();
        glPopMatrix();
        
        
        glPushMatrix();        
        glTranslated(-20, 0, j*-20);
        drawCarausel();
        glPopMatrix();
        
    }
    
    //Draw Ground for Grass Texture.
    drawGround();
    
    //Draw Sun
    glPushMatrix();
    drawSun();
    glPopMatrix();
    
}

void init_lighting_conditions() {
    GLfloat light_ambient[]={1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0}; 
    GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[]={0,0,0,1.0};
    GLfloat light_direction[] = {0,0,-1};
    
    // set up ambient, diffuse, and specular components for light 0
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10.0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
    glShadeModel(GL_SMOOTH); //enable smooth shading  
}


void sun_lighting_conditions() { // This is where you can experiment with lighting setup)    
    
    GLfloat light_ambient[]={1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0}; 
    GLfloat light_position[]={x_sun,y_sun, z_sun, 1.0} ;
    
    // set up ambient, diffuse, and specular components for light 0
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glShadeModel(GL_SMOOTH); //enable smooth shading
}

void window1_navigation(int key,int x,int y) {
    switch (key) {
        case GLUT_KEY_UP: {
            if (theta_win1 < (PI/2 - 0.1)) theta_win1 += 0.1;
            glutPostRedisplay();
            return; }
            break;
        case GLUT_KEY_DOWN:
        {
            if (theta_win1 > (-PI/2 + 0.1)) theta_win1 -= 0.1;
            glutPostRedisplay();
            return;
        }
            break;
        case GLUT_KEY_RIGHT :
        {
            phi_win1 -= 0.1;  //radians
            glutPostRedisplay();
            return;
        }
            break;
        case GLUT_KEY_LEFT:
        {
            phi_win1 += 0.1;  //radians
            glutPostRedisplay();
            return;
        }
            break;
        default:
            break;
    }
    glutPostRedisplay();
}


void window2_navigation(int key,int x,int y) {
    switch (key) {
        case GLUT_KEY_UP: {
            if (theta_win2 < (PI/2 - 0.1)) theta_win2 += 0.1;
            glutPostRedisplay();
            return; }
            break;
        case GLUT_KEY_DOWN:
        {
            if (theta_win2 > (-PI/2 + 0.1)) theta_win2 -= 0.1;
            glutPostRedisplay();
            return;
        }
            break;
        case GLUT_KEY_RIGHT :
        {
            phi_win2 -= 0.1;  //radians
            glutPostRedisplay();
            return;
        }
            break;
        case GLUT_KEY_LEFT:
        {
            phi_win2 += 0.1;  //radians
            glutPostRedisplay();
            return;
        }
            break;
        default:
            break;
    }
    glutPostRedisplay();
}


void window1_keyboard(unsigned char key,int x_cord,int y_cord) {
    switch (key) {
        case 'f':
        {
            xo_win1+=+x_win1;
            zo_win1+=z_win1;
            glutPostRedisplay();
            return;
        }
            break;
        case 'b':
        {
            xo_win1-=x_win1;
            zo_win1-=z_win1;
            glutPostRedisplay();
            return;
        }
            break;
        default:
            break;
    }
}


void window2_keyboard(unsigned char key,int x_cord,int y_cord) {
    switch (key) {
        case 'f':
        {
            xo_win2+=x_win2;
            zo_win2+=z_win2;
            glutPostRedisplay();
            return;
        }
            break;
        case 'b':
        {
            xo_win2-=x_win2;
            zo_win2-=z_win2;
            glutPostRedisplay();
            return;
        }
            break;
        default:
            break;
    }
}

int touch=-1;

void spinCube()
{
    if(theta1_step!=0) divisor = theta1_step;
    GLfloat step_move=(no_of_cycles*4*y_cube_scale)/(360/divisor);
    
    //idle callback.  Angles are incremented by varying amounts each time it is called.
    
    if(car_spin==1) {
        theta1 += theta1_step; //.05; // slow rotation
        if( theta1 > 360.0 ) theta1 = 0;
        
        sun_theta += 1;
        if(sun_theta>360) sun_theta -= 360;
        
        if(move >=y_cube_scale) touch=1;
        else if(move <=-y_cube_scale) touch=-1;
        
        if(touch==1) move -=step_move;
        if(touch==-1) move +=step_move;
    }

    glutSetWindow(window1);
	glutPostRedisplay();  // generate a display event for window1
    
	glutSetWindow(window2);
	glutPostRedisplay();  // generate a display event for window2
    
}

void main_menu_callback(int selection) {
	if (selection == 0) sun_light=-sun_light;  // Turn the Sun Light On/Off. 
    else if (selection == 1) spot_light=-spot_light; //Turn the Spot Light On/off.
    else if(selection ==2) car_spin = -car_spin; //Turn the Caraousel Spin On/Off;
    else if(selection==3) jump_car=1;
    else if(selection==4) jump_horse=1;
    else if(selection==5) fog_choice=-fog_choice;
    else if (selection==6) exit(0);
    glutPostRedisplay();  // generate a display event
}

void create_menu() {
    //create the main menu
	glutCreateMenu(main_menu_callback);
	//attach the color and shape menus to the main menu as submenus
    glutAddMenuEntry("Sun On/Off",0);  //this is not a submenu -> calls callback
	glutAddMenuEntry("Spot Light On/off", 1);
    glutAddMenuEntry("Carausel Spin On/Off", 2);
    glutAddMenuEntry("Get on Carousel", 3);
    glutAddMenuEntry("Get on Horse", 4);
    glutAddMenuEntry("Fog On/Off", 5);
    glutAddMenuEntry("Exit", 6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);  //attach menu to right button
}

void window1_display() {    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color buffer and z buffer
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //TM
    
    glMatrixMode(GL_MODELVIEW);
      // start with an identity in the MODELVIEW transformation
    glLoadIdentity();
    //multiply viewing component of transformation
    if(jump_car==1) {
        if(jump_horse==1) glTranslated(0, -move,0);
        xo_win1=-20 + 8*sin(-(theta1*PI)/180);
        zo_win1=-20 - 8*cos(-(theta1*PI)/180);
        phi_win1=(theta1*PI)/180;
    }
    
    x_win1 = -(radius * cos (theta_win1) * cos (phi_win1));
	y_win1 = radius * sin (theta_win1);
	z_win1 = radius * cos (theta_win1)* sin(phi_win1);

    init_lighting_conditions();  //set up lighting
    
    gluLookAt(xo_win1,yo_win1,zo_win1,xo_win1+x_win1,yo_win1+y_win1,zo_win1+z_win1,0,1,0)  ;

    sun_lighting_conditions();
    
    create_scene();
    
    if(sun_light==1)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);
    
    if(spot_light==1)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);
    
    
    //Enabling/Disabling the Fog.
    if(fog_choice==1) 
        glEnable(GL_FOG);
    
    else
        glDisable(GL_FOG);
    
    glFlush();
    glutSwapBuffers();
    
}

void window2_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color buffer and z buffer
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //TM
    glMatrixMode(GL_MODELVIEW);

    
    glLoadIdentity();  // start with an identity in the MODELVIEW transformation
    
    x_win2 = -(radius * cos (theta_win2) * cos (phi_win2));
	y_win2 = radius * sin (theta_win2);
	z_win2 = radius * cos (theta_win2)* sin(phi_win2);
    
    
    init_lighting_conditions();  //set up lightin
    gluLookAt(xo_win2,yo_win2,zo_win2,xo_win2+x_win2,yo_win2+y_win2,zo_win2+z_win2,0,1,0)  ;
    sun_lighting_conditions();

    create_scene();
    
    if(sun_light==1)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);
    
    if(spot_light==1)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);
    
    
    //Enabling/Disabling the Fog.
    if(fog_choice==1) 
        glEnable(GL_FOG);
    
    else
        glDisable(GL_FOG);
    
    glFlush();
    glutSwapBuffers();
}

int LoadTextureFromPPM(const char *filename, int id)
{
	/* Load the image */
	FILE* fp;
	int i, w, h, d;
	unsigned char* image;
	char head[70]; /* max line <= 70 in PPM (per spec). */
    
	fp = fopen(filename, "rb");
	if (!fp)    {
		perror(filename);
		return -1;
	}
    
	/* grab first two chars of the file and make sure that it has the
	 correct magic cookie for a raw PPM file. */
	fgets(head, 70, fp);
	if (strncmp(head, "P6", 2)) {
		fprintf(stderr, "%s is not a raw PPM file\n", filename);
		return -1;
	}
    
	/* grab the three elements in the header (width, height, maxval). */
	i = 0;
	while (i < 3) {
		fgets(head, 70, fp);
		if (head[0] == '#') {
			/* skip comments. */
			continue;
		}
		switch (i)  {
			case 0:
				i += sscanf(head, "%d %d %d", &w, &h, &d);
				break;
			case 1:
				i += sscanf(head, "%d %d", &h, &d);
				break;
			case 2:
				i += sscanf(head, "%d", &d);
				break;
		}
	}
    
	/* grab all the image data in one fell swoop. */
	image = (unsigned char*) malloc(sizeof(unsigned char) * w * h * 3);
	fread(image, sizeof(unsigned char), w * h * 3, fp);
	fclose(fp);
    
	/* Bind */
    
	glBindTexture(GL_TEXTURE_2D, id);
    
	/* Set parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
	/* Build mipmaps */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, w, h, GL_RGB, GL_UNSIGNED_BYTE,
                      image);
    
	free(image);
	return 0;
}

static GLint fogMode;
float density = 0.025;


void init() {
    
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST); // Enable hidden surface removal
    
    glShadeModel(GL_FLAT);
    glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
    
    LoadTextureFromPPM("/MS_CS_UML/Computer_Graphics_I/Assign3/Prof_Texturing/grass.ppm", 1);
    LoadTextureFromPPM("/MS_CS_UML/Computer_Graphics_I/Assign3/Prof_Texturing/wood.ppm", 2);
    
    //  set up the global method of applying texture
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
    
    // get the version information
	cout<<"Version: "<<glGetString(GL_VERSION)<<endl;
	cout<<"Renderer: "<<glGetString(GL_RENDERER)<<endl;
	cout<<"Vendor: "<<glGetString(GL_VENDOR)<<endl;
    
    
    
    //FOG Parameters.
    GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};  
    
    fogMode = GL_EXP2;  //initial fog mode
    glFogi (GL_FOG_MODE, fogMode);
    glFogfv (GL_FOG_COLOR, fogColor);  //set the fog color to grey
    glFogf (GL_FOG_DENSITY, density);   //set the fog density
    glHint (GL_FOG_HINT, GL_NICEST);  //nicest possible rendering
    glClearColor(0.4, 0.4, 0.4, 1.0);  // set the background color = fog color
    
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    /* need both double buffering and z buffer */
    glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    
    window1 = glutCreateWindow("First Window");
    // register callbacks for first window, which is now current
	glutReshapeFunc(window1_myReshape);
    glutDisplayFunc(window1_display);
    glutSpecialFunc(window1_navigation);
	glutKeyboardFunc(window1_keyboard);
    create_menu();
    init(); //TM
    
	//create the second  window
	window2 = glutCreateWindow("Second Window");
	//define a window position for second window
	glutPositionWindow(520,30);
    // register callbacks for second window, which is now current
	glutReshapeFunc(window2_myReshape);
    glutDisplayFunc(window2_display);
    glutSpecialFunc(window2_navigation);
	glutKeyboardFunc(window2_keyboard);
    init();
    
    glutIdleFunc(spinCube);
    glutMainLoop();
    return 0;
}



#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <cmath>

static GLfloat theta1 = 0.0;  //stores two angles
GLfloat theta1_step=1.0;

using namespace std;

GLfloat y_cube_scale=4;
GLint no_of_cycles = 2;
GLint no_of_carousel_pairs = 4;

GLfloat divisor = 1;
//if(theta1 !=0) 


GLfloat move=0;


#define PI 3.14
GLfloat theta = 0.0, phi = 0.0;  //stores two angles
GLfloat radius = 1.0;
GLfloat xo = 0.0;
GLfloat yo = 0.0;
GLfloat zo = 10.0; // initial viewer position
GLfloat x,y,z; //point on viewing sphere

GLdouble point[3];
GLdouble Znear=10;
GLdouble Zfar=200;
GLdouble YGround = -2.5;
GLdouble sun_theta=0;
GLdouble sun_radius=25;
GLfloat x_sun,y_sun,z_sun=-20;

int sun_light = 1;
int spot_light = 1;
int car_spin =1;

void CalcNormal(GLfloat v1[],GLfloat v2[],GLfloat v3[]) {
	GLfloat v1x, v1y, v1z, v2x, v2y, v2z;
	GLfloat nx, ny, nz;
	GLfloat vLen;
    
	// Calculate vectors
	v1x = v1[0] - v2[0];
	v1y = v1[1] - v2[1];
	v1z = v1[2] - v2[2];
    
	v2x = v2[0] - v3[1];
	v2y = v2[1] - v3[1];
	v2z = v2[2] - v3[2];
    
	// Get cross product of vectors
	nx = (v1y * v2z) - (v1z * v2y);
	ny = (v1z * v2x) - (v1x * v2z);
	nz = (v1x * v2y) - (v1y * v2x);
    
	// Normalise final vector
	vLen = sqrt((nx * nx) + (ny * ny) + (nz * nz));
    
	GLfloat x = (float) (nx / vLen);
	GLfloat y = (float) (ny / vLen);
	GLfloat z = (float) (nz / vLen);
    
	//set the normal
	glNormal3f(x, y, z);
}


GLfloat vertices[][3] = {{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
    {1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0,1.0}, 
    {1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0}};


GLfloat colors[][3] = {{0.0,0.0,0.0},{1.0,0.0,0.0},     //V0(Black),V1(White)
    {1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0},        //V2(Yellow), V3(Green), V4(Blue)
    {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}};       //V5(Magenta), V6(White), V7(Cyan)



void drawpolygon(int a, int b, int c , int d) {
    /* draw a polygon via list of vertices */
    
    glBegin(GL_POLYGON);       
    glVertex3fv(vertices[a]);
    glVertex3fv(vertices[b]);
    glVertex3fv(vertices[c]);
    glVertex3fv(vertices[d]);
    glEnd();
}

void colorcube(void) {
    /* map vertices to faces */
    
    glNormal3f(0,0,-1);
    drawpolygon(0,3,2,1); //Black, Green, Yellow, White
    
    glNormal3f(0,1,0);
    drawpolygon(2,3,7,6); //Yellow,Green,Cyan,White
    
    glNormal3f(-1,0,0);
    drawpolygon(0,4,7,3); //Black,Blue,Cyan,Green
    
    glNormal3f(1,0,0);
    drawpolygon(1,2,6,5); //White,Yellow,White,Cyan
    
    glNormal3f(0,0,1);
    drawpolygon(4,5,6,7); //Blue,Magenta,White,Cyan
    
    glNormal3f(0,-1,0);
    drawpolygon(0,1,5,4); //Black,White,Magenta,Blue
}


void drawRealHorse(void) {
    glNormal3f(0, 1, 1);
    glBegin(GL_POLYGON);
    glVertex3f(0,1,0);
    glVertex3f(2.5,2.75,0);
    glVertex3f(3,3,0);
    glVertex3f(3,2.5,0);
    glVertex3f(3.5,2,0);
    glVertex3f(5.5,1,0);
    glVertex3f(5,0,0);
    glVertex3f(3,1,0);
    glVertex3f(2,-1,0);
    glVertex3f(2,-4,0);
    glVertex3f(1.6,-4,0);
    glVertex3f(1.6,-1,0);
    glVertex3f(-1.6,-1,0);
    glVertex3f(-1.6,-4,0);
    glVertex3f(-2,-4,0);
    glVertex3f(-2,-1,0);
    glVertex3f(-2.5,-1.5,0);
    glVertex3f(-2.75,-1.5,0);
    glVertex3f(-2.75,-2,0);
    glVertex3f(-3,-2,0);
    glVertex3f(-2.8,1.5,0);
    glVertex3f(-2.5,1.5,0);
    glEnd();    
    
}


void drawroof(void) {
    
    GLfloat norm_vertex[][3] = {{0,7,0}, //this vertex is on the y axis at height 7
        {0,4,9}, //this is on the positive z axis at a radius of 9 and height of 4
        {7.794,4,4.5}}; // this is where the next postis, height of 4. Note that 7.794 is 9.0 * sin (60.0)
    
    CalcNormal(norm_vertex[0],norm_vertex[1],norm_vertex[2]);
    glBegin(GL_POLYGON);
    glVertex3fv(norm_vertex[0]);  
    glVertex3fv(norm_vertex[1]);  
    glVertex3fv(norm_vertex[2]);  
    glEnd();
}

void drawfloor(void) {
    
    glBindTexture(GL_TEXTURE_2D, 2); //TM
    glBegin(GL_POLYGON);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0, 0.0);glVertex3f(0,-4,0);  //this vertex is on the y axis at height -4
    glTexCoord2f(0.0, 1.0);glVertex3f(0,-4,9);  //this is on the positive z axis at a radius of 9 and height of -4
    glTexCoord2f(1.0, 1.0);glVertex3f(7.794, -4, 4.5);  // this is where the next postis, height of -4
    //note that 7.794 is 9.0 * sin (60.0)
    glEnd(); 
    glBindTexture(GL_TEXTURE_2D, 0); //TM
}

void myReshape(int w, int h) {
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
            //glColor3f(0,0,1);
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
            //glColor3f(1,1,0);
        }
        drawroof();
        glPopMatrix();
        
        
        //Floors
        glPushMatrix();
        glRotated(theta1+i*60,0,1,0);
        //GLfloat floor_mat_specular[]={0.36, 0.2, 0.09, 1.0};
        GLfloat floor_mat_specular[]={1.0, 1.0, 1.0, 1.0};
        //GLfloat floor_mat_diffuse[]={0.36, 0.2, 0.09, 1.0};
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
    
    glNormal3f(0,1,1);
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    glVertex3f(0,9,0);  //Since the post is translated by 9, its mid-point is at y=9.
    glVertex3f(0,11,0);//Since the post is scaled by 2(2*initial height), y=11 represents top of post.
    glVertex3f(2,10,0);  // Mid point of Flag edge on post.
    glEnd();    
    glPopMatrix();    
}

void drawGround(void) {
    for (GLdouble z = 10;z>=-100;z=z-2) {
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
            glNormal3f(0, 1, 0);
            //glBegin(GL_LINE_LOOP);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);glVertex3f(1,YGround,-1);
            glTexCoord2f(0.0, 1.0);glVertex3f(1,YGround,1);
            glTexCoord2f(1.0, 1.0);glVertex3f(-1,YGround,1);
            glTexCoord2f(1.0, 0.0);glVertex3f(-1,YGround,-1);
            glEnd();
            glPopMatrix();
        }
        
    }
    glBindTexture(GL_TEXTURE_2D, 0); //TM
}

void drawSun(void) {
    x_sun=sun_radius*cos(sun_theta);
    y_sun=sun_radius*sin(sun_theta);
    
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
    
    //Draw Caraousel
    for(int j=1;j<=no_of_carousel_pairs;j++) { //No of Caraousel
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

void init_lighting_conditions() // This is where you can experiment with lighting setup
{
    GLfloat light_ambient[]={1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0}; 
    GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};
    
    GLfloat light_position[]={0,0,0,1.0};
    //GLfloat light_direction[] = {0,0,z};
    GLfloat light_direction[] = {0,0,-1};
    
    // set up ambient, diffuse, and specular components for light 0
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10.0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
    glShadeModel(GL_SMOOTH); //enable smooth shadin  
    glClearColor (1.0, 1.0,1.0, 1.0);
    
}


void sun_lighting_conditions() { // This is where you can experiment with lighting setup)    
    
    GLfloat light_ambient[]={1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0}; 
    GLfloat light_position[]={x_sun,y_sun, z_sun, 1.0} ;
    //GLfloat light_direction[] = {0,1,0 };
    
    // set up ambient, diffuse, and specular components for light 0
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    //glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);
    //glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);
    glShadeModel(GL_SMOOTH); //enable smooth shading
    glClearColor (1.0, 1.0,1.0, 1.0);
}

void navigation(int key,int x,int y) {
    switch (key) {
        case GLUT_KEY_UP: {
            if (theta < (PI/2 - 0.1)) theta += 0.1;
            glutPostRedisplay();
            return; }
            break;
        case GLUT_KEY_DOWN:
        {
            if (theta > (-PI/2 + 0.1)) theta -= 0.1;
            glutPostRedisplay();
            return;
        }
            break;
        case GLUT_KEY_RIGHT :
        {
            phi -= 0.1;  //radians
            glutPostRedisplay();
            return;
        }
            break;
        case GLUT_KEY_LEFT:
        {
            phi += 0.1;  //radians
            glutPostRedisplay();
            return;
        }
            break;
        default:
            break;
    }
    glutPostRedisplay();
}


void keyboard(unsigned char key,int x_cord,int y_cord) {
    switch (key) {
        case 'f':
        {
            xo=xo+x;
            zo+=z;
            glutPostRedisplay();
            return;
        }
            break;
        case 'b':
        {
            xo-=x;
            zo-=z;
            glutPostRedisplay();
            return;
        }
            break;
        default:
            break;
    }
    //glutPostRedisplay();
}


int touch=-1;

void spinCube()
{
    
    if(theta1_step!=0) divisor = theta1_step;
    GLfloat step_move=(no_of_cycles*4*y_cube_scale)/(360/divisor);
    
    //idle callback.  Angles are incremented by varying amounts each time it is called.
    
    if(car_spin==1) {
        theta1 += theta1_step; //.05; // slow rotation
        if( theta1 > 360.0 ) theta1 -= 360.0;
        
        
        sun_theta += .02;
        if(sun_theta>360) sun_theta -= 360;
        
        
        if(move >=y_cube_scale) touch=1;
        else if(move <=-y_cube_scale) touch=-1;
        
        if(touch==1) move -=step_move;
        if(touch==-1) move +=step_move;
        
        
    }
    
    
    glutPostRedisplay();  // generate a display event
}



void main_menu_callback(int selection) {
	if (selection == 0) sun_light=-sun_light;  // Turn the Sun Light On/Off. 
    else if (selection == 1) spot_light=-spot_light; //Turn the Spot Light On/off.
    else if(selection ==2) car_spin = -car_spin; //Turn the Caraousel Spin On/Off;
    else if (selection==3) exit(0);
    glutPostRedisplay();  // generate a display event
}

void create_menu() {
    //create the main menu
	glutCreateMenu(main_menu_callback);
	//attach the color and shape menus to the main menu as submenus
    glutAddMenuEntry("Sun On/Off",0);  //this is not a submenu -> calls callback
	glutAddMenuEntry("Spot Light On/off", 1);
    glutAddMenuEntry("Carausel Spin On/Off", 2);
    glutAddMenuEntry("Exit", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);  //attach menu to right button
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color buffer and z buffer
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //TM
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();  // start with an identity in the MODELVIEW transformation
    //multiply viewing component of transformation
    
    x = radius * cos (theta) * cos (phi);
	y = radius * sin (theta);
	z = -radius * cos (theta)* sin(phi);
    
    init_lighting_conditions();  //set up lightin  
    gluLookAt(xo,yo,zo,xo+x,yo+y,zo+z,0,1,0)  ; 
    
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


void init() {
    //int i;
    //glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    //glEnable(GL_DEPTH_TEST);  //Enabled in Main Function.
    
    
    LoadTextureFromPPM("/MS_CS_UML/Computer_Graphics_I/Assign3/Prof_Texturing/grass.ppm", 1);
    LoadTextureFromPPM("/MS_CS_UML/Computer_Graphics_I/Assign3/Prof_Texturing/wood.ppm", 2);
    
    //  set up the global method of applying texture
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    
    
    // enable everything you need
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_LIGHTING); //Enabled in Main Function.
    //glEnable(GL_LIGHT0); //Enabled in Main Function.
    
    // get the version information
    
	cout<<"Version: "<<glGetString(GL_VERSION)<<endl;
	cout<<"Renderer: "<<glGetString(GL_RENDERER)<<endl;
	cout<<"Vendor: "<<glGetString(GL_VENDOR)<<endl;
    
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
    init(); //TM
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(navigation);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST); // Enable hidden surface removal
    glutMainLoop();
    return 0;
}



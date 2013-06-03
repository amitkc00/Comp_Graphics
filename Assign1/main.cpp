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

//GLenum mode=GL_FILL;
//GLenum mode=GL_LINE;
GLint mode=1;
#define PI 3.1415926535898

float radius=.1;
int Flag=0;

GLsizei wh = 700, ww = 700; // initial window size 
GLfloat size = .01;   // half side length of square 
GLfloat global_x_position, global_y_position;   //stores where the figure is to be drawn
GLfloat end_x_position, end_y_position;
enum {eSquare, eTriangle,eCircle,eHexagon,eLine,eRectangle} current_shape;

float r=1.0,g=1.0,b=1.0;  // to store the global color variables


//Initilization of Structure.
typedef struct drawInfo
{
    GLfloat x1; //To Store x-position
    GLfloat y1; //To Store y-position
    GLfloat x2; //To Store x-position
    GLfloat y2; //To Store y-position
    GLfloat colorRed; //To Store Color of Drawing
    GLfloat colorGreen; //To Store Color of Drawing
    GLfloat colorBlue; //To Store Color of Drawing
    GLenum fill; //To Store Fill/Unfill parameter for Polygon //NOTE Could be Problematic.
    GLenum shape2 ; // To Store shape associated with the point //NOTE
}drawInform;

#define arraySize 100

drawInform drawArray[arraySize];
int counter=0;

void color_select(int color)
{
    if(color == 1) {r = 1.0; g = 0.0; b = 0.0;} //red
    else if(color == 2) {r = 0.0; g = 1.0; b = 0.0;} //green
    else if(color == 3) {r = 0.0; g = 0.0; b = 1.0;} //blue
    else if(color == 4) {r = 0.0; g = 1.0; b = 1.0;} //cyan
    else if(color == 5) {r = 1.0; g = 0.0; b = 1.0;} //magenta
    else if(color == 6) {r = 1.0; g = 1.0; b = 0.0;} //yellow
    else if(color == 7) {r = 1.0; g = 1.0; b = 1.0;} //white
    else if(color == 8) {r = 0.0; g = 0.0; b = 0.0;} //black
}


void drawSquare(float x, float y)
{
	//y=wh-y;  //because the window coordinate system is flipped in y
	
    if(mode==1)
        glBegin(GL_POLYGON);  //drawing the polygon
    else if(mode ==2)
        glBegin(GL_LINE_LOOP);  //drawing the Line Loop
    
	glVertex2f((float)x+size, (float)y+size);
	glVertex2f((float)x-size, (float)y+size);
	glVertex2f((float)x-size, (float)y-size);
	glVertex2f((float)x+size, (float)y-size);
	glEnd();
	glFlush();  //insure that the commands clear the pipeline
}

//This function draws a trianglular polygon in the current color
void drawTriangle(float x, float y)
{
	//y=wh-y;  //because the window coordinate system is flipped in y
	
    if(mode==1)
        glBegin(GL_POLYGON);  //drawing the polygon
    else if(mode ==2)
        glBegin(GL_LINE_LOOP);  //drawing the Line Loop
    
	glVertex2f((float)x, (float)y+size);
	glVertex2f((float)x-0.866f*size, (float)y-0.5f*size);
	glVertex2f((float)x+0.866f*size, (float)y-0.5f*size);
	glEnd();
	glFlush(); //insure that the commands clear the pipeline
}


//Code Reference from http://slabode.exofire.net/circle_draw.shtml
void drawCircle(float cx, float cy) 
{ 
	
    float r = radius;
    int num_segments=100;
    float theta = 2 * 3.1415926 / float(num_segments); 
	float tangetial_factor = tanf(theta);//calculate the tangential factor 
    
	float radial_factor = cosf(theta);//calculate the radial factor 
	
	float x = r;//we start at angle = 0 
    
	float y = 0; 
    
    if(mode==1)
        glBegin(GL_POLYGON);  //drawing the polygon
    else if(mode ==2)
        glBegin(GL_LINE_LOOP);  //drawing the Line Loop
    
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 
        
		float tx = -y; 
		float ty = x; 
        
		//add the tangential vector 
        
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
        
		//correct using the radial factor 
        
		x *= radial_factor; 
		y *= radial_factor; 
	} 
	glEnd(); 
}

void drawHexagon(float x,float y)
{
    
    if(mode==1)
        glBegin(GL_POLYGON);  //drawing the polygon
    else if(mode ==2)
        glBegin(GL_LINE_LOOP);  //drawing the Line Loop
 
    glVertex2f(x+radius*cos(0*(PI/3)),y+radius*sin(0*(PI/3)));
    glVertex2f(x+radius*cos(1*(PI/3)),y+radius*sin(1*(PI/3)));
    glVertex2f(x+radius*cos(2*(PI/3)),y+radius*sin(2*(PI/3)));
    glVertex2f(x+radius*cos(3*(PI/3)),y+radius*sin(3*(PI/3)));
    glVertex2f(x+radius*cos(4*(PI/3)),y+radius*sin(4*(PI/3)));
    glVertex2f(x+radius*cos(5*(PI/3)),y+radius*sin(5*(PI/3)));
    glEnd();
    glFlush();  //insure that the commands clear the pipeline
}

void drawLine(float xStart,float yStart,float xEnd,float yEnd)
{
    
    if(Flag==0){
        
        glBegin(GL_LINES);
        glVertex2f((float)xStart, (float)yStart);
        glVertex2f((float)xEnd, (float)yEnd);
        glEnd();
        glFlush();  //insure that the commands clear the pipeline
        
    }
}

void drawRectangle(float x1Diag,float y1Diag,float x2Diag,float y2Diag)
{
    
    if(mode==1)
    glBegin(GL_POLYGON);  //drawing the polygon
    else if(mode ==2)
        glBegin(GL_LINE_LOOP);  //drawing the Line Loop
    //glPolygonMode(GL_FRONT, mode);
    
    glVertex2f((float)x1Diag, (float)y1Diag);
    glVertex2f((float)x2Diag, (float)y1Diag);
    glVertex2f((float)x2Diag, (float)y2Diag);
    glVertex2f((float)x1Diag, (float)y2Diag);
    glEnd();
    glFlush();  //insure that the commands clear the pipeline
    
    
}


void shape_select(int shape)
{
	if (shape == 1) current_shape = eSquare;
	else if (shape == 2) current_shape = eTriangle;
    else if (shape == 3) current_shape = eCircle;
    else if (shape == 4) current_shape = eHexagon;
    else if (shape == 5) current_shape = eLine;
    else if (shape == 6) current_shape = eRectangle;
    
    
}


void fill_unfill(int filler)
{
    //GLenum mode;
    if(filler==1){
        mode=1;
        //glBegin(GL_LINE_LOOP);
    }
    else if (filler ==2){
        mode=2;
        //glBegin(GL_POLYGON);
    }
}

void screen_clear(int screen)
{
    if(screen==1)
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        //glutPostRedisplay();  // post a display event
        
    }
    else if(screen==2)
    {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        //glutPostRedisplay();  // post a display event
    }
    
    //When screen is flushed, clear the data structure.
    for (int i=0; i<arraySize; i++) 
    {
        drawArray[i].x1=0;
        drawArray[i].y1=0;
        drawArray[i].x2=0;
        drawArray[i].y2=0;
        drawArray[i].colorRed=0;
        drawArray[i].colorGreen=0;
        drawArray[i].colorBlue=0;
        drawArray[i].fill=0;
        drawArray[i].shape2=0;
    }
    
}

void main_menu_callback(int selection)
{
	if (selection == 0) exit(0);  // exit the program if exit is selected
    else if (selection == 1) size = size*1.5f; //scale the size of global size multiplier
}

void create_menu()
{
    int c_menu_id, shape_menu_id,screen_menu_id,filler_menu_id; // menu identifiers for submenus
    
	//create a color menu with callback color_select and assign to menu id c_menu_id
    c_menu_id = glutCreateMenu(color_select);
    glutAddMenuEntry("Red",1);
    glutAddMenuEntry("Green",2);
    glutAddMenuEntry("Blue",3);
    glutAddMenuEntry("Cyan",4);
    glutAddMenuEntry("Magenta",5);
    glutAddMenuEntry("Yellow",6);
    glutAddMenuEntry("WhiteFC",7);
    glutAddMenuEntry("BlackFC",8);
    
    
    filler_menu_id=glutCreateMenu(fill_unfill);
    glutAddMenuEntry("Filled Polygon",1);
    glutAddMenuEntry("Un-Filled Polygon",2);
    
	//create a shape menu with callback shape_select and menu id shape_menu_id
	shape_menu_id = glutCreateMenu(shape_select);
    glutAddMenuEntry("Square",1);
	glutAddMenuEntry("Triangle",2);
    glutAddMenuEntry("Circle",3);
    glutAddMenuEntry("Hexagon",4);
    glutAddMenuEntry("LineDraw",5);
    glutAddMenuEntry("Rectangle", 6);
    
    //create a color select menu for screen clear
    screen_menu_id=glutCreateMenu(screen_clear);
    glutAddMenuEntry("Black",1);
    glutAddMenuEntry("White",2);
    
    //create the main menu
	glutCreateMenu(main_menu_callback);
    
	//attach the color and shape menus to the main menu as submenus
	glutAddSubMenu("Select Color", c_menu_id);
	glutAddSubMenu("Select Shape", shape_menu_id);
    glutAddSubMenu("Select Color for Screen Clear", screen_menu_id);
    glutAddSubMenu("Fill/UnFill Polygon", filler_menu_id);
    glutAddMenuEntry("Exit",0);  //this is not a submenu -> calls callback
	glutAddMenuEntry("Increase Size", 1);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);  //attach menu to right button
}




void reshape(GLsizei w, GLsizei h)
{
    
    //glViewport describes the area of your window which will be drawn by OpenGL. glOrtho or gluOrtho2D define a unit system (OpenGL units) which fit into that (via glViewport defined) area.
    
    //2nd View : gluOrtho defines the range of my x & y axis values. glViewport maps x & y axis to the drawing region of 700 / 700 through (-1,-1) to (1,1) canonical axis.
    
    
    glViewport(0, 0, (GLsizei)w, (GLsizei)h); //Tell OpenGL how to convert from coordinate to pixel value.
    glMatrixMode(GL_PROJECTION); // Switch to setting the camera perspective
    
    //Set the camera perspective.
    glLoadIdentity(); //Reset the camera.
    gluOrtho2D(0.0, (GLdouble) 1.0, 0.0,(GLdouble) 1.0);
    // gluOrtho2D(0.0, (GLdouble) w, 0.0,(GLdouble) h);
    wh=h;
    ww=w;
    
    // NOT SURE WHY WE NEED THIS
    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective.
    glLoadIdentity(); //Reset the drawing Perspective.
    
    //glutPostRedisplay();
}


void accept_left_button_command(int btn, int state, int x, int y)
{
    //eSquare, eTriangle,eCircle,eHexagon
    cout<<"\neLine = "<<eLine<<"eRectangle = "<<eRectangle<<"Flag = "<<Flag;
    if (current_shape==eSquare||current_shape==eTriangle||current_shape==eCircle||current_shape==eHexagon){
        if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN && Flag==0) //this is only for left button events
        {
            global_x_position = (float)x/ww;  //capture x mouse position in global
            global_y_position = (float)(wh-y)/wh;  //capture y mouse position in global
            
            end_x_position = (float)x/ww;
            end_y_position = (float)(wh-y)/wh;	
            
            
            //Save elements of Data Structure here.
            drawArray[counter].x1=global_x_position;
            drawArray[counter].y1=global_y_position; //Problem Place
            drawArray[counter].x2=end_x_position;
            drawArray[counter].y2=end_y_position;
            drawArray[counter].colorRed=r;
            drawArray[counter].colorGreen=g;
            drawArray[counter].colorBlue=b;
            drawArray[counter].fill=mode;
            drawArray[counter].shape2=current_shape;
            counter++;
            cout<<"\nCurrent Shape0 = "<<current_shape;
            cout<<"\nglobal_x_position = "<<global_x_position;
            cout <<"\nglobal_y_position = "<<global_y_position;
            cout<<"\nend_x_position = "<<end_x_position;
            cout <<"\nend_y_position = "<<end_y_position;
            glutPostRedisplay(); //Since dislay event is not automatic for mouse action.
        }
    }
    else if (current_shape==eLine || current_shape==eRectangle) {
        if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN && Flag==0) //this is only for left button events
        {
            global_x_position = (float)x/ww;  //capture x mouse position in global
            global_y_position = (float)(wh-y)/wh;  //capture y mouse position in global
            
            //Save elements of Data Structure here.
            drawArray[counter].x1=global_x_position;
            drawArray[counter].y1=global_y_position; //Problem Place
            
            drawArray[counter].colorRed=r;
            drawArray[counter].colorGreen=g;
            drawArray[counter].colorBlue=b;
            drawArray[counter].fill=mode;
            drawArray[counter].shape2=current_shape;
            cout<<"\nCurrent Shape1 = "<<current_shape;
            Flag=1;
            cout<<"\nIn Line/Rectangle Draw. Value of Flag ="<<Flag;
            
        }
        else if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN && Flag==1){
            end_x_position = (float)x/ww;  //capture x mouse position in global
            end_y_position = (float)(wh-y)/wh;  //capture y mouse position in global
            drawArray[counter].x2=end_x_position;
            drawArray[counter].y2=end_y_position;
            //counter++;
            Flag=0;
            cout<<"\nCurrent Shape2 = "<<current_shape;
            cout<<"\nIn Line/Rectangle Draw. Value of Flag ="<<Flag;
            cout<<"\nglobal_x_position = "<<global_x_position;
            cout <<"\nglobal_y_position = "<<global_y_position;
            cout<<"\nend_x_position = "<<end_x_position;
            cout <<"\nend_y_position = "<<end_y_position;
            
            cout<<"\n\nFor Counter = "<<counter;
            cout<<"\nglobal_x_position = "<<drawArray[counter].x1;
            cout <<"\nglobal_y_position = "<<drawArray[counter].y1;
            cout<<"\nend_x_position = "<<drawArray[counter].x2;
            cout <<"\nend_y_position = "<<drawArray[counter].y2;
            counter++;
            glutPostRedisplay(); //Since dislay event is not automatic for mouse action.
        }
        
    }
}

void display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i=0; i<counter; i++) {
        //Using Approch 1
        
        global_x_position = drawArray[i].x1;  //capture mouse x-position in new window size
        global_y_position = drawArray[i].y1;  //capture mouse x-position in new window size
        
        end_x_position = drawArray[i].x2;
        end_y_position = drawArray[i].y2;
        r=drawArray[i].colorRed;
        g=drawArray[i].colorGreen;
        b=drawArray[i].colorBlue;
        mode=drawArray[i].fill;
   
        cout<<"\n\nDisplay Function";
        cout<<"\n\nFor Counter = "<<i;
        cout<<"\nglobal_x_position = "<<drawArray[i].x1;
        cout <<"\nglobal_y_position = "<<drawArray[i].y1;
        cout<<"\nend_x_position = "<<drawArray[i].x2;
        cout <<"\nend_y_position = "<<drawArray[i].y2;
        cout<<"\nCurrent Shape ="<<drawArray[i].shape2;

        
        glColor3f(r,g,b);
        if (drawArray[i].shape2 == eTriangle) drawTriangle(global_x_position, global_y_position);
        else if (drawArray[i].shape2 == eSquare) drawSquare(global_x_position, global_y_position);
        else if (drawArray[i].shape2 == eCircle) drawCircle(global_x_position, global_y_position);
        else if (drawArray[i].shape2 == eHexagon) drawHexagon(global_x_position, global_y_position);
        else if (drawArray[i].shape2 == eLine) drawLine(global_x_position, global_y_position,end_x_position,end_y_position);
        else if (drawArray[i].shape2 == eRectangle) drawRectangle(global_x_position, global_y_position,end_x_position,end_y_position);

        glFlush();
    }
}




int main(int argc,char** argv)
{
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(wh, ww);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Amit Menu");
    
    glutReshapeFunc(reshape);
    glutMouseFunc (accept_left_button_command);
    glutDisplayFunc(display);
    create_menu();
    glutMainLoop();
    return 0;
    
}
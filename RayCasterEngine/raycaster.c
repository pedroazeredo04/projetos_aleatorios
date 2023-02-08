#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

// defines
#define GLSW 1024
#define GLSH 512
#define MAPSIZE 64
#define PI 3.1415926535
#define TOLERANCE 0.01
#define DEGREE 0.0174533

// class player
typedef struct Player {
    float x, y;  // Player position
    float ms;  // move speed
    float a;  // Angle
} Player; Player p1;

// class map
typedef struct Map {
    int x, y, size;
    int map[];
} Map;

// map itself
int mapX = 8, mapY = 8, mapS = 64;
// mapS is the size in pixels of each map square
int map[] =
{

1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,1,
1,1,0,0,0,1,0,1,
1,0,1,0,0,0,0,1,
1,0,0,0,0,0,0,1,
1,1,0,0,0,1,0,1,
1,0,0,1,0,0,0,1,
1,1,1,1,1,1,1,1,

};

void drawMap() {
    int x, y, xo, yo;
    for (y = 0; y < mapY; y++) {
        for (x = 0; x < mapX; x++) {

            if (map[y*mapX + x] == 1) {
                glColor3f(1,1,1);
            } else {
                glColor3f(0,0,0);
            }

            xo = x * mapS;
            yo = y * mapS;
            glBegin(GL_QUADS);
            glVertex2i(xo        + 1, yo        + 1);
            glVertex2i(xo        + 1, yo + mapS - 1);
            glVertex2i(xo + mapS - 1, yo + mapS - 1);
            glVertex2i(xo + mapS - 1, yo        + 1);
            glEnd();
        }
    }
}

void drawPlayer() 
{
    glColor3f(1, 0.5, 0); // orange
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2d(p1.x, p1.y);
    glEnd();

    glBegin(GL_LINES);
    glLineWidth(3);
    glVertex2i(p1.x, p1.y);
    glVertex2i(p1.x + p1.ms * cos(p1.a)*5, p1.y + p1.ms * sin(p1.a)*5);
    glEnd();
}

float dist(float x1, float y1, float x2, float y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void drawRays() 
{
    float ra = p1.a;  // ray angle
    float rx, ry;  // the ray position
    float xo, yo;  // the next point the ray hits a wall

    int num_of_checks;
    int mapx, mapy, mappos;  // the coordinates in the Map array
    float distW;  // distance to the wall

    ra = ra - 30*DEGREE; // start at 30 degrees to the left
    if (ra < 0) ra += 2*PI;
    if (ra > 2*PI)  ra -= 2*PI;

    for (int ray = 0; ray < 60; ray++)
    {   
        num_of_checks = 0;
        float hdist = 99999999;  // min distance until the ray hits the wall
        float hx, hy;  // horizontal min dist position values

        // Check Horizontal Lines
        if (ra > PI)  // ray facing down
        { 
            ry = (int)(p1.y/MAPSIZE)*MAPSIZE - 0.0001;
            rx = p1.x + (ry - p1.y)/tan(ra);
            yo = -64;
            xo = yo/tan(ra);
        }

        if (ra < PI)  // ray facing up
        {
            ry = (int)(p1.y/MAPSIZE)*MAPSIZE + 64;
            rx = p1.x + (ry - p1.y)/tan(ra);
            yo = 64;
            xo = yo/tan(ra);
        }

        if (abs(ra) < TOLERANCE || abs(ra-PI) < TOLERANCE)  // horizontal ray
        {
            rx = p1.x;
            ry = p1.y;
            num_of_checks = 8;  // dont even enter loop
        }

        while (num_of_checks < 8)
        {
            mapx = (int)(rx/64);
            mapy = (int)(ry/64);
            mappos = mapy*mapX + mapx;

            if (mappos > 0 && mappos < MAPSIZE && map[mappos] == 1) {  // hit wall
                num_of_checks = 8;
                hx = rx;
                hy = ry;
                hdist = dist(p1.x, p1.y, rx, ry);
            } else {  // check next wall
                rx += xo; ry+= yo; num_of_checks += 1;
            }
        }

        num_of_checks = 0;
        float vdist = 99999999;  // min vertical distance until the ray hits the wall
        float vx, vy;  // vertical min dist position values

        // Check Vertical Lines
        if (ra > PI/2 && ra < 3*PI/2)  // ray facing left
        { 
            rx = (int)(p1.x/MAPSIZE)*MAPSIZE - 0.0001;
            ry = p1.y + (rx - p1.x)*tan(ra);
            xo = -64;
            yo = xo*tan(ra);
        }

        if (ra < PI/2 || ra > 3*PI/2)  // ray facing right
        {
            rx = (int)(p1.x/MAPSIZE)*MAPSIZE + 64;
            ry = p1.y + (rx - p1.x)*tan(ra);
            xo = 64;
            yo = xo*tan(ra);
        }

        if (abs(ra - PI/2) < TOLERANCE || abs(ra - 3*PI/2) < TOLERANCE)  // vertical ray
        {
            rx = p1.x;
            ry = p1.y;
            num_of_checks = 8;  // dont even enter loop
        }

        while (num_of_checks < 8)
        {
            mapx = (int)(rx/64);
            mapy = (int)(ry/64);
            mappos = mapy*mapX + mapx;

            if (mappos > 0 && mappos < MAPSIZE && map[mappos] == 1) {  // hit wall
                num_of_checks = 8;
                vx = rx;
                vy = ry;
                vdist = dist(p1.x, p1.y, rx, ry);
            } else {  // check next wall
                rx += xo; ry+= yo; num_of_checks += 1;
            }
        }

        if (hdist < vdist) {
            rx = hx; ry = hy; distW = hdist;
            glColor3f(0.5,0,0.9);
        } else {
            rx = vx; ry = vy; distW = vdist;
            glColor3f(0.2,0,0.7);
        }

        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex2i(p1.x, p1.y);
        glVertex2i(rx, ry);
        glEnd();

        // Draw the 3D walls
        // They will be 320x160 pixels

        float ca = p1.a - ra; // angle between ray angle and player angle
        if (ca < 0) ca += 2*PI; if (ca > 2*PI) ca -= 2*PI;

        distW *= cos(ca);  // fix fish eye effect
        float lineH = (mapS*320)/distW;  // line height
        if (lineH > 512) lineH = 512;
        float offset = (MAPSIZE*8 - lineH)/2;  // offset from the center of the screen
        
        glLineWidth(8);
        glBegin(GL_LINES);
        glVertex2i((ray + 3/2*MAPSIZE + 2)*8, offset);
        glVertex2i((ray + 3/2*MAPSIZE + 2)*8, lineH + offset);
        glEnd();

        ra += 1*DEGREE;
        if (ra < 0) ra += 2*PI;
        if (ra > 2*PI)  ra -= 2*PI;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap();
    drawRays();
    drawPlayer();
    glutSwapBuffers();
    glutReshapeWindow(GLSW, GLSH);
}

void buttons(unsigned char key, int x, int y)
{   
    switch (key) {
        case 'w':
            p1.x += p1.ms * cos(p1.a); p1.y += p1.ms * sin(p1.a); 
            if (map[(int)(p1.y/64) * mapX + (int)(p1.x/64)] == 1) { // hit wall
                p1.x -= p1.ms * cos(p1.a); p1.y -= p1.ms * sin(p1.a); 
            }
            break;

        case 's':
            p1.x -= p1.ms * cos(p1.a); p1.y -= p1.ms * sin(p1.a); 
            if (map[(int)(p1.y/64) * mapX + (int)(p1.x/64)] == 1) {
                p1.x += p1.ms * cos(p1.a); p1.y += p1.ms * sin(p1.a); 
            }
            break;

        case 'a':
            p1.a -= 0.1; if(p1.a < 0) { p1.a += 2* PI; }
            break;

        case 'd':
            p1.a += 0.1; if(p1.a > 2* PI) { p1.a -= 2* PI; }
            break;
    }

    glutPostRedisplay();  // marks the current window as needing to be redisplayed
}

void mouse(int x, int y) {
    p1.a = atan2(y - p1.y, x - p1.x);
    //p1.a = (2*PI/GLSW)*(GLSW/2 - x);
    glutPostRedisplay();
}

void init() 
{
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, GLSW, GLSH, 0);
    p1.x = GLSW/4; p1.y = GLSH/2;
    p1.ms = 2;
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    // @see GLUT_CURSOR_NONE
}
 
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);  // kinda unnecessary
    glutInitWindowSize(GLSW, GLSH);
    glutCreateWindow("RayCasterTest");
    init();
    glutDisplayFunc(display);
    glutPassiveMotionFunc(mouse);
    glutKeyboardFunc(buttons);
    glutMainLoop();
}

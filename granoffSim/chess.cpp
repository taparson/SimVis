/* chess.c - this is a more complex example, that will draw chess pieces
 * on a chess board, and allow the user to move the pieces with the wand.
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>




/*  Create a simple data structure to hold the chess piece */
typedef struct {
  float x;
  float y;
  float z;
  int type;
} piece;

void draw(void);


/* The shared memory variables. */
piece chessPieces[32];
static int pieceCount = 32;


static GLuint floorList, pawnList, kingList, queenList, rookList, bknightList, wknightList, bishopList;

void init_pieces(void)
{
  float initialpos;
  float white[] = { 1, 1, 1, 1 };
  float black[] = { 0.2, 0.2, 0.2, 1 };
  GLfloat shininess[] = { 0.0 };
  GLfloat specular[] = { 0, 0, 0, 1 };
  static float blueMaterial[] = { 0, 0, 1, 1 };
  static float whiteMaterial[] = { 1, 1, 1, 1 };
  int i, j;
  GLUquadricObj *sphereObj, *diskObj, *cylinderObj;
 
  GLfloat pos[] = {0, 0, 0, 1};
  glLightfv(GL_LIGHT0, GL_POSITION, pos);

  glEnable(GL_LIGHT0);

  sphereObj = gluNewQuadric();
  diskObj = gluNewQuadric();
  cylinderObj = gluNewQuadric();

  /* The Pawn */
  pawnList= glGenLists(1);

  glNewList(pawnList, GL_COMPILE);
  glPushMatrix();
  glRotatef(-90, 1, 0, 0);
  gluDisk(diskObj, 0, 2.3, 10, 1);
  gluCylinder(cylinderObj, 2.3, 2.3, 1.0, 10, 2);
  glTranslatef(0.0, 0.0, 1.0);
  gluDisk(diskObj, 0, 2.3, 10, 1);
  gluCylinder(cylinderObj, 2.0, 2.0, 0.7, 10, 2);  
  glTranslatef(0.0, 0.0, 0.7);
  gluDisk(diskObj, 0, 2.0, 10, 1);
  gluCylinder(cylinderObj, 1.6, 1.0, 3, 10, 2);    
  glTranslatef(0.0, 0.0, 3);
  gluDisk(diskObj, 0, 1.6, 10, 1);
  gluCylinder(cylinderObj, 1.6, 1.6, 0.4, 10, 2);    
  glTranslatef(0.0, 0.0, 0.4);
  gluDisk(diskObj, 0, 1.6, 10, 1);
  glTranslatef(0.0, 0.0, 1.2);
  gluSphere(sphereObj, 1.6, 8, 8);
  glPopMatrix();
  glEndList();


  /* The Bishop */
  bishopList= glGenLists(1);

  glNewList(bishopList, GL_COMPILE);
  glPushMatrix();
  glRotatef(-90, 1, 0, 0);
  gluDisk(diskObj, 0, 2.7, 10, 1);
  gluCylinder(cylinderObj, 2.7, 2.7, 1.0, 10, 2);
  glTranslatef(0.0, 0.0, 1.0);
  gluDisk(diskObj, 0, 2.7, 10, 1);
  gluCylinder(cylinderObj, 2.4, 2.4, 0.7, 10, 2);  
  glTranslatef(0.0, 0.0, 0.7);
  gluDisk(diskObj, 0, 2.4, 10, 1);
  gluCylinder(cylinderObj, 1.6, 1.0, 5, 10, 2);    
  glTranslatef(0.0, 0.0, 5);
  gluDisk(diskObj, 0, 1.6, 10, 1);
  gluCylinder(cylinderObj, 1.6, 1.6, 0.4, 10, 2);    
  glTranslatef(0.0, 0.0, 0.4);
  gluDisk(diskObj, 0, 1.6, 10, 1);
  glTranslatef(0.0, 0.0, 1.2);
  glScalef(1, 1, 1.7);
  gluSphere(sphereObj, 1.3, 8, 8);
  glTranslatef(0.0, 0.0, 1.4);
  gluSphere(sphereObj, 0.3, 8, 8);
  glPopMatrix();
  glEndList();

  /* The Rook */
  rookList= glGenLists(1);

  glNewList(rookList, GL_COMPILE);
  glPushMatrix();
  glRotatef(-90, 1, 0, 0);
  gluDisk(diskObj, 0, 2.7, 10, 1);
  gluCylinder(cylinderObj, 2.7, 2.7, 1.0, 10, 2);
  glTranslatef(0.0, 0.0, 1.0);
  gluDisk(diskObj, 0, 2.7, 10, 1);
  gluCylinder(cylinderObj, 2.4, 2.4, 0.7, 10, 2);  
  glTranslatef(0.0, 0.0, 0.7);
  gluDisk(diskObj, 0, 2.4, 10, 1);
  gluCylinder(cylinderObj, 1.6, 1.0, 6, 10, 2);    
  glTranslatef(0.0, 0.0, 6);
  gluDisk(diskObj, 0, 1.6, 10, 1);
  gluCylinder(cylinderObj, 1.6, 1.6, 0.4, 10, 2);    
  glTranslatef(0.0, 0.0, 0.4);
  gluDisk(diskObj, 0, 1.6, 10, 1);
  gluCylinder(cylinderObj, 1.4, 1.4, 1.5, 10, 2);    
  glTranslatef(0.0, 0.0, 1.5);
  gluDisk(diskObj, 0, 1.4, 10, 1);
  glPopMatrix();
  glEndList();

  /* The Queen */
  queenList= glGenLists(1);

  glNewList(queenList, GL_COMPILE);
  glPushMatrix();
  glRotatef(-90, 1, 0, 0);
  gluDisk(diskObj, 0, 2.7, 10, 1);
  gluCylinder(cylinderObj, 2.7, 2.7, 1.0, 10, 2);
  glTranslatef(0.0, 0.0, 1.0);
  gluDisk(diskObj, 0, 2.7, 10, 1);
  gluCylinder(cylinderObj, 2.4, 2.4, 0.7, 10, 2);  
  glTranslatef(0.0, 0.0, 0.7);
  gluDisk(diskObj, 0, 2.4, 10, 1);
  gluCylinder(cylinderObj, 1.6, 0.8, 9, 10, 2);    
  glTranslatef(0.0, 0.0, 9);
  gluDisk(diskObj, 0, 1.2, 10, 1);
  gluCylinder(cylinderObj, 1.2, 1.2, 0.4, 10, 2);    
  glTranslatef(0.0, 0.0, 0.4);
  gluDisk(diskObj, 0, 1.2, 10, 1);
  gluCylinder(cylinderObj, 1.0, 1.5, 2.0, 10, 2);    
  glTranslatef(0.0, 0.0, 2.0);
  gluCylinder(cylinderObj, 1.5, 0, 0.6, 10, 2);    
  glTranslatef(0.0, 0.0, 0.8);
  gluSphere(sphereObj, 0.4, 8, 8);
  glPopMatrix();
  glEndList();

  /* The King */
  kingList= glGenLists(1);

  glNewList(kingList, GL_COMPILE);
  glPushMatrix();
  glRotatef(-90, 1, 0, 0);
  gluDisk(diskObj, 0, 2.7, 10, 1);
  gluCylinder(cylinderObj, 2.7, 2.7, 1.0, 10, 2);
  glTranslatef(0.0, 0.0, 1.0);
  gluDisk(diskObj, 0, 2.7, 10, 1);
  gluCylinder(cylinderObj, 2.4, 2.4, 0.7, 10, 2);  
  glTranslatef(0.0, 0.0, 0.7);
  gluDisk(diskObj, 0, 2.4, 10, 1);
  gluCylinder(cylinderObj, 1.6, 0.8, 9, 10, 2);    
  glTranslatef(0.0, 0.0, 9);
  gluDisk(diskObj, 0, 1.2, 10, 1);
  gluCylinder(cylinderObj, 1.2, 1.2, 0.4, 10, 2);    
  glTranslatef(0.0, 0.0, 0.4);
  gluDisk(diskObj, 0, 1.2, 10, 1);
  gluCylinder(cylinderObj, 1.0, 1.0, 2.0, 10, 2);    
  glTranslatef(0.0, 0.0, 2.0);
  gluDisk(diskObj, 0, 1.4, 10, 1);
  gluCylinder(cylinderObj, 1.4, 1.2, 0.4, 10, 2);    
  glTranslatef(0.0, 0.0, 0.4);
  gluDisk(diskObj, 0, 1.2, 10, 1);
  glTranslatef(0.0, 0.0, 0.2);
  gluSphere(sphereObj, 0.4, 8, 8);
  glPopMatrix();
  glEndList();

  /* The White Knight (each knight faces the oppisite direction) */
  wknightList= glGenLists(1);

  glNewList(wknightList, GL_COMPILE);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
  glPushMatrix();
  glRotatef(-90, 1, 0, 0);
  gluDisk(diskObj, 0, 2.7, 10, 1);
  gluCylinder(cylinderObj, 2.7, 2.7, 1.0, 10, 2);
  glTranslatef(0.0, 0.0, 1.0);
  gluDisk(diskObj, 0, 2.7, 10, 1);
  gluCylinder(cylinderObj, 2.4, 2.4, 0.7, 10, 2);  
  glTranslatef(0.0, 0.0, 0.7);
  gluDisk(diskObj, 0, 2.4, 10, 1);
  glRotatef(90, 1, 0, 0);
  glRotatef(180, 0, 1, 0);
  glScalef(1.0, 1.8, 1.0);
  glBegin(GL_QUADS);
  /* This just includes the commands to draw the horse head */
  #include "head.h"
  glEnd();

  glPopMatrix();
  glEndList();

  /* The White Knight (each knight faces the oppisite direction) */
  bknightList= glGenLists(1);

  glNewList(bknightList, GL_COMPILE);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
  glPushMatrix();
  glRotatef(-90, 1, 0, 0);
  gluDisk(diskObj, 0, 2.7, 10, 1);
  gluCylinder(cylinderObj, 2.7, 2.7, 1.0, 10, 2);
  glTranslatef(0.0, 0.0, 1.0);
  gluDisk(diskObj, 0, 2.7, 10, 1);
  gluCylinder(cylinderObj, 2.4, 2.4, 0.7, 10, 2);  
  glTranslatef(0.0, 0.0, 0.7);
  gluDisk(diskObj, 0, 2.4, 10, 1);
  glRotatef(90, 1, 0, 0);
  glScalef(1.0, 1.8, 1.0);
  glBegin(GL_QUADS);
  /* This just includes the commands to draw the horse head */
  #include "head.h"
  glEnd();

  glPopMatrix();
  glEndList();



  floorList = glGenLists(1);
  glNewList(floorList, GL_COMPILE);

   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   glDisable(GL_CULL_FACE);

   
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);
  glNormal3f(0, 1, 0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, whiteMaterial);
  glVertex3f(-40, 0, -40);
  glVertex3f(40, 0, -40);
  glVertex3f(40, 0, 40);
  glVertex3f(-40, 0, 40);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blueMaterial);
  for (i = -40; i <40; i+=20) {
    for(j = -40; j <40; j+=20) {
      glVertex3f(i, 0.1, j);
      glVertex3f(i+10, 0.1, j);
      glVertex3f(i+10, 0.1, j+10);
      glVertex3f(i, 0.1, j+10);
    }
  }
  for (i = -30; i < 40; i+=20) {
    for(j = -30; j < 40; j+=20) {
      glVertex3f(i, 0.1, j);
      glVertex3f(i+10, 0.1, j);
      glVertex3f(i+10, 0.1, j+10);
      glVertex3f(i, 0.1, j+10);
    }
    }
  glEnd();
  glEndList();

  /* except for the pawns, we have to place every piece individually,
   * in the chessPieces array.  Remember, this just holds the position,
   * and the number of the display list to draw, not the display list 
   * itself.
   */
  initialpos = -45;
  for (i = 0; i< 8; i++) {
    initialpos += 10;
    chessPieces[i].x = initialpos;
    chessPieces[i].y = 0;
    chessPieces[i].z = -25;
    chessPieces[i].type = pawnList;
  }
  chessPieces[8].x = -35;
  chessPieces[8].y = 0;
  chessPieces[8].z = -35;
  chessPieces[8].type = rookList;

  chessPieces[9].x = -25;
  chessPieces[9].y = 0;
  chessPieces[9].z = -35;
  chessPieces[9].type = wknightList;

  chessPieces[10].x = -15;
  chessPieces[10].y = 0;
  chessPieces[10].z = -35;
  chessPieces[10].type = bishopList;

  chessPieces[11].x = -5;
  chessPieces[11].y = 0;
  chessPieces[11].z = -35;
  chessPieces[11].type = kingList;

  chessPieces[12].x = 5;
  chessPieces[12].y = 0;
  chessPieces[12].z = -35;
  chessPieces[12].type = queenList;

  chessPieces[13].x = 15;
  chessPieces[13].y = 0;
  chessPieces[13].z = -35;
  chessPieces[13].type = bishopList;

  chessPieces[14].x = 25;
  chessPieces[14].y = 0;
  chessPieces[14].z = -35;
  chessPieces[14].type = wknightList;

  chessPieces[15].x = 35;
  chessPieces[15].y = 0;
  chessPieces[15].z = -35;
  chessPieces[15].type = rookList;

  initialpos = -45;
  for (i = 16; i< 24; i++) {
    initialpos += 10;
    chessPieces[i].x = initialpos;
    chessPieces[i].y = 0;
    chessPieces[i].z = 25;
    chessPieces[i].type = pawnList;
  }
  chessPieces[24].x = -35;
  chessPieces[24].y = 0;
  chessPieces[24].z = 35;
  chessPieces[24].type = rookList;

  chessPieces[25].x = -25;
  chessPieces[25].y = 0;
  chessPieces[25].z = 35;
  chessPieces[25].type = bknightList;

  chessPieces[26].x = -15;
  chessPieces[26].y = 0;
  chessPieces[26].z = 35;
  chessPieces[26].type = bishopList;

  chessPieces[27].x = -5;
  chessPieces[27].y = 0;
  chessPieces[27].z = 35;
  chessPieces[27].type = queenList;

  chessPieces[28].x = 5;
  chessPieces[28].y = 0;
  chessPieces[28].z = 35;
  chessPieces[28].type = kingList;

  chessPieces[29].x = 15;
  chessPieces[29].y = 0;
  chessPieces[29].z = 35;
  chessPieces[29].type = bishopList;

  chessPieces[30].x = 25;
  chessPieces[30].y = 0;
  chessPieces[30].z = 35;
  chessPieces[30].type = bknightList;

  chessPieces[31].x = 35;
  chessPieces[31].y = 0;
  chessPieces[31].z = 35;
  chessPieces[31].type = rookList;

}

#define SPEED 10.0f


/* draw the objects. */
void draw_pieces(void)
{
  int i;
  float white[] = { 1, 1, 1, 1 };
  float black[] = { .2, 0.2, 0.2, 1 };
  GLfloat shininess[] = { 80.0 };
  GLfloat specular[] = { 1, 1, 1, 1 };
 
  glEnable(GL_LIGHTING);

  /* draw the floor */
  glCallList(floorList);
  
  /* lets make the pieces shiny */
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular); 
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess); 
  

  /* First we draw all the white pieces */
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
  for (i = 0; i < pieceCount/2.0; i++) {
    glPushMatrix();
    glTranslatef(chessPieces[i].x, chessPieces[i].y, chessPieces[i].z);
    glCallList(chessPieces[i].type);
    glPopMatrix();
  }
  
  /* then we draw all the black pieces */
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
  for (i = pieceCount/2.0; i < pieceCount; i++) {
    glPushMatrix();
    glTranslatef(chessPieces[i].x, chessPieces[i].y, chessPieces[i].z);
    glCallList(chessPieces[i].type);
    glPopMatrix();
  }


  glDisable(GL_LIGHTING);
}


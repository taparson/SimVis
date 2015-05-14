//  Copyright Regents of the University of Minnesota and Brown
//  University, 2010-2014.  All rights are reserved.

/*
 * \author Daniel Keefe (dfk)
 *
 * \file  vrg3d-chess.cpp
 *
 * Adapted from vrg3d_demo.cpp.
 */


#include <vrg3d/VRG3D.h>
#include <GL/glut.h>
#include "obj.h"
#include "trachea_demo.cpp"
#include <fstream>

using namespace G3D;

//extern void draw_pieces(), init_pieces();

/** This is a sample VR application using the VRG3D library.  Two key
    methods are filled in here: doGraphics() and doUserInput().  The
    code in these methods demonstrates how to draw graphics and
    respond to input from the mouse, the keyboard, 6D trackers, and VR
    wand buttons.
*/


class MyVRApp : public VRApp
{
public:
  MyVRApp(const std::string &mySetup) : VRApp()
  {
    // initialize the VRApp
     Log  *demoLog = new Log("demo-log.txt");
     init(mySetup, demoLog);
     ///_tracheaApp = new TracheaVRApp(mySetup);
     _mouseToTracker = new MouseToTracker(getCamera(), 2);

     // Initialize the coordinate frame for the display.
    _virtualToRoomSpace = CoordinateFrame();

    // The default starting point has the eye level with the chess
    // board, which is confusing for the viewer on startup, and
    // renders poorly too. Let's move the virtual space up a few units
    // for a more sensible view.
    _virtualToRoomSpace = CoordinateFrame(Vector3(0,-3.0,0)) * _virtualToRoomSpace;

    // This is the background -- the color that appears where there is
    // nothing to render, and we'll use a nice soothing blue.
    _clearColor = Color3(1.0, 1.0, 1.0);
    
    // The actual models of the chess pieces are pretty messy, so they 
    // are confined to another code file for the sake of tidiness.
    //load_trachea('cube.obj');

    intialize();
    //_trachea.read("objectFiles/entireScene.obj");//,"objectFiles/crackedDirtSized.png");
  }

  virtual ~MyVRApp() {delete _tracheaApp;}

  void intialize()  {
    std::string line;
    std::ifstream myfile ("background.obl",std::ifstream::in);
    if (myfile.is_open())
    {  
        while ( getline (myfile,line) ) 
        {
            std::vector<std::string> parts = OBJ::split(line,',');
            if(parts.size() == 3) {
              OBJ o = OBJ();
              o.read(parts[0],parts[1]);
              _background.push_back(o);
            }
        }
        myfile.close();
    }

    ifstream nextfile ("stencil.obl",std::ifstream::in);
    if (nextfile.is_open()) {
      while( getline (nextfile,line) ) {
        std::vector<std::string> parts = OBJ::split(line,',');
        if(parts.size() == 3) {
          OBJ o = OBJ();
          o.read(parts[0],parts[1]);
          _stencil.push_back(o);
        }

      }
      nextfile.close();
    }

  }

  void doUserInput(Array<VRG3D::EventRef> &events)
  {
   //_tracheaApp->doUserInput(events); 
  }

  void doGraphics(RenderDevice *rd)
  {
    // Load a font for the fps display, findVRG3DDataFile looks first
    // in the current directory and then in $G/src/VRG3D/share/
    //
    while(glGetError() != GL_NO_ERROR)
    {  std::cout<<"Flushing gl errors"<<std::endl;
    }

    if (_font.isNull())
    {  std::string fontfile = VRApp::findVRG3DDataFile("eurostyle.fnt");

       if ( FileSystem::exists( fontfile ))
       {   std::cout << fontfile << std::endl;
           _font = GFont::fromFile( fontfile );
       }
    }

    Array<std::string> trackerNames = _trackerFrames.getKeys();

    /*for (int i=0;i<trackerNames.size();i++)
    {  CoordinateFrame trackerFrame = _trackerFrames[trackerNames[i]];
    
        // Draw laser pointer.
        if (trackerNames[i] == "Wand_Tracker") {
            Vector3 lookVector = trackerFrame.lookVector();

            glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT);

            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glLineWidth(2.0f);
            glColor4f(1.0f,0.0f,0.0f,1.0f);
            glBegin(GL_LINES);
            glVertex3f(trackerFrame.translation.x,
                       trackerFrame.translation.y,
                       trackerFrame.translation.z);
            glVertex3f(trackerFrame.translation.x + 6.0 * lookVector.x,
                       trackerFrame.translation.y + 6.0 * lookVector.y,
                       trackerFrame.translation.z + 6.0 * lookVector.z);
            glEnd();

            glPopAttrib();

       }

    }*/


    // The tracker frames above are drawn with the object to world
    // matrix set to the identity because tracking data comes into the
    // system in the Room Space coordinate system.  Room Space is tied
    // to the dimensions of the room and the projection screen within
    // the room, thus it never changes as your program runs.  However,
    // it is often convenient to move objects around in a virtual
    // space that can change relative to the screen.  For these
    // objects, we put a virtual to room space transform on the OpenGL
    // matrix stack before drawing them, as is done here..
    //
    rd->disableLighting();
    rd->pushState();
    rd->setObjectToWorldMatrix(_virtualToRoomSpace);
    
      // Paramters for our light, including color and position 
    GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat position[] = {0.0, 0.0, 3.0, 0.0};
    GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat local_view[] = {0.0};

    glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv (GL_LIGHT0, GL_POSITION, position);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv (GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

      //These functions change how the object gets drawn 
    glEnable (GL_DEPTH_TEST);
    //glShadeModel(GL_SMOOTH);
    //glEnable(GL_CULL_FACE);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);

    //  Use these if you plan to scale an object 
    //glEnable (GL_AUTO_NORMAL);
    //glEnable (GL_NORMALIZE);

    // If you want to do alpha blending:
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // This draws a simple piece of geometry using G3D::Draw at the
    // origin of Virtual Space.
    //
    //    Draw::axes( CoordinateFrame(), rd,
    //          Color3::red(), Color3::green(), Color3::blue(), 0.25 );
    
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glDisable( GL_COLOR_MATERIAL ); 
    
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glDisable(GL_LIGHTING);
    //rd->disableLighting();
    for(int i = 0; i < _background.size(); i++)  {
      _background[i].draw();
    }
    for(int i = 0; i < _stencil.size(); i++)  {
      _stencil[i].draw();
    }

    // glClear(GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_STENCIL_TEST);
    // glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    // glDepthMask(GL_FALSE);
    // glStencilFunc(GL_NEVER, 1, 0xFF);
    // glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)
   
    // // draw stencil pattern
    // glStencilMask(0xFF);
    // glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF
    // for(int i = 0; i < _stencil.size(); i++)  {
    //   _stencil[i].draw();
    // }
   
    // glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    // glDepthMask(GL_TRUE);
    // glStencilMask(0x00);
    // // draw where stencil's value is 0
    // glStencilFunc(GL_EQUAL, 0, 0xFF);
    // /* (nothing to draw) */
    // // draw only where stencil's value is 1
    // glStencilFunc(GL_EQUAL, 1, 0xFF);
   
    // glColor4f(1.0f,1.0f,1.0f,.4f);
    // //_tracheaApp->doGraphics(rd);
    // glColor4f(1.0f,1.0f,1.0f,.4f);
    // for(int i = 0; i < _stencil.size(); i++)  {
    //   _stencil[i].draw();
    // }
 
    // glDisable(GL_STENCIL_TEST);
    rd->popState();

    glDisable(GL_LIGHTING);
    rd->disableLighting();
     
}

protected:
  Table<std::string, CoordinateFrame> _trackerFrames;
  GFontRef          _font;
  MouseToTrackerRef _mouseToTracker;
  CoordinateFrame   _virtualToRoomSpace,_oldVirtualToRoomSpace;
  std::vector<OBJ>  _background,_stencil;
  bool              _train,_grab,_rotate;
  TracheaVRApp      *_tracheaApp;
};




int main( int argc, char **argv )
{
  // The first argument to the program tells us which of the known VR
  // setups to start
  std::string setupStr;
  MyVRApp *app;
  
  if (argc >= 2)
  {  setupStr = std::string(argv[1]);
  }

  // This opens up the graphics window, and starts connections to
  // input devices, but doesn't actually start rendering yet.
    app = new MyVRApp(setupStr);

    // This starts the rendering/input processing loop
    app->run();

  return 0;
}


//
////////////////////  end  common/vrg3d/demo/vrg3d_demo.cpp  ///////////////////

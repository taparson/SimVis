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

  virtual ~MyVRApp() {}

  void intialize()  {
    _grab = false;
    _train = true;
    if(_train)  {
      OBJ t = OBJ();
      t.read("objectFiles/Pterostichus2-1-lq.obj");
      _trachea.push_back(t);
    }
    else  {
      for (int i = 0; i < 25; i++)  {
        OBJ t = OBJ();
        std::string str = "objectFiles/P_Surface";
        std::stringstream ss;
        ss << i;
        str += ss.str();
        str += ".obj";
        t.read(str);
        _trachea.push_back(t);
      }
    }
  }

  void doUserInput(Array<VRG3D::EventRef> &events)
  {
    // MouseToTracker is a really helpful class for testing out VR
    // interactions from the desktop.  This call makes it respond to
    // mouse events and generate new events as if it were a 6DOF
    // tracking device.  We add the new events to the event queue and
    // process them as usual.

    static double joystick_x = 0.0;
    static double joystick_y = 0.0;

    Array<VRG3D::EventRef> newEvents;
    _mouseToTracker->doUserInput(events, newEvents);
    events.append(newEvents);


    for (int i = 0; i < events.size(); i++) {

      if (events[i]->getName() == "kbd_ESC_down") {

	// Exit the program.
        while(glGetError() != GL_NO_ERROR) {  
	  std::cout<<"Flushing gl errors"<<std::endl;
	}

        exit(0);
      }

      // Save all the tracker events that come in so we can use them
      // in the doGraphics routine
      else if (endsWith(events[i]->getName(), "_Tracker"))
      {
        if (_trackerFrames.containsKey(events[i]->getName()))
        {
           _trackerFrames[events[i]->getName()] = events[i]->getCoordinateFrameData();
        }
        else
        {  _trackerFrames.set( events[i]->getName(),
                               events[i]->getCoordinateFrameData() );
        }
      }

      // Respond to events to do some simple navigation
      else if (events[i]->getName() == "kbd_LEFT_down")
      {  _virtualToRoomSpace = CoordinateFrame(Matrix3::fromAxisAngle(Vector3(0,1,0), toRadians(5.0))) * _virtualToRoomSpace;
      }
      else if (events[i]->getName() == "kbd_RIGHT_down")
      {  _virtualToRoomSpace = CoordinateFrame(Matrix3::fromAxisAngle(Vector3(0,1,0), toRadians(-5.0))) * _virtualToRoomSpace;
      }
      else if (events[i]->getName() == "kbd_UP_down")
      {  _virtualToRoomSpace = CoordinateFrame(Matrix3::fromAxisAngle(Vector3(1,0,0), toRadians(5.0))) * _virtualToRoomSpace;
      }
      else if (events[i]->getName() == "kbd_DOWN_down")
      {  _virtualToRoomSpace = CoordinateFrame(Matrix3::fromAxisAngle(Vector3(1,0,0), toRadians(-5.0))) * _virtualToRoomSpace;
      }
      else if (events[i]->getName() == "kbd_SHIFT_LEFT_down")
      {  _virtualToRoomSpace = CoordinateFrame(Vector3(-0.1,0,0)) * _virtualToRoomSpace;
      }
      else if (events[i]->getName() == "kbd_SHIFT_RIGHT_down")
      {  _virtualToRoomSpace = CoordinateFrame(Vector3(0.1,0,0)) * _virtualToRoomSpace;
      }
      else if (events[i]->getName() == "kbd_SHIFT_UP_down")
      {  _virtualToRoomSpace = CoordinateFrame(Vector3(0,0.1,0)) * _virtualToRoomSpace;
      }
      else if (events[i]->getName() == "kbd_SHIFT_DOWN_down")
      {  _virtualToRoomSpace = CoordinateFrame(Vector3(0,-0.1,0)) * _virtualToRoomSpace;
      }


      // Some printouts for other events, just to show how to access other types of event data
      else if (events[i]->getName() == "kbd_SPACE_down")
      {  cout << "Pressed the space key." << endl;
      }
      else if (events[i]->getName() == "Wand_Btn1_down")
      {  cout << "Wand btn 1 pressed." << endl;
      }
      else if (events[i]->getName() == "Wand_Middle_Btn_down")
      {  _grab = true;
      }
      else if (events[i]->getName() == "Wand_Middle_Btn_up")
      {  _grab = false;
      }
      else if (events[i]->getName() == "Wand_Btn2_down")
      {  cout << "Wand btn 2 pressed." << endl;
      }
      else if (events[i]->getName() == "Wand_Btn3_down")
      {  cout << "Wand btn 3 pressed." << endl;
      }
      else if (events[i]->getName() == "Wand_Btn4_down")
      {  cout << "Wand btn 4 pressed." << endl;
      }
      else if (events[i]->getName() == "Wand_Btn6_down")
      {  cout << "Wand btn 6 pressed." << endl;
      }
      else if (events[i]->getName() == "Wand_Btn5_down")
      {  cout << "Wand joystick btn pressed." << endl;
      }
      else if (events[i]->getName() == "Wand_Btn6_down")
      {  cout << "Wand trigger btn pressed." << endl;
      }
      else if (events[i]->getName() == "Wand_Joystick_X")
      {   //cout << "Wand Joystick X = " << events[i]->get1DData() << endl;
          joystick_x = events[i]->get1DData();
      }
      else if (events[i]->getName() == "Wand_Joystick_Y")
      {   //cout << "Wand Joystick Y = " << events[i]->get1DData() << endl;
          joystick_y = events[i]->get1DData();
      }
      else if (events[i]->getName() == "Mouse_Pointer")
      {  static Vector2 lastPos;

         if (events[i]->get2DData() != lastPos)
         {  //  cout << "New mouse position = " << events[i]->get2DData() << endl;
            lastPos = events[i]->get2DData();
         }
      }
      else if (events[i]->getName() == "Mouse_Left_Btn_down")
      {  cout << "Mouse left btn pressed at position " << events[i]->get2DData() << endl;
      }
      else if (beginsWith(events[i]->getName(), "kbd_"))
      {  cout << "Keyboard event 1: " << events[i]->getName() << endl;
	cout << getCamera()->getHeadFrame() << endl;
	cout << _virtualToRoomSpace << endl;
      }
      else if (events[i]->getName() == "SpaceNav_Trans")
      {  cout << "Keyboard event 2: " << events[i]->getName()<< events[i]->get3DData() << endl;
      }
      else if (events[i]->getName() == "SpaceNav_Rot")
      {  cout << "Keyboard event 3: " << events[i]->getName()<< events[i]->get3DData() << endl;
      }
      else if (beginsWith(events[i]->getName(), "TNG_An"))
      {  cout << events[i]->getName() << "  " << events[i]->get1DData() << endl;
      }
      else if (events[i]->getName() == "SynchedTime")
      {  continue;
      }
      else
      {  // This will print out the names of all events, but can be too
         // much if you are getting several tracker updates per frame.
         // Uncomment this to see everything..
         cout << events[i]->getName() << endl;
      }

      // For debugging tracker coordinate systems, it can be useful to print out
      // tracker positions, like this:
      //
      if (events[i]->getName() == "Test_Tracker")
      {  cout << events[i]->getName() << " " << events[i]->getCoordinateFrameData().translation << endl;
      }
      
    // Rotate
    if(!_grab) {
      if (fabs(joystick_x) > 0.01) {
        //fprintf(stderr, "Joystick x: %lf\n", joystick_x);
        double angle = M_PI/180.0*joystick_x;
        angle /= 15.0;
        CoordinateFrame rotation = CoordinateFrame(Matrix3::fromEulerAnglesXYZ(0,angle,0));
        G3D::Vector3 translation = _virtualToRoomSpace.translation;
        //_virtualToRoomSpace.moveTowards(_trackerFrames[string("Wand_Tracker")],std::numeric_limits<float>::max(),0);
        _virtualToRoomSpace.translation = _trackerFrames[string("Wand_Tracker")].translation;
        _virtualToRoomSpace = rotation*_virtualToRoomSpace;
        _virtualToRoomSpace.translation = translation;
      }

      // Translate
      if (fabs(joystick_y) > 0.0 && _trackerFrames.containsKey("Wand_Tracker") == true) {
        _virtualToRoomSpace.translation -= .015f*joystick_y*_trackerFrames[string("Wand_Tracker")].lookVector();
      }
    }
    else  {
      if (fabs(joystick_x) > 0.01) {
        //fprintf(stderr, "Joystick x: %lf\n", joystick_x);
        double angle = M_PI/180.0*joystick_x;
        angle /= 15.0;
        CoordinateFrame rotation = CoordinateFrame(Matrix3::fromEulerAnglesXYZ(0,0,angle));
        G3D::Vector3 translation = _virtualToRoomSpace.translation;
        //_virtualToRoomSpace.moveTowards(_trackerFrames[string("Wand_Tracker")],std::numeric_limits<float>::max(),0);
        _virtualToRoomSpace = rotation*_virtualToRoomSpace;
        _virtualToRoomSpace.translation = translation;
      }

      // Translate
      if (fabs(joystick_y) > 0.0 && _trackerFrames.containsKey("Wand_Tracker") == true) {
        //fprintf(stderr, "Joystick x: %lf\n", joystick_x);
        double angle = M_PI/180.0*joystick_x;
        angle /= 15.0;
        CoordinateFrame rotation = CoordinateFrame(Matrix3::fromEulerAnglesXYZ(angle,0,0));
        G3D::Vector3 translation = _virtualToRoomSpace.translation;
        //_virtualToRoomSpace.moveTowards(_trackerFrames[string("Wand_Tracker")],std::numeric_limits<float>::max(),0);
        _virtualToRoomSpace = rotation*_virtualToRoomSpace;
        _virtualToRoomSpace.translation = translation;
      }
    }
     
    }  
    
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
    GLfloat position[] = {0.0, 3.0, 3.0, 0.0};
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
    
    glPushMatrix();    
    glScalef(.001,.001,.001);
    glColor3f(0.794,0.794,.794);
    glEnable(GL_NORMALIZE);
    for (int i = 0; i < _trachea.size(); i++)  {
      _trachea[i].draw();
    }
    glPopMatrix();
    glColor3f(1.0,1.0,1.0);
    rd->popState();

    glDisable(GL_LIGHTING);
    rd->disableLighting();
     
}

protected:
  Table<std::string, CoordinateFrame> _trackerFrames;
  GFontRef          _font;
  MouseToTrackerRef _mouseToTracker;
  CoordinateFrame   _virtualToRoomSpace;
  std::vector<OBJ>  _trachea;
  bool              _train,_grab;
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

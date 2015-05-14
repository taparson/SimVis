CPP = g++
CPPFLAGS = -Wall -g
LDFLAGS = 

TARGET = chess demo trachea granoff


### These lines are for access to the VRG3D and G3D installation
### directories. All other necessary software is assumed to be
### installed in the system-wide directories.

ifeq ($(shell if [ -d /research/graphics ] ; then echo 1; fi),1)

  # CS Dept
  EXTRA_INCLUDES = -I$(G)/install_linux/include -I$(G)/install_linux/include/vrg3d
  EXTRA_LIBS = -L$(G)/install_linux/lib -L$(G)/install_linux/lib/vrg3d
  SCALABLE_LIB = 

else
ifeq ($(shell if [ -d /gpfs/runtime/opt/cave/ccv ] ; then echo 1; fi), 1)

  # Yurt
  EXTRA_INCLUDES = -I$(G)/install_linux/include -I$(G)/install_linux/include/vrg3d
  EXTRA_LIBS = -L$(G)/install_linux/lib -L$(G)/install_linux/lib/vrg3d
  SCALABLE_LIB = -Xlinker -lmplEasyBlendSDK

else

  # CCV - Granoff
  EXTRA_INCLUDES = -I/share/cave/include -I$(G)/install_linux/include -I$(G)/install_linux/include/vrg3d  -I/gpfs/runtime/opt/vrpn/7.31/include
  EXTRA_LIBS = -L/share/cave/lib  -L$(G)/install_linux/lib -L$(G)/install_linux/lib/vrg3d -L/gpfs/runtime/opt/vrpn/7.31/lib
  SCALABLE_LIB = 

endif
endif


OBJDIR = obj

SRCS = $(shell ls *.cpp)

OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

INCLUDE_DIRS = -I. $(EXTRA_INCLUDES)

SOIL_BASE = soil

INCLUDE_DIRS += -I$(SOIL_BASE)

LIB_DIRS = $(EXTRA_LIBS)

LIB_DIRS += -L$(SOIL_BASE)

LIBS = -Xlinker --start-group  -Xlinker -ldl  -Xlinker -lX11 -Xlinker \
	-lXext  -Xlinker -lpthread  -Xlinker -lz  -Xlinker -ljpeg  -Xlinker \
	-lpng  -Xlinker -lzip  -Xlinker -lSDL -Xlinker -lvrg3d  -Xlinker \
	-lavutil  -Xlinker -lavformat  -Xlinker -lavcodec  -Xlinker -lGLG3D \
	-Xlinker -lG3D -Xlinker -lGLU -Xlinker -lGL -Xlinker -lvrpn \
	-Xlinker -lglut -Xlinker -lXmu $(SCALABLE_LIB) \
	-Xlinker --end-group \
        -lSOIL

all: chess demo trachea granoff

chess: obj/chess.o obj/chess_demo.o
	$(CPP) $(LDFLAGS) -o chess $^ $(LIB_DIRS) $(LIBS)

demo: obj/vrg3d_demo.o
	$(CPP) $(LDFLAGS) -o demo $^ $(LIB_DIRS) $(LIBS)

trachea: obj/trachea_demo.o obj/obj.o
	$(CPP) $(LDFLAGS) -o trachea $^ $(LIB_DIRS) $(LIBS)

granoff: obj/granoff_demo.o obj/obj.o obj/trachea_demo.o
	$(CPP) $(LDFLAGS) -o granoff $^ $(LIB_DIRS) $(LIBS)	
#$(TARGET) : $(OBJS) 
#	$(CPP) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIB_DIRS) $(LIBS)

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(OBJDIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@ $(INCLUDE_DIRS)

clean:
	rm -f $(TARGET) $(OBJDIR)/*.o demo-log.txt
	rm -rf $(OBJDIR)

Kamikaze!

An overtime project for the course of Game Engine dev.
Friday Night, 2016-06-04, 20:00 24:00

Dependencies: 
   SDL 2.0 (for SO interfacing)
   glm (for math functions)

V 0.1

Implementation notes:
cpp files do not implement methods of a single class:
instead, they implement relevant methods of all classes.

main.cpp : 
    interfaces with SO (windows, keyboard, event loop). 
    Uses SDL. 
physics_engine.cpp : 
    all phyiscal evolution methods
rendering_engine.cpp: 
    all rendering methods, plus a global "rendering" function (call me).
    Uses OpenGL.
    Very preliminary: all graphics is placeholders for now.
gamelogic.cpp:
    gamelogic. these would be scripts in Unity.
    
   
tranform.h:
scene.h: (includes classes Ship, and Bullet, for now)
phys_obj.h:
controller.h:
    the data structures.

   
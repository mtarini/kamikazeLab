Kamikaze!

An overtime project for the course of Game Engine dev.
http://vcg.isti.cnr.it/~tarini/teaching/engines2016/

Fri. 2016-06-04, 18:00PM 05:00AM
Fri. 2016-06-24, 17:00PM 02:30AM
Sat. 2016-06-25, 09:00PM 12:00AM

Dependencies: 
   SDL 2.0 (for SO interfacing)
   glm (for math functions)
   ...and nothing else!

V 0.1

A few notes on the structure of the code:

cpp files do not implement methods of a single class:
instead, each implements all methods/global funciton of a 
mdule of a game engine of all classes.

main.cpp : 
    interfaces with SO (windows, keyboard, event loop). 
    Uses SDL. 
physics_engine.cpp : 
    all phyiscal evolution methods
rendering_engine.cpp: 
    all rendering methods, plus a global "rendering" function (call me).
    Uses OpenGL.
asset_manager.cpp
    importers (and/or procedural creation) of any asset
ai.cpp
    all for NPC behaviour
gamelogic.cpp:
    gamelogic: stats, special behaviour of custom objects. 
    (anything which would be scripts in Unity).

Classes / headers for:
    tranform.h  (roto-translation-scale: with quaternion)
    phys_obj.h  (any object in a game)
    controller.h  (the immediate "will" of a ship)
    mesh.h  (indexed, triangular mesh: the appearance of an object)
    texture.h  (just diffuse texture for now)
    collider.h  (geometry proxy for collision detection -- aka hitbox)
    ai_mind.h  (the internal state of an AI controlled NPC) 
    custom_classes.h: (includes classes Ship, Bullet, Arena)
	

more Info inside each file!
   
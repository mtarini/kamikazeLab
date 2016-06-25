#ifndef COLLIDER_H
#define COLLIDER_H

/* Collider (aka hit-box)
 *
 * A geometry proxy for collision detection:
 * Currently, only one type: sphere.
 *
 * TODO:
 *   add new types of colliders (e.g. capsules)
 *   return collision data of collisions (point of impact, normal of said point...),
 *   for better collision detection.
 *
 */
class Collider{
public:
	enum{ SPHERE } type;
	float radius;
};


#endif // COLLIDER_H

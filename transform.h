#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

struct Transform{
	vec3 pos;
	quat ori;
	float scale;

	void setIde(){
		pos = vec3(0,0,0);
		ori = quat(1,0,0,0);
		scale = 1;
	}

	vec3 forward() const {
		//return
		// quat::rotate(ori, vec3(0,0,1) );
		quat tmp = (ori * quat(0, 0,1,0) * conjugate(ori));
		return vec3( tmp.x, tmp.y, tmp.z );

	}

	void setModelMatrix() const;

};

#endif // TRANSFORM_H

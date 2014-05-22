#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "..\..\Core\Component.h"

#include "MeshCache.h"
#include "..\..\Core\GameObject.h"

class RenderingComponent : public Component
{
public:
	MeshData mesh;

	void draw();
	RenderingComponent():Component()
	{
		id = 11;
	}

private:
};

#endif /*MESHCOMPONENT_H*/
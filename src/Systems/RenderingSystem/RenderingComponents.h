#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "..\Component.h"

#include "MeshCache.h"
#include "..\..\Core\GameObject.h"

struct MeshComponent : Component
{
	MeshData mesh;
	GameObject* owner;
};

#endif /*MESHCOMPONENT_H*/
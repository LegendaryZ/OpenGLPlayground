#include "Vertex.h"

Vertex::Vertex(void)
{
	position[0] = 0;
	position[1] = 0;
	position[2] = 0;

	normal[0] = 0;
	normal[1] = 0;
	normal[2] = 0;

	tangent[0] = 0;
	tangent[1] = 0;
	tangent[2] = 0;

	texC[0] = 0;
	texC[1] = 0;
}

Vertex::Vertex( float px, float py, float pz, 
				float nx, float ny, float nz,
				float tx, float ty, float tz,
				float u, float v)
{
	position[0] = px;
	position[1] = py;
	position[2] = pz;

	normal[0] = nx;
	normal[1] = ny;
	normal[2] = nz;

	tangent[0] = tx;
	tangent[1] = ty;
	tangent[2] = tz;

	texC[0] = u;
	texC[1] = v;
}


Vertex::~Vertex(void)
{
}

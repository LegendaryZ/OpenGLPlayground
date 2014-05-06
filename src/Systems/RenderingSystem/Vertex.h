#ifndef VERTEX_H
#define VERTEX_H

class Vertex
{
public:
	Vertex(void);
	Vertex( float px, float py, float pz, 
		    float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v);
	~Vertex(void);

	float position[3];
	float normal[3];
	float tangent[3];
	float texC[2];
};

#endif /*VERTEX_H*/
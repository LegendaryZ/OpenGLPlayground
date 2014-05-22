#ifndef VERTEX_H
#define VERTEX_H

#pragma pack(push, 16)

struct Vertex
{
	float data[16];

	Vertex(void)
	{
		data[0] = 0;
		data[1] = 0;
		data[2] = 0;
		data[3] = 0;
		data[4] = 0;
		data[5] = 0;
		data[6] = 0;
		data[7] = 0;
		data[8] = 0;
		data[9] = 0;
		data[10] = 0;
		data[11] = 0;
		data[12] = 0;
		data[13] = 0;
		data[14] = 0;
		data[15] = 0;
	}

	Vertex( float px, float py, float pz, float pw, 
			float cx, float cy, float cz, float cw,
		    float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v)
	{
		data[0] = px;
		data[1] = py;
		data[2] = pz;
		data[3] = pw;

		data[4] = cx;
		data[5] = cy;
		data[6] = cz;
		data[7] = cw;

		data[8] = nx;
		data[9] = ny;
		data[10] = nz;

		data[11] = tx;
		data[12] = ty;
		data[13] = tz;

		data[14] = u;
		data[15] = v;
	}
};

#pragma pack(pop)

#endif /*VERTEX_H*/
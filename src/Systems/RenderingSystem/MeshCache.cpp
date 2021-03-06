#include "MeshCache.h"

/**
 * @return MeshCache The Singleton Mesh Cache
 **/
MeshCache* MeshCache::getInstance()
{
	static MeshCache* cache;
	if(!cache)
		cache = new MeshCache();
	return cache;
}

MeshCache::MeshCache(void)
{
}


MeshCache::~MeshCache(void)
{
}

/**
 * Load a set of default geomtry into the mesh cache. Includes
 * a box, a sphere, a plane, and a fullscreen quad.
 *
 * @return whether the data was succefully loaded or not
 **/
bool MeshCache::loadBasicGeometry()
{
	//
	// Build a 1x1x1 box
	//

	MeshData* box = new MeshData();

	// Create the vertices.
	Vertex* v = new Vertex[24];

	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;
    
	// Fill in the front face vertex data.
	v[0] = Vertex(-w2, -h2, -d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-w2, +h2, -d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(+w2, +h2, -d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex(+w2, -h2, -d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = Vertex(-w2, -h2, +d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = Vertex(+w2, -h2, +d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = Vertex(+w2, +h2, +d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = Vertex(-w2, +h2, +d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8]  = Vertex(-w2, +h2, -d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9]  = Vertex(-w2, +h2, +d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex(+w2, +h2, +d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(+w2, +h2, -d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = Vertex(-w2, -h2, -d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = Vertex(+w2, -h2, -d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(+w2, -h2, +d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = Vertex(-w2, -h2, +d2, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = Vertex(-w2, -h2, +d2, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = Vertex(-w2, +h2, +d2, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = Vertex(-w2, +h2, -d2, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = Vertex(-w2, -h2, -d2, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = Vertex(+w2, -h2, -d2, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = Vertex(+w2, +h2, -d2, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = Vertex(+w2, +h2, +d2, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = Vertex(+w2, -h2, +d2, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	box->Vertices.assign(&v[0], &v[24]);

	// Create the indices.
	int* i= new int[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	box->Indices.assign(&i[0], &i[36]);

	if(!insertMeshData("box", *box))
		return false;

	return true;
}

/**
 * Retrieve mesh data from the cache
 * 
 * @param key The key of the mesh to be looking for
 *
 * @return MeshData	The MeshData that was requested or an empty set of mesh data
 **/
MeshData MeshCache::getMeshData(const char* key)
{
	std::map<const char*, MeshData>::iterator itr = meshDatas.find(key);

	if(itr == meshDatas.end())
		return MeshData();

	return itr->second;
}

/**
 * Inserts a mesh into the cache
 *
 * @param key  how the mesh will be referenced
 * @param data The Mesh Data to be inserted
 *
 * @return succesfull insertion or failure
 **/
bool MeshCache::insertMeshData(const char* key, MeshData data)
{
	std::map<const char*, MeshData>::iterator itr = meshDatas.find(key);

	if(itr == meshDatas.end())
	{
		meshDatas.insert(std::pair<const char*, MeshData>(key, data));
		return true;
	}
	
	return false;
}

/**
 * Erase a mesh from the cache.
 *
 * @param key	The key of the mesh to be erased
 *
 * @return success or failure
 **/
bool MeshCache::eraseMesh(const char* key)
{
	std::map<const char*, MeshData>::iterator itr = meshDatas.find(key);

	if(itr == meshDatas.end())
		return false;
	
	delete[] itr->first;
	itr->second.Vertices.clear();
	itr->second.Indices.clear();

	meshDatas.erase(itr);
	return true;
}
#ifndef MESHCACHE_H
#define MESHCACHE_H

#include "Vertex.h"
#include <vector>
#include <map>

/**
 * A basic data structure for storing vertexes and indicies of
 * those vertexes to define a mesh
 **/
struct MeshData
{
	std::vector<Vertex> Vertices;
	std::vector<int> Indices;
};

/**
 * A cache for storing mesh data. Only stores each kind of mesh once
 **/
class MeshCache
{
public:
	static MeshCache* getInstance();

	bool loadBasicGeometry();
	MeshData getMeshData(const char* key);
	bool insertMeshData(const char* key, MeshData meshData);
	bool eraseMesh(const char* key);

private:
	std::map<const char*, MeshData> meshDatas; //<! The data structure storing the meshes

	MeshCache(void);
	~MeshCache(void);
};

#endif /*MESHCACHE_H*/


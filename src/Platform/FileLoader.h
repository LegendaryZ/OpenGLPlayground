#pragma once

#include <Windows.h>
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>

//The code for this OBJLoader was taken from : http://www.braynzarsoft.net/Code/index.php?p=VC&code=Obj-Model-Loader

class RenderManager;

class FileLoader
{

public:
	FileLoader(void);
	~FileLoader(void);

	bool LoadFile(ID3D11Device* device, 
	std::wstring fileName, 
	std::string fileNameS,
	ObjModel& objModel,
	std::vector<GameMaterial>& material, 
	std::vector<SurfaceMaterial>& surface, 
    TextureManager& textureMan,
    bool isRHCoordSys,
    bool computeNormals,
	bool flipFaces);

private:
	RenderManager *renderMan;
};

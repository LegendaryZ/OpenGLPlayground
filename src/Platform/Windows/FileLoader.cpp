#include "..\FileLoader.h"


FileLoader::FileLoader(void)
{
	//renderMan = &RenderManager::getInstance();
}

FileLoader::~FileLoader(void)
{
}

bool FileLoader::LoadFile( ID3D11Device* device,
    std::wstring fileName,
	std::string fileNameS,
    ObjModel& objModel,
    std::vector<GameMaterial>& material, 
	std::vector<SurfaceMaterial>& surface,
    TextureManager& textureMan,
    bool isRHCoordSys,
    bool computeNormals,
    bool flipFaces)
{
	#pragma region Base Variable creation and assignments
	HRESULT hr = 0;
	
    std::wifstream fileIn (fileName.c_str());   // Open file
    std::wstring meshMatLib;                    // String to hold our obj material library filename (model.mtl)

    // Arrays to store our model's information
    std::vector<DWORD> indices;
    std::vector<XMFLOAT3> vertPos;
    std::vector<XMFLOAT3> vertNorm;
    std::vector<XMFLOAT2> vertTexCoord;
    std::vector<std::wstring> meshMaterials;

    // Vertex definition indices
    std::vector<int> vertPosIndex;
    std::vector<int> vertNormIndex;
    std::vector<int> vertTCIndex;

    // Make sure we have a default if no tex coords or normals are defined
    bool hasTexCoord = false;
    bool hasNorm = false;

    // Temp variables to store into vectors
    std::wstring meshMaterialsTemp;
    int vertPosIndexTemp;
    int vertNormIndexTemp;
    int vertTCIndexTemp;

    wchar_t checkChar;      // The variable we will use to store one char from file at a time
    std::wstring face;      // Holds the string containing our face vertices
    int vIndex = 0;         // Keep track of our vertex index count
    int triangleCount = 0;  // Total Triangles
    int totalVerts = 0;
    int meshTriangles = 0;
    bool ang = false;
	#pragma endregion
    
	#pragma region OBJ File Contents
	//Check to see if the file was opened
    fileIn.open((L"Assets//" + fileName).c_str());
	
	if (fileIn)
    {
        while(fileIn)
        {           
            checkChar = fileIn.get();   //Get next char

            switch (checkChar)
            {   
			#pragma region '#' Case - Comments
            case '#': // A comment. Skip rest of the line
                checkChar = fileIn.get();
                while(checkChar != '\n')
                    checkChar = fileIn.get();
                break;
			#pragma endregion
			#pragma region 'v' Case - Vertex Descriptions
            case 'v': // Get Vertex Descriptions;
                checkChar = fileIn.get();
                if(checkChar == ' ')  // v - vert position
                {
                    float vz, vy, vx;
                    fileIn >> vx >> vy >> vz;   // Store the next three types

                    if(isRHCoordSys)            // If model is from an RH Coord System
                        vertPos.push_back(XMFLOAT3( vx, vy, vz * -1.0f));   // Invert the Z axis
                    else
                        vertPos.push_back(XMFLOAT3( vx, vy, vz));
                }
                if(checkChar == 't')  // vt - vert tex coords
				{
                    float vtcu, vtcv;
                    fileIn >> vtcu >> vtcv;     // Store next two types

                    if(!isRHCoordSys)            // If model is from an RH Coord System
                        vertTexCoord.push_back(XMFLOAT2(vtcu, 1.0f-vtcv));  // Reverse the "v" axis
                    else
                        vertTexCoord.push_back(XMFLOAT2(vtcu, vtcv));   

                    hasTexCoord = true;         // We know the model uses texture coords
                }
                if(checkChar == 'n')          // vn - vert normal
                {
                    float vnx, vny, vnz;
                    fileIn >> vnx >> vny >> vnz;    // Store next three types

                    if(isRHCoordSys)                // If model is from an RH Coord System
                        vertNorm.push_back(XMFLOAT3( vnx, vny, vnz * -1.0f ));  // Invert the Z axis
                    else
                        vertNorm.push_back(XMFLOAT3( vnx, vny, vnz ));  

                    hasNorm = true;                 // We know the model defines normals
                }
                break;
			#pragma endregion
			#pragma region 'g' Case - New Group (Subsets!)
                // New group (Subset)
            case 'g': // g - defines a group
                checkChar = fileIn.get();
                if(checkChar == ' ')
                {
                    ang = true;
                    objModel.SubsetIndexStart.push_back(vIndex);       // Start index for this subset
                    objModel.Subsets++;
                }
                break;
			#pragma endregion
			#pragma region 'f' Case - Face Index
                // Get Face Index
            case 'f': // f - defines the faces
                checkChar = fileIn.get();
                if(checkChar == ' ')
                {
                    face = L"";               // Holds all vertex definitions for the face (eg. "111 222 333")
                    std::wstring VertDef;   // Holds one vertex definition at a time (eg. "111")
                    triangleCount = 0;      // Keep track of the triangles for this face, since we will have to 
                                            // "retriangulate" faces with more than 3 sides

                    checkChar = fileIn.get();
                    while(checkChar != '\n')
                    {
                        face += checkChar;          // Add the char to our face string
                        checkChar = fileIn.get();   // Get the next Character
                        if(checkChar == ' ')      // If its a space...
                            triangleCount++;        // Increase our triangle count
                    }

                    // Check for space at the end of our face string
                    if(face[face.length()-1] == ' ')
                        triangleCount--;    // Each space adds to our triangle count

                    triangleCount -= 1;     // Ever vertex in the face AFTER the first two are new faces

                    std::wstringstream ss(face);

                    if(face.length() > 0)
                    {
                        int firstVIndex, lastVIndex;    // Holds the first and last vertice's index

                        for(int i = 0; i < 3; ++i)      // First three vertices (first triangle)
                        {
                            ss >> VertDef;  // Get vertex definition (vPos/vTexCoord/vNorm)

                            std::wstring vertPart;
                            int whichPart = 0;      // (vPos, vTexCoord, or vNorm)

                            // Parse this string
                            for(unsigned int j = 0; j < VertDef.length(); ++j)
                            {
                                if(VertDef[j] != '/') // If there is no divider "/", add a char to our vertPart
                                    vertPart += VertDef[j];

                                // If the current char is a divider "/", or its the last character in the string
                                if(VertDef[j] == '/' || j ==  VertDef.length()-1)
                                {
                                    std::wistringstream wstringToInt(vertPart); // Used to convert wstring to int

                                    if(whichPart == 0)  // If it's the vPos
                                    {
                                        wstringToInt >> vertPosIndexTemp;
                                        vertPosIndexTemp -= 1;      // subtract one since c++ arrays start with 0, and obj start with 1

                                        // Check to see if the vert pos was the only thing specified
                                        if(j == VertDef.length()-1)
                                        {
                                            vertNormIndexTemp = 0;
                                            vertTCIndexTemp = 0;
                                        }
                                    }

                                    else if(whichPart == 1) // If vTexCoord
                                    {
                                        if(vertPart != L"")   // Check to see if there even is a tex coord
                                        {
                                            wstringToInt >> vertTCIndexTemp;
                                            vertTCIndexTemp -= 1;   // subtract one since c++ arrays start with 0, and obj start with 1
                                        }
                                        else    // If there is no tex coord, make a default
                                            vertTCIndexTemp = 0;

                                        // If the cur. char is the second to last in the string, then
                                        // there must be no normal, so set a default normal
                                        if(j == VertDef.length()-1)
                                            vertNormIndexTemp = 0;

                                    }                               
                                    else if(whichPart == 2) // If vNorm
                                    {
                                        std::wistringstream wstringToInt(vertPart);

                                        wstringToInt >> vertNormIndexTemp;
                                        vertNormIndexTemp -= 1;     // subtract one since c++ arrays start with 0, and obj start with 1
                                    }

                                    vertPart = L"";   // Get ready for next vertex part
                                    whichPart++;    // Move on to next vertex part                  
                                }
                            }

                            // Check to make sure there is at least one subset
                            if(objModel.Subsets == 0)
                            {
                                objModel.SubsetIndexStart.push_back(vIndex);       // Start index for this subset
                                objModel.Subsets++;
                            }

                            // Avoid duplicate vertices
                            bool vertAlreadyExists = false;
                            if(totalVerts >= 3) // Make sure we at least have one triangle to check
                            {
                                // Loop through all the vertices
                                for(int iCheck = 0; iCheck < totalVerts; ++iCheck)
                                {
                                    // If the vertex position and texture coordinate in memory are the same
                                    // As the vertex position and texture coordinate we just now got out
                                    // of the obj file, we will set this faces vertex index to the vertex's
                                    // index value in memory. This makes sure we don't create duplicate vertices
                                    if(vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
                                    {
                                        if(vertTCIndexTemp == vertTCIndex[iCheck])
                                        {
                                            objModel.Indices.push_back(iCheck);        // Set index for this vertex
                                            vertAlreadyExists = true;       // If we've made it here, the vertex already exists
                                        }
                                    }
                                }
                            }

                            // If this vertex is not already in our vertex arrays, put it there
                            if(!vertAlreadyExists)
                            {
                                vertPosIndex.push_back(vertPosIndexTemp);
                                vertTCIndex.push_back(vertTCIndexTemp);
                                vertNormIndex.push_back(vertNormIndexTemp);
                                totalVerts++;   // We created a new vertex
                                objModel.Indices.push_back(totalVerts-1);  // Set index for this vertex
                            }                           

                            // If this is the very first vertex in the face, we need to
                            // make sure the rest of the triangles use this vertex
                            if(i == 0)
                            {
                                firstVIndex = objModel.Indices[vIndex];    //The first vertex index of this FACE

                            }

                            // If this was the last vertex in the first triangle, we will make sure
                            // the next triangle uses this one (eg. tri1(1,2,3) tri2(1,3,4) tri3(1,4,5))
                            if(i == 2)
                            {                               
                                lastVIndex = objModel.Indices[vIndex]; // The last vertex index of this TRIANGLE
                            }
                            vIndex++;   // Increment index count
                        }

                        meshTriangles++;    // One triangle down

                        // If there are more than three vertices in the face definition, we need to make sure
                        // we convert the face to triangles. We created our first triangle above, now we will
                        // create a new triangle for every new vertex in the face, using the very first vertex
                        // of the face, and the last vertex from the triangle before the current triangle
                        for(int l = 0; l < triangleCount-1; ++l)    // Loop through the next vertices to create new triangles
                        {
                            // First vertex of this triangle (the very first vertex of the face too)
                            objModel.Indices.push_back(firstVIndex);           // Set index for this vertex
                            vIndex++;

                            // Second Vertex of this triangle (the last vertex used in the tri before this one)
                            objModel.Indices.push_back(lastVIndex);            // Set index for this vertex
                            vIndex++;

                            // Get the third vertex for this triangle
                            ss >> VertDef;

                            std::wstring vertPart;
                            int whichPart = 0;

                            // Parse this string (same as above)
                            for(unsigned int j = 0; j < VertDef.length(); ++j)
                            {
                                if(VertDef[j] != '/')
                                    vertPart += VertDef[j];
                                if(VertDef[j] == '/' || j ==  VertDef.length()-1)
                                {
                                    std::wistringstream wstringToInt(vertPart);

                                    if(whichPart == 0)
                                    {
                                        wstringToInt >> vertPosIndexTemp;
                                        vertPosIndexTemp -= 1;

                                        // Check to see if the vert pos was the only thing specified
                                        if(j == VertDef.length()-1)
                                        {
                                            vertTCIndexTemp = 0;
                                            vertNormIndexTemp = 0;
                                        }
                                    }
                                    else if(whichPart == 1)
                                    {
                                        if(vertPart != L"")
                                        {
                                            wstringToInt >> vertTCIndexTemp;
                                            vertTCIndexTemp -= 1;
                                        }
                                        else
                                            vertTCIndexTemp = 0;
                                        if(j == VertDef.length()-1)
                                            vertNormIndexTemp = 0;

                                    }                               
                                    else if(whichPart == 2)
                                    {
                                        std::wistringstream wstringToInt(vertPart);

                                        wstringToInt >> vertNormIndexTemp;
                                        vertNormIndexTemp -= 1;
                                    }

                                    vertPart = L"";
                                    whichPart++;                            
                                }
                            }                   

                            // Check for duplicate vertices
                            bool vertAlreadyExists = false;
                            if(totalVerts >= 3) // Make sure we at least have one triangle to check
                            {
                                for(int iCheck = 0; iCheck < totalVerts; ++iCheck)
                                {
                                    if(vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
                                    {
                                        if(vertTCIndexTemp == vertTCIndex[iCheck])
                                        {
                                            objModel.Indices.push_back(iCheck);        // Set index for this vertex
                                            vertAlreadyExists = true;       // If we've made it here, the vertex already exists
                                        }
                                    }
                                }
                            }

                            if(!vertAlreadyExists)
                            {
                                vertPosIndex.push_back(vertPosIndexTemp);
                                vertTCIndex.push_back(vertTCIndexTemp);
                                vertNormIndex.push_back(vertNormIndexTemp);
                                totalVerts++;                       // New vertex created, add to total verts
                                objModel.Indices.push_back(totalVerts-1);  // Set index for this vertex
                            }

                            // Set the second vertex for the next triangle to the last vertex we got        
                            lastVIndex = objModel.Indices[vIndex]; // The last vertex index of this TRIANGLE

                            meshTriangles++;    // New triangle defined
                            vIndex++;       
                        }
                    }
                }
                break;
			#pragma endregion
			#pragma region 'm' Case - Material Library Filename
            case 'm': // mtllib - material library filename
                checkChar = fileIn.get();
                if(checkChar == 't')
                {
                    checkChar = fileIn.get();
                    if(checkChar == 'l')
                    {
                        checkChar = fileIn.get();
                        if(checkChar == 'l')
                        {
                            checkChar = fileIn.get();
                            if(checkChar == 'i')
                            {
                                checkChar = fileIn.get();
                                if(checkChar == 'b')
                                {
                                    checkChar = fileIn.get();
                                    if(checkChar == ' ')
                                    {
                                        // Store the material libraries file name
                                        fileIn >> meshMatLib;
                                    }
                                }
                            }
                        }
                    }
                }

                break;
			#pragma endregion
			#pragma region 'u' Case - Which Material?
            case 'u': // usemtl - which material to use
                checkChar = fileIn.get();
                if(checkChar == 's')
                {
                    checkChar = fileIn.get();
                    if(checkChar == 'e')
                    {
                        checkChar = fileIn.get();
                        if(checkChar == 'm')
                        {
                            checkChar = fileIn.get();
                            if(checkChar == 't')
                            {
                                checkChar = fileIn.get();
                                if(checkChar == 'l')
                                {
                                    checkChar = fileIn.get();
                                    if(checkChar == ' ')
                                    {
                                        meshMaterialsTemp = L"";  // Make sure this is cleared

                                        fileIn >> meshMaterialsTemp; // Get next type (string)

                                        meshMaterials.push_back(meshMaterialsTemp);
                                        if(!ang)
                                        {
                                            objModel.SubsetIndexStart.push_back(vIndex);       // Start index for this subset
                                            objModel.Subsets++;
                                        }
                                        ang = false;
                                    }
                                }
                            }
                        }
                    }
                }
                break;
			#pragma endregion
            default:                
                break;
            }
        }
    }
    else    // If we could not open the file
    {

		//This is a commented out line because of an undeclared identifier. I don't think it's a big problem anyway.
		
		
		//SwapChain->SetFullscreenState(false, NULL); // Make sure we are out of fullscreen

        // create message
        std::wstring message = L"Could not open: ";
        message += fileName;

        MessageBox(0, message.c_str(),  // Display message
            L"Error", MB_OK);

        return false;
    }

    objModel.SubsetIndexStart.push_back(vIndex); // There won't be another index start after our last subset, so set it here

    // sometimes "g" is defined at the very top of the file, then again before the first group of faces.
    // This makes sure the first subset does not conatain "0" indices.
    if(objModel.SubsetIndexStart[1] == 0)
    {
        objModel.SubsetIndexStart.erase(objModel.SubsetIndexStart.begin()+1);
        objModel.Subsets--;
    }

    // Make sure we have a default for the tex coord and normal
    // if one or both are not specified
    if(!hasNorm)
        vertNorm.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
    if(!hasTexCoord)
        vertTexCoord.push_back(XMFLOAT2(0.0f, 0.0f));
	
	// Close the obj file
    fileIn.close();
	#pragma endregion
	#pragma region MTL File Contents
	// Open the mtl file
	
	fileIn.open((L"Assets//" + meshMatLib).c_str());

    std::wstring lastStringRead;
    int matCount = material.size(); // Total materials

    if (fileIn)
    {
        while(fileIn)
        {
            checkChar = fileIn.get();   // Get next char

            switch (checkChar)
            {
            #pragma region '#' Case - Comments
				// Check for comment
            case '#':
				checkChar = fileIn.get();
                while(checkChar != '\n')
                    checkChar = fileIn.get();
                break;
			#pragma endregion
			#pragma region 'K' Case - Colors
			// Set the colors
            case 'K':
                checkChar = fileIn.get();
                if(checkChar == 'd')  // Diffuse Color
                {
                    checkChar = fileIn.get();   // Remove space

                    fileIn >> material[matCount-1].Diffuse.x;
                    fileIn >> material[matCount-1].Diffuse.y;
                    fileIn >> material[matCount-1].Diffuse.z;
                }

                if(checkChar == 'a')  // Ambient Color
                {                   
                    checkChar = fileIn.get();   // Remove space

                    fileIn >> material[matCount-1].Ambient.x;
                    fileIn >> material[matCount-1].Ambient.y;
                    fileIn >> material[matCount-1].Ambient.z;
                }

                if(checkChar == 's')  // Ambient Color
                {                   
                    checkChar = fileIn.get();   // Remove space

                    fileIn >> material[matCount-1].Specular.x;
                    fileIn >> material[matCount-1].Specular.y;
                    fileIn >> material[matCount-1].Specular.z;
                }
                break;
			#pragma endregion
			#pragma region 'N' Case - Specular!
			case 'N':
                checkChar = fileIn.get();

                if(checkChar == 's')  // Specular Power (Coefficient)
                {                   
                    checkChar = fileIn.get();   // Remove space

                    fileIn >> material[matCount-1].Specular.w;
                }

                break;
			#pragma endregion
			#pragma region 'T' Case - Transparency
				// Check for transparency
            case 'T':
                checkChar = fileIn.get();
                if(checkChar == 'r')
                {
                    checkChar = fileIn.get();   // Remove space
                    float Transparency;
                    fileIn >> Transparency;

                    material[matCount-1].Diffuse.w = Transparency;

                    if(Transparency > 0.0f)
                        material[matCount-1].IsTransparent = true;
                }
                break; 
			#pragma endregion
			#pragma region 'd' Case - Transparency 2 (Some files use 'd' for transparency)
			case 'd':	// Some obj files specify d for transparency
                checkChar = fileIn.get();
                if(checkChar == ' ')
                {
                    float Transparency;
                    fileIn >> Transparency;

                    // 'd' - 0 being most transparent, and 1 being opaque, opposite of Tr
                    Transparency = 1.0f - Transparency;

                    material[matCount-1].Diffuse.w = Transparency;

                    if(Transparency > 0.0f)
                        material[matCount-1].IsTransparent = true;                  
                }
				break;
			#pragma endregion
			#pragma region 'm' Case - Diffuse Map
			// Get the diffuse map (texture)
            case 'm':
                checkChar = fileIn.get();
                if(checkChar == 'a')
                {
                    checkChar = fileIn.get();
                    if(checkChar == 'p')
                    {
                        checkChar = fileIn.get();
                        if(checkChar == '_')
                        {
                            // map_Kd - Diffuse map
                            checkChar = fileIn.get();
                            if(checkChar == 'K')
                            {
                                checkChar = fileIn.get();
                                if(checkChar == 'd')
                                {
                                    std::wstring fileNamePath;

                                    fileIn.get();   // Remove whitespace between map_Kd and file

                                    // Get the file path - We read the pathname char by char since
                                    // pathnames can sometimes contain spaces, so we will read until
                                    // we find the file extension
                                    bool texFilePathEnd = false;
                                    while(!texFilePathEnd)
                                    {
                                        checkChar = fileIn.get();

                                        fileNamePath += checkChar;

                                        if(checkChar == '.')
                                        {
                                            for(int i = 0; i < 3; ++i)
                                                fileNamePath += fileIn.get();

                                            texFilePathEnd = true;
                                        }                           
                                    }

                                    //check if this texture has already been loaded
                                    bool alreadyLoaded = false;
                                    for(unsigned int i = 0; i < textureMan.TextureNameArray.size(); ++i)
                                    {
                                        if(fileNamePath == textureMan.TextureNameArray[i])
                                        {
                                            alreadyLoaded = true;
                                            material[matCount-1].DiffuseTextureID = i;
                                            material[matCount-1].HasDiffTexture = true;
                                        }
                                    }

                                    //if the texture is not already loaded, load it now
                                    if(!alreadyLoaded)
                                    {
                                        ID3D11ShaderResourceView* tempSRV;
										//renderMan->LoadTexture("d", fileNamePath.c_str())
										hr = D3DX11CreateShaderResourceViewFromFile( device, fileNamePath.c_str(),
                                            NULL, NULL, &tempSRV, NULL );
                                        if(SUCCEEDED(hr))
                                        {
                                            textureMan.TextureNameArray.push_back(fileNamePath.c_str());
                                            material[matCount-1].DiffuseTextureID = textureMan.TextureList.size();
                                            textureMan.TextureList.push_back(tempSRV);
                                            material[matCount-1].HasDiffTexture = true;
                                        }
                                    }   
                                }

                                // Get Ambient Map (texture)
                                if(checkChar == 'a')
                                {
                                    std::wstring fileNamePath;

                                    fileIn.get();   // Remove whitespace between map_Kd and file

                                    // Get the file path - We read the pathname char by char since
                                    // pathnames can sometimes contain spaces, so we will read until
                                    // we find the file extension
                                    bool texFilePathEnd = false;
                                    while(!texFilePathEnd)
                                    {
                                        checkChar = fileIn.get();

                                        fileNamePath += checkChar;

                                        if(checkChar == '.')
                                        {
                                            for(int i = 0; i < 3; ++i)
                                                fileNamePath += fileIn.get();

                                            texFilePathEnd = true;
                                        }                           
                                    }

                                    //check if this texture has already been loaded
                                    bool alreadyLoaded = false;
                                    for(unsigned int i = 0; i < textureMan.TextureNameArray.size(); ++i)
                                    {
                                        if(fileNamePath == textureMan.TextureNameArray[i])
                                        {
                                            alreadyLoaded = true;
                                            material[matCount-1].AmbientTextureID = i;
                                            material[matCount-1].HasAmbientTexture = true;
                                        }
                                    }

                                    //if the texture is not already loaded, load it now
                                    if(!alreadyLoaded)
                                    {
                                        ID3D11ShaderResourceView* tempSRV;
                                        hr = D3DX11CreateShaderResourceViewFromFile( device, fileNamePath.c_str(),
                                            NULL, NULL, &tempSRV, NULL );
                                        if(SUCCEEDED(hr))
                                        {
                                            textureMan.TextureNameArray.push_back(fileNamePath.c_str());
                                            material[matCount-1].AmbientTextureID = textureMan.TextureList.size();
                                            textureMan.TextureList.push_back(tempSRV);
                                            material[matCount-1].HasAmbientTexture = true;
                                        }
                                    }   
                                }

                                // Get Specular Map (texture)
                                if(checkChar == 's')
                                {
                                    std::wstring fileNamePath;

                                    fileIn.get();   // Remove whitespace between map_Ks and file

                                    // Get the file path - We read the pathname char by char since
                                    // pathnames can sometimes contain spaces, so we will read until
                                    // we find the file extension
                                    bool texFilePathEnd = false;
                                    while(!texFilePathEnd)
                                    {
                                        checkChar = fileIn.get();

                                        fileNamePath += checkChar;

                                        if(checkChar == '.')
                                        {
                                            for(int i = 0; i < 3; ++i)
                                                fileNamePath += fileIn.get();

                                            texFilePathEnd = true;
                                        }                           
                                    }

                                    //check if this texture has already been loaded
                                    bool alreadyLoaded = false;
                                    for(unsigned int i = 0; i < textureMan.TextureNameArray.size(); ++i)
                                    {
                                        if(fileNamePath == textureMan.TextureNameArray[i])
                                        {
                                            alreadyLoaded = true;
                                            material[matCount-1].SpecularTextureID = i;
                                            material[matCount-1].HasSpecularTexture = true;
                                        }
                                    }

                                    //if the texture is not already loaded, load it now
                                    if(!alreadyLoaded)
                                    {
                                        ID3D11ShaderResourceView* tempSRV;
                                        hr = D3DX11CreateShaderResourceViewFromFile( device, fileNamePath.c_str(),
                                            NULL, NULL, &tempSRV, NULL );
                                        if(SUCCEEDED(hr))
                                        {
                                            textureMan.TextureNameArray.push_back(fileNamePath.c_str());
                                            material[matCount-1].SpecularTextureID = textureMan.TextureList.size();
                                            textureMan.TextureList.push_back(tempSRV);
                                            material[matCount-1].HasSpecularTexture = true;
                                        }
                                    }   
                                }
                            }
                            
                            //map_d - alpha map
                            else if(checkChar == 'd')
                            {
                                std::wstring fileNamePath;

                                fileIn.get();   // Remove whitespace between map_Ks and file

                                // Get the file path - We read the pathname char by char since
                                // pathnames can sometimes contain spaces, so we will read until
                                // we find the file extension
                                bool texFilePathEnd = false;
                                while(!texFilePathEnd)
                                {
                                    checkChar = fileIn.get();

                                    fileNamePath += checkChar;

                                    if(checkChar == '.')
                                    {
                                        for(unsigned int i = 0; i < 3; ++i)
                                            fileNamePath += fileIn.get();

                                        texFilePathEnd = true;
                                    }                           
                                }

                                //check if this texture has already been loaded
                                bool alreadyLoaded = false;
                                for(unsigned int i = 0; i < textureMan.TextureNameArray.size(); ++i)
                                {
                                    if(fileNamePath == textureMan.TextureNameArray[i])
                                    {
                                        alreadyLoaded = true;
                                        material[matCount-1].AlphaTextureID = i;
                                        material[matCount-1].IsTransparent = true;
                                    }
                                }

                                //if the texture is not already loaded, load it now
                                if(!alreadyLoaded)
                                {
                                    ID3D11ShaderResourceView* tempSRV;
                                    hr = D3DX11CreateShaderResourceViewFromFile( device, fileNamePath.c_str(),
                                        NULL, NULL, &tempSRV, NULL );
                                    if(SUCCEEDED(hr))
                                    {
                                        textureMan.TextureNameArray.push_back(fileNamePath.c_str());
                                        material[matCount-1].AlphaTextureID = textureMan.TextureList.size();
                                        textureMan.TextureList.push_back(tempSRV);
                                        material[matCount-1].IsTransparent = true;
                                    }
                                }   
                            }

                            // map_bump - bump map (Normal Map)
                            else if(checkChar == 'b')
                            {
                                checkChar = fileIn.get();
                                if(checkChar == 'u')
                                {
                                    checkChar = fileIn.get();
                                    if(checkChar == 'm')
                                    {
                                        checkChar = fileIn.get();
                                        if(checkChar == 'p')
                                        {
                                            std::wstring fileNamePath;

                                            fileIn.get();   // Remove whitespace between map_bump and file

                                            // Get the file path - We read the pathname char by char since
                                            // pathnames can sometimes contain spaces, so we will read until
                                            // we find the file extension
                                            bool texFilePathEnd = false;
                                            while(!texFilePathEnd)
                                            {
                                                checkChar = fileIn.get();

                                                fileNamePath += checkChar;

                                                if(checkChar == '.')
                                                {
                                                    for(int i = 0; i < 3; ++i)
                                                        fileNamePath += fileIn.get();

                                                    texFilePathEnd = true;
                                                }                           
                                            }

                                            //check if this texture has already been loaded
                                            bool alreadyLoaded = false;
                                            for(unsigned int i = 0; i < textureMan.TextureNameArray.size(); ++i)
                                            {
                                                if(fileNamePath == textureMan.TextureNameArray[i])
                                                {
                                                    alreadyLoaded = true;
                                                    material[matCount-1].NormMapTextureID = i;
                                                    material[matCount-1].HasNormMap = true;
                                                }
                                            }

                                            //if the texture is not already loaded, load it now
                                            if(!alreadyLoaded)
                                            {
                                                ID3D11ShaderResourceView* tempSRV;
                                                hr = D3DX11CreateShaderResourceViewFromFile( device, fileNamePath.c_str(),
                                                    NULL, NULL, &tempSRV, NULL );
                                                if(SUCCEEDED(hr))
                                                {
                                                    textureMan.TextureNameArray.push_back(fileNamePath.c_str());
                                                    material[matCount-1].NormMapTextureID = textureMan.TextureList.size();
                                                    textureMan.TextureList.push_back(tempSRV);
                                                    material[matCount-1].HasNormMap = true;
                                                }
                                            }   
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                break;
			#pragma endregion
			#pragma region 'n' Case - New Material
            case 'n': // newmtl - Declare new material
                checkChar = fileIn.get();
                if(checkChar == 'e')
                {
                    checkChar = fileIn.get();
                    if(checkChar == 'w')
                    {
                        checkChar = fileIn.get();
                        if(checkChar == 'm')
                        {
                            checkChar = fileIn.get();
                            if(checkChar == 't')
                            {
                                checkChar = fileIn.get();
                                if(checkChar == 'l')
                                {
                                    checkChar = fileIn.get();
                                    if(checkChar == ' ')
                                    {
                                        // New material, set its defaults
                                        GameMaterial tempMat;
										//renderMan->PushGameMaterial(tempMat);
										//RenderManager::getInstance().PushGameMaterial(tempMat);
                                        material.push_back(tempMat);

										//XMFLOAT4 Ambient;  //Kill me
										//XMFLOAT4 Diffuse;  //Kill me      
										//XMFLOAT4 Specular; //Kill me
										//XMFLOAT4 Reflect;  //Kill me
										//int DiffuseTextureID;  //Kill me
										//int AmbientTextureID;  //Kill me
										//int SpecularTextureID; //Kill me
										//int AlphaTextureID;    //Kill me
										//int NormMapTextureID;  //Kill me

										//std::wstring MatName;  //Kill me because nobody loves me. Also surfaceKey is better than me
										//fileIn >> material[matCount].MatName;
										material[matCount].SurfaceKey = fileNameS;
										material[matCount].DiffuseKey = fileNameS;
										material[matCount].Name = fileNameS;
										material[matCount].IsTransparent = false;
										material[matCount].HasDiffTexture = false;
										material[matCount].HasAmbientTexture = false;
										material[matCount].HasSpecularTexture = false;
										material[matCount].HasAlphaTexture = false;
										material[matCount].HasNormMap = false;

										GAME_MATERIALS[material[matCount].SurfaceKey] = material[matCount];

										SurfaceMaterial aMaterial;
										aMaterial.Ambient = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
										aMaterial.Diffuse = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
										aMaterial.Specular = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
										aMaterial.Reflect = XMFLOAT4(0,0,0,0); 
										
										//SURFACE_MATERIALS[fileNameS] = aMaterial;
										//SURFACE_MATERIALS.insert(std::pair<string, SurfaceMaterial>(fileNameS, aMaterial));
										surface.push_back(aMaterial);
										/*material[matCount].NormMapTextureID = 0;
										material[matCount].DiffuseTextureID = 0;
										material[matCount].AlphaTextureID = 0;
										material[matCount].SpecularTextureID = 0;
										material[matCount].AmbientTextureID = 0;
										material[matCount].Specular = XMFLOAT4(0,0,0,0);
										material[matCount].Ambient = XMFLOAT4(0,0,0,0);
										material[matCount].Diffuse = XMFLOAT4(0,0,0,0);*/
										matCount++;
                                    }
                                }
                            }
                        }
                    }
                }
                break;
			#pragma endregion
            default:
                break;
            }
        }
    }   
    else    // If we could not open the material library
    {
        //I'm avoiding this line again.
		
		//SwapChain->SetFullscreenState(false, NULL); // Make sure we are out of fullscreen

        std::wstring message = L"Could not open: ";
        message += meshMatLib;

        MessageBox(0, message.c_str(),
            L"Error", MB_OK);

        return false;
    }
	#pragma endregion

	#pragma region Subsets and Vertices
    // Set the subsets material to the index value
    // of the its material in our material array
    for(int i = 0; i < objModel.Subsets; ++i)
    {
        bool hasMat = false;
        for(unsigned int j = 0; j < material.size(); ++j)
        {
            if(meshMaterials[i] == material[j].MatName)
            {
                objModel.SubsetMaterialID.push_back(j);
                hasMat = true;
            }
        }
        if(!hasMat)
            objModel.SubsetMaterialID.push_back(0); // Use first material in array
    }

    std::vector<Vertex> vertices;
    Vertex tempVert;

    // Create our vertices using the information we got 
    // from the file and store them in a vector
    for(int j = 0 ; j < totalVerts; ++j)
    {
        tempVert.Pos = vertPos[vertPosIndex[j]];
        tempVert.Normal = vertNorm[vertNormIndex[j]];
        tempVert.TexC = vertTexCoord[vertTCIndex[j]];

		vertices.push_back(tempVert);
        objModel.Vertices.push_back(tempVert.Pos);
    }

	#pragma endregion

	#pragma region Compute Normals
    //If computeNormals was set to true then we will create our own
    //normals, if it was set to false we will use the obj files normals
    if(computeNormals)
    {
        std::vector<XMFLOAT3> tempNormal;

        //normalized and unnormalized normals
        XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

        //tangent stuff
        std::vector<XMFLOAT3> tempTangent;
        XMFLOAT3 tangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
        float tcU1, tcV1, tcU2, tcV2;

        //Used to get vectors (sides) from the position of the verts
        float vecX, vecY, vecZ;

        //Two edges of our triangle
        XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

        //Compute face normals
        //And Tangents
        for(int i = 0; i < meshTriangles; ++i)
        {
            //Get the vector describing one edge of our triangle (edge 0,2)
            vecX = vertices[objModel.Indices[(i*3)]].Pos.x - vertices[objModel.Indices[(i*3)+2]].Pos.x;
            vecY = vertices[objModel.Indices[(i*3)]].Pos.y - vertices[objModel.Indices[(i*3)+2]].Pos.y;
            vecZ = vertices[objModel.Indices[(i*3)]].Pos.z - vertices[objModel.Indices[(i*3)+2]].Pos.z;       
            edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our first edge

            //Get the vector describing another edge of our triangle (edge 2,1)
            vecX = vertices[objModel.Indices[(i*3)+2]].Pos.x - vertices[objModel.Indices[(i*3)+1]].Pos.x;
            vecY = vertices[objModel.Indices[(i*3)+2]].Pos.y - vertices[objModel.Indices[(i*3)+1]].Pos.y;
            vecZ = vertices[objModel.Indices[(i*3)+2]].Pos.z - vertices[objModel.Indices[(i*3)+1]].Pos.z;     
            edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our second edge

            //Cross multiply the two edge vectors to get the un-normalized face normal
            XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));

            tempNormal.push_back(unnormalized);

            //Find first texture coordinate edge 2d vector
            tcU1 = vertices[objModel.Indices[(i*3)]].TexC.x - vertices[objModel.Indices[(i*3)+2]].TexC.x;
            tcV1 = vertices[objModel.Indices[(i*3)]].TexC.y - vertices[objModel.Indices[(i*3)+2]].TexC.y;

            //Find second texture coordinate edge 2d vector
            tcU2 = vertices[objModel.Indices[(i*3)+2]].TexC.x - vertices[objModel.Indices[(i*3)+1]].TexC.x;
            tcV2 = vertices[objModel.Indices[(i*3)+2]].TexC.y - vertices[objModel.Indices[(i*3)+1]].TexC.y;

            //Find tangent using both tex coord edges and position edges
            tangent.x = (tcV1 * XMVectorGetX(edge1) - tcV2 * XMVectorGetX(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));
            tangent.y = (tcV1 * XMVectorGetY(edge1) - tcV2 * XMVectorGetY(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));
            tangent.z = (tcV1 * XMVectorGetZ(edge1) - tcV2 * XMVectorGetZ(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));

            tempTangent.push_back(tangent);
        }

        //Compute vertex normals (normal Averaging)
        XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        XMVECTOR tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        int facesUsing = 0;
        float tX, tY, tZ;   //temp axis variables

        //Go through each vertex
        for(int i = 0; i < totalVerts; ++i)
        {
            //Check which triangles use this vertex
            for(int j = 0; j < meshTriangles; ++j)
            {
                if(objModel.Indices[j*3] == i ||
                    objModel.Indices[(j*3)+1] == i ||
                    objModel.Indices[(j*3)+2] == i)
                {
                    tX = XMVectorGetX(normalSum) + tempNormal[j].x;
                    tY = XMVectorGetY(normalSum) + tempNormal[j].y;
                    tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

                    normalSum = XMVectorSet(tX, tY, tZ, 0.0f);  //If a face is using the vertex, add the unormalized face normal to the normalSum
    
                    //We can reuse tX, tY, tZ to sum up tangents
                    tX = XMVectorGetX(tangentSum) + tempTangent[j].x;
                    tY = XMVectorGetY(tangentSum) + tempTangent[j].y;
                    tZ = XMVectorGetZ(tangentSum) + tempTangent[j].z;

                    tangentSum = XMVectorSet(tX, tY, tZ, 0.0f); //sum up face tangents using this vertex

                    facesUsing++;
                }
            }

            //Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
            normalSum = normalSum / (float)facesUsing;
            tangentSum = tangentSum / (float)facesUsing;

            //Normalize the normalSum vector and tangent
            normalSum = XMVector3Normalize(normalSum);
            tangentSum =  XMVector3Normalize(tangentSum);

            //Store the normal and tangent in our current vertex
            vertices[i].Normal.x = XMVectorGetX(normalSum);
            vertices[i].Normal.y = XMVectorGetY(normalSum);
            vertices[i].Normal.z = XMVectorGetZ(normalSum);

            vertices[i].Tangent.x = XMVectorGetX(tangentSum);
            vertices[i].Tangent.y = XMVectorGetY(tangentSum);
            vertices[i].Tangent.z = XMVectorGetZ(tangentSum);

            //Clear normalSum, tangentSum and facesUsing for next vertex
            normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
            tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
            facesUsing = 0;

        }
    }
	#pragma endregion

	#pragma region Flip Faces
	if(flipFaces)
	{
		for(unsigned int i = 0; i < objModel.Indices.size(); i+=3)
		{
			DWORD ti0 = objModel.Indices[i];
			objModel.Indices[i] = objModel.Indices[i+2];
			objModel.Indices[i+2] = ti0;
		}
	}
	#pragma endregion

	#pragma region AABB Min/max vert creation
    // Create Axis-Aligned Bounding Box (AABB)
    XMFLOAT3 minVertex = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
    XMFLOAT3 maxVertex = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    objModel.BoundingSphere = 0;
    #pragma endregion
	
	#pragma region Min and Max Vertexes (used for AABB?)
    for(UINT i = 0; i < objModel.Vertices.size(); i++)
    {       
        // The minVertex and maxVertex will most likely not be actual vertices in the model, but vertices
        // that use the smallest and largest x, y, and z values from the model to be sure ALL vertices are
        // covered by the bounding volume

        //Get the smallest vertex 
        minVertex.x = min(minVertex.x, objModel.Vertices[i].x);    // Find smallest x value in model
        minVertex.y = min(minVertex.y, objModel.Vertices[i].y);    // Find smallest y value in model
        minVertex.z = min(minVertex.z, objModel.Vertices[i].z);    // Find smallest z value in model

        //Get the largest vertex 
        maxVertex.x = max(maxVertex.x, objModel.Vertices[i].x);    // Find largest x value in model
        maxVertex.y = max(maxVertex.y, objModel.Vertices[i].y);    // Find largest y value in model
        maxVertex.z = max(maxVertex.z, objModel.Vertices[i].z);    // Find largest z value in model
    }
	#pragma endregion

	#pragma region Model True Center - Commented Out
    // Our AABB [0] is the min vertex and [1] is the max
    ////Model.AABB.push_back(minVertex);
    ////Model.AABB.push_back(maxVertex);
    
    // Get models true center
    /*objModel.Center.x = maxVertex.x - minVertex.x / 2.0f;
    objModel.Center.y = maxVertex.y - minVertex.y / 2.0f;
    objModel.Center.z = maxVertex.z - minVertex.z / 2.0f;*/
	#pragma endregion

	#pragma region Bounding Sphere - Commented Out
	//// Now that we have the center, get the bounding sphere 
 //   for(UINT i = 0; i < objModel.Vertices.size(); i++)
 //   {       
 //       float x = (objModel.Center.x - objModel.Vertices[i].x) * (objModel.Center.x - objModel.Vertices[i].x);
 //       float y = (objModel.Center.y - objModel.Vertices[i].y) * (objModel.Center.y - objModel.Vertices[i].y);
 //       float z = (objModel.Center.z - objModel.Vertices[i].z) * (objModel.Center.z - objModel.Vertices[i].z);

 //       // Get models bounding sphere
 //       objModel.BoundingSphere = max(objModel.BoundingSphere, (x+y+z));
 //   }
	//
 //   // We didn't use the square root when finding the largest distance since it slows things down.
 //   // We can square root the answer from above to get the actual bounding sphere now
 //   objModel.BoundingSphere = sqrt(objModel.BoundingSphere);
	#pragma endregion

	#pragma region Add Vertices and Indices to the Mesh Maps
	string name(fileName.begin(), fileName.end());
	name.erase(name.end() - 4, name.end());

	MeshMaps::MESH_MAPS[name].bufferKey = name;
	MeshMaps::MESH_MAPS[name].normalizeVertices = computeNormals;

	for (unsigned int indexIndex = 0; indexIndex < objModel.Indices.size(); ++indexIndex)
		MeshMaps::MESH_MAPS[name].indices.push_back(objModel.Indices[indexIndex]);

	for (unsigned int vertexIndex = 0; vertexIndex < objModel.Vertices.size(); ++vertexIndex)
	{
		MeshMaps::MESH_MAPS[name].vertices.push_back(vertices[vertexIndex]);
	}
	#pragma endregion

	#pragma region Index and Vertex Buffer Setting and assignment - Commented Out
	//Create index buffer
    //D3D11_BUFFER_DESC indexBufferDesc;
    //ZeroMemory( &indexBufferDesc, sizeof(indexBufferDesc) );

    //indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    //indexBufferDesc.ByteWidth = sizeof(DWORD) * meshTriangles * 3;
    //indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    //indexBufferDesc.CPUAccessFlags = 0;
    //indexBufferDesc.MiscFlags = 0;

    //D3D11_SUBRESOURCE_DATA iinitData;

    //iinitData.pSysMem = &objModel.Indices[0];
    //device->CreateBuffer(&indexBufferDesc, &iinitData, &objModel.IndexBuff);

    ////Create Vertex Buffer
    //D3D11_BUFFER_DESC vertexBufferDesc;
    //ZeroMemory( &vertexBufferDesc, sizeof(vertexBufferDesc) );

    //vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    //vertexBufferDesc.ByteWidth = sizeof( Vertex ) * totalVerts;
    //vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    //vertexBufferDesc.CPUAccessFlags = 0;
    //vertexBufferDesc.MiscFlags = 0;

    //D3D11_SUBRESOURCE_DATA vertexBufferData; 

    //ZeroMemory( &vertexBufferData, sizeof(vertexBufferData) );
    //vertexBufferData.pSysMem = &vertices[0];
    //hr = device->CreateBuffer( &vertexBufferDesc, &vertexBufferData, &objModel.VertBuff);
	#pragma endregion
	
	//FLEE THE DEATH METHOD
	return true;
}
//------------------------------------------------------------------------------
//
// File Name:	MeshLibrary.c
// Author(s):	Trey Mongeon (tmongeon), Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "MeshLibrary.h"
#include "Mesh.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct MeshLibrary
{
	// This variable is not required but could be used for tracking the number
	//   of Mesh objects currently in the list.
	unsigned int meshCount;

	// A list of all currently loaded meshes.
	// This list can be a fixed-length array (minimum size of 10 entries)
	// or a dynamically-sized array, such as a linked list.
	const Mesh* meshList[16];

} MeshLibrary;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static MeshLibrary meshes;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Mesh Library.
// (NOTE: Make sure to initialize all memory to zero.)
void MeshLibraryInit()
{
	meshes.meshCount = 0;

	for ( unsigned int i = 0; i < _countof(meshes.meshList); ++i)
	{
		meshes.meshList[i] = 0;
	}
}



// Free all Mesh objects in the Mesh Library.
// (NOTE: You must call MeshFree() to free each Mesh object.)
// (HINT: The list should contain nothing but NULL pointers once this function is done.)
void MeshLibraryFreeAll()
{
	for (unsigned int i = 0; i < meshes.meshCount; ++i)
	{
		MeshFree(&meshes.meshList[i]);
	}
	meshes.meshCount = 0;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


static void MeshLibraryAdd(const Mesh* mesh)
{
	if (mesh == NULL)
	{
		return;
	}
		meshes.meshList[meshes.meshCount] = mesh;
		++meshes.meshCount;
}


static const Mesh* MeshLibraryFind(const char* meshName)
{
	if (meshName == NULL)
	{
		return NULL;
	}
	for (unsigned int i = 0; i < meshes.meshCount; ++i)
	{
		if (MeshIsNamed(meshes.meshList[i], meshName))
		{
			return meshes.meshList[i];
		}
	}
	return NULL;
}


// Build a mesh and add it to the mesh library, if it doesn't already exist.
//   1: Verify that a valid name was specified (not NULL).
//   2: Search for an existing mesh with a matching name.
//   3: If an existing mesh was NOT found, then create a new one from the data file:
//      a: Use sprintf_s() to construct a path name using meshName.
//	       (HINT: The correct path name should be constructed using "Data/%s.txt".)
//      b: Call StreamOpen(), passing the pathname.
//      c: If the stream was opened successfully,
//	       1: Call MeshCreate() to create an empty Mesh object.
//         2: Call MeshRead() to read the contents of the mesh from the file.
//	       3: Call MeshLibraryAdd(), passing the created mesh.
//	       4: Close the stream.
//   4: Return the mesh (either existed, created, or NULL).
// Params:
//	 meshName = The name of the mesh to be created.
// Returns:
//	 If the mesh already existed or was created successfully,
//	   then return a pointer to the mesh,
//	   else return NULL.
const Mesh* MeshLibraryBuild(const char* meshName)
{
	if (meshName == NULL)
	{
		return NULL;
	}

	const Mesh* foundMesh = MeshLibraryFind(meshName);
	if (foundMesh == NULL)
	{
		Mesh* makeMesh = NULL;

		char filePath[FILENAME_MAX] = "";
		sprintf_s(filePath, _countof(filePath), "Data/%s.txt", meshName);
		Stream stream = StreamOpen(filePath);

		if (stream)
		{
			makeMesh = MeshCreate();
			MeshRead(makeMesh, stream);
			MeshLibraryAdd(makeMesh);
			StreamClose(&stream);
			return makeMesh;
		}
	}
	return foundMesh;
}

/******************************************************************************
Class:Mesh
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description: Class to represent the geometric data that makes up the meshes
we render on screen.

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Vector4.h"
#include "Colour.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Common.h"

#include <string>
#include <fstream>

using std::ifstream;
using std::string;

enum PrimitiveType {
	PRIMITIVE_POINTS,
	PRIMITIVE_LINES,
	PRIMITIVE_TRIANGLES
};

class Mesh	{
	friend class SoftwareRasteriser;
public:
	Mesh(void);
	~Mesh(void);

	static Mesh* GeneratePoint(const Vector3 &pos);
	static Mesh* GenerateLine(const Vector3 &from, const Vector3 &to);
	static Mesh* GenerateTriangle(Vector3 &v0 = Vector3(0.5f, -0.5f, 0.0f), Vector3 &v1 = Vector3(0.0f, 0.5f, 0.5f), Vector3 &v2 = Vector3(-0.5f, -0.5f, 0.0f));
	static Mesh* GenerateAlphaTriangle();
	static Mesh* LoadMeshFile(const string &fileName);

	PrimitiveType	GetType() { return type;}

protected:
	PrimitiveType	type;

	uint			numVertices;

	Vector4*		vertices;
	Colour*			colours;
	Vector2*		textureCoords;	//We get onto what to do with these later on...
};


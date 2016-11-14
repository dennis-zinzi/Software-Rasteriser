#include "Mesh.h"

Mesh::Mesh(void)	{
	type			= PRIMITIVE_POINTS;

	numVertices		= 0;

	vertices		= NULL;
	colours			= NULL;
	textureCoords	= NULL;
}

Mesh::~Mesh(void)	{
	delete[] vertices;
	delete[] colours;
	delete[] textureCoords;
}

/* --------------------------------------------------
 *
 * Drawing Functions
 *
 -------------------------------------------------- */
Mesh* Mesh::GenerateLine(const Vector3 &from, const Vector3 &to){
	Mesh *m = new Mesh;

	m->numVertices = 2;

	m->vertices = new Vector4[m->numVertices];
	m->vertices[0] = Vector4(from.x, from.y, from.z, 1.0f);
	m->vertices[1] = Vector4(to.x, to.y, to.z, 1.0f);

	//Add color to interpolate lines
	m->colours = new Colour[m->numVertices];
	//Default first vertex to be red
	m->colours[0] = Colour(255, 0, 0, 0);
	//Default second vertex to be blue
	m->colours[1] = Colour(0, 0, 255, 0);

	m->type = PRIMITIVE_LINES;

	return m;
}

Mesh* Mesh::GenerateTriangle(){
	Mesh *m = new Mesh;
	m->type = PRIMITIVE_TRIANGLES;
	m->numVertices = 3;

	m->vertices = new Vector4[m->numVertices];
	//Currently uses default sizes for Triangles, might change later to accept params insted
	m->vertices[0] = Vector4(0.5f, -0.5f, 0.0f, 1.0f);
	m->vertices[1] = Vector4(0.0f, 0.5f, 0.0f, 1.0f);
	m->vertices[2] = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);

	m->colours = new Colour[m->numVertices];
	//Currently assigns each vertex to be Red, Green, and Blue respectively
	m->colours[0] = Colour(255, 0, 0, 255);
	m->colours[1] = Colour(0, 255, 0, 255);
	m->colours[2] = Colour(0, 0, 255, 255);

	m->textureCoords = new Vector2[m->numVertices];
	m->textureCoords[0] = Vector2(0.0f, 0.0f);
	m->textureCoords[1] = Vector2(0.5f, 1.0f);
	m->textureCoords[2] = Vector2(1.0f, 0.0f);

	return m;
}

Mesh* Mesh::GenerateAlphaTriangle(){
	Mesh *m = new Mesh;
	m->type = PRIMITIVE_TRIANGLES;
	m->numVertices = 3;

	m->vertices = new Vector4[m->numVertices];
	//Currently uses default sizes for Triangles, might change later to accept params insted
	m->vertices[0] = Vector4(0.5f, -0.5f, 0.0f, 1.0f);
	m->vertices[1] = Vector4(0.0f, 0.5f, 0.0f, 1.0f);
	m->vertices[2] = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);

	m->colours = new Colour[m->numVertices];
	//Currently assigns each vertex to be Red, Green, and Blue respectively
	m->colours[0] = Colour(255, 0, 0, 100);
	m->colours[1] = Colour(0, 255, 0, 100);
	m->colours[2] = Colour(0, 0, 255, 100);

	m->textureCoords = new Vector2[m->numVertices];
	m->textureCoords[0] = Vector2(0.0f, 0.0f);
	m->textureCoords[1] = Vector2(0.5f, 1.0f);
	m->textureCoords[2] = Vector2(1.0f, 0.0f);

	return m;
}

Mesh* Mesh::LoadMeshFile(const string &fileName){
	ifstream f(fileName);

	if(!f){
		return NULL;
	}

	Mesh *m = new Mesh;
	m->type = PRIMITIVE_TRIANGLES;

	f >> m->numVertices;

	int hasTex = 0,
		hasColor = 0;

	f >> hasTex;
	f >> hasColor;

	m->vertices = new Vector4[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Colour[m->numVertices];

	for(int i = 0; i < m->numVertices; ++i){
		f >> m->vertices[i].x;
		f >> m->vertices[i].y;
		f >> m->vertices[i].z;
	}

	if(hasColor){
		for(int i = 0; i < m->numVertices; ++i){
			f >> m->colours[i].r;
			f >> m->colours[i].g;
			f >> m->colours[i].b;
			f >> m->colours[i].a;
		}
	}
	if(hasTex){
		for(uint i = 0; i < m->numVertices; ++i){
			f >> m->textureCoords[i].x;
			f >> m->textureCoords[i].y;
		}
	}
	return m;
}
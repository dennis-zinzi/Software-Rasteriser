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

	m->type = PRIMITIVE_LINES;

	return m;
}
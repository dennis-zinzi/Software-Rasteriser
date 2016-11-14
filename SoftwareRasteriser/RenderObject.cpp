#include "RenderObject.h"


RenderObject::RenderObject(void)	{
	texture = NULL;
	mesh	= NULL;
}


RenderObject::~RenderObject(void)	{
	if(mesh){
		delete mesh;
		mesh = NULL;
	}
	if(texture){
		//delete texture;
		texture = NULL;
	}
}

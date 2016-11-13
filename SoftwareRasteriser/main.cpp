#include "SoftwareRasteriser.h"
#include "RenderObject.h"
#include "Mesh.h"
#include "Texture.h"

int main() {
	//Create a 800x600 widow to draw in
	SoftwareRasteriser r(800,600);

	//Create objects to draw here
	
	/* Tutorial 3 obj */
	//RenderObject *obj = new RenderObject;
	//obj->mesh = Mesh::GenerateLine(Vector3(0, 0.0f, 0), Vector3(0.5f, 0.5f, 0));

	/* Tutorial 4 objects */
	//RenderObject *tri = new RenderObject;
	//tri->mesh = Mesh::GenerateTriangle();

	/* Tutorial 5 objects */
	RenderObject *line = new RenderObject;
	line->mesh = Mesh::GenerateLine(Vector3(0, 0.0f, 0), Vector3(0.5f, 0.5f, 0));
	
	RenderObject *tri = new RenderObject;
	tri->mesh = Mesh::GenerateTriangle();

	while(r.UpdateWindow() && !Keyboard::KeyDown(KEY_ESCAPE)){
		r.ClearBuffers();

		//Put draw functions here!
		
		/* Tutorial 3 obj */
		//r.DrawObject(obj);

		/* Tutorial 4 objects */
		//r.DrawObject(tri);

		/* Tutorial 5 objects */
		r.DrawObject(line);
		r.DrawObject(tri);

		//NEVER ACTUALLY USED
		//if (Mouse::ButtonDown(MOUSE_LEFT)) {
		//	std::cout << "Mouse is at position: " << Mouse::GetAbsolutePosition().x << " , " << Mouse::GetAbsolutePosition().y << std::endl;
		//}


		r.SwapBuffers();
	}

	/* Delete RenderObjects to avoid memory leaks */
	
	/* Tutorial 3 obj */
	//delete obj;

	/* Tutorial 4 objects */
	//delete tri;

	/* Tutorial 5 objects */
	delete line;
	delete tri;

	return 0;
}
#include "SoftwareRasteriser.h"
#include "RenderObject.h"
#include "Mesh.h"
#include "Texture.h"

int main() {
	//Create a 800x600 widow to draw in
	SoftwareRasteriser r(800,600);

	//Create objects to draw here
	RenderObject *obj = new RenderObject;
	obj->mesh = Mesh::GenerateLine(Vector3(0, 0.0f, 0), Vector3(0.5f, 0.5f, 0));

	while(r.UpdateWindow() && !Keyboard::KeyDown(KEY_ESCAPE)){
		r.ClearBuffers();

		//Put draw functions here!
		r.DrawObject(obj);

		//NEVER ACTUALLY USED
		//if (Mouse::ButtonDown(MOUSE_LEFT)) {
		//	std::cout << "Mouse is at position: " << Mouse::GetAbsolutePosition().x << " , " << Mouse::GetAbsolutePosition().y << std::endl;
		//}


		r.SwapBuffers();
	}

	//Delete RenderObjects to avoid memory leaks
	delete obj;

	return 0;
}
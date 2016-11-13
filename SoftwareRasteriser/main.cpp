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
	//RenderObject *line = new RenderObject;
	//line->mesh = Mesh::GenerateLine(Vector3(0, 0.0f, 0), Vector3(0.5f, 0.5f, 0));
	//
	//RenderObject *tri = new RenderObject;
	//tri->mesh = Mesh::GenerateTriangle();

	/* Tutorial 6 objects */
	/*RenderObject *modTri = new RenderObject;
	modTri->mesh = Mesh::GenerateTriangle();

	Matrix4 translate = Matrix4::Translation(Vector3(0.5f, 0.0f, 0.0f)),
		rotate = Matrix4::Rotation(45.0f, Vector3(0.0f, 0.0f, 1.0f)),
		scale = Matrix4::Scale(Vector3(2.0f, 0.5f, 1.0f));

	modTri->modelMatrix = translate * rotate * scale;*/
	Mesh *testTri = Mesh::GenerateTriangle();

	RenderObject *o1 = new RenderObject;
	RenderObject *o2 = new RenderObject;
	RenderObject *o3 = new RenderObject;
	RenderObject *o4 = new RenderObject;

	o1->mesh = testTri;
	o2->mesh = testTri;
	o3->mesh = testTri;
	o4->mesh = testTri;

	o1->modelMatrix = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0.0f));
	o2->modelMatrix = Matrix4::Translation(Vector3(-0.5f, 0.5f, 0.0f));
	o3->modelMatrix = Matrix4::Translation(Vector3(0.5f, -0.5f, 0.0f));
	o4->modelMatrix = Matrix4::Translation(Vector3(0.5f, 0.5f, 0.0f));


	Matrix4 viewMatrix;

	while(r.UpdateWindow() && !Keyboard::KeyDown(KEY_ESCAPE)){
		r.ClearBuffers();

		//Testing of viewMatrix updating rendered object (Added in Tutorial 6)
		if(Keyboard::KeyDown(KEY_A)){
			viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-0.01f, 0.0f, 0.0f));
		}
		if(Keyboard::KeyDown(KEY_D)){
			viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.01f, 0.0f, 0.0f));
		}
		if(Keyboard::KeyDown(KEY_W)){
			viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, 0.01f, 0.0f));
		}
		if(Keyboard::KeyDown(KEY_S)){
			viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, -0.01f, 0.0f));
		}
		if(Keyboard::KeyDown(KEY_RIGHT)){
			viewMatrix = viewMatrix * Matrix4::RotateZ(1.0f);
		}
		if(Keyboard::KeyDown(KEY_LEFT)){
			viewMatrix = viewMatrix * Matrix4::RotateZ(-1.0f);
		}
		if(Keyboard::KeyDown(KEY_UP)){
			viewMatrix = viewMatrix * Matrix4::RotateX(1.0f);
		}
		if(Keyboard::KeyDown(KEY_DOWN)){
			viewMatrix = viewMatrix * Matrix4::RotateX(-1.0f);
		}
		if(Keyboard::KeyDown(KEY_L)){
			viewMatrix = viewMatrix * Matrix4::RotateY(-1.0f);
		}
		if(Keyboard::KeyDown(KEY_J)){
			viewMatrix = viewMatrix * Matrix4::RotateY(-1.0f);
		}
		r.SetViewMatrix(viewMatrix);

		//Put draw functions here!
		
		/* Tutorial 3 obj */
		//r.DrawObject(obj);

		/* Tutorial 4 objects */
		//r.DrawObject(tri);

		/* Tutorial 5 objects */
		//r.DrawObject(line);
		//r.DrawObject(tri);

		/* Tutorial 6 objects */
		//r.DrawObject(modTri);
		r.DrawObject(o1);
		r.DrawObject(o2);
		r.DrawObject(o3);
		r.DrawObject(o4);

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
	//delete line;
	//delete tri;

	/* Tutorial 6 objects */
	//delete modTri;
	//As o1, o2, o3, o4 use same mesh, only need to delete it once 
	delete testTri;

	return 0;
}
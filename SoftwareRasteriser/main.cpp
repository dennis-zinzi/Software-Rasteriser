#include "SoftwareRasteriser.h"
#include "RenderObject.h"
#include "Mesh.h"
#include "Texture.h"

using std::vector;
using std::cout;
using std::endl;

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
	/*Mesh *testTri = Mesh::GenerateTriangle();

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
	o4->modelMatrix = Matrix4::Translation(Vector3(0.5f, 0.5f, 0.0f));*/

	/* Tutorial 7 objects */
	/*Mesh *triMesh = Mesh::GenerateTriangle();

	RenderObject *o1 = new RenderObject;
	o1->mesh = triMesh;
	o1->modelMatrix = Matrix4::Translation(Vector3(2.0f, 0.0f, -5.0f));

	RenderObject *o2 = new RenderObject;
	o2->mesh = triMesh;
	o2->modelMatrix = Matrix4::Translation(Vector3(2.0f, 0.0f, -25.0f));

	RenderObject *o3 = new RenderObject;
	o3->mesh = triMesh;
	o3->modelMatrix = Matrix4::Translation(Vector3(2.0f, 0.0f, -50.0f));*/

	/*RenderObject *cube = new RenderObject;
	cube->mesh = Mesh::LoadMeshFile("../cube.asciimesh");
	cube->modelMatrix = Matrix4::Translation(Vector3(0, 0, -5));*/

	/* Tutorial 8 objects */
	/*Mesh *triMesh = Mesh::GenerateTriangle();

	RenderObject *o1 = new RenderObject;
	o1->mesh = triMesh;
	o1->modelMatrix = Matrix4::Translation(Vector3(2.0f, 0.0f, -5.0f));

	RenderObject *o2 = new RenderObject;
	o2->mesh = triMesh;
	o2->modelMatrix = Matrix4::Translation(Vector3(2.0f, 0.0f, -25.0f));*/

	/* Tutorial 10 objects */
	/*RenderObject *oTex = new RenderObject;
	oTex->mesh = Mesh::GenerateTriangle();
	oTex->texture = Texture::TextureFromTGA("../brick.tga");
	oTex->modelMatrix = Matrix4::Translation(Vector3(0.0f, 0.0f, -5.0f));*/

	/* Tutorial 11 objects */
	//RenderObject *oAdvTex = new RenderObject;
	//oAdvTex->mesh = Mesh::GenerateTriangle();
	//oAdvTex->texture = Texture::TextureFromTGA("../brick.tga");
	//oAdvTex->modelMatrix = Matrix4::Translation(Vector3(0.0f, 0.0f, -5.0f));


	/* Tutorial 9 objects */
	//Mesh *triM = Mesh::GenerateTriangle();

	//RenderObject *o1 = new RenderObject;
	//RenderObject *o2 = new RenderObject;
	//RenderObject *o3 = new RenderObject;
	//RenderObject *o4 = new RenderObject;

	//o1->mesh = triM;
	//o2->mesh = triM;
	//o3->mesh = triM;
	//o4->mesh = triM;

	//o1->modelMatrix = Matrix4::Translation(Vector3(-0.6f, -0.6f, -2.0f));
	//o2->modelMatrix = Matrix4::Translation(Vector3(-0.6f, 0.6f, -2.0f));
	//o3->modelMatrix = Matrix4::Translation(Vector3(0.6f, -0.6f, -2.0f));
	//o4->modelMatrix = Matrix4::Translation(Vector3(0.6f, 0.6f, -2.0f));
	
	//RenderObject *o5 = new RenderObject;
	//o5->mesh = triM;
	//o5->modelMatrix = Matrix4::RotateX(45.0f) * Matrix4::Scale(Vector3(1.414f, 1.414f, 1.414f));

	//RenderObject *o6 = new RenderObject;
	//o6->mesh = triM;
	//o6->modelMatrix = Matrix4::RotateY(180.0f);

	/* Tutorial 12 objects */
	//RenderObject *o1 = new RenderObject;
	//o1->mesh = Mesh::GenerateTriangle();
	//o1->modelMatrix = Matrix4::Translation(Vector3(0.0f, 0.0f, -6.0f));

	//RenderObject *o2 = new RenderObject;
	//o2->mesh = Mesh::GenerateAlphaTriangle();
	//o2->modelMatrix = Matrix4::Translation(Vector3(0.0f, 0.0f, -5.0f));
	

	
	/* Scene objects */
	RenderObject **points = new RenderObject*[100];
	for(int i = 0; i < 100; i++){
		points[i] = new RenderObject;
		points[i]->mesh = Mesh::GeneratePoint(Vector3(i + 0.01f, 0.0f, -6.0f));
		points[i]->modelMatrix = Matrix4::Translation(Vector3(i+0.01f, 0.0f, -6.0f));
	}
	




	//Aspect ratio to render in (Added in Tutorial 7)
	float aspect = 800.0f / 600.0f;

	//Object's view/camera matrix (Added in Tutorial 6)
	Matrix4 viewMatrix;

	//Added in Tutorial 11
	//Vector3 camTranslate = Vector3(0.0f, 0.0f, -8.0f);

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
		if(Keyboard::KeyDown(KEY_E)){
			viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, 0.0f, 0.01f));
		}
		if(Keyboard::KeyDown(KEY_Q)){
			viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, 0.0f, -0.01f));
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
			viewMatrix = viewMatrix * Matrix4::RotateY(1.0f);
		}
		if(Keyboard::KeyDown(KEY_J)){
			viewMatrix = viewMatrix * Matrix4::RotateY(-1.0f);
		}
		r.SetViewMatrix(viewMatrix);

		//Change filtering technique (Added in Tutorial 11)
		if(Keyboard::KeyDown(KEY_F)){
			r.SwitchTextureFiltering();
		}

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
		//r.DrawObject(o1);
		//r.DrawObject(o2);
		//r.DrawObject(o3);
		//r.DrawObject(o4);

		/* Tutorial 7 objects */
		//Test different projection matrices
		//r.SetProjectionMatrix(Matrix4::Perspective(1.0f, 100.0f, aspect, 45.0f));
		r.SetProjectionMatrix(Matrix4::Perspective(1.0f, 100.0f, aspect, 60.0f));
		//r.SetProjectionMatrix(Matrix4::Perspective(6.0f, 100.0f, aspect, 45.0f));
		//r.SetProjectionMatrix(Matrix4::Perspective(1.0f, 45.0f, aspect, 45.0f));

		/*r.DrawObject(o1);
		r.DrawObject(o2);
		r.DrawObject(o3);*/
		//r.DrawObject(cube);

		/* Tutorial 8 objects */
		/*r.SetProjectionMatrix(Matrix4::Perspective(1.0f, 100.0f, aspect, 60.0f));
		r.DrawObject(o1);
		r.DrawObject(o2);*/

		/* Tutorial 10 objects */
		//r.SetProjectionMatrix(viewMatrix * Matrix4::Orthographic(-1, 1, 10, -20, 10, -20));
		//r.DrawObject(oTex);

		/* Tutorial 11 objects */
		//r.DrawObject(oAdvTex);

		/* Tutorial 9 objects */
		//r.DrawObject(o1);
		//r.DrawObject(o2);
		//r.DrawObject(o3);
		//r.DrawObject(o4);
		
		//r.DrawObject(o5);
		//r.DrawObject(o6);

		/* Tutorial 12 objects */
		//r.DrawObject(o2); 
		//r.DrawObject(o1);



		/* Coursework objects */
		for(int i = 0; i < 100; i++){
			r.DrawObject(points[i]);
		}
		

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
	//delete testTri;

	/* Tutorial 7 objects */
	//As o1, o2, o3 use same mesh, only need to delete it once 
	//delete triMesh;
	//delete cube;

	/* Tutorial 8 objects */
	//delete triMesh;

	/* Tutorial 10 objects */
	//delete oTex;

	/* Tutorial 11 objects */
	//delete oAdvTex;

	/* Tutorial 9 objects */
	//As o1, o2, o3, o4, and o5 use same mesh, only need to delete it once 
	//delete triM;

	/* Tutorial 12 objects */
	//As o1 and o2use same mesh, only need to delete it once 
	//delete o1;
	//delete o2;

	return 0;
}
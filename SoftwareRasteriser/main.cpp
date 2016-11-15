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
	

	
	/* ------------------------------------------
	 *
	 *			 Scene objects
	 *
	 * ------------------------------------------
	 */
	Mesh *generalTriMesh = Mesh::GenerateTriangle();
	
	//Space star-field using points
	RenderObject **points = new RenderObject*[100];
	for(int x = 0; x < 100; x++){
		points[x] = new RenderObject[100];
		for(int y = 0; y < 100; y++){
			points[x][y].mesh = Mesh::GeneratePoint(Vector3(x + 0.01f, y + 0.01f, -(float)((rand() / 99) + 1)));

			points[x][y].modelMatrix = Matrix4::Translation(Vector3(-50.0f + (x*0.01f), -50.0f + (y*0.01f), 0.0f));
		}
	}

	//Asteroid object using lines
	const int ASTEROID_LINES = 9;
	RenderObject **asteroid1 = new RenderObject*[ASTEROID_LINES];
	RenderObject **asteroid2 = new RenderObject*[ASTEROID_LINES];
	RenderObject **asteroid3 = new RenderObject*[ASTEROID_LINES];

	for(int i = 0; i < ASTEROID_LINES; i++){
		asteroid1[i] = new RenderObject;
		asteroid1[i]->modelMatrix = Matrix4::Translation(Vector3(-0.5f, 2.2f, 0.0f));

		asteroid2[i] = new RenderObject;
		asteroid2[i]->modelMatrix = Matrix4::Translation(Vector3(3.7f, -1.3f, 0.0f));

		asteroid3[i] = new RenderObject;
		asteroid3[i]->modelMatrix = Matrix4::Translation(Vector3(-1.1f, 0.8f, 0.0f)) * 
			Matrix4::RotateZ(33.5f) * Matrix4::Scale(Vector3(3.0f, 2.5f, 1.0f));
	}

	float asteroid1Z = -20.0f;

	asteroid1[0]->mesh = Mesh::GenerateLine(Vector3(-0.3f, -0.6f, asteroid1Z), Vector3(-0.55f, -0.45f, asteroid1Z));
	asteroid1[1]->mesh = Mesh::GenerateLine(Vector3(-0.55f, -0.45f, asteroid1Z), Vector3(-0.5f, -0.2f, asteroid1Z));
	asteroid1[2]->mesh = Mesh::GenerateLine(Vector3(-0.5f, -0.2f, asteroid1Z), Vector3(-0.25f, 0.0f, asteroid1Z));
	asteroid1[3]->mesh = Mesh::GenerateLine(Vector3(-0.25f, 0.0f, asteroid1Z), Vector3(0.0f, 0.0f, asteroid1Z));
	asteroid1[4]->mesh = Mesh::GenerateLine(Vector3(0.0f, 0.0f, asteroid1Z), Vector3(0.2f, 0.2f, asteroid1Z));
	asteroid1[5]->mesh = Mesh::GenerateLine(Vector3(0.2f, 0.2f, asteroid1Z), Vector3(0.5f, -0.1f, asteroid1Z));
	asteroid1[6]->mesh = Mesh::GenerateLine(Vector3(0.5f, -0.1f, asteroid1Z), Vector3(0.35f, -0.4f, asteroid1Z));
	asteroid1[7]->mesh = Mesh::GenerateLine(Vector3(0.35f, -0.4f, asteroid1Z), Vector3(0.1f, -0.55f, asteroid1Z));
	asteroid1[8]->mesh = Mesh::GenerateLine(Vector3(0.1f, -0.55f, asteroid1Z), Vector3(-0.3f, -0.6f, asteroid1Z));

	float asteroid2Z = -55.0f;

	asteroid2[0]->mesh = Mesh::GenerateLine(Vector3(0.3f, 0.6f, asteroid2Z), Vector3(0.55f, 0.45f, asteroid2Z));
	asteroid2[1]->mesh = Mesh::GenerateLine(Vector3(0.55f, 0.45f, asteroid2Z), Vector3(0.5f, 0.2f, asteroid2Z));
	asteroid2[2]->mesh = Mesh::GenerateLine(Vector3(0.5f, 0.2f, asteroid2Z), Vector3(0.25f, 0.0f, asteroid2Z));
	asteroid2[3]->mesh = Mesh::GenerateLine(Vector3(0.25f, 0.0f, asteroid2Z), Vector3(0.0f, 0.0f, asteroid2Z));
	asteroid2[4]->mesh = Mesh::GenerateLine(Vector3(0.0f, 0.0f, asteroid2Z), Vector3(-0.2f, -0.2f, asteroid2Z));
	asteroid2[5]->mesh = Mesh::GenerateLine(Vector3(-0.2f, -0.2f, asteroid2Z), Vector3(-0.5f, 0.1f, asteroid2Z));
	asteroid2[6]->mesh = Mesh::GenerateLine(Vector3(-0.5f, 0.1f, asteroid2Z), Vector3(-0.35f, 0.4f, asteroid2Z));
	asteroid2[7]->mesh = Mesh::GenerateLine(Vector3(-0.35f, 0.4f, asteroid2Z), Vector3(-0.1f, 0.55f, asteroid2Z));
	asteroid2[8]->mesh = Mesh::GenerateLine(Vector3(-0.1f, 0.55f, asteroid2Z), Vector3(0.3f, 0.6f, asteroid2Z));

	float asteroid3Z = -63.5f;

	asteroid3[0]->mesh = Mesh::GenerateLine(Vector3(-0.3f, -0.6f, asteroid3Z), Vector3(-0.55f, -0.45f, asteroid3Z));
	asteroid3[1]->mesh = Mesh::GenerateLine(Vector3(-0.55f, -0.45f, asteroid3Z), Vector3(-0.5f, -0.2f, asteroid3Z));
	asteroid3[2]->mesh = Mesh::GenerateLine(Vector3(-0.5f, -0.2f, asteroid3Z), Vector3(-0.25f, 0.0f, asteroid3Z));
	asteroid3[3]->mesh = Mesh::GenerateLine(Vector3(-0.25f, 0.0f, asteroid3Z), Vector3(0.0f, 0.0f, asteroid3Z));
	asteroid3[4]->mesh = Mesh::GenerateLine(Vector3(0.0f, 0.0f, asteroid3Z), Vector3(0.2f, 0.2f, asteroid3Z));
	asteroid3[5]->mesh = Mesh::GenerateLine(Vector3(0.2f, 0.2f, asteroid3Z), Vector3(0.5f, -0.1f, asteroid3Z));
	asteroid3[6]->mesh = Mesh::GenerateLine(Vector3(0.5f, -0.1f, asteroid3Z), Vector3(0.35f, -0.4f, asteroid3Z));
	asteroid3[7]->mesh = Mesh::GenerateLine(Vector3(0.35f, -0.4f, asteroid3Z), Vector3(0.1f, -0.55f, asteroid3Z));
	asteroid3[8]->mesh = Mesh::GenerateLine(Vector3(0.1f, -0.55f, asteroid3Z), Vector3(-0.3f, -0.6f, asteroid3Z));


	//Spaceship made of triangles
	const int SHIP_TRIS = 3;
	RenderObject **spaceShip = new RenderObject*[SHIP_TRIS];
	Texture *shipTex = Texture::TextureFromTGA("../brick.tga");/*("../shipTex1.tga");*/

	for(int i = 0; i < SHIP_TRIS; i++){
		spaceShip[i] = new RenderObject;
		//spaceShip[i]->mesh = generalTriMesh;
		spaceShip[i]->texture = shipTex;
		spaceShip[i]->modelMatrix = Matrix4::Translation(Vector3(0.0f, /*-2.5f*/0.0f, -6.0f));/*
			//Matrix4::RotateY(i % 2 == 1 ? 25.0f : -25.0f) *
			Matrix4::Scale(Vector3(1.5f, 1.5f, 1.0f));*/
	}

	spaceShip[0]->mesh = Mesh::GenerateTriangle(Vector3(0.5f, -0.5f, 0.0f), Vector3(0.0f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, 0.0f));
	spaceShip[1]->mesh = Mesh::GenerateTriangle(Vector3(0.0f, 0.5f, 1.0f), Vector3(0.0f, 0.5f, 0.5f), Vector3(0.5f, -0.5f, 0.0f));
	spaceShip[2]->mesh = Mesh::GenerateTriangle(Vector3(-0.5f, -0.5f, 0.0f), Vector3(0.0f, 0.5f, 0.5f), Vector3(0.0f, 0.5f, 1.0f));


	//Triangle star at center of screen
	RenderObject **star = new RenderObject*[2];
	star[0] = new RenderObject;
	star[1] = new RenderObject;
	
	star[0]->mesh = generalTriMesh;
	star[1]->mesh = generalTriMesh;

	star[0]->modelMatrix = Matrix4::Translation(Vector3(0.0f, 0.0f, -99.0f)) * 
		Matrix4::Scale(Vector3(3.5f, 3.5f, 1.0f));
	star[1]->modelMatrix = Matrix4::Translation(Vector3(0.3f, 0.0f/*-2.0f*/, -99.0f)) *
		Matrix4::Scale(Vector3(3.5f, 3.5f, 1.0f));


	//semi-Transparent triangle
	RenderObject *transparentTri = new RenderObject;
	transparentTri->mesh = Mesh::GenerateAlphaTriangle();
	transparentTri->modelMatrix = Matrix4::Translation(Vector3(1.4f, -1.44f, -76.0f));


	//Triangle wheel
	const int WHEEL_TRIANGLES = 2;
	RenderObject **triWheel = new RenderObject*[WHEEL_TRIANGLES];

	for(int i = 0; i < WHEEL_TRIANGLES; i++){
		triWheel[i] = new RenderObject;
		//triWheel[i]->modelMatrix = Matrix4::Translation(Vector3(-0.75f, 1.0f, -6.0f));
		triWheel[i]->modelMatrix = Matrix4::Translation(Vector3(0.0f, /*-2.5f*/0.0f, -32.0f));
	}

	float wheelZ = -32.0f;

	triWheel[0]->mesh = Mesh::GenerateTriangle(Vector3(0.75f, 0.25f, -1.0f), Vector3(0.5f, 0.75f, -1.0f), Vector3(1.0f, 0.25f, -1.0f));/*Mesh::GenerateTriangle(Vector3(2.5f, 1.5f, wheelZ), Vector3(1.75f, 2.5f, wheelZ), Vector3(3.0f, 1.5f, wheelZ))*/;
	triWheel[1]->mesh = Mesh::GenerateTriangle(Vector3(1.75f, 1.75f, wheelZ), Vector3(1.75f, 2.5f, wheelZ), Vector3(2.5f, 1.5f, wheelZ));





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
			/*viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-0.01f, 0.0f, 0.0f));*/
			for(int i = 0; i < SHIP_TRIS; i++){
				spaceShip[i]->modelMatrix = spaceShip[i]->GetModelMatrix() * Matrix4::Translation(Vector3(-0.01f, 0.0f, 0.0f));
			}
		}
		if(Keyboard::KeyDown(KEY_D)){
			//viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.01f, 0.0f, 0.0f));
			for(int i = 0; i < SHIP_TRIS; i++){
				spaceShip[i]->modelMatrix = spaceShip[i]->GetModelMatrix() * Matrix4::Translation(Vector3(0.01f, 0.0f, 0.0f));
			}
		}
		if(Keyboard::KeyDown(KEY_W)){
			//viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, 0.01f, 0.0f));
			for(int i = 0; i < SHIP_TRIS; i++){
				spaceShip[i]->modelMatrix = spaceShip[i]->GetModelMatrix() * Matrix4::Translation(Vector3(0.0f, 0.01f, 0.0f));
			}
		}
		if(Keyboard::KeyDown(KEY_S)){
			//viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, -0.01f, 0.0f));
			for(int i = 0; i < SHIP_TRIS; i++){
				spaceShip[i]->modelMatrix = spaceShip[i]->GetModelMatrix() * Matrix4::Translation(Vector3(0.0f, -0.01f, 0.0f));
			}
		}
		if(Keyboard::KeyDown(KEY_E)){
			viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, 0.0f, 0.1f));
			for(int i = 0; i < SHIP_TRIS; i++){
				spaceShip[i]->modelMatrix = spaceShip[i]->GetModelMatrix() * Matrix4::Translation(Vector3(0.0f, 0.0f, -0.1f));
			}
		}
		if(Keyboard::KeyDown(KEY_Q)){
			viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, 0.0f, -0.1f));
			for(int i = 0; i < SHIP_TRIS; i++){
				spaceShip[i]->modelMatrix = spaceShip[i]->GetModelMatrix() * Matrix4::Translation(Vector3(0.0f, 0.0f, 0.1f));
			}
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
		//r.DrawObject(o1); 
		//r.DrawObject(o2);



		/* Coursework objects */
		//Make star objects rotate each frame
		star[0]->modelMatrix = star[0]->GetModelMatrix() * Matrix4::Rotation(17.0f, Vector3(0.0f, 0.0f, 1.0f));
		star[1]->modelMatrix = star[1]->GetModelMatrix() * Matrix4::Rotation(-17.0f, Vector3(0.0f, 0.0f, 1.0f));

		r.DrawObject(star[0]);
		r.DrawObject(star[1]);
		
		for(int i = 0; i < ASTEROID_LINES; i++){
			r.DrawObject(asteroid1[i]);
			r.DrawObject(asteroid2[i]);
			r.DrawObject(asteroid3[i]);
		}

		for(int i = 0; i < WHEEL_TRIANGLES; i++){
			r.DrawObject(triWheel[i]);
		}

		for(int i = 0; i < SHIP_TRIS; i++){
			r.DrawObject(spaceShip[i]);
		}

		for(int x = 0; x < 100; x++){
			for(int y = 0; y < 100; y++){
				r.DrawObject(&points[x][y]);
			}
		}
		
		r.DrawObject(transparentTri);

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


	/* Coursework objects */
	for(int x = 0; x < 100; x++){
		delete[] points[x];
	}
	delete[] points;

	for(int i = 0; i < ASTEROID_LINES; i++){
		delete asteroid1[i];
		delete asteroid2[i];
		delete asteroid3[i];
	}
	delete[] asteroid1;
	delete[] asteroid2;
	delete[] asteroid3;

	/*for(int i = 0; i < 4; i++){
		delete spaceShip[i];
	}*/
	//delete shipTris;
	//delete shipTex;

	delete[] spaceShip;

	for(int i = 0; i < WHEEL_TRIANGLES; i++){
		delete triWheel[i];
	}
	delete[] triWheel;

	delete generalTriMesh;
	delete transparentTri;


	return 0;
}
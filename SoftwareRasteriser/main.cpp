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

	//Asteroid objects using lines
	const int ASTEROID_LINES = 9;
	RenderObject **asteroid1 = new RenderObject*[ASTEROID_LINES];
	RenderObject **asteroid2 = new RenderObject*[ASTEROID_LINES];
	RenderObject **asteroid3 = new RenderObject*[ASTEROID_LINES];
	RenderObject **asteroid4 = new RenderObject*[ASTEROID_LINES];

	for(int i = 0; i < ASTEROID_LINES; i++){
		asteroid1[i] = new RenderObject;
		asteroid1[i]->modelMatrix = Matrix4::Translation(Vector3(-0.5f, 2.2f, 0.0f));

		asteroid2[i] = new RenderObject;
		asteroid2[i]->modelMatrix = Matrix4::Translation(Vector3(3.7f, -1.3f, 0.0f));

		asteroid3[i] = new RenderObject;
		asteroid3[i]->modelMatrix = Matrix4::Translation(Vector3(-1.1f, 0.8f, 0.0f)) * 
			Matrix4::RotateZ(33.5f) * Matrix4::Scale(Vector3(3.0f, 2.5f, 1.0f));

		asteroid4[i] = new RenderObject;
		asteroid4[i]->modelMatrix = Matrix4::Translation(Vector3(-2.7f, -3.9f, 0.0f)) *
			Matrix4::RotateZ(-53.5f) * Matrix4::Scale(Vector3(4.0f, 1.5f, 1.0f));
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

	float asteroid4Z = -41.0f;

	asteroid4[0]->mesh = Mesh::GenerateLine(Vector3(-0.3f, -0.6f, asteroid4Z), Vector3(-0.55f, -0.45f, asteroid4Z));
	asteroid4[1]->mesh = Mesh::GenerateLine(Vector3(-0.55f, -0.45f, asteroid4Z), Vector3(-0.5f, -0.2f, asteroid4Z));
	asteroid4[2]->mesh = Mesh::GenerateLine(Vector3(-0.5f, -0.2f, asteroid4Z), Vector3(-0.25f, 0.0f, asteroid4Z));
	asteroid4[3]->mesh = Mesh::GenerateLine(Vector3(-0.25f, 0.0f, asteroid4Z), Vector3(0.0f, 0.0f, asteroid4Z));
	asteroid4[4]->mesh = Mesh::GenerateLine(Vector3(0.0f, 0.0f, asteroid4Z), Vector3(0.2f, 0.2f, asteroid4Z));
	asteroid4[5]->mesh = Mesh::GenerateLine(Vector3(0.2f, 0.2f, asteroid4Z), Vector3(0.5f, -0.1f, asteroid4Z));
	asteroid4[6]->mesh = Mesh::GenerateLine(Vector3(0.5f, -0.1f, asteroid4Z), Vector3(0.35f, -0.4f, asteroid4Z));
	asteroid4[7]->mesh = Mesh::GenerateLine(Vector3(0.35f, -0.4f, asteroid4Z), Vector3(0.1f, -0.55f, asteroid4Z));
	asteroid4[8]->mesh = Mesh::GenerateLine(Vector3(0.1f, -0.55f, asteroid4Z), Vector3(-0.3f, -0.6f, asteroid4Z));


	//Spaceship made of triangle primitives
	const int SHIP_TRIS = 4;
	RenderObject **spaceShip = new RenderObject*[SHIP_TRIS];
	Texture *shipTex = Texture::TextureFromTGA("../brick.tga");/*("../shipTex1.tga");*/

	for(int i = 0; i < SHIP_TRIS; i++){
		spaceShip[i] = new RenderObject;
		spaceShip[i]->texture = shipTex;
		spaceShip[i]->modelMatrix = Matrix4::Translation(Vector3(0.0f, -2.5f, -6.0f)) *
			Matrix4::Scale(Vector3(1.25f, 1.25f, 1.0f));
	}

	//Create custom vertices for spaceship triangles
	spaceShip[0]->mesh = Mesh::GenerateTriangle(Vector3(0.5f, -0.5f, 0.0f), Vector3(0.0f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, 0.0f));
	spaceShip[1]->mesh = Mesh::GenerateTriangle(Vector3(0.0f, 0.5f, 1.0f), Vector3(0.0f, 0.5f, 0.5f), Vector3(0.5f, -0.5f, 0.0f));
	spaceShip[2]->mesh = Mesh::GenerateTriangle(Vector3(-0.5f, -0.5f, 0.0f), Vector3(0.0f, 0.5f, 0.5f), Vector3(0.0f, 0.5f, 1.0f));
	spaceShip[3]->mesh = Mesh::GenerateTriangle(Vector3(0.5f, -0.5f, 0.0f), Vector3(0.0f, 0.5f, 1.0f), Vector3(-0.5f, -0.5f, 0.0f));


	//Rotating object
	RenderObject *rotTri = new RenderObject;
	rotTri->texture = Texture::TextureFromTGA("../shipTex1.tga");
	rotTri->mesh = generalTriMesh;
	rotTri->modelMatrix = Matrix4::Translation(Vector3(10.0f, 10.5f, -80.0f)) *
		Matrix4::Scale(Vector3(2.0f, 2.0f, 1.0f));


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
	transparentTri->modelMatrix = Matrix4::Translation(Vector3(0.4f, -1.44f, -76.0f)) *
		Matrix4::Scale(Vector3(2.0f, 2.0f, 1.0f));



	//Aspect ratio to render in (Added in Tutorial 7)
	float aspect = 800.0f / 600.0f;

	//Object's view/camera matrix (Added in Tutorial 6)
	Matrix4 viewMatrix;


	while(r.UpdateWindow() && !Keyboard::KeyDown(KEY_ESCAPE)){
		r.ClearBuffers();

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
			viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, 0.0f, 0.25f));
			for(int i = 0; i < SHIP_TRIS; i++){
				spaceShip[i]->modelMatrix = spaceShip[i]->GetModelMatrix() * Matrix4::Translation(Vector3(0.0f, 0.0f, -0.25f));
			}
		}
		if(Keyboard::KeyDown(KEY_Q)){
			viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.0f, 0.0f, -0.25f));
			for(int i = 0; i < SHIP_TRIS; i++){
				spaceShip[i]->modelMatrix = spaceShip[i]->GetModelMatrix() * Matrix4::Translation(Vector3(0.0f, 0.0f, 0.25f));
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

		//Change texture filtering technique
		if(Keyboard::KeyDown(KEY_F)){
			r.SwitchTextureFiltering();
		}

		//Put draw functions here!
		
		//Set projection matrix
		r.SetProjectionMatrix(Matrix4::Perspective(1.0f, 100.0f, aspect, 60.0f));


		/* Coursework objects */
		//Make star objects rotate each frame
		star[0]->modelMatrix = star[0]->GetModelMatrix() * Matrix4::RotateZ(17.0f);
		star[1]->modelMatrix = star[1]->GetModelMatrix() * Matrix4::RotateZ(-17.0f);

		r.DrawObject(star[0]);
		r.DrawObject(star[1]);

		//Make triangle rotate
		rotTri->modelMatrix = rotTri->GetModelMatrix() * Matrix4::Translation(Vector3(-0.01f, -0.01f, 0.1f)) * Matrix4::Rotation(10.0f, Vector3(1.0f, 1.0f, 0.0f));

		//render every asteroid
		for(int i = 0; i < ASTEROID_LINES; i++){
			r.DrawObject(asteroid1[i]);
			r.DrawObject(asteroid2[i]);
			r.DrawObject(asteroid3[i]);
			r.DrawObject(asteroid4[i]);
		}

		//Render every textured triangle composing the ship
		for(int i = 0; i < SHIP_TRIS; i++){
			r.DrawObject(spaceShip[i]);
		}

		//Draw the stars using points
		for(int x = 0; x < 100; x++){
			for(int y = 0; y < 100; y++){
				r.DrawObject(&points[x][y]);
			}
		}

		//Draw rotating moving triangle
		r.DrawObject(rotTri);

		//Draw transparent triangle (must be last or alpha blending won't work)
		r.DrawObject(transparentTri);


		r.SwapBuffers();
	}

	/* Delete RenderObjects to avoid memory leaks */
	
	/* Coursework objects */

	//Delete contents of each sub-points array, then array itself 
	for(int x = 0; x < 100; x++){
		delete[] points[x];
	}
	delete[] points;

	//Delete asteroid objects
	for(int i = 0; i < ASTEROID_LINES; i++){
		delete asteroid1[i];
		delete asteroid2[i];
		delete asteroid3[i];
		delete asteroid4[i];
	}

	delete[] asteroid1;
	delete[] asteroid2;
	delete[] asteroid3;
	delete[] asteroid4;

	//Delete spaceship
	delete[] spaceShip;

	//Delete triangles used as meshes
	delete generalTriMesh;
	delete transparentTri;


	return 0;
}
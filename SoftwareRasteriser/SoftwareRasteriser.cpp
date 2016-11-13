#include "SoftwareRasteriser.h"
#include <cmath>
#include <math.h>
/*
While less 'neat' than just doing a 'new', like in the tutorials, it's usually
possible to render a bit quicker to use direct pointers to the drawing area
that the OS gives you. For a bit of a speedup, you can uncomment the define below
to switch to using this method.

For those of you new to the preprocessor, here's a quick explanation:

Preprocessor definitions like #define allow parts of a file to be selectively enabled
or disabled at compile time. This is useful for hiding parts of the codebase on a
per-platform basis: if you have support for linux and windows in your codebase, obviously
the linux platform won't have the windows platform headers available, so compilation will
fail. So instead you can hide away all the platform specific stuff:

#if PLATFORM_WINDOWS
 DoSomeWindowsStuff();
#elif PLATFORM_LINUX
 DoSomeLinuxStuff();
 #else
 #error Unsupported Platform Specified!
 #endif

 As in our usage, it also allows you to selectively compile in some different functionality
 without any 'run time' cost - if it's not enabled by the preprocessor, it won't make it to
 the compiler, so no assembly will be generated.

Also, I've implemented the Resize method for you, in a manner that'll behave itself
no matter which method you use. I kinda forgot to do that, so there was a chance you'd
get exceptions if you resized to a bigger screen area. Sorry about that.
*/
//#define USE_OS_BUFFERS

SoftwareRasteriser::SoftwareRasteriser(uint width, uint height)	: Window(width, height){
	currentDrawBuffer	= 0;

#ifndef USE_OS_BUFFERS
	//Hi! In the tutorials, it's mentioned that we need to form our front + back buffer like so:
	for (int i = 0; i < 2; ++i) {
		buffers[i] = new Colour[screenWidth * screenHeight];
	}
#else
	//This works, but we can actually save a memcopy by rendering directly into the memory the 
	//windowing system gives us, which I've added to the Window class as the 'bufferData' pointers
	for (int i = 0; i < 2; ++i) {
		buffers[i] = (Colour*)bufferData[i];
	}
#endif

	depthBuffer	= new unsigned short[screenWidth * screenHeight];

	float zScale = (pow(2.0f,16) - 1) * 0.5f;

	Vector3 halfScreen = Vector3((screenWidth - 1) * 0.5f, (screenHeight - 1) * 0.5f, zScale);

	portMatrix = Matrix4::Translation(halfScreen) * Matrix4::Scale(halfScreen);
}

SoftwareRasteriser::~SoftwareRasteriser(void)	{
#ifndef USE_OS_BUFFERS
	for(int i = 0; i < 2; ++i) {
		delete[] buffers[i];
	}
#endif
	delete[] depthBuffer;
}

void SoftwareRasteriser::Resize() {
	Window::Resize(); //make sure our base class gets to do anything it needs to

#ifndef USE_OS_BUFFERS
	for (int i = 0; i < 2; ++i) {
		delete[] buffers[i];
		buffers[i] = new Colour[screenWidth * screenHeight];
	}
#else
	for (int i = 0; i < 2; ++i) {
		buffers[i] = (Colour*)bufferData[i];
	}
#endif

	delete[] depthBuffer;
	depthBuffer = new unsigned short[screenWidth * screenHeight];

	float zScale = (pow(2.0f, 16) - 1) * 0.5f;

	Vector3 halfScreen = Vector3((screenWidth - 1) * 0.5f, (screenHeight - 1) * 0.5f, zScale);

	portMatrix = Matrix4::Translation(halfScreen) * Matrix4::Scale(halfScreen);
}

Colour*	SoftwareRasteriser::GetCurrentBuffer() {
	return buffers[currentDrawBuffer];
}

void	SoftwareRasteriser::ClearBuffers() {
	Colour* buffer = GetCurrentBuffer();

	unsigned int clearVal = 0xFF000000;
	unsigned int depthVal = ~0;

	for(uint y = 0; y < screenHeight; ++y) {
		for(uint x = 0; x < screenWidth; ++x) {
			buffer[(y * screenWidth) + x].c  = clearVal;
			depthBuffer[(y * screenWidth) + x] = depthVal;
		}
	}
}

void	SoftwareRasteriser::SwapBuffers() {
	PresentBuffer(buffers[currentDrawBuffer]);
	currentDrawBuffer = !currentDrawBuffer;
}

void SoftwareRasteriser::DrawObject(RenderObject *o) {
	switch(o->GetMesh()->GetType()){
		case PRIMITIVE_POINTS:
			RasterisePointsMesh(o);
			break;
		case PRIMITIVE_LINES:
			RasteriseLinesMesh(o);
			break;
		case PRIMITIVE_TRIANGLES:
			RasteriseTriMesh(o);
			break;
	}
}


BoundingBox SoftwareRasteriser::CalculateBoxForTri(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2){
	BoundingBox box;

	//Start with the first Vertex value
	box.topLeft.x = v0.x;
	//Swap to second if less
	box.topLeft.x = min(box.topLeft.x, v1.x);
	//Swap to third if less
	box.topLeft.x = min(box.topLeft.x, v2.x);
	//Screen bound
	box.topLeft.x = max(box.topLeft.x, 0.0f);

	box.topLeft.y = v0.y;
	box.topLeft.y = min(box.topLeft.y, v1.y);
	box.topLeft.y = min(box.topLeft.y, v2.y);
	//Screen bound
	box.topLeft.y = max(box.topLeft.y, 0.0f);


	//Start with the first Vertex value
	box.bottomRight.x = v0.x;
	//Swap to second if less
	box.bottomRight.x = max(box.bottomRight.x, v1.x);
	//Swap to third if less
	box.bottomRight.x = max(box.bottomRight.x, v2.x);
	//Screen bound
	box.bottomRight.x = min(box.bottomRight.x, screenWidth);

	box.bottomRight.y = v0.y;
	box.bottomRight.y = max(box.bottomRight.y, v1.y);
	box.bottomRight.y = max(box.bottomRight.y, v2.y);
	//Screen bound
	box.bottomRight.y = min(box.bottomRight.y, screenHeight);

	return box;
}

float SoftwareRasteriser::ScreenAreaOfTri(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2){
	float area = ((v0.x * v1.y) + (v1.x * v2.y) + (v2.x * v0.y)) - //Fist diagonals
		((v1.x * v0.y) + (v2.x * v1.y) + (v0.x * v2.y)); //Second diagonals

	return area * 0.5f;
}


/* --------------------------------------------------
*
* Rasterisation Preperation Functions
*
-------------------------------------------------- */
void SoftwareRasteriser::RasterisePointsMesh(RenderObject *o) {
	Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();

	for(uint i = 0; i < o->GetMesh()->numVertices; ++i){
		Vector4 vertexPos = mvp * o->GetMesh()->vertices[i];
		vertexPos.SelfDivisionByW();

		Vector4 screenPos = portMatrix * vertexPos;
		ShadePixel((uint)screenPos.x, (uint)screenPos.y, Colour::White);
	}
}

void SoftwareRasteriser::RasteriseLinesMesh(RenderObject*o) {
	Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();
	Mesh *m = o->GetMesh();

	for(uint i = 0; i < m->numVertices; i += 2){
		Vector4 v0 = mvp * m->vertices[i],
			v1 = mvp * m->vertices[i + 1];

		//Add colors to vertices
		Colour c0 = m->colours[i],
			c1 = m->colours[i + 1];

		v0.SelfDivisionByW();
		v1.SelfDivisionByW();

		RasteriseLine(v0, v1, c0, c1);
	}
}

void SoftwareRasteriser::RasteriseTriMesh(RenderObject *o) {
	Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();
	Mesh *m = o->GetMesh();

	for(uint i = 0; i < m->numVertices; i += 3){
		Vector4 v0 = mvp * m->vertices[i],
			v1 = mvp * m->vertices[i + 1],
			v2 = mvp * m->vertices[i + 2];

		v0.SelfDivisionByW();
		v1.SelfDivisionByW();
		v2.SelfDivisionByW();

		RasteriseTri(v0, v1, v2,
			//Add vertex colors
			m->colours[i], m->colours[1], m->colours[2]);
	}
}


/* --------------------------------------------------
 *
 * Actual Rasterisation Functions 
 *
 -------------------------------------------------- */

void SoftwareRasteriser::RasteriseLine(const Vector4 &vertA, const Vector4 &vertB,
	const Colour &colA, const Colour &colB,
	const Vector2 &texA, const Vector2 &texB){

	//Transform the NDC coordinates to screen coordinates
	Vector4 v0 = portMatrix * vertA,
		v1 = portMatrix * vertB;
	
	//Determine direction line is going
	Vector4 dir = v1 - v0;

	int xDir = (dir.x < 0.0f) ? -1 : 1, //Move Left or Right?
		yDir = (dir.y < 0.0f) ? -1 : 1, //Move Up or Down?
		x = (int)v0.x, //Current x-axis plot point
		y = (int)v0.y; //Current y-axis plot point

	int *target = NULL;
	int *scan = NULL;

	int scanVal = 0,
		targetVal = 0;

	float slope = 0.0f;

	//Bind our loop by this value
	int range = 0;

	//If we're steep, we have to scan over y
	if(abs(dir.y) > abs(dir.x)){
		slope = dir.x / dir.y;
		range = (int)abs(dir.y);

		//Error on x
		target = &x;
		//Iterate over y
		scan = &y;

		scanVal = yDir;
		targetVal = xDir;
	}
	//If not steep, scan over x
	else{
		slope = dir.y / dir.x;
		range = (int)abs(dir.x);

		target = &y; //Error on y
		scan = &x; //Iterate over x

		scanVal = xDir;
		targetVal = yDir;
	}

	float absSlope = abs(slope),
		error = 0.0f;

	//Saves a divide later
	float reciprocalRange = 1.0f / range;

	for(int i = 0; i < range; ++i){
		float t = i * reciprocalRange;

		Colour currentCol = colB * t + colA * (1.0f - t);

		//Start of depth buffer integration
		float zVal = (v1.z * (i * reciprocalRange)) + (v0.z * (1.0f - (i * reciprocalRange)));

		if(DepthFunc((int)x, (int)y, zVal)){
			ShadePixel(x, y, currentCol);
		}
		//End of depth buffer integration


		error += absSlope;

		if(error > 0.5f){
			error -= 1.0f;
			//Either advance along x or y
			*target += targetVal;
		}

		//Either iterate over x or y
		*scan += scanVal;
	}
}


void SoftwareRasteriser::RasteriseTri(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2,
	const Colour &c0, const Colour &c1, const Colour &c2,
	const Vector3 &t0, const Vector3 &t1, const Vector3 &t2){

	//Transform vectors to be in viewport space
	Vector4 vA = portMatrix * v0,
		vB = portMatrix * v1,
		vC = portMatrix * v2;

	BoundingBox b = CalculateBoxForTri(vA, vB, vC);

	float triArea = ScreenAreaOfTri(vA, vB, vC);
	float reciprocalArea = 1.0f / triArea;

	float subTriArea[3];
	Vector4 screenPos(0.0f, 0.0f, 0.0f, 1.0f);

	for(float y = b.topLeft.y; y < b.bottomRight.y; ++y){
		for(float x = b.topLeft.x; x < b.bottomRight.x; ++x){
			screenPos.x = x;
			screenPos.y = y;

			subTriArea[0] = abs(ScreenAreaOfTri(vA, screenPos, vB));
			subTriArea[1] = abs(ScreenAreaOfTri(vB, screenPos, vC));
			subTriArea[2] = abs(ScreenAreaOfTri(vC, screenPos, vA));

			float triSum = subTriArea[0] + subTriArea[1] + subTriArea[2];

			if(triSum > (triArea + 1.0f)){
				//Current pixel is not in the triangle, so ignore it
				continue;
			}
			if(triSum < 1.0f){
				//Tiny triangle, ignore it
				continue;
			}

			//Change shading pixel to add appropriate interpolated color
			//ShadePixel((uint)x, (uint)y, Colour::White);
			
			float alpha = subTriArea[0] * reciprocalArea,
				beta = subTriArea[1] * reciprocalArea,
				gamma = subTriArea[2] * reciprocalArea;

			//Start of depth buffer integrations
			float zVal = (vA.z * alpha) + (vB.z * beta) + (vC.z * gamma);

			if(!DepthFunc((int)x, (int)y, zVal)){
				continue;
			}
			//End of depth buffer integrations

			Colour subColor = ((c0 * alpha) + (c1 * beta) + (c2 * gamma));

			ShadePixel((uint)x, (uint)y, subColor);
		}
	}

}
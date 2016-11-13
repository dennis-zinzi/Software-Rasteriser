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

	depthBuffer		=	new unsigned short[screenWidth * screenHeight];

	float zScale	= (pow(2.0f,16) - 1) * 0.5f;

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

		v0.SelfDivisionByW();
		v1.SelfDivisionByW();

		RasteriseLine(v0, v1);
	}
}

void SoftwareRasteriser::RasteriseTriMesh(RenderObject *o) {

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

	for(int i = 0; i < range; ++i){
		ShadePixel(x, y, Colour::White);

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
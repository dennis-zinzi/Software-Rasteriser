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
#define MAX_VERTICES 15

//000000
const int INSIDE_CS = 0;
//000001
const int LEFT_CS = 1;
//000010
const int RIGHT_CS = 2;
//000100
const int BOTTOM_CS = 4;
//001000
const int TOP_CS = 8;
//010000
const int NEAR_CS = 16;
//100000
const int FAR_CS = 32;


SoftwareRasteriser::SoftwareRasteriser(uint width, uint height)	: Window(width, height){
	currentDrawBuffer	= 0;
	currentTex = NULL;
	texSampleState = SAMPLE_NEAREST;

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
	currentTex = o->texture;

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


void SoftwareRasteriser::CalculateWeights(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2,
	const Vector4 &pnt, float &aplha, float &beta, float &gamma){

	float triArea = ScreenAreaOfTri(v0, v1, v2);
	float reciprocalArea = 1.0f / triArea;

	float subTriArea[3];
	subTriArea[0] = abs(ScreenAreaOfTri(v0, pnt, v1));
	subTriArea[1] = abs(ScreenAreaOfTri(v1, pnt, v2));
	subTriArea[2] = abs(ScreenAreaOfTri(v2, pnt, v0));

	aplha = subTriArea[1] * reciprocalArea;
	beta = subTriArea[2] * reciprocalArea;
	gamma = subTriArea[0] * reciprocalArea;
}


int SoftwareRasteriser::HomogenousOutcode(const Vector4 &in){
	int outCode = INSIDE_CS;
	
	if(in.x < -in.w){
		outCode |= LEFT_CS;
	}
	else if(in.x > in.w){
		outCode |= RIGHT_CS;
	}

	if(in.y < -in.w){
		outCode |= BOTTOM_CS;
	}
	else if(in.y > in.w){
		outCode |= TOP_CS;
	}

	if(in.z < -in.w){
		outCode |= NEAR_CS;
	}
	else if(in.z > in.w){
		outCode |= FAR_CS;
	}

	return outCode;
}


float SoftwareRasteriser::ClipEdge(const Vector4 &inA, const Vector4 &inB, int axis){
	float ratio = 0.0f;

	switch(axis){
		case LEFT_CS:
			ratio = (-inA.w - inA.x) / ((inB.x - inA.x) + inB.w - inA.w);
			break;
		case RIGHT_CS:
			ratio = (inA.w - inA.x) / ((inB.x - inA.x) - inB.w + inA.w);
			break;

		case BOTTOM_CS:
			ratio = (-inA.w - inA.y) / ((inB.y - inA.y) + inB.w - inA.w);
			break;
		case TOP_CS:
			ratio = (inA.w - inA.y) / ((inB.y - inA.y) - inB.w + inA.w);
			break;

		case NEAR_CS:
			ratio = (-inA.w - inA.z) / ((inB.z - inA.z) + inB.w - inA.w);
			break;
		case FAR_CS:
			ratio = (inA.w - inA.z) / ((inB.z - inA.z) - inB.w + inA.w);
			break;
	}

	return min(1.0f, ratio);
}


bool SoftwareRasteriser::CohenSutherlandLine(Vector4 &inA, Vector4 &inB,
	Colour &colA, Colour &colB,
	Vector3 &texA, Vector3 &texB){

	for(int i = 0; i < 6; ++i){
		int planeCode = 1 << i;

		int outsideA = HomogenousOutcode(inA) & planeCode,
			outsideB = HomogenousOutcode(inB) & planeCode;

		if(outsideA && outsideB){
			//if both outside of the plane, discard
			return false;
		}
		if(!outsideA && !outsideB){
			//Both inside of the plane, continue
			continue;
		}
		else{
			float clipRatio = ClipEdge(inA, inB, planeCode);

			if(outsideA){
				inA = Vector4::Lerp(inA, inB, clipRatio);
				colA = Colour::Lerp(colA, colB, clipRatio);
				texA = Vector3::Lerp(texA, texB, clipRatio);
			}
			else{
				inB = Vector4::Lerp(inA, inB, clipRatio);
				colB = Colour::Lerp(colA, colB, clipRatio);
				texB = Vector3::Lerp(texA, texB, clipRatio);
			}
		}
	}
	return true;
}


void SoftwareRasteriser::SutherlandHodgmanTri(Vector4 &v0, Vector4 &v1, Vector4 &v2,
	const Colour &c0, const Colour &c1, const Colour &c2,
	const Vector2 &t0, const Vector2 &t1, const Vector2 &t2){

	Vector4 posIn[MAX_VERTICES];
	Colour colIn[MAX_VERTICES];
	Vector3 texIn[MAX_VERTICES];

	Vector4 posOut[MAX_VERTICES];
	Colour colOut[MAX_VERTICES];
	Vector3 texOut[MAX_VERTICES];

	posIn[0] = v0;
	posIn[1] = v1;
	posIn[2] = v2;

	colIn[0] = c0;
	colIn[1] = c1;
	colIn[2] = c2;

	texIn[0] = Vector3(t0.x, t0.y, 1.0f);
	texIn[1] = Vector3(t1.x, t1.y, 1.0f);
	texIn[2] = Vector3(t2.x, t2.y, 1.0f);

	int inSize = 3;

	for(int i = 0; i <= 6; i++){
		int planeCode = 1 << i;

		Vector4 prevPos = posIn[inSize - 1];
		Colour prevCol = colIn[inSize - 1];
		Vector3 prevTex = texIn[inSize - 1];

		int outSize = 0;

		for(int j = 0; j < inSize; ++j){
			int outsideA = HomogenousOutcode(posIn[j]) & planeCode,
				outSideB = HomogenousOutcode(prevPos) & planeCode;

			if(outsideA ^ outSideB){
				float clipRatio = ClipEdge(posIn[j], prevPos, planeCode);

				posOut[outSize] = Vector4::Lerp(posIn[j], prevPos, clipRatio);
				colOut[outSize] = Colour::Lerp(colIn[j], prevCol, clipRatio);
				texOut[outSize] = Vector3::Lerp(texIn[j], prevTex, clipRatio);

				outSize++;
			}

			if(!outsideA){
				posOut[outSize] = posIn[j];
				colOut[outSize] = colIn[j];
				texOut[outSize] = texIn[j];
				
				outSize++;
			}

			prevPos = posIn[j];
			prevCol = colIn[j];
			prevTex = texIn[j];
			//End of vertex processing loop
		}

		for(int j = 0; j < outSize; ++j){
			posIn[j] = posOut[j];
			colIn[j] = colOut[j];
			texIn[j] = texOut[j];
		}

		inSize = outSize;
		//End of plane clipping loop
	}

	for(int i = 0; i < inSize; ++i){
		texIn[i] = Vector3(texIn[i].x, texIn[i].y, 1.0f) / posIn[i].w;
		posIn[i].SelfDivisionByW();
	}

	for(int i = 2; i < inSize; ++i){
		RasteriseTri(posIn[0], posIn[i - 1], posIn[i],
			colIn[0], colIn[i - 1], colIn[i],
			texIn[0], texIn[i - 1], texIn[i]);
	}
}

/* --------------------------------------------------
*
* Rasterisation Preperation Functions
*
-------------------------------------------------- */
void SoftwareRasteriser::RasterisePointsMesh(RenderObject *o) {
	Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();
	Mesh *m = o->GetMesh();

	for(uint i = 0; i < m->numVertices; ++i){
		Vector4 vertexPos = mvp * m->vertices[i];
		Colour c0 = m->colours[i];

		vertexPos.SelfDivisionByW();

		Vector4 screenPos = portMatrix * vertexPos;
		ShadePixel((uint)screenPos.x, (uint)screenPos.y, c0);
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

		//Apply textures to vertices (Added in Tutorial 9)
		Vector3 t0 = Vector3(m->textureCoords[i].x, m->textureCoords[i].y, 1.0f),
			t1 = Vector3(m->textureCoords[i+1].x, m->textureCoords[i+1].y, 1.0f);

		if(!CohenSutherlandLine(v0, v1, c0, c1, t0, t1)){
			continue;
		}

		t0.z = 1.0f;
		t1.z = 1.0f;

		t0 /= v0.w;
		t1 /= v1.w;

		v0.SelfDivisionByW();
		v1.SelfDivisionByW();

		RasteriseLine(v0, v1, c0, c1, t0, t1);
	}
}

void SoftwareRasteriser::RasteriseTriMesh(RenderObject *o) {
	Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();
	Mesh *m = o->GetMesh();

	for(uint i = 0; i < m->numVertices; i += 3){
		Vector4 v0 = mvp * m->vertices[i],
			v1 = mvp * m->vertices[i + 1],
			v2 = mvp * m->vertices[i + 2];

		/*Vector3 t0 = Vector3(m->textureCoords[i].x, m->textureCoords[i].y, 1.0f) / v0.w,
			t1 = Vector3(m->textureCoords[i+1].x, m->textureCoords[i+1].y, 1.0f) / v1.w,
			t2 = Vector3(m->textureCoords[i+2].x, m->textureCoords[i+2].y, 1.0f) / v2.w;

		v0.SelfDivisionByW();
		v1.SelfDivisionByW();
		v2.SelfDivisionByW();*/

		float triArea = ScreenAreaOfTri(v0, v1, v2);

		if(triArea < 0.0f){
			continue;
		}

		SutherlandHodgmanTri(v0, v1, v2,
			m->colours[i], m->colours[i + 1], m->colours[i + 2],
			m->textureCoords[i], m->textureCoords[i + 1], m->textureCoords[i + 2]);
			//Vector2(t0.x, t0.y), Vector2(t1.x, t1.y), Vector2(t2.x, t2.y));

		//RasteriseTri(v0, v1, v2,
		//	//Add vertex colors
		//	m->colours[i], m->colours[1], m->colours[2],
		//	t0, t1, t2);
	}
}


/* --------------------------------------------------
 *
 * Actual Rasterisation Functions 
 *
 -------------------------------------------------- */

void SoftwareRasteriser::RasteriseLine(const Vector4 &vertA, const Vector4 &vertB,
	const Colour &colA, const Colour &colB,
	const Vector3 &texA, const Vector3 &texB){

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

		Vector3 subTex = (texB * (i * reciprocalRange)) + (texA * (1.0f - (i * reciprocalRange)));
		subTex.x /= subTex.z;
		subTex.y /= subTex.z;

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
				std::cout << "STOP dinky tri" << std::endl;
				//Tiny triangle, ignore it
				continue;
			}

			//Change shading pixel to add appropriate interpolated color
			//ShadePixel((uint)x, (uint)y, Colour::White);
			
			float alpha = subTriArea[1] * reciprocalArea,
				beta = subTriArea[2] * reciprocalArea,
				gamma = subTriArea[0] * reciprocalArea;

			//Start of depth buffer integrations
			float zVal = (vA.z * alpha) + (vB.z * beta) + (vC.z * gamma);

			if(!DepthFunc((int)x, (int)y, zVal)){
				continue;
			}
			//End of depth buffer integrations

			if(currentTex){
				Vector3 subTex = (t0 * alpha) + (t1 * beta) + (t2 * gamma);

				//Convert coordinates back into world-linear space
				subTex.x /= subTex.z;
				subTex.y /= subTex.z;

				if(texSampleState == SAMPLE_NEAREST){
					ShadePixel((uint)x, (uint)y, currentTex->NearestTexSample(subTex));
				}
				else if(texSampleState == SAMPLE_BILINEAR){
					ShadePixel((uint)x, (uint)y, currentTex->BilinearTexSample(subTex));
				}
				else if(texSampleState == SAMPLE_MIPMAP_NEAREST){
					float xAlpha,
						xBeta,
						xGamma;

					float yAlpha,
						yBeta,
						yGamma;

					CalculateWeights(vA, vB, vC, screenPos + Vector4(1.0f, 0.0f, 0.0f, 0.0f),
						xAlpha, xBeta, xGamma);

					CalculateWeights(vA, vB, vC, screenPos + Vector4(0.0f, 1.0f, 0.0f, 0.0f),
						yAlpha, yBeta, yGamma);

					Vector3 xDerivs = (t0 * xAlpha) + (t1 * xBeta) + (t2 * xGamma),
						yDerivs = (t0 * yAlpha) + (t1 * yBeta) + (t2 * yGamma);

					//Return to a linear texture-space
					xDerivs.x /= xDerivs.z;
					xDerivs.y /= xDerivs.z;

					yDerivs.x /= yDerivs.z;
					yDerivs.y /= yDerivs.z;

					//Get the rate of change on the x-axis
					xDerivs -= subTex;
					//Get the rate of change on the y-axis
					yDerivs -= subTex;

					float maxU = max(abs(xDerivs.x), abs(yDerivs.x)),
						maxV = max(abs(xDerivs.x), abs(yDerivs.y));

					float maxChange = abs(max(maxU, maxV));

					int lambda = (int)abs(log(maxChange) / log(2.0f));

					//Sample from the usual texture coords, with new Level Of Detail (LOD)
					ShadePixel((uint)x, (uint)y, currentTex->NearestTexSample(subTex, lambda));
				}
			}
			else{
				Colour subColor = ((c0 * alpha) + (c1 * beta) + (c2 * gamma));

				//ShadePixel((uint)x, (uint)y, subColor);
				BlendPixel(x, y, subColor);
			}
		}
	}

}
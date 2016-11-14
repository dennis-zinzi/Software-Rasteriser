/******************************************************************************
Class:SoftwareRasteriser
Implements:Window
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description: Class to encapsulate the various rasterisation techniques looked
at in the course material.

This is the class you'll be modifying the most!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Matrix4.h"
#include "Mesh.h"
#include "Texture.h"
#include "RenderObject.h"
#include "Common.h"
#include "Window.h"

#include <vector>

using std::vector;

struct BoundingBox {
	Vector2 topLeft;
	Vector2 bottomRight;
};

enum SampleState{
	SAMPLE_NEAREST,
	SAMPLE_BILINEAR,
	SAMPLE_MIPMAP_NEAREST,
	SAMPLE_MIMPAP_BILINEAR
};

class RenderObject;
class Texture;

class SoftwareRasteriser : public Window	{
public:
	SoftwareRasteriser(uint width, uint height);
	~SoftwareRasteriser(void);

	void	DrawObject(RenderObject*o);

	void	ClearBuffers();
	void	SwapBuffers();

	inline void SetViewMatrix(const Matrix4 &m) {
		viewMatrix = m;
		viewProjMatrix = projectionMatrix * viewMatrix;
	}
	
	inline void SetProjectionMatrix(const Matrix4 &m) {
		projectionMatrix = m;
		viewProjMatrix = projectionMatrix * viewMatrix;
	}

	static float ScreenAreaOfTri(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2);

	inline void SwitchTextureFiltering(){
		if(texSampleState == SAMPLE_NEAREST){
			texSampleState = SAMPLE_BILINEAR;
			std::cout << "NOW BILINEAR" << std::endl;
		}
		else if(texSampleState == SAMPLE_BILINEAR){
			texSampleState = SAMPLE_MIPMAP_NEAREST;
			std::cout << "NOW MIPMAP NEAREST" << std::endl;
		}
		else /*if(texSampleState == SAMPLE_MIPMAP_NEAREST)*/{
			//texSampleState = SAMPLE_MIMPAP_BILINEAR;
			texSampleState = SAMPLE_NEAREST;
			std::cout << "NOW NEAREST" << std::endl;
		}
		/* uncomment when implemented bilinear mipmap sampling
		else{
			texSampleState = SAMPLE_NEAREST;
		}
		*/
	}

protected:
	Colour*	GetCurrentBuffer();

	void	RasterisePointsMesh(RenderObject*o);
	void	RasteriseLinesMesh(RenderObject*o);

	virtual void Resize();

	void	RasteriseLine(const Vector4 &vertA, const Vector4 &vertB, 
		const Colour &colA = Colour(), const Colour &colB = Colour(), 
		const Vector3 &texA = Vector3() , const Vector3 &texB = Vector3());



	inline void	ShadePixel(uint x, uint y, const Colour&c) {
		if(y >= screenHeight) {
			return;
		}
		if(x >= screenWidth) {
			return;
		}

		int index =  (y * screenWidth) + x;

		buffers[currentDrawBuffer][index] = c;
	}

	inline void BlendPixel(int x, int y, const Colour &source){
		if(y >= screenHeight || y < 0){
			//current pixel is off-screen
			return;
		}
		if(x >= screenWidth || x < 0){
			//current pixel is off-screen
			return;
		}

		int index = (y * screenWidth) + x;

		Colour &dest = buffers[currentDrawBuffer][index];

		unsigned char sFactor = source.a,
			dFactor = 255 - source.a;

		dest.r = ((source.r * sFactor) + (dest.r * dFactor)) / 255;
		dest.g = ((source.g * sFactor) + (dest.g * dFactor)) / 255;
		dest.b = ((source.b * sFactor) + (dest.b * dFactor)) / 255;
		dest.a = ((source.a * sFactor) + (dest.a * dFactor)) / 255;
	}

	inline bool DepthFunc(int x, int y, float depthValue){
		int index = (y * screenWidth) + x;

		uint castVal = (uint)depthValue;

		if(castVal > depthBuffer[index]){
			return false;
		}

		depthBuffer[index] = castVal;
		return true;
	}


	void	RasteriseTriMesh(RenderObject*o);

	void	RasteriseTri(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2, 
		const Colour &c0 = Colour(), const Colour &c1 = Colour(), const Colour &c2= Colour(),
		const Vector3 &t0 = Vector3(), const Vector3 &t1= Vector3(), const Vector3 &t2	= Vector3());
	

	BoundingBox CalculateBoxForTri(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2);

	void CalculateWeights(const Vector4 &v0, const Vector4 &v1, const Vector4 &v2,
		const Vector4 &pnt, float &aplha, float &beta, float &gamma);

	bool CohenSutherlandLine(Vector4 &inA, Vector4 &inB,
		Colour &colA, Colour &colB,
		Vector3 &texA, Vector3 &texB);

	void SutherlandHodgmanTri(Vector4 &v0, Vector4 &v1, Vector4 &v2,
		const Colour &c0 = Colour(), const Colour &c1 = Colour(), const Colour &c2 = Colour(),
		const Vector2 &t0 = Vector2(), const Vector2 &t1 = Vector2(), const Vector2 &t2 = Vector2());

	float ClipEdge(const Vector4 &inA, const Vector4 &inB, int axis);

	int HomogenousOutcode(const Vector4 &in);

	/* Member variables */
	int	currentDrawBuffer;

	Colour*	buffers[2];

	unsigned short*	depthBuffer;

	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;
	Matrix4 textureMatrix;

	Matrix4	viewProjMatrix;

	Matrix4	portMatrix;

	Texture *currentTex;

	SampleState texSampleState;
};


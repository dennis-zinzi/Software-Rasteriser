#include "Texture.h"

Texture::Texture(void)	{
	width	= 0;
	height	= 0;

	texels = NULL;
}

Texture::~Texture(void)	{
	delete[] texels;
}

Texture* Texture::TextureFromTGA(const string &filename) {
	Texture * t = new Texture();
	std::ifstream file;

	std::cout << "Loading TGA from(" << filename << ")" << std::endl;
	file.open(filename.c_str(), std::ios::binary);
	if(!file.is_open()){
		std::cout << "TextureFromTGA file error" << std::endl;
		return t ;
	}

	unsigned char TGAheader[18];

	std::cout << "sizeof(TGAheader) is " << sizeof(TGAheader) << std::endl;

	file.read((char*)TGAheader,sizeof(TGAheader));

	t->width	= (TGAheader[12] + (TGAheader[13] << 8));
	t->height	= (TGAheader[14] + (TGAheader[15] << 8));

	int size = t->width * t->height * (TGAheader[16] / 8);

	t->texels = new Colour[t->width*t->height];

	file.read( (char*) t->texels ,size);
	file.close();

	t->CreateMipmaps();

	return t;
}

const Colour& Texture::NearestTexSample(const Vector3 &coords, int miplevel){
	miplevel = min(miplevel, mipLevels.size() - 1);
	miplevel = (mipLevels.size() - 1) - miplevel;

	int texWidth = width >> miplevel,
		texHeight = height >> miplevel;

	int x = (int)(coords.x * (texWidth - 1)),
		y = (int)(coords.y * (texHeight - 1));

	return ColourAtPoint(x, y, miplevel);
}

const Colour& Texture::BilinearTexSample(const Vector3 &coords, int miplevel){
	int texWidth = width,
		texHeight = height;

	int x = (int)(coords.x * texWidth),
		y = (int)(coords.y * texHeight);

	//Current tex point
	const Colour &tLeft = ColourAtPoint(x, y);
	//Texel to the right
	const Colour &tRight = ColourAtPoint(x + 1, y);
	//Texel below
	const Colour &tBottomLeft = ColourAtPoint(x, y + 1);
	//Texel below and to the right
	const Colour &tBottomRight = ColourAtPoint(x + 1, y + 1);

	float fracX = (coords.x * texWidth) - x, //might be a divide?
		fracY = (coords.y * texHeight) - y;

	Colour top = Colour::Lerp(tLeft, tRight, fracX),
		bottom = Colour::Lerp(tBottomLeft, tBottomRight, fracX);

	return Colour::Lerp(top, bottom, fracY);
}


void Texture::CreateMipmaps(){
	int tempWidth = width,
		tempHeight = height;

	mipLevels.push_back(texels);

	int numLevels = 0;
	while(tempWidth > 1 && tempHeight > 1){
		//Right bit-shift on ints halves it
		tempWidth >>= 1;
		tempHeight >>= 1;

		Colour *newLevel = new Colour[tempWidth * tempHeight];

		GenerateMipLevel(mipLevels.back(), newLevel, numLevels);

		numLevels++;

		mipLevels.push_back(newLevel);

		delete[] newLevel;
	}
}


void Texture::GenerateMipLevel(Colour *src, Colour *dest, int mipLevel){
	int srcWidth = width >> mipLevel,
		srcHeight = height >> mipLevel;

	//Destination will be half the size, thus shift by 1
	int destWidth = width >> (mipLevel + 1),
		destHeight = height >> (mipLevel + 1);

	//Increments at half the rate of for loop
	int outY = 0;

	for(int y = 0; y < srcHeight; y += 2){
		//Increments at half the rate of for loop
		int outX = 0;

		for(int x = 0; x < srcWidth; x += 2){
			Colour out;

			out += src[(y * srcHeight) + x] * 0.25f;
			out += src[(y * srcHeight) + (x + 1)] * 0.25f;
			out += src[((y + 1) * srcHeight) + x] * 0.25f;
			out += src[((y + 1) * srcHeight) + (x + 1)] * 0.25f;

			dest[outY * destHeight + outX] = out;

			outX++;
		}

		outY++;
	}
}
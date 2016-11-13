#include "SoftwareRasteriser.h"

#include "Mesh.h"
#include "Texture.h"

int main() {
	SoftwareRasteriser r(800,600);

	while(r.UpdateWindow()) {
		r.ClearBuffers();
		//Put draw functions here!

		if (Mouse::ButtonDown(MOUSE_LEFT)) {
			std::cout << "Mouse is at position: " << Mouse::GetAbsolutePosition().x << " , " << Mouse::GetAbsolutePosition().y << std::endl;
		}
		r.SwapBuffers();
	}

	return 0;
}
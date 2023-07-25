#ifndef __FREEFLYCAMERA_HPP__
#define __FREEFLYCAMERA_HPP__

#include "GL/gl3w.h"
#include "define.hpp"
#include "BaseCamera.hpp"

namespace M3D_ISICG
{
	// Freefly camera.
	class FreeFlyCamera :public BaseCamera
	{
	public:
		FreeFlyCamera() : BaseCamera() {}
		~FreeFlyCamera();
	};
} // namespace M3D_ISICG

#endif // __CAMERA_HPP__

#ifndef __TRACKBALLCAMERA_HPP__
#define __TRACKBALLCAMERA_HPP__

#include "GL/gl3w.h"
#include "define.hpp"
#include "BaseCamera.hpp"

namespace M3D_ISICG
{
	// Freefly camera.
	class TrackBallCamera :public BaseCamera
	{
	public:
		TrackBallCamera() {}
		~TrackBallCamera();
		void moveFront(const float p_delta) override;

		void moveRight(const float p_delta) override;

		void moveUp(const float p_delta) override;

		void updateCamera();

		float azimuth = 0;
		float polar = 0;
		float radius=3;
		Vec3f center = Vec3f(0, 0, 0 );

	};
} // namespace M3D_ISICG

#endif // __TRACKBALLCAMERA_HPP__

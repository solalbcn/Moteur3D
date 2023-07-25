#include "trackBallCamera.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

namespace M3D_ISICG
{
	void TrackBallCamera::moveRight(const float p_delta)
	{
		azimuth += p_delta;
		updateCamera();
	}

	 void TrackBallCamera::moveUp(const float p_delta)
	{
		 polar += p_delta;
		 updateCamera();
	}
	 void TrackBallCamera::moveFront(const float p_delta)
	 {
	 }
	 void TrackBallCamera::updateCamera()
	 {
		 int x = _position.x + radius * glm::cos(polar) * glm::cos(azimuth);
		 int y = _position.y + radius * glm::sin(polar);
		 int z = _position.z + radius * glm::cos(polar) * glm::sin(azimuth);
	 }

} // namespace M3D_ISICG

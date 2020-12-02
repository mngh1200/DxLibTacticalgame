#include "Animation.h"

namespace Entity {


	Animation::Animation(int timeMs, int y1, int y0, function<float(float, float, float, float)> func)
		: Animation()
	{
		frameMax_ = (timeMs - ONE_FRAME_MS + 1) / ONE_FRAME_MS;
		y1_ = y1;
		y0_ = y0;

		calcFunk_ = func;
	}


	bool Animation::update(float* y)
	{
		*y = calcFunk_(frameCount_, frameMax_, y1_, y0_);

		// I—¹”»’è
		if (frameCount_ >= frameMax_)
		{
			return true;
		}
		frameCount_++;
		return false;
	}

	bool Animation::update(int* y)
	{
		*y = calcFunk_(frameCount_, frameMax_, y1_, y0_);

		// I—¹”»’è
		if (frameCount_ >= frameMax_)
		{
			return true;
		}
		frameCount_++;
		return false;
	}
}
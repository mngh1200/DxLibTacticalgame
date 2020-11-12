#pragma once
#include "Shape.h"

namespace Entity
{
	class Rect : public Shape
	{
	public:
		Rect(int x, int y, int w, int h);
		~Rect() {};

		bool isHit(int x, int y) const override;

	private:

	};
}
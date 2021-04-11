#pragma once
#include <climits>
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "DxLib.h"

using namespace std;

/**
 * @file Panel.h
 * @brief ‰æ–Ê“à‹éŒ`—v‘f
 */

namespace Entity
{
	class Panel : public Figure
	{
	public:
		Panel() : color_(-1) {};
		~Panel() {};

		void setPos(int x, int y);
		void setSize(int w, int h);
		void setShape(int x, int y, int w, int h);
		void setColor(int colorType);

		void render() const;

	private:
		//! F
		int color_;
	};
}
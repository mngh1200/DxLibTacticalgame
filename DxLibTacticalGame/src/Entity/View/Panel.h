#pragma once
#include <climits>
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "DxLib.h"

using namespace std;

/**
 * @file Panel.h
 * @brief 画面内矩形要素
 */

namespace Entity
{
	class Panel : public Figure
	{
	public:
		Panel() : color_(-1) {};
		~Panel() {};

		void setX(int x);
		void setY(int y);
		void setPos(int x, int y);
		void setSize(int w, int h);
		void setShape(int x, int y, int w, int h);
		void setColor(int colorType);

		void render() const;

	private:
		//! 色
		int color_;
	};
}
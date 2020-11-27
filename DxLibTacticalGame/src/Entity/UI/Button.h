#pragma once
#include <climits>
#include "FrameWork/Game.h"
#include "Entity/Object.h"
#include "Entity/Shape.h"
#include "DxLib.h"
#include "Define.h"

/**
 * @file Button.h
 * @brief ƒ{ƒ^ƒ“—v‘f
 */

namespace Entity
{
	class Button : public Object
	{
	public:
		Button();
		Button(Shape shape);
		~Button() {};

		void onMouseOver() override;
		bool animationUpdate() override;

		void render() const override;

	};
}
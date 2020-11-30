#pragma once
#include <climits>
#include <string>
#include "FrameWork/Game.h"
#include "Entity/Object.h"
#include "Entity/Shape.h"

using namespace std;

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
		Button(string text, int& x, int& y, int& w, int& h);
		virtual ~Button() {};
	protected:
		string text_;
	};
}
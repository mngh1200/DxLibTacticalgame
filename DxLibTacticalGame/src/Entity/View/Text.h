#pragma once
#include <climits>
#include <string>
#include "Entity/Shape.h"
#include "ViewObject.h"
#include "DxLib.h"
#include "Define.h"

using namespace std;

namespace Entity
{
	class Text : public ViewObject
	{
	public:
		Text();
		Text(Shape shape) : Text() {};
		Text(string text, Shape shape) : Text(shape) {};
		~Text() {};

		void render() const;

	private:
		//! •¶Žš—ñ
		string text_;
	};
}
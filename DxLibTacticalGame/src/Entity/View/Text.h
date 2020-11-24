#pragma once
#include <climits>
#include <string>
#include "Entity/Shape.h"
#include "Entity/Figure.h"
#include "DxLib.h"
#include "Define.h"

using namespace std;

namespace Entity
{
	class Text : public Figure
	{
	public:
		Text();
		Text(string text, int x, int y);
		~Text() {};

		void render() const;

	private:
		//! •¶Žš—ñ
		string text_;
	};
}
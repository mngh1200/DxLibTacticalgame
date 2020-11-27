#pragma once
#include <climits>
#include <string>
#include "Entity/Shape.h"
#include "Entity/Figure.h"
#include "DxLib.h"
#include "Define.h"

using namespace std;

/**
 * @file Text.h
 * @brief 画面内テキスト要素
 */

namespace Entity
{
	class Text : public Figure
	{
	public:
		Text();
		Text(string text, int x, int y, int font, int align = LEFT, int color = -1);
		~Text() {};

		void setColor(int color);

		void render() const;

		/**
		 * @enum Enum
		 * 配置
		 */
		enum Align
		{
			LEFT,
			CENTER,
			RIGHT
		};

	private:
		//! 文字列
		string text_;

		//! フォントの種類
		int font_;

		//! 色
		int color_;
	};
}
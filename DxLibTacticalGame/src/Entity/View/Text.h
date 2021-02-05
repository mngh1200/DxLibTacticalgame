#pragma once
#include <climits>
#include <string>
#include "Utility/ResourceManager.h"
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
		Text(string text, int x, int y, int fontType, int colorType, int align = LEFT);
		~Text() {};

		void setText(string text);

		void setY(int y);

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

		//! 基準のx座標
		int baseX_;

		//! 揃え
		int align_;
	};
}
#include "Text.h"

namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	Text::Text() : text_(""), font_(-1), color_(-1)
	{
		type_ = TEXT;
	}

	/**
	 * @fn
	 * コンストラクタ
	 * @param (text) 表示文字
	 * @param (x) x座標
	 * @param (y) y座標
	 * @param (font) フォント
	 */
	Text::Text(string text, int x, int y, int font, int align, int color) : Text()
	{
		text_ = text;
		font_ = font;
		shape_.y = y;
		color_ = color;

		if (align == CENTER) // 中央揃え
		{
			int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text.c_str());
			shape_.x = x - width / 2;
		}
		else if (align == RIGHT) // 右揃え
		{
			int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text.c_str());
			shape_.x = x - width;
		}
		else // 左揃え
		{
			shape_.x = x;
			
		}
		font_ = DxLib::CreateFontToHandle("メイリオ", 24, 3, DX_FONTTYPE_ANTIALIASING);

	}

	/**
	 * @fn
	 * 文字色をセット
	 * @param (color) 色
	 */
	void Text::setColor(int color)
	{
		color_ = color;
	}

	/**
	 * @fn
	 * 描画処理
	 */
	void Text::render() const
	{
		DxLib::DrawFormatStringToHandle(shape_.x, shape_.y, color_, font_, text_.c_str());
	}
}
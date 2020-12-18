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
	 * @param (fontType) 対象フォント（fontManagerのFontTypeのenum値）
	 * @param (colorType) テキスト色（fontManagerのColorTypeのenum値）
	 * @param (align) x座標の基準（左、中央、右揃えを指定）
	 */
	Text::Text(string text, int x, int y, int fontType, int colorType, int align) : Text()
	{
		Utility::ResourceManager& resourceManager = Utility::ResourceManager::getInstance();

		text_ = text;
		shape_.y = y;
		font_ = resourceManager.getHdlFont(fontType);
		color_ = resourceManager.getColor(colorType);

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
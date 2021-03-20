#include "Text.h"

namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	Text::Text() : text_(""), font_(-1), color_(-1), align_(Align::LEFT), baseX_(0)
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
	Text::Text(const char* text, int x, int y, int fontType, int colorType, int align) : Text()
	{
		Utility::ResourceManager& resourceManager = Utility::ResourceManager::getInstance();
		align_ = align;

		baseX_ = x;
		shape_.y = y;
		font_ = resourceManager.getHdlFont(fontType);
		color_ = resourceManager.getColor(colorType);

		setText(text);
	}

	/**
	 * @fn
	 * テキストセット
	 * @param (text) 変更テキスト
	 */
	void Text::setText(const char* text)
	{
		text_ = text;

		if (align_ == CENTER) // 中央揃え
		{
			int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text);
			shape_.x = baseX_ - width / 2;
		}
		else if (align_ == RIGHT) // 右揃え
		{
			int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text);
			shape_.x = baseX_ - width;
		}
		else // 左揃え
		{
			shape_.x = baseX_;
		}
	}

	/**
	 * @fn
	 * y座標セット
	 * @param (y) y座標
	 */
	void Text::setY(int y)
	{
		shape_.y = y;
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
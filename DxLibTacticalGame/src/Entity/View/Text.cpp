#include "Text.h"

namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	Text::Text() :
		text_(""),
		font_(-1),
		color_(-1),
		align_(Align::LEFT),
		baseX_(0),
		backgroundColor_(-1),
		padding_(0)
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

		// サイズ取得
		int lineCount;
		DxLib::GetDrawStringSizeToHandle(&shape_.w, &shape_.h, &lineCount, text, DxLib::GetStringLength(text), font_);

		shape_.w += padding_ * 2; // 余白追加
		shape_.h += padding_ * 2; // 余白追加

		adjustAlign();
	}

	/**
	 * @fn
	 * 余白セット
	 * @param (padding) 余白
	 */
	void Text::setPadding(int padding)
	{
		// 余白変更によるサイズ調整
		shape_.w += padding - padding_;
		shape_.h += padding - padding_;

		padding_ = padding;

		adjustAlign();
	}
	
	/**
	 * @fn
	 * 背景色セット
	 * @param (backgroundColorType) セットするカラータイプ
	 */
	void Text::setBackgroundColor(int backgroundColorType)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		backgroundColor_ = rm.getColor(backgroundColorType);
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
		// 背景色がセットされている場合
		if (backgroundColor_ != -1)
		{
			DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), backgroundColor_, TRUE);
		}

		DxLib::DrawFormatStringToHandle(shape_.x + padding_, shape_.y + padding_, color_, font_, text_.c_str());
	}

	/**
	 * @fn
	 * 揃え方向による調整
	 */
	void Text::adjustAlign()
	{
		if (align_ == CENTER) // 中央揃え
		{
			shape_.x = baseX_ - shape_.w / 2;
		}
		else if (align_ == RIGHT) // 右揃え
		{
			shape_.x = baseX_ - shape_.w;
		}
		else // 左揃え
		{
			shape_.x = baseX_;
		}
	}
}
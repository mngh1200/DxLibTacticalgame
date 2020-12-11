#include "TextButton.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	TextButton::TextButton() :
		text_(""),
		isCenter_(true),
		textX_(0),
		textY_(0),
		color_{},
		textColor_{}
	{
		type_ = Type::BUTTON;

		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		font_ = fontManager.getHdlFont(FontType::NORMAL);
	}

	/**
	 * @fn
	 * コンストラクタ
	 */
	TextButton::TextButton(int colorType, int textColorType) : TextButton()
	{
		setColor(colorType, textColorType, State::ALL);
	}

	/**
	 * @fn
	 * ボタン位置とサイズ指定
	 * @param (x) X座標
	 * @param (y) Y座標
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	void TextButton::setShape(int x, int y, int w, int h)
	{
		setPos(x, y);
		setSize(w, h);
	}

	/**
	 * @fn
	 * ボタン位置指定
	 * @param (x) X座標
	 * @param (y) Y座標
	 */
	void TextButton::setPos(int x, int y)
	{
		// テキストとボタンの相対座標を計算
		shape_.x = x;
		shape_.y = y;
	}

	/**
	 * @fn
	 * ボタンサイズ指定
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	void TextButton::setSize(int w, int h)
	{
		shape_.w = w;
		shape_.h = h;
		
		// 中心にテキストを揃える
		setTextCenter();
		setTextMiddle();
	}

	/**
	 * @fn
	 * テキスト指定
	 * @param (text) テキスト
	 */
	void TextButton::setText(string text)
	{
		text_ = text;
		setTextCenter();
	}

	/**
	 * @fn
	 * テキスト指定
	 * @param (text) テキスト
	 * @param (x) ボタンとテキストの相対座標
	 * @param (fontType) フォント
	 */
	void TextButton::setTextX(int x)
	{
		textX_ = x;
		isCenter_ = false;
	}

	/**
	 * @fn
	 * テキスト指定(ボタンの中央に配置)
	 * @param (text) テキスト
	 * @param (fontType) フォント
	 */
	void TextButton::setText(string text, int fontType)
	{
		text_ = text;
		setFont(fontType);
	}

	/**
	 * @fn
	 * フォント指定
	 * @param (fontType) フォント
	 */
	void TextButton::setFont(int fontType)
	{
		Utility::FontManager& fontM = Utility::FontManager::getInstance();
		font_ = fontM.getHdlFont(fontType);
		setTextCenter();
		setTextMiddle();
	}

	/**
	 * @fn
	 * ボタンの色指定
	 * @param (backgroundColor) ボタン色
	 * @param (textColor) テキスト色
	 * @param (state) 対象の状態
	 */
	void TextButton::setColor(int backgroundColorType, int textColorType, int state)
	{
		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		if (state == State::ALL)
		{
			
			for (int i = 0; i < State::ALL; ++i)
			{
				color_[i] = fontManager.getColor(backgroundColorType);
				textColor_[i] = fontManager.getColor(textColorType);
			}
		}
		else
		{
			color_[state] = fontManager.getColor(backgroundColorType);
			textColor_[state] = fontManager.getColor(textColorType);
		}
	}

	/**
	 * @fn
	 * ボタンの背景色指定
	 * @param (backgroundColor) ボタン色
	 * @param (state) 対象の状態
	 */
	void TextButton::setBackgroundColor(int backgroundColorType, int state)
	{
		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		if (state == State::ALL)
		{

			for (int i = 0; i < State::ALL; ++i)
			{
				color_[i] = fontManager.getColor(backgroundColorType);
			}
		}
		else
		{
			color_[state] = fontManager.getColor(backgroundColorType);
		}
	}

	/**
	 * @fn
	 * ボタンのテキスト色指定
	 * @param (textColorType) テキスト色
	 * @param (state) 対象の状態
	 */
	void TextButton::setTextColor(int textColorType, int state)
	{
		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		if (state == State::ALL)
		{

			for (int i = 0; i < State::ALL; ++i)
			{
				textColor_[i] = fontManager.getColor(textColorType);
			}
		}
		else
		{
			textColor_[state] = fontManager.getColor(textColorType);
		}
	}

	/**
	 * @fn
	 * 描画
	 */
	void TextButton::render() const
	{
		// 現在の状態取得
		int nowState = State::NORMAL;
		if (isMouseDown_)
		{
			nowState = State::MOUSE_DOWN;
		}
		else if (isMouseOver_)
		{
			nowState = State::MOUSE_OVER;
		}

		// ボタン
		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), color_[nowState], TRUE);

		// テキスト
		DxLib::DrawFormatStringToHandle(shape_.x + textX_, shape_.y + textY_, textColor_[nowState], font_, text_.c_str());
	}

	/**
	 * @fn
	 * テキストを中央揃え
	 */
	void TextButton::setTextCenter()
	{
		if (isCenter_)
		{
			// 中央揃えにするため座標取得
			int width = DxLib::GetDrawFormatStringWidthToHandle(font_, text_.c_str());
			textX_ = (shape_.w - width) / 2;
		}
	}

	/**
	 * @fn
	 * テキストを中段揃え
	 */
	void TextButton::setTextMiddle()
	{
		// 中段に表示するための相対座標取得
		int fontHeight = DxLib::GetFontSizeToHandle(font_);
		textY_ = (shape_.h - fontHeight) / 2;
	}

}
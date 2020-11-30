#include "MenuScreenButton.h"

namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	MenuScreenButton::MenuScreenButton() : textY_(0)
	{
		type_ = Figure::BUTTON;
		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		backgroundColor_ = fontManager.getColor(ColorType::BUTTON);
		textColor_ = fontManager.getColor(ColorType::NORMAL_TEXT);
	}

	/**
	 * @fn
	 * コンストラクタ
	 * @param (text) ボタンの文言
	 * @param (x) x座標
	 * @param (y) y座標
	 * @param (w) 幅
	 * @param (h) 高さ
	 */
	MenuScreenButton::MenuScreenButton(string text, int x, int y, int w, int h) : MenuScreenButton()
	{
		shape_ = Shape(x, y, w, h);
		text_ = text;
		textY_ = (y + h / 2) - TEXT_SIZE / 2;
	}

	/**
	 * @fn
	 * 描画
	 */
	void MenuScreenButton::render() const
	{

		Utility::FontManager& fontManager = Utility::FontManager::getInstance();

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), backgroundColor_, TRUE);

		DxLib::DrawBox(shape_.x, shape_.y, shape_.x + ACCENT_WIDTH, shape_.getY2(), fontManager.getColor(ColorType::NORMAL_TEXT), TRUE);

		DxLib::DrawFormatStringToHandle(shape_.x + ACCENT_WIDTH + TEXT_PADDING_LEFT, textY_, textColor_, fontManager.getHdlFont(FontType::MAIN_MENU), text_.c_str());
	}


	/**
	 * @fn
	 * マウスが左ボタンで押下された瞬間の処理
	 */
	void MenuScreenButton::onMouseLeftDown()
	{
	}

	/**
	 * @fn
	 * マウスの左ボタンが押し上げられた瞬間の処理
	 */
	void MenuScreenButton::onMouseLeftUp()
	{
	}

	/**
	 * @fn
	 * マウスにホバーされた瞬間の処理
	 */
	void MenuScreenButton::onMouseOver()
	{
		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		backgroundColor_ = fontManager.getColor(ColorType::NORMAL_TEXT);
		textColor_ = fontManager.getColor(ColorType::BUTTON);
	}

	/**
	 * @fn
	 * マウスがホバー状態から離れたときの処理
	 */
	void MenuScreenButton::onMouseOut()
	{
		Utility::FontManager& fontManager = Utility::FontManager::getInstance();
		backgroundColor_ = fontManager.getColor(ColorType::BUTTON);
		textColor_ = fontManager.getColor(ColorType::NORMAL_TEXT);
	}
}
#include "MenuScreenButton.h"

namespace Entity {

	/**
	 * @fn
	 * コンストラクタ
	 */
	MenuScreenButton::MenuScreenButton() : textY_(0), animation_ {}
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

		baseShape_ = shape_;
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
	 * アニメーション更新
	 * @return true:終了
	 */
	bool MenuScreenButton::animationUpdate()
	{
		if (animationId_ == EXPANSION || animationId_ == SHRINK)
		{
			return animation_.update(&shape_, baseShape_, 1.2F);
		}
		return true;
	}


	/**
	 * @fn
	 * マウスが左ボタンで押下された瞬間の処理
	 */
	void MenuScreenButton::onMouseLeftDown()
	{
		// 拡大
		FrameWork::Game& game = FrameWork::Game::getInstance();
		game.objectsControl.addAnimationObj(EXPANSION, getLayerId(), getObjectId());
		animation_ = Animation(100);
	}

	/**
	 * @fn
	 * マウスの左ボタンが押し上げられた瞬間の処理
	 */
	void MenuScreenButton::onMouseLeftUp()
	{
		// 縮小
		FrameWork::Game& game = FrameWork::Game::getInstance();
		game.objectsControl.addAnimationObj(SHRINK, getLayerId(), getObjectId());
		animation_ = Animation(100, Animation::REVERSE);
		animation_.adjustFrame(shape_, baseShape_, 1.2F);
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

		// 縮小
		if (isMouseDown_)
		{
			FrameWork::Game& game = FrameWork::Game::getInstance();
			game.objectsControl.addAnimationObj(SHRINK, getLayerId(), getObjectId());
			animation_ = Animation(100, Animation::REVERSE);
			animation_.adjustFrame(shape_, baseShape_, 1.2F);
		}
	}
}
#include "MenuScreenButton.h"

namespace Entity {
	const float MenuScreenButton::ANIMATION_SCALE = 1.1f;

	/**
	 * @fn
	 * コンストラクタ
	 */
	MenuScreenButton::MenuScreenButton() : animation_ {}
	{
		setTextX(TEXT_PADDING_LEFT);
		setColor(ColorType::MAIN_COLOR, ColorType::SUB_COLOR, State::NORMAL);
		setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, State::MOUSE_OVER);
		setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, State::MOUSE_DOWN);
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
		setShape(x, y, w, h);
		setText(text, FontType::BLACK_S48);
		baseShape_ = shape_;
	}

	/**
	 * @fn
	 * アニメーション作成(ObjectsControl::addAnimationObjメソッド専用で呼び出す)
	 * @return アニメーション作成可能な場合trueを返す
	 */
	bool MenuScreenButton::createAnimation(int animationId)
	{
		// 既に実行済みのアニメーションがある場合は作成不可
		if (animationId == animationId_)
		{
			return false;
		}
		
		if (animationId == EXPANSION) // 拡大
		{
			animation_ = Animation(ANIMATION_MS);
			return true;
		}
		else if (animationId == SHRINK) // 縮小
		{
			animation_ = Animation(ANIMATION_MS, Animation::REVERSE);
			animation_.adjustFrame(shape_, baseShape_, ANIMATION_SCALE);
			return true;
		}
		return false;
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
			bool isEnd = animation_.update(&shape_, baseShape_, ANIMATION_SCALE);
			setSize(shape_.w, shape_.h); // テキスト位置調整用に再適用
			return isEnd;
		}
		return true;
	}


	/**
	 * @fn
	 * マウスが左ボタンで押下された瞬間の処理
	 */
	void MenuScreenButton::onMouseLeftDown()
	{
		joinAnimationList(EXPANSION); // 拡大
		
		// サウンド
		Utility::FontManager& fontM = Utility::FontManager::getInstance();
		DxLib::PlaySoundMem(fontM.getSound(SoundKind::MOUSE_DOWN), DX_PLAYTYPE_BACK);

	}

	/**
	 * @fn
	 * マウスの左ボタンが押し上げられた瞬間の処理
	 */
	void MenuScreenButton::onMouseLeftUp()
	{
		joinAnimationList(SHRINK); // 縮小
	}

	/**
	 * @fn
	 * マウスがホバー状態から離れたときの処理
	 */
	void MenuScreenButton::onMouseOut()
	{
		if (isMouseDown_)
		{
			joinAnimationList(SHRINK); // 縮小
		}
	}

}
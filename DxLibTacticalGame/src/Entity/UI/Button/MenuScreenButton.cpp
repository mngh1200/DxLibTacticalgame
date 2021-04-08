#include "MenuScreenButton.h"

namespace Entity {
	const float MenuScreenButton::ANIMATION_SCALE = 1.1f;

	/**
	 * @fn
	 * コンストラクタ
	 */
	MenuScreenButton::MenuScreenButton() : animation_ {}
	{
		setColor(ColorType::SUB_COLOR_LITE, ColorType::SUB_COLOR, State::NORMAL);
		setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, State::MOUSE_OVER);
		setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, State::MOUSE_DOWN);

		setShape(X, 0, W, H);
		setFont(FontType::BLACK_S32);
		baseShape_ = shape_;
	}

	/**
	 * @fn
	 * Y座標セット
	 * @param(y) y座標
	 */
	void MenuScreenButton::setY(int y)
	{
		setPos(X, y);
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
	 * @param(x) X座標
	 * @param(y) Y座標
	 */
	void MenuScreenButton::onMouseLeftDown(int x, int y)
	{
		joinAnimationList(EXPANSION); // 拡大
		
		// サウンド
		Utility::ResourceManager::playSound(SoundKind::CLICK);
	}

	/**
	 * @fn
	 * マウスの左ボタンが押し上げられた瞬間の処理
	 * @param(x) X座標
	 * @param(y) Y座標
	 */
	void MenuScreenButton::onMouseLeftUp(int x, int y)
	{
		joinAnimationList(SHRINK); // 縮小
	}

	/**
	 * @fn
	 * マウスがホバー状態から離れたときの処理
	 * @param(x) X座標
	 * @param(y) Y座標
	 */
	void MenuScreenButton::onMouseOut(int x, int y)
	{
		if (isMouseDown_)
		{
			joinAnimationList(SHRINK); // 縮小
		}
	}

}
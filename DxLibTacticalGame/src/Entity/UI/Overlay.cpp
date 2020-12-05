#include "Overlay.h"

namespace Entity {
	const int Overlay::COLOR = DxLib::GetColor(0, 0, 0); //! オーバーレイの色

	/**
	 * @fn
	 * コンストラクタ
	 */
	Overlay::Overlay(bool isOpen) : isOpen_(isOpen), alpha_(MAX_ALPHA), animation_({})
	{
		type_ = OVERLAY;
		shape_ = Shape(0, 0, WIN_W, WIN_H);
	}

	/**
	 * @fn
	 * アニメーション作成(ObjectsControl::addAnimationObjメソッド専用で呼び出す)
	 * @return アニメーション作成可能な場合trueを返す
	 */
	bool Overlay::createAnimation(int animationId)
	{
		// 既に実行済みのアニメーションがある場合は作成不可
		if (animationId == animationId_)
		{
			return false;
		}

		int direction = Animation::REVERSE;

		if (!isOpen_)
		{
			direction = Animation::NORMAL;
		}

		animation_ = Animation(ANIMATION_TIME, direction);
		return true;
	}

	/**
	 * @fn
	 * アニメーション
	 * @return アニメーション終了時にtrue
	 */
	bool Overlay::animationUpdate()
	{
		if (animation_.update(&alpha_, 0, MAX_ALPHA))
		{
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * 描画処理
	 */
	void Overlay::render() const
	{

 		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), COLOR, TRUE);

		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
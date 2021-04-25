#include "ExtraEffect.h"

namespace Entity {

	/**
	 * @fn
	 * 初期処理
	 * ダメージエフェクトを生成
	 * @param (atkUnit) 攻撃側ユニット
	 * @param (defUnit) 防御側ユニット
	 * @param (text) テキスト
	 * @param (num) 連番
	 */
	void ExtraEffect::initExtraEffect(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, const char* text, int num)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		shape_.set(-W, BASE_Y + (H + MARGIN) * num);
		shape_.setSize(W, H);

		// 攻撃側または、防御側ユニットが表示位置と重なる可能性がある場合、位置調整
		if (atkUnit->getX() < W || defUnit->getX() < W)
		{
			shape_.x = WIN_W;
			isLeft_ = false;
		}

		num_ = num;
		text_ = text;

		joinAnimationList(AnimationKind::SHOW);
	}

	/**
	 * @fn
	 * 非表示
	 */
	void ExtraEffect::hide()
	{
		joinAnimationList(AnimationKind::HIDE);
	}

	/**
	 * @fn
	 * 描画
	 */
	void ExtraEffect::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);
		DxLib::DrawStringToHandle(shape_.x + PADDING, shape_.y + PADDING, text_.c_str(), rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FontType::NORMAL_S24));
	}

	/**
	 * @fn
	 * アニメーション更新
	 */
	bool ExtraEffect::animationUpdate()
	{
		if (animationId_ == AnimationKind::SHOW || animationId_ == AnimationKind::HIDE) // 表示
		{
			bool isFin = false;
			if (isLeft_)
			{
				isFin = animation_.update(&shape_.x, -W, 0);
			}
			else
			{
				isFin = animation_.update(&shape_.x, WIN_W, WIN_W - W);
			}

			if (isFin && animationId_ == AnimationKind::HIDE)
			{
				destroy(); // 非表示アニメーション終了時、要素除去
			}

			return isFin;
		}
		return false;
	}

	/**
	 * @fn
	 * アニメーション作成
	 * @param (animationId) アニメーションの種類
	 */
	bool ExtraEffect::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::SHOW)
		{
			animation_ = Animation(ANIMATION_TIME, 0, 1, NUM_DELAY * num_);
			return true;
		}
		else if (animationId == AnimationKind::HIDE)
		{
			animation_ = Animation(ANIMATION_TIME, Animation::Direction::REVERSE);
			return true;
		}
		return false;
	}
}
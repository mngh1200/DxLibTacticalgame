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

		isPlayer_ = !atkUnit->isEnemy();

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

		int colorType = ColorType::PLAYER_COLOR_LITE;
		if (!isPlayer_)
		{
			colorType = ColorType::ENEMY_COLOR_LITE;
		}

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);
		DxLib::DrawBox(shape_.x, shape_.y, shape_.x + COLOR_W, shape_.getY2(), rm.getColor(colorType), TRUE);
		DxLib::DrawStringToHandle(shape_.x + PADDING_LEFT, shape_.y + PADDING_V, text_.c_str(), rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FontType::NORMAL_S20));
	}

	/**
	 * @fn
	 * アニメーション更新
	 */
	bool ExtraEffect::animationUpdate()
	{
		if (waitTimer_.isWorking() && animationId_ == AnimationKind::SHOW) // 終了遅延タイマー稼働中の場合
		{
			if (waitTimer_.checkAndCountTimer()) // カウント終了でアニメーション終了
			{
				return true;
			}

			if (animation_.getDelayFinishLog_()) // アニメーション開始遅延の終了時
			{
				Utility::ResourceManager::playSound(SoundKind::EXTRA_EFFECT); // 効果音
			}
		}
		else if (animationId_ == AnimationKind::SHOW || animationId_ == AnimationKind::HIDE) // 表示
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

			if (isFin && animationId_ == AnimationKind::SHOW)
			{
				// 終了遅延タイマー
				waitTimer_.setTimer(EnemyTurnCont::getAnimationMs(SHOW_WAIT_MS));
				isFin = false;
			}
			else if (isFin && animationId_ == AnimationKind::HIDE)
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
			animation_ = Animation(EnemyTurnCont::getAnimationMs(ANIMATION_TIME), 0, 1, EnemyTurnCont::getAnimationMs(NUM_DELAY * num_));
			return true;
		}
		else if (animationId == AnimationKind::HIDE)
		{
			animation_ = Animation(EnemyTurnCont::getAnimationMs(ANIMATION_TIME), Animation::Direction::REVERSE);
			return true;
		}
		return false;
	}
}
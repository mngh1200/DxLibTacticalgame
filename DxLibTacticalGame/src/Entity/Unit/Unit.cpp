#include "Unit.h"

namespace Entity {
	/**
	 * @fn
	 * 初期処理
	 */
	void Unit::init(int x, int y, bool isEnemy)
	{
		type_ = Figure::UNIT;

		setPos(x, y);

		isEnemy_ = isEnemy;

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		if (isEnemy)
		{
			imageId_ = rm.getImage(ImageType::PLAYER, LANCER, 0);
		}
		else
		{
			imageId_ = rm.getImage(ImageType::PLAYER, LANCER, 2);
		}

		// テスト処理（仮ステータス）
		viewHp_ = hpm_ = hp_ = 10;
		atk_ = 6;
		def_ = 1;
		mov_ = 2;
	}

	/**
	 * @fn
	 * 描画
	 */
	void Unit::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		if (alpha_ != 255) // 不透明度
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
		}

		if (state_ == State::SELECTED) // 選択中
		{
			Map::drawMoveableMass(shape_.x, shape_.y);
			Map::drawSelectedMass(shape_.x, shape_.y);
		}

		if (isMouseOver_) // ホバー中
		{
			Map::drawHoverMass(shape_.x, shape_.y);
		}

		DxLib::DrawGraph(shape_.x, shape_.y, imageId_, TRUE);

		// HPバー
		int hpx1 = shape_.x + HP_PADDING;
		int hpy1 = shape_.y + HP_Y;
		int hpx2 = shape_.getX2() - HP_PADDING;
		int hpy2 = hpy1 + HP_H;

		int hpw = (int)((float)(CHIP_SIZE - HP_PADDING * 2) * ((float)viewHp_ / (float)hpm_));

		DxLib::DrawBox(hpx1, hpy1, hpx2, hpy2, rm.getColor(ColorType::NEGATIVE_COLOR), TRUE);
		DxLib::DrawBox(hpx1, hpy1, hpx1 + hpw, hpy2, rm.getColor(ColorType::POSITIVE_LITE_COLOR), TRUE);

		if (alpha_ != 255) // 不透明度
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	/**
	 * @fn
	 * アニメーション更新
	 */
	bool Unit::animationUpdate()
	{
		// 移動
		if (animationId_ == AnimationKind::MOVE)
		{
			if (animation_.update(&shape_.x, &shape_.y, Map::getRealX(baseX_), Map::getRealY(baseY_), Map::getRealX(x_), Map::getRealY(y_)))
			{
				state_ = State::SELECTED;
				shape_.disabledHit = false;
				return true;
			}
		}
		else if (animationId_ == AnimationKind::ATACK) // 攻撃
		{
			int baseX = Map::getRealX(x_);
			int baseY = Map::getRealY(y_);
			return animation_.update(&shape_.x, &shape_.y, baseX, baseY, targetRealX_, targetRealY_);
		}
		else if (animationId_ == AnimationKind::DAMAGE) // ダメージ
		{
			animationSub_.update(&viewHp_, prevHp_, hp_);

			int baseX = Map::getRealX(x_);

			if (animation_.update(&shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE))
			{
				animationSub_.forceFinish();
				return !checkDead();
			}
		}
		else if (animationId_ == AnimationKind::DESTROY) // 死亡
		{
			if (animation_.update(&alpha_, 255, 0))
			{
				destroy();
				return true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * アニメーション作成
	 * @param (animationId) アニメーションの種類
	 */
	bool Unit::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::MOVE) // 移動
		{
			animation_ = Animation(100);
			shape_.disabledHit = true; // イベント無効
			return true;
		}
		else if (animationId == AnimationKind::ATACK) // 攻撃
		{
			animation_ = Animation(ANIME_ATACK_MS, Animation::Direction::AlTERNATE, 2);
			return true;
		}
		else if (animationId == AnimationKind::DAMAGE) // ダメージ
		{
			animation_ = Animation(ANIME_DAMAGE_MS, 0, ANIME_DAMAGE_REPEAT, ANIME_ATACK_MS / 2);
			int baseX = Map::getRealX(x_);
			animation_.adjustFrame(shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE);
			animation_.adjustLastFrame(shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE);

			// HPバーアニメーション
			animationSub_ = Animation(ANIME_DAMAGE_MS * ANIME_DAMAGE_REPEAT);
			return true;
		}
		else if (animationId == AnimationKind::DESTROY) // 死亡
		{
			animation_ = Animation(600);
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * 移動
	 * @param (x) 移動先のx座標(マス基準)
	 * @param (y) 移動先のy座標(マス基準)
	 */
	void Unit::move(int x, int y)
	{
		x_ = x;
		y_ = y;
		joinAnimationList(AnimationKind::MOVE);
	}

	/**
	 * @fn
	 * 移動元に戻る
	 */
	void Unit::back()
	{
		setPos(baseX_, baseY_);
	}

	/**
	 * @fn
	 * 位置指定
	 * @param (x) x座標(マス基準)
	 * @param (y) y座標(マス基準)
	 */
	void Unit::setPos(int x, int y)
	{
		baseX_ = x_ = x;
		baseY_ = y_ = y;
		shape_.set(Map::getRealX(x_), Map::getRealY(y_), CHIP_SIZE, CHIP_SIZE);
	}


	/**
	 * @fn
	 * ダメージ値
	 * @return 死亡時 trueを返す
	 */
	bool Unit::damage(int damage)
	{
		prevHp_ = hp_;
		hp_ -= damage;
		joinAnimationList(AnimationKind::DAMAGE);
		
		if (hp_ <= 0)
		{
			hp_ = 0;
			return true;
		}
		
		return false;
	}

	/**
	 * @fn
	 * ユニット死亡
	 * @return ユニット死亡時 true
	 */
	bool Unit::checkDead()
	{
		if (hp_ <= 0)
		{
			changeAnimation(AnimationKind::DESTROY); // 死亡、アニメーション継続
			return true;
		}
		return false;
		
	}

	/**
	 * @fn
	 * 選択または選択解除
	 * @param (isSelect) 選択するときはtrue、選択解除するときはfalse
	 * @return 選択状態が変更されたときはtrueを返す
	 */
	bool Unit::select(bool isSelect)
	{
		if (isSelect && state_ == State::NORMAL)
		{
			state_ = State::SELECTED;
			return true;
		}
		else if (!isSelect)
		{	
			if (state_ == State::MOVE || state_ == State::SELECTED)
			{
				state_ = State::NORMAL;
				setPos(baseX_, baseY_);
				if (animationId_ == AnimationKind::MOVE)
				{
					animation_.forceFinish
					();
				}
				
				return true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * 攻撃
	 * @return 攻撃力
	 */
	int Unit::atack(int targetRealX, int targetRealY)
	{
		targetRealX_ = shape_.x + (targetRealX - shape_.x) / 2;
		targetRealY_ = shape_.y + (targetRealY - shape_.y) / 2;
		joinAnimationList(AnimationKind::ATACK);
		return getAtk();
	}
}
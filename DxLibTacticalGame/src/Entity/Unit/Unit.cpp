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
		hpm_ = hp_ = 10;
		atk_ = 2;
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

		if (state_ == State::SELECTED) // 選択中
		{
			DxLib::DrawGraph(shape_.x, shape_.y, rm.getImage(ImageType::IMAGE, ImageId::MASS_MOVE), TRUE);
		}
		DxLib::DrawGraph(shape_.x, shape_.y, imageId_, TRUE);

		// HPバー
		int hpx1 = shape_.x + HP_PADDING;
		int hpy1 = shape_.y + HP_Y;
		int hpx2 = shape_.getX2() - HP_PADDING;
		int hpy2 = hpy1 + HP_H;

		int hpw = (int)((float)(CHIP_SIZE - HP_PADDING * 2) * ((float)hp_ / (float)hpm_));

		DxLib::DrawBox(hpx1, hpy1, hpx2, hpy2, rm.getColor(ColorType::NEGATIVE_COLOR), TRUE);
		DxLib::DrawBox(hpx1, hpy1, hpx1 + hpw, hpy2, rm.getColor(ColorType::POSITIVE_LITE_COLOR), TRUE);
		
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
			return animation_.increaseFrame();
		}
		else if (animationId_ == AnimationKind::DAMAGE) // ダメージ
		{
			if (animation_.increaseFrame())
			{
				if (hp_ <= 0)
				{
					changeAnimation(AnimationKind::DESTROY); // 死亡、アニメーション継続
				}
				else
				{
					return true;
				}
			}
		}
		else if (animationId_ == AnimationKind::DESTROY) // 死亡
		{
			if (animation_.increaseFrame())
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
			animation_ = Animation(400);
			return true;
		}
		else if (animationId == AnimationKind::DAMAGE) // ダメージ
		{
			animation_ = Animation(200, 0, 1, 200);
			return true;
		}
		else if (animationId == AnimationKind::DESTROY) // 死亡
		{
			animation_ = Animation(200);
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



	void Unit::damage(int damage)
	{
		hp_ -= damage;
		if (hp_ <= 0)
		{
			hp_ = 0;
		}
		joinAnimationList(AnimationKind::DAMAGE);
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
				animation_.forceFinish();
				return true;
			}
		}
		return false;
	}
}
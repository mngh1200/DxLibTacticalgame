#include "Unit.h"

namespace Entity {
	const vector<string> Unit::LEN_TEXT = {"短", "中", "長"};
	float Unit::animatinTimeRate = 1.0f;

	/**
	 * @fn
	 * 初期処理
	 */
	void Unit::init(int x, int y, int kind, bool isEnemy)
	{
		type_ = Figure::UNIT;

		setPos(x, y);

		isEnemy_ = isEnemy;

		setKind(kind);
	}

	/**
	 * @fn
	 * ユニットの種類をセット
	 */
	void Unit::setKind(int kind)
	{
		info_.kind = kind;

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		if (kind == UnitKey::CAVALRY) // 騎兵
		{
			info_.name = "騎兵";
			info_.hpm = 30;
			info_.atk = 8;
			info_.def = 1;
			info_.mov = 4;
			info_.len = 0;
			info_.ability = Ability::Kind::RUSH;
		}
		else if (kind == UnitKey::GUNNER) // 銃兵
		{
			info_.name = "銃兵";
			info_.hpm = 20;
			info_.atk = 9;
			info_.def = 0;
			info_.mov = 2;
			info_.len = 2;
			info_.range = 3;
			info_.ability = Ability::Kind::THROUGH;
		}
		else // kind == UnitKey::LANCER の動作 // 槍兵
		{
			info_.name = "槍兵";
			info_.hpm = 30;
			info_.atk = 6;
			info_.def = 2;
			info_.mov = 2;
			info_.len = 1;
			info_.kind = UnitKey::LANCER;
			info_.ability = Ability::Kind::AMBUSH;
		}

		viewHp_ = info_.hp = info_.hpm;

		imageId_ = rm.getImage(ImageType::PLAYER, info_.kind, isEnemy_ ? 0 : 2);
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
			// Map::drawMoveableMass(shape_.x, shape_.y);
			Map::drawSelectedMass(shape_.x, shape_.y);
		}

		if (isMouseOver_) // ホバー中
		{
			Map::drawHoverMass(shape_.x, shape_.y);
		}

		if (alpha_ != 255) // 不透明度
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
		}

		DxLib::DrawGraph(shape_.x, shape_.y, imageId_, TRUE);

		// HPバー
		int hpx1 = shape_.x + HP_PADDING;
		int hpy1 = shape_.y + HP_Y;
		int hpx2 = shape_.getX2() - HP_PADDING;
		int hpy2 = hpy1 + HP_H;

		int hpw = (int)((float)(CHIP_SIZE - HP_PADDING * 2) * ((float)viewHp_ / (float)info_.hpm));

		DxLib::DrawBox(hpx1, hpy1, hpx2, hpy2, rm.getColor(ColorType::NEGATIVE_COLOR), TRUE);
		DxLib::DrawBox(hpx1, hpy1, hpx1 + hpw, hpy2, rm.getColor(ColorType::POSITIVE_LITE_COLOR), TRUE);

		if (alpha_ != 255) // 描画モードを標準に戻す
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
			animationSub_.update(&viewHp_, prevHp_, info_.hp);

			int baseX = Map::getRealX(x_);

			if (animation_.update(&shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE))
			{
				viewHp_ = info_.hp;
				animationSub_.forceFinish();
				return !checkDead();
			}
		}
		else if (animationId_ == AnimationKind::AVOID) // 回避
		{
			int baseX = Map::getRealX(x_);
			int baseY = Map::getRealY(y_);
			return animation_.update(&shape_.x, &shape_.y, baseX, baseY, baseX - shape_.w / 2 , baseY);
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
	 * 指定の倍率で計算したアニメーション時間を返す
	 * @param (baseMs) 基準のミリ秒
	 * @return アニメーション時間（ms）
	 */
	static int getAnimationMS(int baseMs)
	{
		return (int)(Unit::animatinTimeRate * baseMs);
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
			animation_ = Animation(getAnimationMS(100));
			shape_.disabledHit = true; // イベント無効
			return true;
		}
		else if (animationId == AnimationKind::ATACK) // 攻撃
		{
			animation_ = Animation(getAnimationMS(ANIME_ATACK_MS), Animation::Direction::AlTERNATE, 2);
			return true;
		}
		else if (animationId == AnimationKind::DAMAGE) // ダメージ
		{
			animation_ = Animation(getAnimationMS(ANIME_DAMAGE_MS / ANIME_DAMAGE_REPEAT), 0, ANIME_DAMAGE_REPEAT, getAnimationMS(ANIME_ATACK_MS));
			int baseX = Map::getRealX(x_);
			animation_.adjustFrame(shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE);
			animation_.adjustLastFrame(shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE);

			// HPバーアニメーション
			animationSub_ = Animation(getAnimationMS(ANIME_DAMAGE_MS), 0, 1, getAnimationMS(ANIME_ATACK_MS));

			return true;
		}
		else if (animationId == AnimationKind::AVOID) // 回避
		{
			animation_ = Animation(getAnimationMS(ANIME_DAMAGE_MS / 2), Animation::Direction::AlTERNATE, 2, getAnimationMS(ANIME_ATACK_MS - 100));
			return true;
		}
		else if (animationId == AnimationKind::DESTROY) // 死亡
		{
			animation_ = Animation(getAnimationMS(600));
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
	 * 射程のテキストを返す
	 * @return テキスト
	 */
	string Unit::getLenText() const
	{
		try
		{
			return LEN_TEXT.at(info_.len);
		}
		catch (out_of_range&) {}
		
		return "";
	}


	/**
	 * @fn
	 * ダメージ値
	 * @return 死亡時 trueを返す
	 */
	bool Unit::damage(int damage)
	{
		prevHp_ = info_.hp;
		info_.hp -= damage;
		joinAnimationList(AnimationKind::DAMAGE);

		// ダメージエフェクト
		DamageEffect::makeDamageEffect(shape_.x, shape_.y, damage);
		
		if (info_.hp <= 0)
		{
			info_.hp = 0;
			return true;
		}
		
		return false;
	}

	/**
	 * @fn
	 * 回避
	 */
	void Unit::avoid()
	{
		joinAnimationList(AnimationKind::AVOID);
		DamageEffect::makeDamageEffect(shape_.x, shape_.y, DamageEffect::MISS);
	}

	/**
	 * @fn
	 * ユニット死亡
	 * @return ユニット死亡時 true
	 */
	bool Unit::checkDead()
	{
		if (info_.hp <= 0)
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
	 * ターン終了時の処理
	 */
	void Unit::turnEnd()
	{
		isActed_ = false;
		alpha_ = 255; // テスト処理
	}

	/**
	 * @fn
	 * 行動終了
	 */
	void Unit::endAction()
	{
		isActed_ = true;
		alpha_ = 100; // テスト処理
	}

	/**
	 * @fn
	 * 攻撃
	 * @param (targetRealX) 攻撃対象のX座標
	 * @param (targetRealY) 攻撃対象のY座標
	 */
	void Unit::atack(int targetRealX, int targetRealY)
	{
		targetRealX_ = shape_.x + (targetRealX - shape_.x) / 2;
		targetRealY_ = shape_.y + (targetRealY - shape_.y) / 2;
		joinAnimationList(AnimationKind::ATACK);
	}
}
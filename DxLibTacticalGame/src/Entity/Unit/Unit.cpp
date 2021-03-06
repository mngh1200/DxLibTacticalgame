#include "Unit.h"

namespace Entity {
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
			info_.atk = 9;
			info_.def = 3;
			info_.mov = 4;
			info_.len = 0;
			info_.ability = Ability::Kind::RUSH;
		}
		else if (kind == UnitKey::GUNNER) // 銃兵
		{
			info_.name = "銃兵";
			info_.hpm = 20;
			info_.atk = 12;
			info_.def = 2;
			info_.mov = 2;
			info_.len = 2;
			info_.range = 4;
			info_.isCloseAtack = false;
			info_.ability = Ability::Kind::THROUGH;
		}
		else // kind == UnitKey::LANCER の動作 // 槍兵
		{
			info_.name = "槍兵";
			info_.hpm = 30;
			info_.atk = 9;
			info_.def = 4;
			info_.mov = 2;
			info_.len = 1;
			info_.kind = UnitKey::LANCER;
			info_.ability = Ability::Kind::AMBUSH;
		}

		viewHp_ = info_.hp = info_.hpm;

		setImage(UnitImageKind::NORMAL);
	}

	/**
	 * @fn
	 * ユニットの状態を元に画像セット
	 * @param (state) 状態
	 */
	void Unit::setImage(int state)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		imageId_ = rm.getImage(ImageType::UNIT, state, !isEnemy_ ? info_.kind : info_.kind + UnitKey::UNIT_KIND_LEN);
	}

	/**
	 * @fn
	 * 描画
	 */
	void Unit::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		if (state_ == State::PREPARE_MOVE || state_ == State::SELECT_ACTION) // 選択中
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

		// 本体
		DxLib::DrawGraph(shape_.x, shape_.y, imageId_, TRUE);

		// HPバー
		int hpx1 = shape_.x + HP_PADDING;
		int hpy1 = shape_.y + HP_Y;
		int hpx2 = shape_.getX2() - HP_PADDING;
		int hpy2 = hpy1 + HP_H;

		int hpw = (int)(HP_W * ((float)viewHp_ / (float)info_.hpm));

		DxLib::DrawBox(hpx1, hpy1, hpx2, hpy2, rm.getColor(ColorType::SUB_COLOR), TRUE);
		DxLib::DrawBox(hpx1, hpy1, hpx1 + hpw, hpy2, rm.getColor(ColorType::MAIN_COLOR), TRUE);

		// ダメージ予測
		if (predictHp_ != -1)
		{
			int damageX = (int)(HP_W * ((float)predictHp_ / (float)info_.hpm));

			DxLib::DrawBox(hpx1 + damageX, hpy1, hpx1 + hpw, hpy2, rm.getColor(ColorType::ENEMY_COLOR_LITE), TRUE);
			
		}

		if (alpha_ != 255) // 描画モードを標準に戻す
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		// 行動選択時（ユニット上に「待機」表記）
		if (!isEnemy_ && state_ == State::SELECT_ACTION)
		{
			const int x = shape_.x + WAIT_BTN_MARGIN_LEFT;
			const int y = shape_.y + WAIT_BTN_MARGIN_TOP;

			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
			DxLib::DrawBox(x, y, x + WAIT_BTN_W, y + WAIT_BTN_H, rm.getColor(ColorType::MAIN_COLOR), TRUE);
			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DxLib::DrawStringToHandle(x + WAIT_BTN_PADDING_LEFT, y + WAIT_BTN_PADDING_TOP, "待機", rm.getColor(ColorType::SUB_COLOR_DARK), rm.getHdlFont(FontType::NORMAL_S14));
		}

		renderExtra();
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
				state_ = State::SELECT_ACTION;
				shape_.disabledHit = false;
				return true;
			}
		}
		else if (animationId_ == AnimationKind::ATACK) // 攻撃
		{
			return updateAtackAnimation();
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
	 * 攻撃アニメーションの更新処理
	 * @return アニメーション時間（ms）
	 */
	bool Unit::updateAtackAnimation()
	{
		int baseX = Map::getRealX(x_);
		int baseY = Map::getRealY(y_);
		int targetRealX = baseX + (targetRealX_ - baseX) / 2;
		int targetRealY = baseY + (targetRealY_ - baseY) / 2;
		return animation_.update(&shape_.x, &shape_.y, baseX, baseY, targetRealX, targetRealY);
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
			animation_ = Animation(EnemyTurnCont::getAnimationMs(100));
			shape_.disabledHit = true; // イベント無効

			if (x_ != baseX_ || y_ != baseY_)
			{
				Utility::ResourceManager::playSound(SoundKind::MOVE);
			}

			return true;
		}
		else if (animationId == AnimationKind::ATACK) // 攻撃
		{
			createAtackAnimation();
			return true;
		}
		else if (animationId == AnimationKind::DAMAGE) // ダメージ
		{
			animation_ = Animation(EnemyTurnCont::getAnimationMs(ANIME_DAMAGE_MS / ANIME_DAMAGE_REPEAT), 0, ANIME_DAMAGE_REPEAT, EnemyTurnCont::getAnimationMs(ANIME_ATACK_MS));
			int baseX = Map::getRealX(x_);
			animation_.adjustFrame(shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE);
			animation_.adjustLastFrame(shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE);

			// HPバーアニメーション
			animationSub_ = Animation(EnemyTurnCont::getAnimationMs(ANIME_DAMAGE_MS), 0, 1, EnemyTurnCont::getAnimationMs(ANIME_ATACK_MS));

			return true;
		}
		else if (animationId == AnimationKind::AVOID) // 回避
		{
			animation_ = Animation(EnemyTurnCont::getAnimationMs(ANIME_DAMAGE_MS / 2), Animation::Direction::AlTERNATE, 2, EnemyTurnCont::getAnimationMs(ANIME_ATACK_MS - 100));
			return true;
		}
		else if (animationId == AnimationKind::DESTROY) // 死亡
		{
			animation_ = Animation(EnemyTurnCont::getAnimationMs(600));
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * 攻撃時アニメーション作成
	 */
	void Unit::createAtackAnimation()
	{
		Utility::ResourceManager::playSound(SoundKind::ATTACK_CLOSE);
		animation_ = Animation(EnemyTurnCont::getAnimationMs(ANIME_ATACK_MS), Animation::Direction::AlTERNATE, 2);
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
	 * 特殊ステータスのラベルと説明文を返す
	 * @param (list) 参照渡し用の変数 <ラベル, 説明文>
	 */
	void Unit::getExtraStatusList(vector<pair<string, string>>& list) const
	{
		// スキル
		list.push_back(make_pair(info_.ability.getLabel(), info_.ability.getDescription()));

		// 騎馬ユニットであるか
		if (isHorse())
		{
			list.push_back(make_pair("騎馬", "森林での移動力 -1"));
		}

		getExtraStatusListExtra(list);
	}

	/**
	 * @fn
	 * ダメージ予測をセット
	 * @param (predictDamage) 予測ダメージ量
	 */
	void Unit::setPredictDamage(int predictDamage)
	{
		predictHp_ = info_.hp - predictDamage;

		if (predictHp_ < 0)
		{
			predictHp_ = 0;
		}
	}

	/**
	 * @fn
	 * ダメージ予測をクリア
	 */
	void Unit::clearPredictDamage()
	{
		predictHp_ = -1;
	}

	/**
	 * @fn
	 * ダメージ値
	 * @param (damage) ダメージ量
	 * @param (direction) 攻撃された方向
	 * @param (isCloseAttack) 近接攻撃であるか
	 * @return 死亡時 trueを返す
	 */
	bool Unit::damage(int damage, int direction, bool isCloseAtack)
	{
		prevHp_ = info_.hp;
		info_.hp -= damage;
		joinAnimationList(AnimationKind::DAMAGE);

		// 攻撃された方向をログに残す
		if (isCloseAtack)
		{
			if (!(closeAttackedLogs_ & direction)) // 既に追加されている場合は追加しない
			{
				closeAttackedLogs_ += direction;
			}
		}

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
	 * HPをシステム上の処理で強制的に削る場合のメソッド
	 * @param (decrease) 減少量
	 */
	void Unit::forceDecreaseHp(int decrease)
	{
		info_.hp -= decrease;
		viewHp_ = info_.hp;
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
			state_ = State::PREPARE_MOVE;
			return true;
		}
		else if (!isSelect)
		{	
			if (isSelected())
			{
				state_ = State::NORMAL;
				setPos(baseX_, baseY_);
				if (animationId_ == AnimationKind::MOVE)
				{
					animation_.forceFinish();
				}
				
				return true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * ターン終了時の処理
	 * @param (isOwnEnd) ターンエンドした側のユニットの場合true
	 */
	void Unit::turnEnd(bool isOwnEnd)
	{
		isActed_ = false;
		setImage(UnitImageKind::NORMAL);
		turnEndExtra(isOwnEnd);
		closeAttackedLogs_ = Direction::NONE_DIRECTION;
	}

	/**
	 * @fn
	 * 行動終了
	 */
	void Unit::endAction()
	{
		isActed_ = true;
		setImage(UnitImageKind::ACTED);
	}

	/**
	 * @fn
	 * 攻撃
	 * @param (targetRealX) 攻撃対象のX座標
	 * @param (targetRealY) 攻撃対象のY座標
	 */
	void Unit::atack(int targetRealX, int targetRealY)
	{
		targetRealX_ = targetRealX;
		targetRealY_ = targetRealY;
		joinAnimationList(AnimationKind::ATACK);
	}
}
#include "EnemyTurnCont.h"

namespace Entity {
	const float EnemyTurnCont::PLAYMARK_W = (float)BUTTON_H * (float)sqrt(3) / 2.0f;
	const string EnemyTurnCont::ENEMY_SET_TEXT = "相手プレイヤーが\nユニットを配置中";
	const string EnemyTurnCont::ENEMY_TURN_TEXT = "相手プレイヤー\nターン中";
	float EnemyTurnCont::animationRate = 1.0f;

	/**
	 * @fn
	 * コンストラクタ
	 */
	EnemyTurnCont::EnemyTurnCont() : 
		speed_(Speed::NORMAL), 
		cacheSpeed_(Speed::NORMAL),
		mode_(Mode::SPEED_CONT),
		textRelationY_(0),
		textAlpha_(255)
	{
		shape_.set(WIN_W - BuiConfirmButton::WIDTH - BuiConfirmButton::MARGIN, BATTLE_UI_AREA_Y + BuiConfirmButton::MARGIN);
		shape_.setSize(BuiConfirmButton::WIDTH, BATTLE_UI_AREA_H - BuiConfirmButton::MARGIN * 2);

		for (int i = 0; i < Speed::LEN; ++i)
		{
			int x = shape_.x + PADDING_X + BUTTON_RELATIVE_XN * i;
			int y = shape_.y + PADDING_Y;
			buttonsShape_[i] = Shape(x, y, BUTTON_W, BUTTON_H);
		}

		shape_.disabledHit = true;

		// テキストの相対Y座標取得
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		int width, height, lineCount;

		DxLib::GetDrawStringSizeToHandle(&width, &height, &lineCount, ENEMY_SET_TEXT.c_str(),
			DxLib::GetStringLength(ENEMY_SET_TEXT.c_str()), rm.getHdlFont(FONT_TYPE));

		textRelationY_ = (shape_.h - height) / 2;
	}

	/**
	 * @fn
	 * 再生マーク描画
	 */
	void drawPlaymark(int x, int y, float wf, int h, int color)
	{
		float xf = (float)x;
		float yf = (float)y;
		float hf = (float)h;

		DxLib::DrawTriangleAA(xf, yf, xf, yf + hf, xf + wf, yf + hf / 2.0f, color, TRUE);
	}

	/**
	 * @fn
	 * 描画
	 */
	void EnemyTurnCont::render() const
	{
		if (shape_.disabledHit)
		{
			return; // 非表示
		}

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// 枠
		DxLib::DrawRoundRectAA((float)shape_.x, (float)shape_.y, (float)shape_.getX2(), (float)shape_.getY2(),
			(float)BuiConfirmButton::RECT_ROUND, (float)BuiConfirmButton::RECT_ROUND, BuiConfirmButton::RECT_ROUND_POS_NUM, rm.getColor(ColorType::SUB_COLOR), TRUE);

		if (mode_ == Mode::SPEED_CONT)
		{
			// 各ボタン
			for (int i = 0; i < Speed::LEN; ++i)
			{
				Shape shape = buttonsShape_[i];
				int imagePos = i;
				int color = rm.getColor(ColorType::MAIN_COLOR);

				if (speed_ == i)
				{
					imagePos += Speed::LEN * 2;
					color = rm.getColor(ColorType::MAIN_COLOR_ON_DARK);
				}

				if (i == Speed::SLOW) // スロー再生
				{
					DxLib::DrawBox(shape.x, shape.y, shape.x + 5, shape.getY2(), color, TRUE);
					drawPlaymark(shape.x + 5 + 5, shape.y, PLAYMARK_W, shape.h, color);
				}
				else if (i == Speed::NORMAL) // 再生
				{
					drawPlaymark(shape.x + (shape.w - (int)PLAYMARK_W) / 2, shape.y, PLAYMARK_W, shape.h, color);
				}
				else if (i == Speed::FAST) // 早送り
				{
					drawPlaymark(shape.x, shape.y, PLAYMARK_W, shape.h, color);
					drawPlaymark(shape.getX2() - (int)PLAYMARK_W, shape.y, PLAYMARK_W, shape.h, color);
				}
			}
		}
		else // テキスト表記
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, textAlpha_);

			string text = ENEMY_TURN_TEXT; // 敵ターン中の場合

			if (mode_ == Mode::ENEMY_SET_MESSAGE) // 敵自由配置の場合
			{
				text = ENEMY_SET_TEXT;
			}
			
			DxLib::DrawStringToHandle(shape_.x + TEXT_MARGIN_LEFT, shape_.y + textRelationY_, text.c_str(), rm.getColor(TEXT_COLOR_TYPE), rm.getHdlFont(FONT_TYPE));

			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	/**
	 * @fn
	 * 表示
	 */
	void EnemyTurnCont::onMouseClick(int x, int y)
	{
		if (mode_ != Mode::SPEED_CONT)
		{
			return;
		}

		int oldSpeed = speed_;
		for (int i = 0; i < Speed::LEN; ++i)
		{
			if (buttonsShape_[i].isHit(x, y))
			{
				changeSpeed(i);

				if (oldSpeed != speed_) // スピードが変更されていれば効果音
				{
					Utility::ResourceManager::playSound(SoundKind::CHECK);
				}

				break;
			}
		}
	}

	/**
	 * @fn
	 * 進行速度変更
	 */
	void EnemyTurnCont::changeSpeed(int speed)
	{
		if (speed != speed_)
		{
			speed_ = speed;
		}

		float rate = 1.0f; // 倍率

		if (speed_ == Speed::FAST)
		{
			 rate = 0.5f;
		}
		else if (speed_ == Speed::NORMAL)
		{
			
		}
		else if (speed_ == Speed::SLOW)
		{
			rate = 2.0f;
		}

		// ユニットアニメーションと敵操作のタイマーの時間調整
		animationRate = rate;
		// Unit::animatinTimeRate = rate;
		// Battle::EnemyBattleController::timerRate = rate;
	}

	/**
	 * @fn
	 * 表示
	 */
	void EnemyTurnCont::show()
	{
		shape_.disabledHit = false;
		changeSpeed(cacheSpeed_);
	}

	/**
	 * @fn
	 * 非表示
	 */
	void EnemyTurnCont::hide()
	{
		cacheSpeed_ = speed_;
		shape_.disabledHit = true;
		changeSpeed(Speed::NORMAL);
	}

	/**
	 * @fn
	 * モードを指定
	 * @param (mode) 対象モード
	 */
	void EnemyTurnCont::setMode(int mode)
	{
		mode_ = mode;

		if (mode_ != Mode::SPEED_CONT)
		{
			joinAnimationList();
		}
	}


	/**
	 * @fn
	 * アニメーション更新
	 * @return true:終了
	 */
	bool EnemyTurnCont::animationUpdate()
	{
		animation_.update(&textAlpha_, 255, 0);
		return false;
	}


	/**
	 * @fn
	 * 現在のアニメーション時間倍率を元にアニメーション時間を計算
	 * @param ベースとなるアニメーション時間
	 * @return アニメーション時間
	 */
	int EnemyTurnCont::getAnimationMs(int ms)
	{
		return (int)(animationRate * ms);
	}

	/**
	 * @fn
	 * アニメーション作成(ObjectsControl::addAnimationObjメソッド専用で呼び出す)
	 * @return アニメーション作成可能な場合trueを返す
	 */
	bool EnemyTurnCont::createAnimation(int animationId)
	{
		animation_ = Animation(1000, Animation::Direction::AlTERNATE_REVERSE, Animation::REPEAT_INFINITE, 0, Easing::InCirc<float>);
		return true;
	}
}
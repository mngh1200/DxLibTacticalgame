#include "EnemySpeedController.h"
#include "Battle/EnemyBattleController.h"

namespace Entity {
	const float EnemySpeedController::PLAYMARK_W = (float)BUTTON_H * (float)sqrt(3) / 2.0f;

	/**
	 * @fn
	 * コンストラクタ
	 */
	EnemySpeedController::EnemySpeedController() : speed_(Speed::NORMAL), cacheSpeed_(Speed::NORMAL)
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
	void EnemySpeedController::render() const
	{
		if (shape_.disabledHit)
		{
			return; // 非表示
		}

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// 枠
		DxLib::DrawRoundRectAA((float)shape_.x, (float)shape_.y, (float)shape_.getX2(), (float)shape_.getY2(),
			(float)BuiConfirmButton::RECT_ROUND, (float)BuiConfirmButton::RECT_ROUND, BuiConfirmButton::RECT_ROUND_POS_NUM, rm.getColor(ColorType::SUB_COLOR), TRUE);

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

			// DxLib::DrawExtendGraphF((float)shape.x, (float)shape.y, (float)shape.getX2(), (float)shape.getY2(), rm.getImage(ImageType::BUI, BUI_ImageKind::SPEED, imagePos), TRUE);
		}
	}

	/**
	 * @fn
	 * 表示
	 */
	void EnemySpeedController::onMouseClick(int x, int y)
	{
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
	void EnemySpeedController::changeSpeed(int speed)
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
		Unit::animatinTimeRate = rate;
		Battle::EnemyBattleController::timerRate = rate;
	}

	/**
	 * @fn
	 * 表示
	 */
	void EnemySpeedController::show()
	{
		shape_.disabledHit = false;
		changeSpeed(cacheSpeed_);
	}

	/**
	 * @fn
	 * 非表示
	 */
	void EnemySpeedController::hide()
	{
		cacheSpeed_ = speed_;
		shape_.disabledHit = true;
		changeSpeed(Speed::NORMAL);
	}
}
﻿#include "TurnEndButton.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	TurnEndButton::TurnEndButton()
	{
		setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, State::NORMAL);
		setColor(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR, State::MOUSE_DOWN);
		setColor(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR, State::MOUSE_OVER);

		shape_.set(WIN_W - WIDTH - MARGIN, BATTLE_UI_AREA_Y + MARGIN, WIDTH, HEIGHT);
		
		setText("ターン終了", FontType::NORMAL_S24);
	}

	/**
	 * @fn
	 * 描画
	 */
	void TurnEndButton::render() const
	{
		if (shape_.disabledHit)
		{
			return; // 非表示
		}

		// 現在の状態取得
		int nowState = State::NORMAL;
		if (isMouseDown_)
		{
			nowState = State::MOUSE_DOWN;
		}
		else if (isMouseOver_)
		{
			nowState = State::MOUSE_OVER;
		}

		// ボタン
		DxLib::DrawRoundRectAA((float)shape_.x, (float)shape_.y, (float)shape_.getX2(), (float)shape_.getY2(), 
			(float)RECT_ROUND, (float)RECT_ROUND, RECT_ROUND_POS_NUM, color_[nowState], TRUE);

		// テキスト
		DxLib::DrawFormatStringToHandle(shape_.x + textX_, shape_.y + textY_, textColor_[nowState], font_, text_.c_str());
	}

	/**
	 * @fn
	 * 表示
	 */
	void TurnEndButton::show()
	{
		shape_.disabledHit = false;
	}

	/**
	 * @fn
	 * 非表示
	 */
	void TurnEndButton::hide()
	{
		shape_.disabledHit = true;
	}
}
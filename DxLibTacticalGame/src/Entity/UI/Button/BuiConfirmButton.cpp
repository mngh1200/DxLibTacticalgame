#include "BuiConfirmButton.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	BuiConfirmButton::BuiConfirmButton() : isVisible_(true), isDisabled_(false)
	{
		setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, State::NORMAL);
		setColor(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR, State::MOUSE_DOWN);
		setColor(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR, State::MOUSE_OVER);

		shape_.set(WIN_W - WIDTH - MARGIN, BATTLE_UI_AREA_Y + MARGIN, WIDTH, HEIGHT);
		
		setText("", FontType::NORMAL_S24);
	}

	/**
	 * @fn
	 * 描画
	 */
	void BuiConfirmButton::render() const
	{
		if (!isVisible_)
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
	void BuiConfirmButton::show()
	{
		isVisible_ = true;
		shape_.disabledHit = isDisabled_;
	}

	/**
	 * @fn
	 * 非表示
	 */
	void BuiConfirmButton::hide()
	{
		isVisible_ = false;
		shape_.disabledHit = true;
	}

	/**
	 * @fn
	 * 有効状態セット
	 * @param (disable) セットする状態
	 */
	void BuiConfirmButton::setDisabled(bool disable)
	{
		isDisabled_ = disable;
		shape_.disabledHit = disable;

		if (disable)
		{
			setColor(ColorType::NEGATIVE_COLOR_DARK, ColorType::NEGATIVE_COLOR, State::ALL);
		}
		else
		{
			setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, State::NORMAL);
			setColor(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR, State::MOUSE_DOWN);
			setColor(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR, State::MOUSE_OVER);
		}

		if (!isVisible_)
		{
			shape_.disabledHit = true;
		}
	}
}
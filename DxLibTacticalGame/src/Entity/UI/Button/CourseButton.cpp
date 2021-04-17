#include "CourseButton.h"

namespace Entity {
	const float CourseButton::HOVER_ANIMATION_SCALE = 1.1f;

	/**
	 * @fn
	 * コンストラクタ
	 */
	CourseButton::CourseButton() : isSelected_(false), state_(State::NORMAL)
	{
		setColor(ColorType::SUB_COLOR_LITE2, ColorType::SUB_COLOR);
		setFont(FontType::NORMAL_S20);
		setTextX(20);
	}

	/**
	 * @fn
	 * コンストラクタ
	 * @param (x) x座標
	 * @param (y) y座標
	 * @param (w) 幅
	 * @param (h) 高さ
	 * @param (text) ボタンのテキスト
	 * @param (state) ボタンの状態
	 */
	CourseButton::CourseButton(int x, int y, int w, int h, const char* text, int state) : CourseButton()
	{
		state_ = state;
		setShape(x, y, w, h);
		setText(text);
	}

	/**
	 * @fn
	 * コース選択状態または未選択状態にする
	 * @param (selected) true: 選択状態, false: 未選択状態
	 * @param (isSound) trueの場合、選択時にサウンド出力
	 */
	void CourseButton::setSelected(bool isSelected, bool isSound)
	{
		isSelected_ = isSelected;

		if (isSelected_)
		{
			setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR);
			if (isSound)
			{
				Utility::ResourceManager::playSound(SoundKind::CHECK);
			}
		}
		else
		{
			setColor(ColorType::SUB_COLOR_LITE2, ColorType::SUB_COLOR);
		}
	}

	/**
	 * @fn
	 * 描画
	 */
	void CourseButton::render() const
	{
		// 親クラスの描画処理実行
		TextButton::render();

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();


		if (state_ == State::CLEAR) // クリアステージ
		{
			int colorType = ColorType::POSITIVE_COLOR;

			if (isSelected_)
			{
				colorType = ColorType::POSITIVE_LITE_COLOR;
			}

			DxLib::DrawFormatStringToHandle(shape_.getX2() - 100, shape_.y + textY_, rm.getColor(colorType), font_, "Clear");
		}
		else if (state_ == State::NEW) // Newステージ
		{
			int colorType = ColorType::ENEMY_COLOR;

			if (isSelected_)
			{
				colorType = ColorType::ENEMY_COLOR_LITE;
			}

			DxLib::DrawFormatStringToHandle(shape_.getX2() - 100, shape_.y + textY_, rm.getColor(colorType), font_, "New");
		}
	}
}
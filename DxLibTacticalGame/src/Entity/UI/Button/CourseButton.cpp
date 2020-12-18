﻿#include "CourseButton.h"

namespace Entity {
	const float CourseButton::HOVER_ANIMATION_SCALE = 1.1f;

	/**
	 * @fn
	 * コンストラクタ
	 */
	CourseButton::CourseButton() : animation_ {}, isSelected_(false), status_('!')
	{
		setColor(ColorType::MAIN_COLOR, ColorType::SUB_COLOR);
	}

	/**
	 * @fn
	 * コンストラクタ
	 * @param (x) x座標
	 * @param (y) y座標
	 * @param (status) コースのクリア状況
	 */
	CourseButton::CourseButton(int x, int y, char status, bool isNew) : CourseButton()
	{
		status_ = status;
		baseShape_ = Shape(x, y, SIZE, SIZE);

		// 新コースの場合
		if (isNew)
		{
			setShape(x, y, 0, 0);
			setText("", FontType::BLACK_S48);
		}
		else
		{
			setShape(x, y, SIZE, SIZE);
			setText(string(1, status), FontType::BLACK_S48);
		}




		// クリア状況次第でテキストと色変更
		if (status == Status::S)
		{
			setTextColor(ColorType::ACCENT_COLOR);
		}
		else if (status == Status::A)
		{
			setTextColor(ColorType::PLAYER_COLOR);
		}
		else if (status == Status::B)
		{
			setTextColor(ColorType::SUB_COLOR);
		}
		else if (status == Status::C)
		{
			setTextColor(ColorType::NEGATIVE_COLOR);
		}
		else if (status == Status::NO_CLEAR)
		{
			setTextColor(ColorType::ENEMY_COLOR);
		}
		
	}

	/**
	 * @fn
	 * コース選択状態または未選択状態にする
	 * @param (selected) true: 選択状態, false: 未選択状態
	 */
	void CourseButton::setSelected(bool isSelected)
	{
		isSelected_ = isSelected;

		if (isSelected_)
		{
			setBackgroundColor(ColorType::MAIN_COLOR_ON);
		}
		else
		{
			setBackgroundColor(ColorType::MAIN_COLOR);
		}
	}

	/**
	 * @fn
	 * アニメーション作成(ObjectsControl::addAnimationObjメソッド専用で呼び出す)
	 * @return アニメーション作成可能な場合trueを返す
	 */
	bool CourseButton::createAnimation(int animationId)
	{
		// 既に実行済みのアニメーションがある場合は作成不可
		if (animationId == animationId_)
		{
			return false;
		}
		
		if (animationId == EXPANSION) // 拡大
		{
			animation_ = Animation(HOVER_ANIMATION_MS);
			return true;
		}
		else if (animationId == SHRINK) // 縮小
		{
			animation_ = Animation(HOVER_ANIMATION_MS, Animation::REVERSE);
			animation_.adjustFrame(shape_, baseShape_, HOVER_ANIMATION_SCALE);
			return true;
		}
		else if (animationId == BORN) // 新コースアニメーション
		{
			animation_ = Animation(BORN_ANIMATION_MS, Animation::REVERSE, 1, BORN_ANIMATION_DELAY, Easing::InOutCirc<float>);
			setText("");

			// サウンド
			Utility::ResourceManager::playSound(SoundKind::BORN);
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * 描画
	 */
	void CourseButton::render() const
	{
		// ボタン
		DxLib::DrawRoundRect(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), RECT_ROUND, RECT_ROUND, color_[State::NORMAL], TRUE);

		// テキスト
		DxLib::DrawFormatStringToHandle(shape_.x + textX_, shape_.y + textY_, textColor_[State::NORMAL], font_, text_.c_str());
	}

	/**
	 * @fn
	 * アニメーション更新
	 * @return true:終了
	 */
	bool CourseButton::animationUpdate()
	{
		if (animationId_ == EXPANSION || animationId_ == SHRINK)
		{
			bool isEnd = animation_.update(&shape_, baseShape_, HOVER_ANIMATION_SCALE);
			setSize(shape_.w, shape_.h); // テキスト位置調整用に再適用
			return isEnd;
		}
		else if (animationId_ == BORN)
		{
			if (animation_.update(&shape_, baseShape_, 0.0f))
			{
				setSize(shape_.w, shape_.h); // 文字位置調整用
				setSelected(true);
				return true;
			}
			else if (shape_.h >= BORN_DISP_TEXT_SIZE) // 文字表示
			{
				setText(string(1, status_));
				setSize(shape_.w, shape_.h); // 文字位置調整用
			}
			return false;
		}
		return true;
	}

	/**
	 * @fn
	 * マウスがクリックした瞬間の処理
	 */
	void CourseButton::onMouseClick()
	{
		// サウンド
		Utility::ResourceManager::playSound(SoundKind::CHECK);
	}

	/**
	 * @fn
	 * マウスがホバーした瞬間
	 */
	void CourseButton::onMouseOver()
	{
		joinAnimationList(EXPANSION); // 拡大
	}

	/**
	 * @fn
	 * マウスがホバー状態から離れたときの処理
	 */
	void CourseButton::onMouseOut()
	{
		joinAnimationList(SHRINK); // 縮小
	}



}
#pragma once
#include <climits>
#include <string>
#include "Entity/UI/Button/TextButton.h"
#include "Utility/ResourceManager.h"
#include "Animation/Animation.h"

using namespace std;

/**
 * @file CourseButton.h
 * @brief セレクト画面のコースセレクトボタン
 */

namespace Entity
{
	class CourseButton : public TextButton
	{
	
	
	public:
		CourseButton();
		CourseButton(int x, int y, char status, bool isNew = false);
		~CourseButton() {};

		void render() const override;

		bool animationUpdate() override;

		void onMouseClick() override;
		void onMouseOver() override;
		void onMouseOut() override;

		void setSelected(bool isSelected);

		// クリア状況
		enum Status : char
		{
			S = 'S',
			A = 'A',
			B = 'B',
			C = 'C',
			NO_CLEAR = '!'
		};

		static const int SIZE = 100; //! 幅、高さ

		// アニメーションの種類
		enum AnimationId
		{
			EXPANSION,
			SHRINK,
			BORN
		};

	protected:
		bool createAnimation(int animationId);

	private:
		constexpr static int RECT_ROUND = 10; //! 角丸の値
		constexpr static int HOVER_ANIMATION_MS = 100;	//! 拡大、縮小の時間
		static const float HOVER_ANIMATION_SCALE;	//! サイズ倍率(floatはcppで定義する必要あり)

		constexpr static int BORN_ANIMATION_MS = 500; //! NEWコースアニメーションの時間
		constexpr static int BORN_ANIMATION_DELAY = 100; //! NEWコースアニメーションの遅延時間
		constexpr static int BORN_DISP_TEXT_SIZE = 48; //! どの大きさの時点で文字を表示するか

		//! 選択状態にあるか
		bool isSelected_;

		//! アニメーションクラス
		Animation animation_;

		//! 基準の大きさ
		Shape baseShape_;

		//! コースのクリア状況
		char status_;
	};

	
}
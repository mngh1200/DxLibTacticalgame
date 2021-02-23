#pragma once
#include <climits>
#include <string>
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "Entity/Unit/Unit.h"
#include "DxLib.h"
#include "Define.h"

using namespace std;

/**
 * @file TutorialArrow.h
 * @brief ダメージエフェクト（ダメージ量またはMISSの表示）
 */

namespace Entity
{
	class TutorialArrow : public Figure
	{
	public:
		TutorialArrow() : animation_{}, baseY_(0) {};
		~TutorialArrow() {};

		constexpr static int W = 32; //! 幅
		constexpr static int H = 50; //! 高さ

		static shared_ptr<TutorialArrow> makeTutorialArrow(int x, int y);

		void start(int x, int y);

		void render() const override;

		bool animationUpdate() override;

	protected:
		bool createAnimation(int animationId);

	private:
		constexpr static int ANIME_MS = 400; //! アニメーション1回分の時間
		constexpr static int DISPLAY_TIME = 8000; //! 表示時間
		constexpr static int ANIMATION_COUNT = DISPLAY_TIME / ANIME_MS; //! アニメーションを繰り返す回数

		constexpr static int MOVE_Y = 15; //! アニメーションで動作する量

		int baseY_; //! 基準のY座標

		Animation animation_; //! アニメーション

		// アニメーションの種類
		enum AnimationKind
		{
			UP_DOWN
		};
	};
}
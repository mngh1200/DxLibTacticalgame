#pragma once
#include <climits>
#include <string>
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "Entity/Unit/Unit.h"
#include "Utility/Timer.h"
#include "Battle/BUI/EnemyTurnCont.h"
#include "DxLib.h"
#include "Define.h"

using namespace std;
using namespace Battle;

/**
 * @file ExtraEffect.h
 * @brief 特殊効果エフェクト（スキルや連携等の表示）
 */

namespace Entity
{
	class ExtraEffect : public Figure
	{
	public:
		ExtraEffect() : text_{}, isPlayer_(true), num_(0), isLeft_(true), animation_{}, waitTimer_{} {};
		~ExtraEffect() {};

		void initExtraEffect(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, const char* text, int num);

		void hide();

		void render() const override;

		bool animationUpdate() override;

	protected:
		bool createAnimation(int animationId);

	private:
		constexpr static int BASE_Y = 100; //! 基準のY座標
		constexpr static int W = 180; //! 幅
		constexpr static int H = 36; //! 高さ

		constexpr static int COLOR_W = 20; //! 所属カラー着色幅

		constexpr static int MARGIN = 12; //! 余白
		constexpr static int PADDING_V = 7; //! 余白（内側上下）
		constexpr static int PADDING_LEFT = 30; //! 余白（内側左）

		constexpr static int ANIMATION_TIME = 200; //! アニメーション時間
		constexpr static int SHOW_WAIT_MS = 600; //! アニメーション終了遅延時間
		constexpr static int NUM_DELAY = 200; //! 連番による遅延時間

		//! テキスト
		string text_;

		//! プレイヤー側か
		bool isPlayer_;

		//! 表示位置が左側であるか
		bool isLeft_;

		//! 何番目に表示するか
		int num_;

		//! アニメーション終了遅延用のタイマー
		Utility::Timer waitTimer_;

		//! アニメーションクラス
		Animation animation_;

		// アニメーションの種類
		enum AnimationKind
		{
			SHOW,
			HIDE
		};
	};
}
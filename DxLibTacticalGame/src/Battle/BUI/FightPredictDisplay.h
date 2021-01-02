#pragma once
#include <climits>
#include <string>
#include <memory>
#include "Entity/Unit/Unit.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "Battle/Fight.h"
#include "BattleUIDefine.h"

using namespace std;

/**
 * @file FightPredictDisplay.h
 * @brief 戦闘予測表示欄
 */

namespace Entity
{
	class FightPredictDisplay : public Object
	{
	public:
		FightPredictDisplay();
		~FightPredictDisplay() {};

		void render() const override;
		bool animationUpdate() override;

		void emplaceFight(const Battle::Fight* fight);
		void reset();

	protected:
		bool createAnimation(int animationId) override;

	private:
		constexpr static int AREA_WIDTH = 450; //! エリアの幅
		constexpr static int CENTER_WIDTH = 50; //! 中央部の幅
		constexpr static int CENTER_PADDING = 10; //! 中央部の余白
		constexpr static int DIRECTION_W = CENTER_WIDTH - CENTER_PADDING * 2; // 攻撃方向の幅

		constexpr static int PLAYER_X = AREA_WIDTH + CENTER_WIDTH;

		constexpr static int ANIMATION_MS = 500;	//! アニメーション時間
		constexpr static int ANIMATION_Y0 = WIN_H;  //! アニメーション開始位置

		void drawFightPredict(int x, Battle::FightData& fightData, bool isEnemy) const;
		void drawAtackDirection(int y, bool isPlayer) const;

		// アニメーション種類
		enum AnimationKind
		{
			DISPLAY
		};

		//! 戦闘予測データ
		const Battle::Fight* fight_;

		//! アニメーション用変数
		Animation animation_;
	};
}
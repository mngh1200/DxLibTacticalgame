#pragma once
#include <climits>
#include <string>
#include <memory>
#include "Utility/ResourceManager.h"
#include "Entity/Object.h"
#include "TerrainEffectDisplay.h"
#include "Entity/Unit/Unit.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "BattleUIDefine.h"

using namespace std;

/**
 * @file UnitStatusDisplay.h
 * @brief ユニットステータス表示欄
 */

namespace Entity
{
	class UnitStatusDisplay : public Object
	{
	public:
		UnitStatusDisplay();
		~UnitStatusDisplay() {};

		void render() const override;
		bool animationUpdate() override;

		void setTargetUnit(shared_ptr<Unit> unit);
		void clear();

	protected:
		bool createAnimation(int animationId) override;

	private:
		constexpr static int WIDTH = 450; //! 幅
		constexpr static int X = TerrainEffectDisplay::WIDTH + 40; //! X座標

		constexpr static int ANIMATION_MS = 500;	//! アニメーション時間
		constexpr static int ANIMATION_Y0 = WIN_H;  //! アニメーション開始位置

		// アニメーション種類
		enum AnimationKind
		{
			DISPLAY
		};

		//! 対象マス
		shared_ptr<Unit> targetUnit_;

		//! アニメーション用変数
		Animation animation_;
	};
}
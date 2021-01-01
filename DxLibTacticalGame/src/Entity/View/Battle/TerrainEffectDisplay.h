#pragma once
#include <climits>
#include <string>
#include <memory>
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "Entity/Battle/Mass.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "Battle/BattleUIDefine.h"


using namespace std;

/**
 * @file TerrainEffectDisplay.h
 * @brief 地形効果表示欄
 */

namespace Entity
{
	class TerrainEffectDisplay : public Figure
	{
	public:
		TerrainEffectDisplay();
		~TerrainEffectDisplay() {};

		constexpr static int WIDTH = 210; //! 幅

		void render() const override;
		bool animationUpdate() override;

		void setTargetMass(shared_ptr<Mass> mass);
		void clear();

	protected:
		bool createAnimation(int animationId) override;

	private:
		constexpr static int ANIMATION_Y0 = WIN_H;  //! アニメーション開始位置

		// アニメーション種類
		enum AnimationKind
		{
			DISPLAY
		};

		//! 対象マス
		shared_ptr<Mass> targetMass_;

		//! アニメーション用変数
		Animation animation_;
	};
}
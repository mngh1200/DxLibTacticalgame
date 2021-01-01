#pragma once
#include <climits>
#include <string>
#include <memory>
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "Entity/Battle/Mass.h"
#include "Animation/Animation.h"
#include "DxLib.h"

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

		void setTargetMass(shared_ptr<Mass> mass, int realX);
		void clear();

	protected:
		bool createAnimation(int animationId) override;

	private:
		constexpr static int LINE_MARGIN = 3;  //! 一行の余白
		constexpr static int LINE_PADDING = 6; //! 一行の余白
		constexpr static int LINE_HEIGHT = 18 + (LINE_MARGIN + LINE_PADDING) * 2; //! 一行当たりの高さ
		constexpr static int LINE_COUNT = 2;   //! 何行あるか

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
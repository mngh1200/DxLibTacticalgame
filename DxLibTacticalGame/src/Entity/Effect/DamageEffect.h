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
 * @file DamageEffect.h
 * @brief ダメージエフェクト（ダメージ量またはMISSの表示）
 */

namespace Entity
{
	class DamageEffect : public Figure
	{
	public:
		DamageEffect() : isDisplay_(false), imageIds_{} {};
		~DamageEffect() {};

		constexpr static int MISS = -1; //! ミスの場合の値

		static void makeDamageEffect(int unitX, int unitY, int damage);

		void init(int unitX, int unitY, int damage);

		void render() const override;

		bool animationUpdate() override;

	protected:
		bool createAnimation(int animationId);

	private:

		constexpr static int H = 32; //! ミス表示の幅
		constexpr static int MISS_W = 115; //! ミス表示の幅
		constexpr static int DAMAGE_W = 26; //! ダメージ数値の幅（１桁分）

		vector<int> imageIds_; //! 表示画像

		bool isDisplay_; //! 描画可否

		Animation animation_; //! アニメーション

		// アニメーションの種類
		enum AnimationKind
		{
			DISPLAY
		};
	};
}
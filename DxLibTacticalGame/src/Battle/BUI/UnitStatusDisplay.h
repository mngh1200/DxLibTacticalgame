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
	// スキル等の特殊ステータスの表示内容を管理するクラス
	struct ExtraStatus
	{
		string label; // ラベル
		string description; // 説明文
		Shape shape; // 矩形座標
	};

	class UnitStatusDisplay : public Object
	{
	public:
		UnitStatusDisplay();
		~UnitStatusDisplay() {};

		void render() const override;
		bool animationUpdate() override;

		void updateByEvents(int x, int y);

		void setTargetUnit(shared_ptr<Unit> unit);
		void clear();

	protected:
		bool createAnimation(int animationId) override;

	private:
		constexpr static int WIDTH = 450; //! 幅
		constexpr static int X = TerrainEffectDisplay::WIDTH + 40; //! X座標

		constexpr static int NAME_W = BUI_ZEN_W * 3; //! 名前欄の幅
		constexpr static int EXTRA_STATUS_MARGIN = 6; //! 特殊ステータス欄の余白

		constexpr static int ANIMATION_MS = 500;	//! アニメーション時間
		constexpr static int ANIMATION_Y0 = WIN_H;  //! アニメーション開始位置

		constexpr static int EXTRA_STATUS_ID_NONE = -1; //! 特殊ステータス未指定時のID

		// アニメーション種類
		enum AnimationKind
		{
			DISPLAY
		};

		//! 対象マス
		shared_ptr<Unit> targetUnit_;

		//! アニメーション用変数
		Animation animation_;

		//! 特殊ステータスのリスト
		vector<ExtraStatus> extraStatusList_;

		//! ホバー中の特殊ステータス
		int extraStatusHoverId_;
	};
}
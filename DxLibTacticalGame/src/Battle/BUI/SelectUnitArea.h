#pragma once
#include <climits>
#include <string>
#include <memory>
#include "Utility/ResourceManager.h"
#include "Entity/Unit/Unit.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "BattleUIDefine.h"


using namespace std;

/**
 * @file SelectUnitArea.h
 * @brief ユニット選択欄
 */

namespace Entity
{
	class SelectUnitArea : public Object
	{
	public:
		SelectUnitArea();
		~SelectUnitArea() {};

		constexpr static int COUNT_WIDTH = 100; //! カウントエリアの幅
		constexpr static int UNIT_MARGIN = 10; //! ユニット選択肢の余白

		void render() const override;

		void onMouseClick(int x, int y) override;

		void setCountMax(int countMax);
		bool addCount();
		void removeCount();

		int getCount() const { return count_; } // 現在の配置数を返す
		int getSelectUnitId() const { return selectUnitId_; }; // 選択中のユニットIDを返す

	private:
		constexpr static int FONT_TYPE = FontType::NORMAL_S32; //! フォントの種類
		constexpr static int UNIT_Y = BATTLE_UI_AREA_Y + (BUI_H - CHIP_SIZE) / 2; //! ユニット選択肢のY座標

		pair<int, int> countPos_;	//! 配置数の座標
		int countMax_;	//! 最大配置可能数
		int count_;		//! 配置数

		int selectUnitId_; //! 選択中のユニットID

		vector<Shape> unitList_; //! ユニット選択肢の矩形座標
	};
}
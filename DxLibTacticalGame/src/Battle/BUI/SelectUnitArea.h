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

		void render() const override;

		void onMouseClick(int x, int y) override;

		int getSelectUnitId() const { return selectUnitId_; }; // 選択中のユニットIDを返す

	private:
		constexpr static int UNIT_Y = BATTLE_UI_AREA_Y + (BUI_H - CHIP_SIZE) / 2; //! ユニット選択肢のY座標
		constexpr static int UNIT_MARGIN = 10; //! ユニット選択肢の余白

		int selectUnitId_; //! 選択中のユニットID

		vector<Shape> unitList_; //! ユニット選択肢の矩形座標
	};
}
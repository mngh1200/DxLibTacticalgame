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
 * @brief ���j�b�g�I��
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

		int getSelectUnitId() const { return selectUnitId_; }; // �I�𒆂̃��j�b�gID��Ԃ�

	private:
		constexpr static int UNIT_Y = BATTLE_UI_AREA_Y + (BUI_H - CHIP_SIZE) / 2; //! ���j�b�g�I������Y���W
		constexpr static int UNIT_MARGIN = 10; //! ���j�b�g�I�����̗]��

		int selectUnitId_; //! �I�𒆂̃��j�b�gID

		vector<Shape> unitList_; //! ���j�b�g�I�����̋�`���W
	};
}
#pragma once
#include <climits>
#include <string>
#include "Entity/UI/Button/TextButton.h"
#include "Utility/ResourceManager.h"
#include "Battle/BUI/BattleUIDefine.h"

using namespace std;

/**
 * @file BuiConfirmButton.h
 * @brief BUI用の確定ボタン
 */

namespace Entity
{
	class BuiConfirmButton : public TextButton
	{

	public:
		BuiConfirmButton();
		~BuiConfirmButton() {};

		constexpr static int WIDTH = 180; //! 幅
		constexpr static int MARGIN = 10; //! 余白
		constexpr static int HEIGHT = BATTLE_UI_AREA_H - MARGIN * 2;

		constexpr static int RECT_ROUND = 15; //! 角丸の値
		constexpr static int RECT_ROUND_POS_NUM = 16; //! 角丸のpos_num値

		void render() const override;

		void show();
		void hide();

	private:

	};

	
}
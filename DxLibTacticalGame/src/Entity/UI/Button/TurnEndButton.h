#pragma once
#include <climits>
#include <string>
#include "Entity/UI/Button/TextButton.h"
#include "Utility/ResourceManager.h"
#include "Battle/BUI/BattleUIDefine.h"

using namespace std;

/**
 * @file TurnEndButton.h
 * @brief ^[I¹{^
 */

namespace Entity
{
	class TurnEndButton : public TextButton
	{

	public:
		TurnEndButton();
		~TurnEndButton() {};

		constexpr static int WIDTH = 180; //! 
		constexpr static int MARGIN = 10; //! ]
		constexpr static int HEIGHT = BATTLE_UI_AREA_H - MARGIN * 2;

		constexpr static int RECT_ROUND = 15; //! pÛÌl
		constexpr static int RECT_ROUND_POS_NUM = 16; //! pÛÌpos_numl

		void render() const override;

		void show();
		void hide();

	private:

	};

	
}
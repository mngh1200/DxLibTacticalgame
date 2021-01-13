#pragma once
#include <climits>
#include <cmath>
#include "Entity/Unit/Unit.h"
#include "Entity/UI/Button/TurnEndButton.h"
#include "Utility/ResourceManager.h"
#include "Battle/BUI/BattleUIDefine.h"

using namespace std;

/**
 * @file EnemySpeedController.h
 * @brief 敵ターン進行速度調整ボタン
 */

namespace Entity
{
	class EnemySpeedController : public Object
	{

	public:
		EnemySpeedController();
		~EnemySpeedController() {};

		void render() const override;
		void onMouseClick(int x, int y) override;

		void show();
		void hide();

	private:
		constexpr static int PADDING_X = 22; //! ボタンの余白 横
		constexpr static int PADDING_Y = 16; //! ボタンの余白 縦
		constexpr static int BUTTON_H = BATTLE_UI_AREA_H - (TurnEndButton::MARGIN + PADDING_Y) * 2; //! ボタンのサイズ
		constexpr static int BUTTON_W = BUTTON_H + 10;
		constexpr static int BUTTON_RELATIVE_XN = (TurnEndButton::WIDTH - PADDING_X * 2 - BUTTON_W) / 2; //! N番目のボタンの相対X座標

		const static float PLAYMARK_W; //! 再生マークの幅

		//! 進行速度
		enum Speed
		{
			SLOW,
			NORMAL,
			FAST,
			LEN
		};

		//! 進行速度
		int speed_;

		//! 敵ターン終了時に進行速度を保持しておく
		int cacheSpeed_;

		void changeSpeed(int speed);

		//! 各ボタンの座標とサイズ
		Shape buttonsShape_[Speed::LEN];
	};

	
}
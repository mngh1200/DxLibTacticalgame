#pragma once
#include <climits>
#include <cmath>
#include "Entity/Unit/Unit.h"
#include "Entity/UI/Button/BuiConfirmButton.h"
#include "Utility/ResourceManager.h"
#include "Battle/BUI/BattleUIDefine.h"
#include "Animation/Animation.h"

using namespace std;

namespace Battle
{
	class EnemyBattleController;
}

/**
 * @file EnemyTurnCont.h
 * @brief 敵ターン中にバトルUIの右下に設置するオブジェクト（進行速度調整や状況テキスト）
 */

namespace Entity
{
	class EnemyTurnCont : public Object
	{

	public:
		EnemyTurnCont();
		~EnemyTurnCont() {};

		void render() const override;
		void onMouseClick(int x, int y) override;

		void show();
		void hide();

		void setMode(int mode);
		
		bool animationUpdate() override;

		// モードの種類
		enum Mode
		{
			SPEED_CONT, //! スピード調整
			ENEMY_TURN_MESSAGE, //! 敵ターン中である表記
			ENEMY_SET_MESSAGE	//! 敵の自由配置中である表記
		};

		//! 敵ターン中のアニメーション時間倍率
		static float animationRate;

		static int getAnimationMs(int ms);

	protected:
		bool createAnimation(int animationId);
	private:
		constexpr static int PADDING_X = 22; //! ボタンの余白 横
		constexpr static int PADDING_Y = 16; //! ボタンの余白 縦
		constexpr static int BUTTON_H = BATTLE_UI_AREA_H - (BuiConfirmButton::MARGIN + PADDING_Y) * 2; //! ボタンのサイズ
		constexpr static int BUTTON_W = BUTTON_H + 10;
		constexpr static int BUTTON_RELATIVE_XN = (BuiConfirmButton::WIDTH - PADDING_X * 2 - BUTTON_W) / 2; //! N番目のボタンの相対X座標

		const static float PLAYMARK_W; //! 再生マークの幅

		constexpr static int FONT_TYPE = FontType::NORMAL_S18; // フォントタイプ
		constexpr static int TEXT_COLOR_TYPE = ColorType::MAIN_COLOR; // テキストカラータイプ

		constexpr static int TEXT_MARGIN_LEFT = 10; //! テキストの余白

		const static string ENEMY_TURN_TEXT;	//! 敵ターン中である表記テキスト
		const static string ENEMY_SET_TEXT;		//! 敵の自由配置中である表記テキスト

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

		//! モード
		int mode_;

		//! テキストの相対Y座標
		int textRelationY_;

		//! テキストの不透明度
		int textAlpha_;

		//! アニメーション
		Animation animation_;
	};

	
}
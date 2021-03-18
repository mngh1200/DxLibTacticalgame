#pragma once
/**
 * @file BattleDefine.h
 * @brief バトル関連の定数
 */

 //! 画像チップのサイズ
constexpr int CHIP_SIZE = 64;

//! マップのマスの個数
constexpr int MAP_MASS_W = 20;
constexpr int MAP_MASS_H = 10;

 //! マップ表示位置
constexpr int MAP_Y = 0;

//! ダメージエフェクトの画像枚数
constexpr int DMAGE_EFFECT_LEN = 10; 
constexpr int DMAGE_EFFECT_W = 26;
constexpr int DMAGE_EFFECT_H = 32;

//! 各方向におけるxの変化量
constexpr int dx[4] = { 0, -1, 0, 1 };

//! 各方向におけるxの変化量
constexpr int dy[4] = { -1, 0, 1, 0 };

namespace
{
	// 兵種管理
	enum UnitKey {
		FREE = -1,	//! 自由設置
		LANCER,		//! 槍兵
		CAVALRY,	//! 騎兵
		GUNNER,		//! 銃兵
		UNIT_KIND_LEN
	};

	// 方向 (複数方向の判定を可能にするため２進数で定義)
	enum Direction {
		NONE_DIRECTION = 0b0000,
		TOP = 0b0001,
		RIGHT = 0b0010,
		BOTTOM = 0b0100,
		LEFT = 0b1000
	};

	// 操作の種類
	enum ActionKind
	{
		NO_ACTION,
		MOVE_ACT,
		WAIT_ACT,
		ATACK_ACT,
		SET_ACT
	};
}




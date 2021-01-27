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

namespace
{
	// 兵種管理
	enum UnitKey {
		LANCER,
		CAVALRY,
		GUNNER,
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
}




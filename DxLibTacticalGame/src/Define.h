#pragma once

/**
 * @file Define.h
 * @brief 共通定数を保持
 */

//! ウィンドウ解像度(幅)
constexpr int WIN_W = 1280;

//! ウィンドウ解像度（高さ）
constexpr int WIN_H = 720;

//! マウスのクリック時のイベントタイプ
constexpr int MOUSE_INPUT_LOG_CLICK = 2;
constexpr int MOUSE_INPUT_LOG_USED = 3; // すでに他のイベントで使用した場合

//! 透過度の最大値
constexpr int MAX_ALPHA = 255;

//! ステージ数
constexpr int MAX_STAGE = 10;

//! チュートリアルステージ数
constexpr int TUTORIAL_STAGE_NUM = 6;

namespace
{
	enum StageRank : int {
		LOCK,  //! 未開放ステージ
		NEW,   //! 新規ステージ
		NONE,  //! ランクなし
		CLEAR, //! クリア済み
		NULL_STAGE,  //! 存在しないステージ
		RANK_LEN
	};
}




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

//! 透過度の最大値
constexpr int MAX_ALPHA = 255;

//! 画像チップのサイズ
constexpr int CHIP_SIZE = 64;

//! マップのマスの個数
constexpr int MAP_MASS_W = 20;
constexpr int MAP_MASS_H = 10;

//! マップ表示位置
constexpr int MAP_Y = WIN_H - MAP_MASS_H * CHIP_SIZE;

//! マップ画面のUIエリア
constexpr int BATTLE_UI_AREA_Y = 0;
constexpr int BATTLE_UI_AREA_H = MAP_Y - BATTLE_UI_AREA_Y;

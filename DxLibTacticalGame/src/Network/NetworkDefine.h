#pragma once

/**
 * @file NetworkDefine.h
 * @brief ネットワーク関連の共通定数を保持
 */

//! ポート番号
constexpr int PORT = 9850;

//! データの区切り
constexpr char DATA_SPLIT = ';';

//! データ中の値の区切り
constexpr char VALUE_SPLIT = ',';

namespace Network
{
	enum NetworkDataType
	{
		NONE,		//! なし
		RULE,		//! ルール設定
		CONT_LOG	//! 操作ログ
	};

	// ルールデータ
	struct RuleData
	{
		int unitNum = 6;	//! ユニット数
		int mapId = 0;	//! マップID
	};

	// バトルデータ
	struct ContLog
	{
		int x = 0;			//! 対象x座標
		int y = 0;			//! 対象y座標
		int unitId = 0;		//! ユニットID
		int actionKind = -1;	//! 行動の種類
		int hitValue = 0;	//! 命中の乱数値
	};
}




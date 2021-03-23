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
		SIGNAL,		//! 単一の数値情報送受信用（ターン終了や配置完了等）
		RULE,		//! ルール設定
		CONT_LOG	//! 操作ログ
	};

	// 単一情報の種類
	enum SignalKind
	{
		NO_SIGNAL,		//! なし
		TURN_END,		//! ターン終了
		SET_END,		//! 自由配置完了
		CLIENT_FIRST	//! クライアント側が先攻
	};

	// ルールデータ
	struct RuleData
	{
		int unitNum = 6;	//! ユニット数
		int mapId = 0;	//! マップID
	};

	// プレイヤー操作ログ
	struct ContLog
	{
		int x = 0;			//! 対象x座標
		int y = 0;			//! 対象y座標
		int unitId = 0;		//! ユニットID
		int actionKind = -1;//! 行動の種類 (BattleDefine.hのActionKindで定義)
		int extraValue = 0;	//! その他の変数値（乱数による命中値やユニット配置時の兵種）
	};
}




#pragma once
#include <climits>
#include "Ability.h"
#include "Define.h"
#include "Battle/BattleDefine.h"

using namespace std;

/**
 * @file UnitInfo.h
 * @brief ユニットのステータス管理クラス
 */

namespace Entity
{
	struct UnitInfo
	{
		//! 名前
		string name = "";

		//! ユニット種類
		int kind = UnitKey::LANCER;

		/* 各種ステータス */
		int hp = 0;  //! 現在HP
		int hpm = 0; //! 最大HP
		int atk = 0; //! 攻撃
		int def = 0; //! 防御
		int mov = 0; //! 移動
		int len = 0; //! 射程
		int range = 1; //! マップ上の攻撃範囲

		//! 近接攻撃をするか
		bool isCloseAtack = true;

		//! スキル
		Ability ability = Ability::Kind::NONE;

		//! 射程を示すテキスト
		const static vector<string> LEN_TEXT; 

		string getLenText() const;
	};
}
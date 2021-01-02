#pragma once
#include <climits>
#include <map>
#include "Entity/Battle/Mass.h"
#include "Entity/Unit/Unit.h"

using namespace std;
using namespace Entity;

/**
 * @file Fight.h
 * @brief 個人間の戦闘を管理するクラス
 */

namespace Battle
{
	// 攻撃側、防御側どちらかの戦闘予測データ
	struct FightData
	{
		shared_ptr<Unit> unit;	//! 対称ユニット
		int damage;				//! 与ダメージ
		int hitRate;			//! 命中率
		bool isAtk;				//! 攻撃するか
	};

	class Fight
	{
	public:
		Fight() : 
			actSide_{nullptr, 0, 0, true},
			psvSide_{nullptr, 0, 0, false},
			isActSideFirst_(true),
			phase_(Phase::NO_FIGHT)
		{};
		~Fight() {};

		void init(shared_ptr<Map> map);

		bool setPrepare(shared_ptr<Unit> actUnit, shared_ptr<Unit> psvUnit);
		void reset();

		void start();
		bool checkUpdate();

		FightData getFightData(bool isPlayer) const;

		bool isPrepared() const;

		bool isActSideFirst() const { return isActSideFirst_; }; // 攻撃実行側が先攻であるか

	private:
		void makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, shared_ptr<Mass> mass, bool isAct);
		bool atack(bool isActSideAtack);

		//! マップの参照
		shared_ptr<Map> map_;

		//! 攻撃側データ
		FightData actSide_;

		//! 防御側データ
		FightData psvSide_;

		//! 攻撃実行側が先攻であるか
		bool isActSideFirst_;

		//! フェイズ
		enum class Phase
		{
			NO_FIGHT,
			FIRST_ATK,
			SECOND_ATK,
		};

		//! フェイズ
		Phase phase_;
	};
}
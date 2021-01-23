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
		bool isCloseAttack;		//! 近接攻撃であるか
		int direction = Direction::NONE; //! 攻撃方向
		vector<string> extraEffects; //! 特殊効果（アビリティや連携等）
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
		bool setPrepare(shared_ptr<Unit> actUnit, int actMassX, int actMassY, shared_ptr<Unit> psvUnit);
		void reset();

		void start();
		bool checkUpdate();

		const FightData& getFightData(bool isAct) const;

		bool isPrepared() const;

		bool isActSideFirst() const { return isActSideFirst_; }; // 攻撃実行側が先攻であるか

	private:
		int getCoordinatedAttack(int atkDirection, int atkedLogs);
		void makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, shared_ptr<Mass> mass, bool isAct, int coordinatedAttack);
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

		// 連携、挟撃判定用
		enum CoordinatedAttack
		{
			NONE,
			COORDINATED,// 連携
			PINCHING	// 挟撃
		};
	};
}
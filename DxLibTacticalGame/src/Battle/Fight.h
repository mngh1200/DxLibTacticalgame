#pragma once
#include <climits>
#include <map>
#include "Entity/Battle/Mass.h"
#include "Entity/Unit/Unit.h"

using namespace std;
using namespace Entity;

/**
 * @file Fight.h
 * @brief �l�Ԃ̐퓬���Ǘ�����N���X
 */

namespace Battle
{
	// �U�����A�h�䑤�ǂ��炩�̐퓬�\���f�[�^
	struct FightData
	{
		shared_ptr<Unit> unit;	//! �Ώ̃��j�b�g
		int damage;				//! �^�_���[�W
		int hitRate;			//! ������
		bool isAtk;				//! �U�����邩
		bool isCloseAttack;		//! �ߐڍU���ł��邩
		int direction = Direction::NONE; //! �U������
		vector<string> extraEffects; //! ������ʁi�A�r���e�B��A�g���j
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

		bool isActSideFirst() const { return isActSideFirst_; }; // �U�����s������U�ł��邩

	private:
		int getCoordinatedAttack(int atkDirection, int atkedLogs);
		void makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, shared_ptr<Mass> mass, bool isAct, int coordinatedAttack);
		bool atack(bool isActSideAtack);

		//! �}�b�v�̎Q��
		shared_ptr<Map> map_;

		//! �U�����f�[�^
		FightData actSide_;

		//! �h�䑤�f�[�^
		FightData psvSide_;

		//! �U�����s������U�ł��邩
		bool isActSideFirst_;

		//! �t�F�C�Y
		enum class Phase
		{
			NO_FIGHT,
			FIRST_ATK,
			SECOND_ATK,
		};

		//! �t�F�C�Y
		Phase phase_;

		// �A�g�A��������p
		enum CoordinatedAttack
		{
			NONE,
			COORDINATED,// �A�g
			PINCHING	// ����
		};
	};
}
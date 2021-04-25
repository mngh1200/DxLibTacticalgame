#pragma once
#include <climits>
#include <map>
#include "Entity/Battle/Mass.h"
#include "Entity/Unit/Gunner.h"
#include "Entity/Effect/ExtraEffect.h"

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
		// �U���̖�����
		enum HitState
		{
			UNSETTLED,	//! ���m��
			HITTED,		//! ����
			MISS,		//! ������ꂽ
		};

		shared_ptr<Unit> unit;	//! �Ώ̃��j�b�g
		bool isFirst;			//! ��s�ł��邩
		bool isAtk = false;		//! �U�����邩
		int damage = 0;				//! �^�_���[�W
		int hitRate = 0;			//! ������
		int hitState = HitState::UNSETTLED; //! �U���̖�����
		bool isCloseAttack = true;		//! �ߐڍU���ł��邩
		int direction = Direction::NONE_DIRECTION; //! �U������
		vector<string> extraEffects; //! ������ʁi�A�r���e�B��A�g���j
	};

	class Fight
	{
	public:
		Fight() : 
			actSide_{nullptr, 0, 0, true},
			psvSide_{nullptr, 0, 0, false},
			phase_(Phase::NO_FIGHT),
			extraEffectList_{}
		{};
		~Fight() {};

		void init(shared_ptr<Map> map);

		bool setPrepare(shared_ptr<Unit> actUnit, shared_ptr<Unit> psvUnit);
		bool setPrepare(shared_ptr<Unit> actUnit, int actMassX, int actMassY, shared_ptr<Unit> psvUnit);
		void reset();

		void setHitState(int actHitState, int psvHitState);

		void start();
		bool checkUpdate();

		void showPredictUnitDamage();
		void hidePredictUnitDamage();

		const FightData& getFightData(bool isAct) const;

		bool isPrepared() const;

		bool isActSideFirst() const { return actSide_.isFirst; }; // �U�����s������U�ł��邩

	private:
		int getCoordinatedAttack(int atkDirection, int atkedLogs);
		void makeFightData(FightData* fightData, shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, shared_ptr<Mass> mass, bool isAct, int coordinatedAttack);
		void attack(FightData* atkSide, FightData* defSide);
		bool getAttackState(bool isActSideAtack, FightData& atkSide, FightData& defSide);
		void showExtraEffect(FightData* atkSide, FightData* defSide);
		void clearExtraEffect();

		//! �}�b�v�̎Q��
		shared_ptr<Map> map_;

		//! �U�����f�[�^
		FightData actSide_;

		//! �h�䑤�f�[�^
		FightData psvSide_;

		//! ������ʕ\���v�f�̃��X�g
		vector<shared_ptr<ExtraEffect>> extraEffectList_;

		//! �t�F�C�Y
		enum class Phase
		{
			NO_FIGHT,
			FIRST_EFFECT,	//! ��U�̌��ʕ\��
			FIRST_ATK,		//! ��U�̍U��
			SECOND_EFFECT,	//! ��U�̌��ʕ\��
			SECOND_ATK		//! ��U�̍U��
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
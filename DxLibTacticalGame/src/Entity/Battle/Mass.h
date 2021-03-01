#pragma once
#include <climits>
#include <memory>
#include <string>
#include "Utility/ResourceManager.h"

using namespace std;

namespace Entity
{
	class Map;
}

/**
 * @file Mass.h
 * @brief �}�X�f�[�^
 */

namespace Entity
{
	class Mass
	{
	public:
		Mass() : 
			kindId_(Kind::OUT_OF_MAP),
			state(State::NORMAL),
			passingMov(-1),
			imageId_(-1),
			ableUnitSet(false),
			isAttacked(false),
			angle_(0.0) {};
		Mass(int kindId);
		~Mass() {};

		// �}�X���
		enum Kind
		{
			PLAIN,		//! ����
			FOREST,		//! �X
			RIVER,		//! ��
			MOUNTAIN,	//! �R
			FORT_PLAYER,//! �ԁi���R�j
			FORT_ENEMY, //! �ԁi�G�R�j
			LEN,
			OUT_OF_MAP	//! �͈͊O
		};

		void init(int adjacent);

		int getCost() const;
		int getAgl() const;
		int getDef() const;

		int getKind() const;
		int getImageId() const;
		double getAngle() const { return angle_; }; // �摜�̉�]�ʂ�Ԃ�

		string getText(int line) const;

		bool isMovable() const { return state == MOVABLE; }

		//! �ʉߎ���mov�l
		int passingMov;

		//! �}�X�̏�
		int state;

		//! ���j�b�g�z�u�\�ł��邩
		bool ableUnitSet;

		// �}�X�̏�
		enum State
		{
			NORMAL,
			MOVABLE,	//! �ړ��\
			ATK_ABLE	//! �U���\
		};

		//! �G�̍U���͈͓��ł��邩
		bool isAttacked;

	private:
		void setImageIdFromKind(int kindId, int adjacent);

		static const vector<int> COST;		//! �ړ��R�X�g
		static const vector<int> AGL_UP;	//! ���UP
		static const vector<int> DEF_UP;    //! �h��UP
		static const vector<string> TEXT1;  //! �n�`���ʐ�����

		//! �n�`���
		int kindId_;

		//! �摜
		int imageId_;

		//! �摜�̉�]
		double angle_;
	};
}
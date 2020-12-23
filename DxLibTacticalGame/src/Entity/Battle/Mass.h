#pragma once
#include <climits>
#include <memory>
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
		Mass() : kindId_(OUT_OF_MAP), state(State::NORMAL), passingMov(-1), imageId_(-1) {};
		Mass(int kindId);
		~Mass() {};

		// �}�X���
		enum Kind
		{
			PLAIN,		// ����
			FOREST,		// �X
			RIVER,		// ��
			MOUNTAIN,	// �R
			LEN,
			OUT_OF_MAP	// �͈͊O
		};

		int getCost() const;
		int getAgl() const;
		int getDef() const;

		int getKind() const;
		int getImageId() const;

		bool isMovable() const { return passingMov >= 0; }

		//! �ʉߎ���mov�l
		int passingMov;

		//! �}�X�̏�
		int state;

		// �}�X�̏�
		enum State
		{
			NORMAL,
			MOVABLE,
			ATK_ABLE
		};

	private:
		void setImageIdFromKind(int kindId);

		//! �n�`���
		int kindId_;

		//! �摜
		int imageId_;
	};
}
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
		Mass() : kindId_(OUT_OF_MAP), state(State::NORMAL);
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

		//! �n�`����(�����A�X�A��A�R)
		constexpr static int COST[] = {1, 2, 3, INT_MAX}; //! �ړ��R�X�g
		constexpr static int AGL_UP[] = {0, 20, -20, 0};  //! ���UP
		constexpr static int DEF_UP[] = {0,  3,  -3, 0};  //! �h��UP

		int getCost() const;
		int getAgl() const;
		int getDef() const;

		int getKind() const;
		int getImageId() const;

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
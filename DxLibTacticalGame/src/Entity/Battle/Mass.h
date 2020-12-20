#pragma once
#include <climits>
#include <memory>
#include "Utility/ResourceManager.h"

using namespace std;

namespace Entity
{
	class Map;
	class Unit;
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
		Mass() = delete;
		Mass(int kindId);
		~Mass() {};

		// �}�b�v���
		enum Kind
		{
			PLAIN,		// ����
			FOREST,		// �X
			RIVER,		// ��
			MOUNTAIN,	// �R
			LEN
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

		void setUnit(shared_ptr<Entity::Unit> unit);
		void resetUnit();
		shared_ptr<Entity::Unit> getUnit() const;

	private:
		void setImageIdFromKind(int kindId);

		//! �n�`���
		int kindId_;

		//! �摜
		int imageId_;

		//! �}�X�ɂ��郆�j�b�g
		weak_ptr<Entity::Unit> unit_;
	};
}
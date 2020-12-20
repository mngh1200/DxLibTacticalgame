#pragma once
#include <climits>
#include <vector>
#include "Entity/Object.h"
#include "Entity/Battle/Mass.h"
#include "Utility/ResourceManager.h"

using namespace std;

namespace Entity
{
	class Unit;
}

/**
 * @file TextButton.h
 * @brief �e�L�X�g�t���{�^���v�f
 */

namespace Entity
{
	class Map : public Object
	{
	public:
		Map();
		~Map() {};

		void render() const override;

		void setUnit(int x, int y, shared_ptr<Unit> unit);

		void onClickPlayerUnit(int x, int y);

		void onClickMass(int massX, int massY);

		static int getRealX(int massX);
		static int getRealY(int massY);
		static int getMassX(int realX);
		static int getMassY(int realY);

	private:
		bool deselectUnit();

		//! ���̃}�X�T�C�Y
		int w_;

		//! �c�̃}�X�T�C�Y
		int h_;

		//! �}�X�̃f�[�^
		vector<vector<Mass>> mass_;

		//! �I�𒆂̃��j�b�g
		weak_ptr<Entity::Unit> selectedUnit_;
	};


}
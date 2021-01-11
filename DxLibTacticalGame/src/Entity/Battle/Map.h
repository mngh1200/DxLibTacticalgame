#pragma once
#include <climits>
#include <vector>
#include <map>
#include <algorithm>
#include "Entity/Object.h"
#include "Entity/Battle/Mass.h"
#include "Utility/ResourceManager.h"

using namespace std;

namespace Entity
{
	class Unit;
}

/**
 * @file Map.h
 * @brief �퓬��ʂ̃}�b�v(�}�X�ƃ��j�b�g���Ǘ�)
 */

namespace Entity
{
	class Map : public Object
	{
	public:
		Map();
		Map(int);
		~Map() {};

		void render() const override;

		bool setUnit(shared_ptr<Entity::Unit> unit);
		shared_ptr<Unit> getUnit(int massX, int massY);
		void eraseUnit(shared_ptr<Unit> unit);

		map<pair<int, int>, shared_ptr<Unit>>::iterator unitsBegin();
		map<pair<int, int>, shared_ptr<Unit>>::iterator unitsEnd();

		void confirmMove(shared_ptr<Unit> unit);

		void displayMovableRange(shared_ptr<Unit> unit);
		void displayAtackRange(shared_ptr<Unit> unit, bool isExistEnemyOnly = false);
		

		shared_ptr<Mass> getMass(int massX, int massY);
		void clearMassState();

		bool isRange(int x, int y) const;

		static void drawMoveableMass(int x, int y);
		static void drawAtackMass(int x, int y);
		static void drawHoverMass(int x, int y);
		static void drawSelectedMass(int x, int y);

		static int getRealX(int massX);
		static int getRealY(int massY);
		static int getMassX(int realX);
		static int getMassY(int realY);

		static int getMassDistance(int x0, int y0, int x1, int  y1);

	private:
		void searchMovableMass(shared_ptr<Unit> unit, int x, int y, int move, bool isInit = true, bool isAtackAbleArea = false);
		void displayAtackRange(shared_ptr<Unit> unit, int x, int y, bool isExistEnemyOnly = false);
		void setAtackMass(int x, int y, bool isExistEnemyOnly = false);

		constexpr static int MASS_EFFECT_ALPHA = 50; //! �}�X�̌��ʐF�̕s�����x

		//! �}�X�̃f�[�^
		vector<vector<shared_ptr<Mass>>> mass_;

		//! �}�X��XY���W����ɂ������j�b�g�̃}�b�v
		map<pair<int, int>, shared_ptr<Entity::Unit>> units_;
	};


}
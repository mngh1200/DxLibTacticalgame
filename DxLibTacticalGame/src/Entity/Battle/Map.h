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
	class Gunner;
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
		~Map() {};

		void loadStageData(std::array<std::array<int, MAP_MASS_W>, MAP_MASS_H>& mapData);
		void loadUnits(vector<vector<int>>& units);

		void render() const override;

		void setUnit(int massX, int massY, int kind, bool isEnemy = false);
		shared_ptr<Unit> getUnit(int massX, int massY);
		void eraseUnit(shared_ptr<Unit> unit);

		vector<vector<shared_ptr<Mass>>>::iterator massBegin();
		vector<vector<shared_ptr<Mass>>>::iterator massEnd();
		map<pair<int, int>, shared_ptr<Unit>>::iterator unitsBegin();
		map<pair<int, int>, shared_ptr<Unit>>::iterator unitsEnd();

		void confirmMove(shared_ptr<Unit> unit);

		void displayMovableRange(shared_ptr<Unit> unit);
		void getMovableMasses(shared_ptr<Unit> unit, int x, int y, int move, vector<pair<int, int>> &posList, bool isInit = true);

		void displayAtackAbleRange(shared_ptr<Unit> unit, int x, int y, bool isExistEnemyOnly = false);
		void getAtackAbleMasses(shared_ptr<Unit> unit, int x, int y, vector<pair<int, int>>& posList, bool isExistEnemyOnly = false);
		

		shared_ptr<Mass> getMass(int massX, int massY);
		void clearMassState();
		void clearMassUnitSet();

		bool isRange(int x, int y) const;
		pair<int, int> getPlayerFortMass() const { return playerFortMass_; }; // �v���C���[�̍ԃ}�X��Ԃ�
		pair<int, int> getEnemyFortMass() const { return enemyFortMass_; }; // �G�R�̍ԃ}�X��Ԃ�

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
		void getAtackAbleMass(shared_ptr<Unit> unit, int x, int y, vector<pair<int, int>>& posList, bool isExistEnemyOnly = false);

		constexpr static int MASS_EFFECT_ALPHA = 50; //! �}�X�̌��ʐF�̕s�����x

		//! �}�X�̃f�[�^
		vector<vector<shared_ptr<Mass>>> mass_;

		//! �}�X��XY���W����ɂ������j�b�g�̃}�b�v
		map<pair<int, int>, shared_ptr<Entity::Unit>> units_;

		//! �G���A�������̍ԃ}�X�̍��W
		pair<int, int> playerFortMass_, enemyFortMass_;
	};


}
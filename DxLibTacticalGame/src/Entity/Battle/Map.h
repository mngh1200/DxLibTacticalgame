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
		~Map() {};

		void render() const override;

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

	private:
		constexpr static int MASS_EFFECT_ALPHA = 50; //! �}�X�̌��ʐF�̕s�����x

		//! �}�X�̃f�[�^
		vector<vector<shared_ptr<Mass>>> mass_;
	};


}
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

		bool isRange(int x, int y) const;

		static int getRealX(int massX);
		static int getRealY(int massY);
		static int getMassX(int realX);
		static int getMassY(int realY);

	private:

		//! ���̃}�X�T�C�Y
		int w_;

		//! �c�̃}�X�T�C�Y
		int h_;

		//! �}�X�̃f�[�^
		vector<vector<Mass>> mass_;
	};


}
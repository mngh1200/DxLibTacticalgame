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
 * @brief 戦闘画面のマップ(マスとユニットを管理)
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

		//! 横のマスサイズ
		int w_;

		//! 縦のマスサイズ
		int h_;

		//! マスのデータ
		vector<vector<Mass>> mass_;
	};


}
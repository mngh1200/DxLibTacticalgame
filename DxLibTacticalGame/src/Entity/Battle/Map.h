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
 * @brief テキスト付きボタン要素
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

		//! 横のマスサイズ
		int w_;

		//! 縦のマスサイズ
		int h_;

		//! マスのデータ
		vector<vector<Mass>> mass_;

		//! 選択中のユニット
		weak_ptr<Entity::Unit> selectedUnit_;
	};


}
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
 * @brief マスデータ
 */

namespace Entity
{
	class Mass
	{
	public:
		Mass() : kindId_(OUT_OF_MAP), state(State::NORMAL);
		Mass(int kindId);
		~Mass() {};

		// マス種類
		enum Kind
		{
			PLAIN,		// 草原
			FOREST,		// 森
			RIVER,		// 川
			MOUNTAIN,	// 山
			LEN,
			OUT_OF_MAP	// 範囲外
		};

		//! 地形効果(草原、森、川、山)
		constexpr static int COST[] = {1, 2, 3, INT_MAX}; //! 移動コスト
		constexpr static int AGL_UP[] = {0, 20, -20, 0};  //! 回避UP
		constexpr static int DEF_UP[] = {0,  3,  -3, 0};  //! 防御UP

		int getCost() const;
		int getAgl() const;
		int getDef() const;

		int getKind() const;
		int getImageId() const;

		//! マスの状況
		int state;

		// マスの状況
		enum State
		{
			NORMAL,
			MOVABLE,
			ATK_ABLE
		};

	private:
		void setImageIdFromKind(int kindId);

		//! 地形種類
		int kindId_;

		//! 画像
		int imageId_;
	};
}
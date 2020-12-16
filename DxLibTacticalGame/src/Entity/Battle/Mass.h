#pragma once
#include <climits>
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
		Mass() = delete;
		Mass(int kindId);
		~Mass() {};

		// マップ種類
		enum Kind
		{
			PLAIN,		// 草原
			FOREST,		// 森
			RIVER,		// 川
			MOUNTAIN,	// 山
			LEN
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

	private:
		void setImageIdFromKind(int kindId);

		//! 地形種類
		int kindId_;

		// 画像
		int imageId_;
	};
}
#pragma once
#include <climits>
#include <memory>
#include <string>
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
		Mass() : kindId_(Kind::OUT_OF_MAP), state(State::NORMAL), passingMov(-1), imageId_(-1) {};
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

		int getCost() const;
		int getAgl() const;
		int getDef() const;

		int getKind() const;
		int getImageId() const;

		string getText(int line) const;

		bool isMovable() const { return passingMov >= 0; }

		//! 通過時のmov値
		int passingMov;

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
		static const vector<int> COST;		//! 移動コスト
		static const vector<int> AGL_UP;	//! 回避UP
		static const vector<int> DEF_UP;    //! 防御UP
		static const vector<string> TEXT1;  //! 地形効果説明文

		void setImageIdFromKind(int kindId);

		//! 地形種類
		int kindId_;

		//! 画像
		int imageId_;
	};
}
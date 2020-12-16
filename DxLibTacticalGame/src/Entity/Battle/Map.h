#pragma once
#include <climits>
#include <vector>
#include "Entity/Object.h"
#include "Entity/Battle/Mass.h"
#include "Utility/FontManager.h"

using namespace std;

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

	private:
		//! 横のマスサイズ
		int w_;

		//! 縦のマスサイズ
		int h_;

		//! マスのデータ
		vector<vector<Mass>> mass_;
	};


}
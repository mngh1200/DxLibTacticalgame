#pragma once
#include <climits>
#include <vector>
#include "Entity/Object.h"
#include "Entity/Battle/Mass.h"
#include "Utility/FontManager.h"

using namespace std;

/**
 * @file TextButton.h
 * @brief �e�L�X�g�t���{�^���v�f
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
		//! ���̃}�X�T�C�Y
		int w_;

		//! �c�̃}�X�T�C�Y
		int h_;

		//! �}�X�̃f�[�^
		vector<vector<Mass>> mass_;
	};


}
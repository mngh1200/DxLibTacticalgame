#pragma once
#include <climits>
#include <string>
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "Battle/Fight.h"
#include "Entity/Unit/Unit.h"
#include "DxLib.h"
#include "Define.h"

using namespace std;
using namespace Battle;

/**
 * @file HitDisplay.h
 * @brief �������\��
 */

namespace Entity
{
	class HitDisplay : public Figure
	{
	public:
		HitDisplay() : isShow_(false), hitRate_(100) {};
		~HitDisplay() {};
		
		void show(Fight* fight);
		void hide();

		void render() const override;

	private:
		constexpr static int MARGIN = 10;	//! �����]��
		constexpr static int PADDING = 5; //! �����]��

		constexpr static int H = 28;		//! ����
		constexpr static int LABEL_W = 35;	//! ���x����
		constexpr static int VALUE_W = 62;	//! �l�\�������̕�

		//! �\�����Ă��邩
		bool isShow_;

		//! �m��
		int hitRate_;
	};
}
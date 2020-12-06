#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Entity/Shape.h"
#include "DxLib.h"
#include "Utility/ResourceManager.h"
#include "Define.h"

/**
 * @file Back.h
 * @brief îwåióvëf
 */

namespace Entity
{
	class Back : public Object
	{
	public:
		Back();
		~Back() {};
		void render() const override;

	private:
		int imageId_;
	};
}
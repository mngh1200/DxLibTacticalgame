#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Entity/Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace Entity
{
	class Button : public Object
	{
	public:
		Button();
		~Button() {};

		void render() const override;

	};
}
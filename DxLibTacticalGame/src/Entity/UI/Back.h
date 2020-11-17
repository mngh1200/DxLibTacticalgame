#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Entity/Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace Entity
{
	class Back : public Object
	{
	public:
		Back();
		Back(Shape shape);
		~Back() {};

		void render() const override;
	private:
		ObjectType objectType_;

	};
}
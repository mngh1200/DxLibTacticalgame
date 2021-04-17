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

		void init(int kind);
		void render() const override;

		// îwåiÇÃéÌóﬁ
		enum BackKind : int
		{
			NORMAL_IMAGE,
			DARK_IMAGE,
			MONO_COLOR,
		};

	private:
		const static int SELECT_LEFT_WIDTH = 800;

		int imageId_; // ÉCÉÅÅ[ÉWID
	};
}
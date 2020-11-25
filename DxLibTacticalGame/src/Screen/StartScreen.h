#pragma once
#include <climits>
#include <memory>
#include "ScreenBase.h"
#include "Entity/UI/Back.h"
#include "Entity/UI/Button.h"
#include "Entity/View/Text.h"

using namespace std;

namespace Screen
{
	class StartScreen : public ScreenBase
	{
	public:
		StartScreen() {};
		~StartScreen() {};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	};
}
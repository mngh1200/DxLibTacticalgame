#include "MenuScreen.h"
#include "SelectScreen.h"

namespace Screen
{
	/**
	 * @fn
	 * 初期処理
	*/
	void SelectScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>(Entity::Back::Screen::SELECT));

		int viewId = 0;
		
		// 左上のテキスト
		objectsControl.addFigure(Layer::UI, ++viewId, make_shared<Entity::Text>("コースセレクト", COURSE_MARGIN_X, PADDING_TOP, ::FontType::NORMAL, ::ColorType::BUTTON));

		// コースタイトル テスト処理
		courseTitle_ = ++viewId;
		objectsControl.addFigure(Layer::UI, courseTitle_, make_shared<Entity::Text>("チュートリアル1", LEFT_AREA_WIDTH + RIGHT_AREA_PADDING_LEFT, RIGHT_AREA_PADDING_TOP, FontType::MAIN_MENU, ColorType::NORMAL_TEXT));

		// コースボタン
		for (int i = 0; i < 13; i++) // テスト処理
		{
			// X座標
			int x = (i % COURSE_COLUMN_NUM) * (Entity::CourseButton::SIZE + COURSE_MARGIN_X) + COURSE_MARGIN_X;
			int y = (i / COURSE_COLUMN_NUM) * (Entity::CourseButton::SIZE + COURSE_MARGIN_Y) + COURSE_TOP;
			int status = Entity::CourseButton::Status::B;

			// テスト処理
			if (i == 2)
			{
				status = Entity::CourseButton::Status::S;
			}
			else if (i == 3)
			{
				status = Entity::CourseButton::Status::A;
			}
			else if (i == 5)
			{
				status = Entity::CourseButton::Status::C;
			}
			else if (i == 12)
			{
				status = Entity::CourseButton::Status::NO_CLEAR;
			}

			objectsControl.addObject(Layer::COURSE_BUTTON, i, make_shared<Entity::CourseButton>(x, y, status));
		}
		
		


		// オーバーレイセット
		createOverlay(true);
	}

	/**
	 * @fn
	 * イベント取得後のスクリーン更新処理
	*/
	void SelectScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();


		if (hitObjSp)
		{
			Entity::ObjectsControl& objCont = FrameWork::Game::getInstance().objectsControl;

			// コースボタンのクリックイベント
			if (hitObjSp->getLayerId() == Layer::COURSE_BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				// 選択中のボタンの解除
				weak_ptr<Entity::Object> prevObjWp = objCont.getObjectWp(Layer::COURSE_BUTTON, selectedCourseId_);
				shared_ptr<Entity::Object> prevObjSp = prevObjWp.lock();
				if (prevObjSp)
				{
					shared_ptr<Entity::CourseButton> prevSelected = dynamic_pointer_cast<Entity::CourseButton>(prevObjSp);
					prevSelected->setSelected(false);
				}

				// 新しい選択中のボタンを有効化
				shared_ptr<Entity::CourseButton> courseBtn = dynamic_pointer_cast<Entity::CourseButton>(hitObjSp);
				courseBtn->setSelected(true);
				selectedCourseId_ = hitObjSp->getObjectId();
			}
		}
	}

	/**
	 * @fn
	 * アニメーション終了後のスクリーン更新処理
	*/
	void SelectScreen::updateByAnimation()
	{
		isOpenOverlayEnded();
	}

}
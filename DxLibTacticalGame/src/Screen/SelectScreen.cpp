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

		int viewId = UIid::UIID_LEN;
		
		// 左上のテキスト
		objectsControl.addFigure(Layer::UI, ++viewId, make_shared<Entity::Text>("コースセレクト", COURSE_MARGIN_X, PADDING_TOP, ::FontType::NORMAL_S24, ::ColorType::MAIN_COLOR));

		// コースタイトル テスト処理
		objectsControl.addFigure(Layer::UI, UIid::COURSE_NAME, make_shared<Entity::Text>("チュートリアル1", LEFT_AREA_WIDTH + RIGHT_AREA_PADDING_LEFT, COURSE_TOP, FontType::NORMAL_S32, ColorType::SUB_COLOR));

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
		
		// スタートボタン
		shared_ptr<Entity::TextButton> startBtn = make_shared<Entity::TextButton>(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR);
		startBtn->setShape(LEFT_AREA_WIDTH + START_MARGIN, START_Y, WIN_W - LEFT_AREA_WIDTH - START_MARGIN * 2, START_HEIGHT);
		startBtn->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_OVER);
		startBtn->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		startBtn->setText("スタート", FontType::BLACK_S48);
		objectsControl.addObject(Layer::UI, UIid::START_BTN, startBtn);

		// 戻るボタン
		shared_ptr<Entity::TextButton> backBtn = make_shared<Entity::TextButton>(NULL, ColorType::SUB_COLOR);
		backBtn->setShape(WIN_W - BACK_SIZE, 0, BACK_SIZE, BACK_SIZE);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		backBtn->setText("×", FontType::NORMAL_S32);
		objectsControl.addObject(Layer::UI, UIid::BACK_BTN, backBtn);


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
			else if (hitObjSp->getLayerId() == Layer::UI && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				// UIボタンクリック
				if (hitObjSp->getObjectId() == UIid::START_BTN)
				{
					// スタートボタン
				}
				else if (hitObjSp->getObjectId() == UIid::BACK_BTN)
				{
					// 戻るボタン　メインメニューに戻る
					openScreen_ = Screen::MAIN_MENU;
					createOverlay(false);
				}

			}
		}
	}

	/**
	 * @fn
	 * アニメーション終了後のスクリーン更新処理
	*/
	void SelectScreen::updateByAnimation()
	{
		isOpenOverlayEnded(); // オーバーレイ（open）終了判定用

		if (isCloseOverlayEnded()) // オーバーレイ（close）終了判定用
		{
			if (openScreen_ == Screen::MAIN_MENU)
			{
				// メインメニューに遷移
				FrameWork::Game::getInstance().setScreen(new MenuScreen());
			}
		}
	}

}
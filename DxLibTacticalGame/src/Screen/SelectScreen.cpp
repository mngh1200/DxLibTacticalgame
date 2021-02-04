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

		Utility::SaveManager& saveManager = Utility::SaveManager::getInstance();

		objectsControl.setLayer(Layer::LEN);
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>(Entity::Back::Screen::SELECT));

		int viewId = UIid::UIID_LEN;
		
		// 左上のテキスト
		objectsControl.addFigure(Layer::UI, ++viewId, make_shared<Entity::Text>("コースセレクト", COURSE_MARGIN_X, PADDING_TOP, ::FontType::NORMAL_S24, ::ColorType::MAIN_COLOR));

		// コースタイトル
		stageTitle_ = make_shared<Entity::Text>("チュートリアル1", LEFT_AREA_WIDTH + RIGHT_AREA_PADDING_LEFT, COURSE_TOP, FontType::NORMAL_S32, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, UIid::COURSE_NAME, stageTitle_);

		// コースボタン

		for (int i = 0; i < MAX_STAGE; i++) // テスト処理
		{
			// X座標
			int x = (i % COURSE_COLUMN_NUM) * (Entity::CourseButton::SIZE + COURSE_MARGIN_X) + COURSE_MARGIN_X;
			int y = (i / COURSE_COLUMN_NUM) * (Entity::CourseButton::SIZE + COURSE_MARGIN_Y) + COURSE_TOP;
			int status = saveManager.getRank(i);

			if (status == StageRank::NEW) // 新コース
			{
				selectedCourseId_ = newCourseId_ = i;
				saveManager.updateRank(i, StageRank::NONE);
				saveManager.save();
			}
			else if (status == StageRank::NONE) // 未クリアコース
			{
				selectedCourseId_ = i;
			}
			else if (status == StageRank::LOCK) // 非公開コース
			{
				continue;
			}

			objectsControl.addObject(Layer::COURSE_BUTTON, i, make_shared<Entity::CourseButton>(x, y, status, i == newCourseId_));
		}
		
		// スタートボタン
		shared_ptr<Entity::TextButton> startBtn = make_shared<Entity::TextButton>(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR);
		startBtn->setShape(LEFT_AREA_WIDTH + START_MARGIN, START_Y, WIN_W - LEFT_AREA_WIDTH - START_MARGIN * 2, START_HEIGHT);
		startBtn->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_OVER);
		startBtn->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		startBtn->setText("スタート", FontType::BLACK_S48);
		objectsControl.addObject(Layer::UI, UIid::START_BTN, startBtn);

		// 戻るボタン
		shared_ptr<Entity::TextButton> backBtn = make_shared<Entity::TextButton>(ColorType::MAIN_COLOR, ColorType::SUB_COLOR);
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
	 * @param (hitObjWp) 接触オブジェクトの弱参照
	 * @param (x) マウスのx座標
	 * @param (y) マウスのy座標
	 * @param (button) ボタンの種類
	 * @param (eventType) イベントの種類（マウスダウン or マウスアップ or マウスクリック）
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

				updateStageInfo();
			}
			else if (hitObjSp->getLayerId() == Layer::UI && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				// UIボタンクリック
				if (hitObjSp->getObjectId() == UIid::START_BTN)
				{
					// スタートボタン
					openScreen_ = Screen::BATTLE;
					createOverlay(false);

					// サウンド
					Utility::ResourceManager::playSound(SoundKind::CLICK);
				}
				else if (hitObjSp->getObjectId() == UIid::BACK_BTN)
				{
					// 戻るボタン　メインメニューに戻る
					openScreen_ = Screen::MAIN_MENU;
					createOverlay(false);

					// サウンド
					// Utility::ResourceManager::playSound(SoundKind::BACK);
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
		FrameWork::Game& game = FrameWork::Game::getInstance();

		if (isOpenOverlayEnded()) // オーバーレイ（open）終了判定用
		{
			// NEWコース表示アニメーションシーンセット
			if (newCourseId_ != -1)
			{
				nowScene_ = Scene::BORN;
				game.setScreenLock(true);
				game.objectsControl.addAnimationObj(Entity::CourseButton::AnimationId::BORN, Layer::COURSE_BUTTON, newCourseId_);
			}
			else if (selectedCourseId_ != -1)
			{
				nowScene_ = Scene::SELECT;

				// 未クリアコースを選択
				shared_ptr<Entity::CourseButton> courseBtn = dynamic_pointer_cast<Entity::CourseButton>(game.objectsControl.getObjectWp(Layer::COURSE_BUTTON, selectedCourseId_).lock());
				courseBtn->setSelected(true);
			}
		}
		else if (isCloseOverlayEnded()) // オーバーレイ（close）終了判定用
		{
			if (openScreen_ == Screen::MAIN_MENU)
			{
				// メインメニューに遷移
				FrameWork::Game::getInstance().setScreen(new MenuScreen());
			}
			else if (openScreen_ == Screen::BATTLE)
			{
				// バトル画面に遷移
				BattleScreen* battleScreen = new BattleScreen();
				battleScreen->setStage(selectedCourseId_);
				FrameWork::Game::getInstance().setScreen(battleScreen);
			}
		}
		else if (nowScene_ == Scene::BORN)
		{
			shared_ptr<Entity::Object> obj = game.objectsControl.getObjectWp(Layer::COURSE_BUTTON, newCourseId_).lock();
			if (!obj || obj->isAnimation() == false) // 終了
			{
				nowScene_ = Scene::SELECT;
				game.setScreenLock(false);
			}
		}
	}

	/**
	 * @fn
	 * 選択ステージ情報更新
	*/
	void SelectScreen::updateStageInfo()
	{
		// ステージデータ読み込み
		string title;
		string hint;
		std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H > mapData;
		vector<vector<int>> units;
		vector<int> checkWinData;
		Utility::ResourceManager::loadStageData(stageKind_, selectedCourseId_, &title, &hint, &mapData, &checkWinData, &units);

		stageTitle_->setText(title);
	}

}
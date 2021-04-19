#include "MenuScreen.h"
#include "SelectScreen.h"

namespace Screen
{
	const char* SelectScreen::STAGE_KIND = "stage";

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

		// 背景
		shared_ptr<Back> back = make_shared<Entity::Back>();
		back->init(Back::BackKind::DARK_IMAGE);
		objectsControl.addObject(Layer::BACK, 0, back);
		
		// タイトル
		objectsControl.addFigure(Layer::UI, 
			make_shared<Entity::Text>("ステージ選択", PADDING, PADDING, (int)FontType::NORMAL_S32, (int)ColorType::SUB_COLOR));

		// タイトルの下線
		shared_ptr<Panel> underLine = make_shared<Panel>();
		underLine->setShape(PADDING, CONTENT_TOP - UNDER_LINE_MARGIN, 500, UNDER_BAR_HEIGHT);
		underLine->setColor(ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, underLine);


		/* ステージ選択欄 ここから */


		// ステージ選択部分のパネル
		/*
		shared_ptr<Panel> stageListPanel = make_shared<Panel>();
		stageListPanel->setShape(PADDING, CONTENT_TOP, STAGE_LIST_W, STAGE_LIST_H);
		stageListPanel->setColor(ColorType::MAIN_COLOR);
		objectsControl.addFigure(Layer::PANEL, stageListPanel);
		*/

		// ステージボタン

		for (int i = 0; i < MAX_STAGE; i++)
		{
			int status = saveManager.getRank(i);
			int buttonState = CourseButton::State::NORMAL;

			if (status == StageRank::NEW) // 新コース
			{
				buttonState = CourseButton::State::NEW;
				selectedCourseId_ = newCourseId_ = i;
				saveManager.updateRank(i, StageRank::NONE); // 一度セレクト画面を開いた後は未クリアステージ化
				saveManager.save();
			}
			else if (status == StageRank::CLEAR)
			{
				buttonState = CourseButton::State::CLEAR;
			}
			else if (status == StageRank::NONE && selectedCourseId_ == -1) // 未クリアコース
			{
				selectedCourseId_ = i;
			}
			else if (status == StageRank::LOCK) // 非公開コース
			{
				continue;
			}

			// ステージタイトル取得
			string title;
			Utility::ResourceManager::loadStageTitle(STAGE_KIND, i, &title);

			// Y座標
			int y = CONTENT_TOP/* + STAGE_LIST_PADDING*/ + i * (STAGE_BUTTON_H + STAGE_BUTTON_MARGIN);

			objectsControl.addObject(Layer::COURSE_BUTTON, i, 
				make_shared<Entity::CourseButton>(PADDING/* + STAGE_LIST_PADDING*/, y,
					STAGE_LIST_W/* - STAGE_LIST_PADDING * 2*/, STAGE_BUTTON_H, title.c_str(), buttonState));
		}

		if (selectedCourseId_ == -1)
		{
			selectedCourseId_ = MAX_STAGE - 1;
		}
	

		/* ステージ選択欄 ここまで */



		/* ステージ情報欄 ここから */


		// ステージ情報欄パネル
		shared_ptr<Panel> stageInfoPanel = make_shared<Panel>();
		stageInfoPanel->setShape(STAGE_INFO_X, CONTENT_TOP, STAGE_INFO_W, STAGE_INFO_H);
		stageInfoPanel->setColor(ColorType::MAIN_COLOR);
		objectsControl.addFigure(Layer::PANEL, stageInfoPanel);

		// ステージ情報欄タイトル
		shared_ptr<Text> stageInfoTitle = make_shared<Text>("ステージ情報", STAGE_INFO_X, CONTENT_TOP, FontType::NORMAL_S20, ColorType::MAIN_COLOR);
		stageInfoTitle->setBackgroundColor(ColorType::SUB_COLOR);
		stageInfoTitle->setPadding(STAGE_INFO_HEAD_V_PADDING, STAGE_INFO_PADDING);
		stageInfoTitle->setW(STAGE_INFO_W);
		objectsControl.addFigure(Layer::UI, stageInfoTitle);

		// ステージタイトル
		stageTitle_ = make_shared<Entity::Text>("", INFO_TEXT_X, INFO_TEXT_Y,
			FontType::NORMAL_S20, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, stageTitle_);

		// ステージタイトルの下線
		shared_ptr<Panel> titleUnderLine = make_shared<Panel>();
		titleUnderLine->setShape(INFO_TEXT_X, INFO_TEXT_Y + LINE_HEIGHT, STAGE_INFO_W - STAGE_INFO_PADDING * 2, UNDER_BAR_HEIGHT);
		titleUnderLine->setColor(ColorType::SUB_COLOR_LITE);
		objectsControl.addFigure(Layer::UI, titleUnderLine);


		// 勝利条件 + 敗北条件 (一部のY座標は後から指定)

		winLabel_ = make_shared<Entity::Text>("勝利条件:", INFO_TEXT_X, INFO_TEXT_Y + LINE_HEIGHT + SECTION_MARGIN, FontType::NORMAL_S20, ColorType::SUB_COLOR_BIT_LITE);
		objectsControl.addFigure(Layer::UI, winLabel_);

		winValue_ = make_shared<Entity::Text>("", INFO_TEXT_X, INFO_TEXT_Y + 2 * LINE_HEIGHT + SECTION_MARGIN, FontType::NORMAL_S20, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, winValue_);

		loseLabel_ = make_shared<Entity::Text>("敗北条件:", INFO_TEXT_X, 0, FontType::NORMAL_S20, ColorType::SUB_COLOR_BIT_LITE);
		objectsControl.addFigure(Layer::UI, loseLabel_);

		loseValue_ = make_shared<Entity::Text>("", INFO_TEXT_X, 0, FontType::NORMAL_S20, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, loseValue_);

		// アンダーバー
		sectionUnderBar_ = make_shared<Entity::Panel>();
		sectionUnderBar_->setShape(INFO_TEXT_X, 0, STAGE_INFO_W - STAGE_INFO_PADDING * 2, UNDER_BAR_HEIGHT);
		sectionUnderBar_->setColor(ColorType::SUB_COLOR_LITE);
		objectsControl.addFigure(Layer::UI, sectionUnderBar_);


		// ステージヒント
		hintLabel_ = make_shared<Entity::Text>("ヒント:", INFO_TEXT_X, 0, FontType::NORMAL_S20, ColorType::SUB_COLOR_BIT_LITE);
		objectsControl.addFigure(Layer::UI, hintLabel_);

		stageHint_ = make_shared<Entity::Text>("", INFO_TEXT_X, 0, FontType::NORMAL_S20, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, stageHint_);


		/* ステージ情報欄 ここまで */


		
		// スタートボタン
		shared_ptr<Entity::TextButton> startBtn = make_shared<Entity::TextButton>(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR);
		startBtn->setShape(INFO_TEXT_X, BTN_Y, BTN_W, BTN_H);
		startBtn->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_OVER);
		startBtn->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		startBtn->setText("決定", FontType::BLACK_S32);
		objectsControl.addObject(Layer::UI, UIid::START_BTN, startBtn);

		// 戻るボタン
		shared_ptr<Entity::TextButton> backBtn = make_shared<Entity::TextButton>(ColorType::SUB_COLOR_LITE2, ColorType::SUB_COLOR);
		backBtn->setShape(INFO_TEXT_X + BTN_W + BTN_MARGIN, BTN_Y, BTN_W, BTN_H);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::SUB_COLOR_LITE, Entity::TextButton::State::MOUSE_OVER);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		backBtn->setText("戻る", FontType::BLACK_S32);
		objectsControl.addObject(Layer::UI, UIid::BACK_BTN, backBtn);
		
		// ステージ情報更新
		updateStageInfo();

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

		// イベント対象オブジェクトがない場合、終了
		if (!hitObjSp)
		{
			return;
		}


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
			courseBtn->setSelected(true, true);
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

	/**
	 * @fn
	 * アニメーション終了後のスクリーン更新処理
	*/
	void SelectScreen::updateByAnimation()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();

		if (isOpenOverlayEnded()) // オーバーレイ（open）終了判定用
		{
			if (selectedCourseId_ != -1)
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
		vector<int> checkWinData;
		Utility::ResourceManager::loadStageData(STAGE_KIND, selectedCourseId_, &title, &hint, &checkWinData);

		int lineCount = 0;

		// タイトル
		stageTitle_->setText(title.c_str());

		// 勝敗条件情報読み込み
		Battle::CheckWin checkWin;
		checkWin.loadData(checkWinData);

		++lineCount; // 勝利条件ラベル分

		// 勝利条件内容

		string winValue;
		int winValueLineCount;

		checkWin.getWinConditionsText(&winValue, &winValueLineCount);

		winValue_->setText(winValue.c_str());
		lineCount += winValueLineCount;

		// 空行
		++lineCount;

		// 敗北条件ラベル（y座標のみ変更）
		loseLabel_->setY(INFO_TEXT_Y + LINE_HEIGHT * lineCount + SECTION_MARGIN);
		++lineCount;
		
		// 敗北条件内容
		string loseValue;
		int loseValueLineCount;

		checkWin.getLoseConditionsText(&loseValue, &loseValueLineCount);

		loseValue_->setText(loseValue.c_str());
		loseValue_->setY(INFO_TEXT_Y + LINE_HEIGHT * lineCount + SECTION_MARGIN);
		lineCount += loseValueLineCount;

		// 下線部
		sectionUnderBar_->setY(INFO_TEXT_Y + LINE_HEIGHT * lineCount + SECTION_MARGIN);

		// ヒント
		hintLabel_->setY(INFO_TEXT_Y + SECTION_MARGIN * 2 + LINE_HEIGHT * lineCount);
		++lineCount;

		stageHint_->setText(hint.c_str());
		stageHint_->setY(INFO_TEXT_Y + SECTION_MARGIN * 2 + LINE_HEIGHT * lineCount);
	}
}
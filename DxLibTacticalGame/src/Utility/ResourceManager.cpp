#include "ResourceManager.h"
#include "Battle/CheckWin.h"
#define THROWS_EX(ret, msg) { if (ret == -1) { throw msg; } }

namespace Utility {
	/**
	 * @fn
	 * シングルトンにするためのインスタンス関数
	 */
	ResourceManager& ResourceManager::getInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	/**
	 * @fn
	 * デストラクタ
	 */
	ResourceManager::~ResourceManager()
	{
		// フォントファイルを解放
		for (auto itr = fontHandleList_.begin(); itr != fontHandleList_.end(); ++itr)
		{
			bool success = RemoveFontMemResourceEx(*itr);

			if (!success)
			{
				DxLib::printfDx("failue unload font");
			}

		}

		for (auto&& mItr = image_.begin(); mItr != image_.end(); ++mItr)
		{
			vector<int*> vec = mItr->second;
			for (auto itr = vec.begin(); itr != vec.end(); ++itr)
			{
				delete* itr;
			}
		}
	}

	/**
	 * @fn
	 * リソースのロード
	 * @return 終了フラグ（0が正常）
	 */
	int ResourceManager::load()
	{
		int ret = 0;
		loadImages();
		loadFonts();
		loadColors();
		loadSounds();

		// mapList_[MapId::STAGE1] = loadMapCsv("resource/map/stage1.csv");

		return ret;
	}
	/**
	 * @fn
	 * 画像のリソースIDを取得
	 * @param (type) リソースタイプ
	 * @param (kind) リソースの種類
	 * @param (pos) リソースの位置
	 * @return 対象リソースのID
	 */
	int ResourceManager::getImage(int type, int kind, int pos) const
	{
		return image_.at(type).at(kind)[pos];
	}

	/**
	 * @fn
	 * フォントのリソースIDを取得
	 * @param (kind) リソースの種類
	 * @return 対象リソースのID
	 */
	int ResourceManager::getHdlFont(int kind) const
	{
		return hdlFont_[kind];
	}

	/**
	 * @fn
	 * カラーデータを取得
	 * @param (kind) カラーデータの種類
	 * @return カラーデータ
	 */
	int ResourceManager::getColor(int kind) const
	{
		return colorType_[kind];
	}

	/**
	 * @fn
	 * 音声のリソースIDを取得
	 * @param (kind) リソースの種類
	 * @return 対象リソースのID
	 */
	int ResourceManager::getSound(int kind) const
	{
		return sounds_[kind];
	}

	/**
	 * @fn
	 * 音声を出力
	 * @param (kind) 音声の種類
	 */
	void ResourceManager::playSound(int kind) {
		DxLib::PlaySoundMem(getInstance().sounds_[kind], DX_PLAYTYPE_BACK);
	}

	/**
	 * @fn
	 * マップチップ用の読み取り関数
	 * @param (resourcePath) リソースパス
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadMapchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath,
			8, 4, 2, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}
	/**
	 * @fn
	 * キャラチップ用の読み取り関数
	 * @param (resourcePath) リソースパス
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadCharacterchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath, 16, 4, 4, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}
	/**
	 * @fn
	 * 1枚絵の読み取り関数
	 * @param (resourcePath) リソースパス
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadImage(const char* resourcePath, int* handle) {
		*handle = DxLib::LoadGraph(resourcePath);
		THROWS_EX(*handle, resourcePath);
	}

	/**
	 * @fn
	 * マップチップ用の読み取り関数（リソースから）
	 * @param (resourceName) リソース名
	 * @param (resourceType) リソースタイプ
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadMapchip(const char* resourceName, const char* resourceType, int* handle) {
		int ret = DxLib::LoadDivGraphToResource(resourceName, resourceType,
			MAP_CHIP_LEN, MAP_CHIP_LEN, 1, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourceName);
	}
	/**
	 * @fn
	 * キャラチップ用の読み取り関数（リソースから）
	 * @param (resourceName) リソース名
	 * @param (resourceType) リソースタイプ
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadCharacterchip(const char* resourceName, const char* resourceType, int* handle) {
		int ret = DxLib::LoadDivGraphToResource(resourceName, resourceType, UNIT_KIND_LEN * 2, UNIT_KIND_LEN, 2, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourceName);
	}
	/**
	 * @fn
	 * 1枚絵の読み取り関数（リソースから）
	 * @param (resourceName) リソース名
	 * @param (resourceType) リソースタイプ
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadImage(const char* resourceName, const char* resourceType, int* handle) {
		*handle = DxLib::LoadGraphToResource(resourceName, resourceType);
		THROWS_EX(*handle, resourceName);
	}

	/**
	 * @fn
	 * 複数枚絵の読み取り関数（リソースから）
	 * @param (resourceName) リソース名
	 * @param (resourceType) リソースタイプ
	 * @param (allNum) 画像数
	 * @param (xNum) 画像列数
	 * @param (yNum) 画像行数
	 * @param (w) 画像１枚あたりの幅
	 * @param (h) 画像１枚あたりの高さ
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadDivImage(const char* resourceName, const char* resourceType, int allNum, int xNum, int yNum, int w, int h, int* handle)
	{
		int ret = DxLib::LoadDivGraphToResource(resourceName, resourceType, allNum, xNum, yNum, w, h, handle);
		THROWS_EX(ret, resourceName);
	}

	/**
	 * @fn
	 * 全画像データをロード
	 */
	int ResourceManager::loadImages()
	{
		int ret = 0;
		image_.insert(std::make_pair(ImageType::IMAGE, vector<int*>()));

		for (int i = 0; i < ImageId::IMAGE_ID_LEN; i++)
		{
			image_.at(ImageType::IMAGE).push_back(new int[1]);
		}
		// 背景
		loadImage(MAKEINTRESOURCE(BACKGROUND_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::IMAGE).at(ImageId::BACKGROUND_IMAGE_ID));
		
		// 背景（暗め）
		loadImage(MAKEINTRESOURCE(BACKGROUND_DARK_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::IMAGE).at(ImageId::BACKGROUND_DARK_IMAGE_ID));

		// タイトル
		loadImage(MAKEINTRESOURCE(TITLE_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::IMAGE).at(ImageId::TITLE_IMAGE_ID));

		// マス読込
		image_.insert(std::make_pair(ImageType::MAP, vector<int*>()));
		image_.at(ImageType::MAP).push_back(new int[8]);
		loadMapchip(MAKEINTRESOURCE(MAP_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::MAP).at(MapImageKind::ALL));

		// 山
		image_.at(ImageType::MAP).push_back(new int[MOUNTAIN_ALL]);
		loadDivImage(MAKEINTRESOURCE(MAP_MOUNTAIN), MAKEINTRESOURCE(IMAGE_FILE),
			MOUNTAIN_ALL, MOUNTAIN_W, MOUNTAIN_H, CHIP_SIZE, CHIP_SIZE,
			image_.at(ImageType::MAP).at(MapImageKind::MOUNTAIN));

		// 川
		image_.at(ImageType::MAP).push_back(new int[RIVER_NUM]);
		loadDivImage(MAKEINTRESOURCE(MAP_RIVER), MAKEINTRESOURCE(IMAGE_FILE),
			RIVER_NUM, RIVER_NUM, 1, CHIP_SIZE, CHIP_SIZE,
			image_.at(ImageType::MAP).at(MapImageKind::RIVER));

		// マス効果
		image_.at(ImageType::MAP).push_back(new int[MassStatePos::MASS_STATE_LEN]);
		loadDivImage(MAKEINTRESOURCE(MASS_STATE), MAKEINTRESOURCE(IMAGE_FILE),
			MassStatePos::MASS_STATE_LEN, MassStatePos::MASS_STATE_LEN, 1, CHIP_SIZE, CHIP_SIZE,
			image_.at(ImageType::MAP).at(MapImageKind::STATE));


		// プレイヤー画像の読み込み
		image_.insert(std::make_pair(ImageType::UNIT, vector<int*>()));
		image_.at(ImageType::UNIT).push_back(new int[UNIT_KIND_LEN * 2]);
		loadCharacterchip(MAKEINTRESOURCE(UNIT_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::UNIT).at(UnitImageKind::NORMAL));
		image_.insert(std::make_pair(ImageType::UNIT, vector<int*>()));
		image_.at(ImageType::UNIT).push_back(new int[UNIT_KIND_LEN * 2]);
		loadCharacterchip(MAKEINTRESOURCE(UNIT_ACTED_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::UNIT).at(UnitImageKind::ACTED));

		// ダメージエフェクト
		image_.insert(std::make_pair(ImageType::EFFECT, vector<int*>()));
		image_.at(ImageType::EFFECT).push_back(new int[DMAGE_EFFECT_LEN]);
		loadDivImage(MAKEINTRESOURCE(EFFECT_DAMAGE_IMAGE), MAKEINTRESOURCE(IMAGE_FILE),
			DMAGE_EFFECT_LEN, DMAGE_EFFECT_LEN, 1, DMAGE_EFFECT_W, DMAGE_EFFECT_H,
			image_.at(ImageType::EFFECT).at(EffectId::DAMAGE));
		
		image_.at(ImageType::EFFECT).push_back(new int[1]);
		loadImage(MAKEINTRESOURCE(EFFECT_MISS_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::EFFECT).at(EffectId::MISS));

		// 弾丸
		image_.at(ImageType::EFFECT).push_back(new int[BULLET_POS_LEN]);
		loadDivImage(MAKEINTRESOURCE(BULLET_IMAGE), MAKEINTRESOURCE(IMAGE_FILE),
			BULLET_POS_LEN, BULLET_POS_LEN, 1, CHIP_SIZE / 4, CHIP_SIZE /4,
			image_.at(ImageType::EFFECT).at(EffectId::BULLET));

		image_.at(ImageType::EFFECT).push_back(new int[1]);
		loadImage(MAKEINTRESOURCE(EFFECT_ARROW_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::EFFECT).at(EffectId::ARROW));

		loadFlag = true;
		return ret;
	}

	/**
	 * @fn
	 * 全フォントデータを取得
	 */
	int ResourceManager::loadFonts()
	{
		int ret = 0;

		loadFont("resource/font/rounded-mplus/rounded-mplus-1p-regular.ttf");
		loadFont("resource/font/rounded-mplus/rounded-mplus-1p-black.ttf");

		hdlFont_[FontType::NORMAL_S14] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 14, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::NORMAL_S18] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 18, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::NORMAL_S20] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 20, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::NORMAL_S24] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 24, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::NORMAL_S32] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 32, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::BLACK_S24] = DxLib::CreateFontToHandle("Rounded M+ 1p black", 24, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::BLACK_S32] = DxLib::CreateFontToHandle("Rounded M+ 1p black", 32, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::BLACK_S48] = DxLib::CreateFontToHandle("Rounded M+ 1p black", 48, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::BLACK_S64] = DxLib::CreateFontToHandle("Rounded M+ 1p black", 64, 3, DX_FONTTYPE_ANTIALIASING_8X8);

		// フォントの行間
		DxLib::SetFontLineSpaceToHandle(24, hdlFont_[FontType::NORMAL_S18]);
		DxLib::SetFontLineSpaceToHandle(28, hdlFont_[FontType::NORMAL_S20]);
		DxLib::SetFontLineSpaceToHandle(32, hdlFont_[FontType::NORMAL_S24]);

		return ret;
	}

	/**
	 * @fn
	 * 全カラーデータを取得
	 */
	int ResourceManager::loadColors()
	{
		int ret = 0;
		// 色取得
		colorType_[ColorType::WHITE] = DxLib::GetColor(255, 255, 255);

		colorType_[ColorType::MAIN_COLOR] = DxLib::GetColor(250, 244, 232);
		colorType_[ColorType::MAIN_COLOR_ON] = DxLib::GetColor(254, 213, 175);
		colorType_[ColorType::MAIN_COLOR_ON_DARK] = 0xFB8B24;

		colorType_[ColorType::SUB_COLOR] = DxLib::GetColor(124, 104, 95);
		colorType_[ColorType::SUB_COLOR_LITE] = DxLib::GetColor(224, 215, 204);
		colorType_[ColorType::SUB_COLOR_LITE2] = 0xD6CCC1;
		colorType_[ColorType::SUB_COLOR_BIT_LITE] = 0xA18C80;
		colorType_[ColorType::SUB_COLOR_DARK] = DxLib::GetColor(76, 64, 59);

		colorType_[ColorType::TITLE_TEXT] = DxLib::GetColor(211, 181, 155);

		colorType_[ColorType::PLAYER_COLOR] = DxLib::GetColor(35, 87, 137);
		colorType_[ColorType::PLAYER_COLOR_LITE] = DxLib::GetColor(186, 197, 204);

		colorType_[ColorType::ENEMY_COLOR] = DxLib::GetColor(209, 6, 6);
		colorType_[ColorType::ENEMY_COLOR_LITE] = DxLib::GetColor(233, 183, 177);

		colorType_[ColorType::POSITIVE_COLOR] = DxLib::GetColor(116, 142, 84);
		colorType_[ColorType::POSITIVE_LITE_COLOR] = DxLib::GetColor(185, 202, 165);

		colorType_[ColorType::NEGATIVE_COLOR] = DxLib::GetColor(139, 137, 130);
		colorType_[ColorType::NEGATIVE_COLOR_DARK] = 0x55534E;
		colorType_[ColorType::NEGATIVE_COLOR_LITE] = 0xB5B3AA;

		colorType_[ColorType::ACCENT_COLOR] = DxLib::GetColor(204, 75, 194);

		return ret;
	}

	/**
	 * @fn
	 * 全音声データを取得
	 */
	int ResourceManager::loadSounds()
	{
		int ret = 0;
		// 効果音取得
		sounds_[SoundKind::CLICK] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(CLICK_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(210, sounds_[SoundKind::CLICK]);

		sounds_[SoundKind::CHECK] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(CHECK_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(220, sounds_[SoundKind::CHECK]);

		sounds_[SoundKind::BACK] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(BACK_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(160, sounds_[SoundKind::BACK]);

		sounds_[SoundKind::BORN] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(BORN_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(170, sounds_[SoundKind::BORN]);

		sounds_[SoundKind::ATTACK_CLOSE] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(ATTACK_CLOSE_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(140, sounds_[SoundKind::ATTACK_CLOSE]);

		sounds_[SoundKind::ATTACK_FIRE] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(ATTACK_FIRE_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(150, sounds_[SoundKind::ATTACK_FIRE]);

		sounds_[SoundKind::MOVE] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(MOVE_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(130, sounds_[SoundKind::MOVE]);

		sounds_[SoundKind::WAIT] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(WAIT_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(160, sounds_[SoundKind::WAIT]);

		sounds_[SoundKind::SELECT_UNIT] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(SELECT_UNIT_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(120, sounds_[SoundKind::SELECT_UNIT]);

		sounds_[SoundKind::WIN] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(WIN_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(180, sounds_[SoundKind::WIN]);

		sounds_[SoundKind::LOSE] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(LOSE_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(160, sounds_[SoundKind::LOSE]);

		sounds_[SoundKind::MESSAGE] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(MESSAGE_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(160, sounds_[SoundKind::MESSAGE]);

		sounds_[SoundKind::EXTRA_EFFECT] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(EXTRA_EFFECT_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(140, sounds_[SoundKind::EXTRA_EFFECT]);

		return ret;
	}

	/**
	 * @fn
	 * フォントデータを読み込み
	 * @param (fontFilePath) フォントファイルのパス
	 */
	void ResourceManager::loadFont(const LPCSTR fontFilePath)
	{
		int FontFileSize = (int)(FileRead_size(fontFilePath));
		// フォントファイルを開く
		int FontFileHandle = FileRead_open(fontFilePath);

		// フォントデータ格納用のメモリ領域を確保
		void* Buffer = new void* [FontFileSize];
		// フォントファイルを丸ごとメモリに読み込む
		FileRead_read(Buffer, FontFileSize, FontFileHandle);

		// AddFontMemResourceEx引数用
		DWORD font_num = 0;

		// メモリに読み込んだフォントデータをシステムに追加
		HANDLE handle = AddFontMemResourceEx(Buffer, FontFileSize, NULL, &font_num);
		if (handle == 0)
		{
			DxLib::printfDx("フォントデータの読み込みに失敗しました");
			return;
		}

		fontHandleList_.push_back(handle);

		FileRead_close(FontFileHandle);
		delete[] Buffer;

		/*
		if (AddFontResource(fontFilePath) > 0) 
			PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
		}
		else {
			// フォント読込エラー処理
			MessageBox(NULL, "フォント読込失敗", "", MB_OK);
		}
		*/
	}

	/**
	 * @fn
	 * ステージデータのタイトルだけロード
	 * @param (stageKind) ステージの種類
	 * @param (id) ステージID
	 * @param (title) ステージタイトル取得用
	 */
	void ResourceManager::loadStageTitle(const string stageKind, const int id, string* title)
	{
		string hint;
		vector<int> checkWinData;
		vector<int> extraRules;
		std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H > mapData;
		vector<vector<int>> units;
		Utility::ResourceManager::loadStageData(stageKind, id, title, &hint, &checkWinData, &extraRules, &mapData, &units, UntilStageLoad::TITLE);
	}

	/**
	 * @fn
	 * ステージデータのロード
	 * @param (stageKind) ステージの種類
	 * @param (id) ステージID
	 * @param (title) ステージタイトル取得用
	 * @param (hint) ヒント取得用
	 * @param (checkWinData) 勝敗条件取得用
	 * @param (isUntilCheckWin) タイトル、ヒント、勝敗条件のみ取得したい場合はtrue (デフォルトfalse)
	 */
	void ResourceManager::loadStageData(const string stageKind, const int id, string* title, string* hint, vector<int>* checkWinData)
	{
		vector<int> extraRules;
		std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H > mapData;
		vector<vector<int>> units;
		Utility::ResourceManager::loadStageData(stageKind, id, title, hint, checkWinData, &extraRules, &mapData, &units, UntilStageLoad::CHECK_WIN);
	}

	/**
	 * @fn
	 * ステージデータのロード
	 * @param (stageKind) ステージの種類
	 * @param (id) ステージID
	 * @param (title) ステージタイトル取得用
	 * @param (hint) ヒント取得用
	 * @param (checkWinData) 勝敗条件取得用
	 * @param (extraRules) その他のルール (自由配置可能数)
	 * @param (mapData) マップデータ取得用
	 * @param (units) 配置ユニット取得用
	 * @param (isUntilCheckWin) タイトル、ヒント、勝敗条件のみ取得したい場合はtrue (デフォルトfalse)
	 */
	void ResourceManager::loadStageData(const string stageKind, const int id, string* title, string* hint, vector<int>* checkWinData, vector<int>* extraRules, std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H >* mapData, vector<vector<int>>* units, int untilLoad)
	{
		std::string str_buf;
		std::string str_conma_buf;

		// リソースからステージファイル読み込み
		/*
		HRSRC hrc = FindResourceA(NULL, MAKEINTRESOURCE(STAGE0 + id), MAKEINTRESOURCE(CSV));
		DWORD datasize = SizeofResource(NULL, hrc);
		HGLOBAL hgb = LoadResource(NULL, hrc);
		LPVOID data = LockResource(hgb);

		FreeResource(hrc);

		if (!hgb || datasize <= 0 || !data)
		{
			DxLib::printfDx("error: ステージファイルの読込に失敗");
			return;
		}

		string dataString = regex_replace(string((char*)data, datasize), regex("\r\n"), "\n"); ;
		std::stringstream ss{ dataString };

		std::ifstream ifs_csv_file;
		ifs_csv_file.basic_ios<char>::rdbuf(ss.rdbuf());
		ifs_csv_file.rdbuf()->pubsetbuf((char*)data, datasize);

		// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
		/*
		string str = "resource/map/" + stageKind + to_string(id) + ".csv";
		const LPCSTR csvFilePath = str.c_str();

		std::ifstream ifs_csv_file(p);
		*/

		string filePath = "resource/map/" + stageKind + to_string(id) + ".csv";

		char readLine[256];

		// CSVファイル読込
		int csvHandle = FileRead_open(filePath.c_str());

		if (csvHandle == 0)
		{
			DxLib::printfDx("ステージデータの読込に失敗しました。");
			return;
		}

		// ステージタイトル
		DxLib::FileRead_gets(readLine, 256, csvHandle);
		str_buf = string(readLine);
		*title = str_buf;

		if (untilLoad == UntilStageLoad::TITLE) // タイトルまでしか読み込まない場合
		{
			DxLib::FileRead_close(csvHandle);
			return;
		}

		// ヒント
		DxLib::FileRead_gets(readLine, 256, csvHandle);
		str_buf = string(readLine);
		*hint = regex_replace(str_buf, regex("\\\\n"), "\n");

		// 空行
		DxLib::FileRead_gets(readLine, 256, csvHandle);
		str_buf = string(readLine);

		// 勝敗ルール読み込み
		DxLib::FileRead_gets(readLine, 256, csvHandle);
		str_buf = string(readLine);
		std::istringstream i_stream(str_buf);
		while (getline(i_stream, str_conma_buf, ','))
		{
			(*checkWinData).push_back(stoi(str_conma_buf));
		}

		if (untilLoad == UntilStageLoad::CHECK_WIN) // 勝敗条件までしか読み込まない場合
		{
			DxLib::FileRead_close(csvHandle);
			return;
		}

		// その他ルール
		DxLib::FileRead_gets(readLine, 256, csvHandle);
		str_buf = string(readLine);
		i_stream = istringstream(str_buf);
		while (getline(i_stream, str_conma_buf, ','))
		{
			(*extraRules).push_back(stoi(str_conma_buf));
		}

		// 空行を読み込む想定
		DxLib::FileRead_gets(readLine, 256, csvHandle);
		str_buf = string(readLine);

		// マス初期化
		for (int i = 0; i < MAP_MASS_H; i++) {
			for (int j = 0; j < MAP_MASS_W; j++) {
				(*mapData)[i][j] = -1;
			}
		}

		int lineCount = 0;
		while (lineCount < MAP_MASS_H)
		{
			if (DxLib::FileRead_gets(readLine, 256, csvHandle) == -1)
			{
				return;
			}

			str_buf = string(readLine);

			if (str_buf == "")
			{
				break; // 空行の場合終了
			}

			// 「,」区切りごとにデータを読み込むためにistringstream型にする
			std::istringstream i_stream(str_buf);
			int colCount = 0;
			// 「,」区切りごとにデータを読み込む
			while (getline(i_stream, str_conma_buf, ','))
			{
				// csvファイルに書き込む
				(*mapData)[lineCount][colCount] = stoi(str_conma_buf);
				++colCount;
			}
			++lineCount;
		}

		// 設置ユニットを読み込む
		while (DxLib::FileRead_gets(readLine, 256, csvHandle) != -1)
		{
			str_buf = string(readLine);

			// 「,」区切りごとにデータを読み込むためにistringstream型にする
			std::istringstream i_stream(str_buf);
			vector<int> unitData;

			// 「,」区切りごとにデータを読み込む
			while (getline(i_stream, str_conma_buf, ','))
			{
				unitData.push_back(stoi(str_conma_buf));
			}
			units->push_back(unitData);
		}

		// ifs_csv_file.close();
		DxLib::FileRead_close(csvHandle);
	}

	/**
	 * @fn
	 * ロード済みであるか
	 * @return ロード済みの場合 true
	 */
	boolean ResourceManager::isLoaded() const {
		return loadFlag;
	}

}

//ヘッダファイル読み込み
#include "game.h"		//ゲーム全体のヘッダファイル
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理

//構造体の定義

//キャラクターの構造体
struct CHARACTOR
{
	int handle = -1;		//画像のハンドル（管理番号）
	char path[255];			//画像の場所

	int x;					//X位置
	int y;					//Y位置
	int width;				//幅
	int height;				//高さ

	int speed = 1;			//移動速度

	RECT coll;				//当たり判定の領域(四角)
	BOOL IsDraw = FALSE;	//画像が描画できる
};

//動画の構造体
struct MOVIE
{
	int handle = -1;		//動画のハンドル
	char path[255];			//動画のパス

	int x;					//X位置
	int y;					//Y位置
	int width;				//幅
	int height;				//高さ

	int Volume = 255;		//ボリューム（最小）0～255（最大）
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;	//音楽のハンドル
	char path[255];		//音楽のパス

	int Volume = -1;	//ボリューム（MIN 0 ～ 255 MAX）
	int playType = -1;
};

//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームのシーン
GAME_SCENE OldGameScene;	//前回のゲームのシーン
GAME_SCENE NextGameScene;	//次のゲームのシーン

//プレイ背景の動画
MOVIE playMovie;

//プレイヤー
CHARACTOR player;

//ゴール
CHARACTOR Goal;

//音楽
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;

//効果音
AUDIO PlayerSE;

//画面の切り替え
BOOL IsFadeOut = FALSE;		//フェードアウト
BOOL IsFadeIn = FALSE;		//フェードイン

//シーン切り替え
int fadeTimeMill = 2000;		//切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//ミリ秒をフレーム秒で変換

//フェードアウト
int fadeOutCntInit = 0;				//初期値
int fadeOutCnt = fadeOutCntInit;	//フェードアウトのカウンタ
int fadeOutCntMax = fadeTimeMax;	//フェードアウトのカウンタMAX

//フェードアイン
int fadeInCntInit = fadeTimeMax;	//初期値
int fadeInCnt = fadeInCntInit;		//フェードアウトのカウンタ
int fadeInCntMax = fadeTimeMax;				//フェードアウトのカウンタMAX

//プロトタイプ宣言
VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面の処理
VOID TitleDraw(VOID);	//タイトル画面の描画

VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面の処理
VOID PlayDraw(VOID);	//プレイ画面の描画

VOID End(VOID);			//エンド画面
VOID EndProc(VOID);		//エンド画面の処理
VOID EndDraw(VOID);		//エンド画面の描画

VOID Change(VOID);		//切り替え画面
VOID ChangeProc(VOID);	//切り替え画面の処理
VOID ChangeDraw(VOID);	//切り替え画面の描画

VOID ChangeScene(GAME_SCENE scene);	//シーン切り替え

VOID CollUpdatePlayer(CHARACTOR* chara);	//当たり判定の領域を更新
VOID CollUpdate(CHARACTOR* chara);			//当たり判定

BOOL OnCollRect(RECT a, RECT b);			//矩形と矩形の当たり判定

BOOL GameLoad(VOID);	//ゲームのデータを読み込み
VOID GameInit(VOID);

// プログラムは WinMain から始まります
//Windowsのプログラミング方法 = (WinAPI)で動いている
//DxLibは、DirectXという、ゲームプログラミングを簡単に使える仕組み
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txtを出力しない
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetMainWindowText("GAME_TITLE");					//ウィンドウのタイトル
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景の色
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルを読み込み
	SetWindowStyleMode(GAME_WINDOW_BAR);				//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);								//ディスプレイの垂直同期を有効にする
	SetAlwaysRunFlag(TRUE);								//ウィンドウをずっとアクティブにする

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//最初のシーンは、タイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//ゲーム全体の初期化

	//ゲーム読み込み
	if (!GameLoad())
	{
		//データの読み込みに失敗したとき
		DxLib_End();		//DxLib終了
		return -1;			//異常終了
	}

	//ゲームの初期化
	GameInit();
	
	//無限ループ
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	//メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; }	//画面を消去する

		//キーボード入力の更新
		AllKeyUpdate();

		//FPS値の更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();				//プレイ画面
			break;
		case GAME_SCENE_END:
			End();				//エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();			//切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}

		//FPS値を描画
		FPSDraw();

		//FPS値を待つ
		FPSWait();
		
		ScreenFlip();	//ダブルバッファリングした画面を描画
	}

	//終わるときの処理
	DeleteGraph(playMovie.handle);		//動画をメモリ上から削除
	DeleteGraph(player.handle);			//画像をメモリ上から削除
	DeleteGraph(Goal.handle);			//画像をメモリ上から削除

	DeleteSoundMem(TitleBGM.handle);	//音楽をメモリ上から削除
	DeleteSoundMem(PlayBGM.handle);		//音楽をメモリ上から削除
	DeleteSoundMem(EndBGM.handle);		//音楽をメモリ上から削除

	DeleteSoundMem(PlayerSE.handle);		//音楽をメモリ上から削除

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	return 0;				// ソフトの終了 
}

	/// <summary>
/// ゲームのデータを読み込み
/// </summary>
/// <returns>読み込めたらTRUE/読み込めなかったらFALSE</returns>
	BOOL GameLoad()
	{
		//プレイ動画の背景を読み込み
		strcpyDx(playMovie.path, ".\\Movie\\PlayMovie.mp4");		//パスのコピー
		playMovie.handle = LoadGraph(playMovie.path);				//画像の読み込み

		//動画が読み込めなかったときは、エラー(-1)が入る
		if (playMovie.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//メインのウィンドウハンドル
				playMovie.path,				//メッセージ本文
				"動画読み込みエラー！",		//メッセージタイトル
				MB_OK						//ボタン
			);

			return FALSE;		//読み込み失敗

		}

		//画像の幅と高さを取得
		GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

		//動画のボリューム
		playMovie.Volume = 255;

		//プレイヤーの画像を読み込み
		strcpyDx(player.path, ".\\Image\\Player.png");		//パスのコピー
		player.handle = LoadGraph(player.path);				//画像の読み込み

		//画像が読み込めなかったときは、エラー(-1)が入る
		if (player.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//メインのウィンドウハンドル
				player.path,				//メッセージ本文
				"画像読み込みエラー！",		//メッセージタイトル
				MB_OK						//ボタン
			);

			return FALSE;
		}

		//画像の幅と高さを取得
		GetGraphSize(player.handle, &player.width, &player.height);

		//ゴールの画像を読み込み
		strcpyDx(Goal.path, ".\\Image\\goal.png");		//パスのコピー
		Goal.handle = LoadGraph(Goal.path);				//画像の読み込み

		//画像が読み込めなかったときは、エラー(-1)が入る
		if (Goal.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//メインのウィンドウハンドル
				Goal.path,				//メッセージ本文
				"画像読み込みエラー！",		//メッセージタイトル
				MB_OK						//ボタン
			);

			return FALSE;
		}

		//画像の幅と高さを取得
		GetGraphSize(Goal.handle, &Goal.width, &Goal.height);

		//音楽の画像を読み込み
		strcpyDx(TitleBGM.path, ".\\Audio\\線香花火.mp3");		//パスのコピー
		TitleBGM.handle = LoadSoundMem(TitleBGM.path);

		//音楽が読み込めなかったときは、エラー(-1)が入る
		if (TitleBGM.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//メインのウィンドウハンドル
				TitleBGM.path,				//メッセージ本文
				"音楽読み込みエラー！",		//メッセージタイトル
				MB_OK						//ボタン
			);

			return FALSE;
		}

		TitleBGM.playType = DX_PLAYTYPE_LOOP;
		TitleBGM.Volume = 255;

		//音楽の画像を読み込み
		strcpyDx(PlayBGM.path, ".\\Audio\\Frenzy_Circus.mp3");		//パスのコピー
		PlayBGM.handle = LoadSoundMem(PlayBGM.path);

		//音楽が読み込めなかったときは、エラー(-1)が入る
		if (PlayBGM.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//メインのウィンドウハンドル
				PlayBGM.path,				//メッセージ本文
				"音楽読み込みエラー！",		//メッセージタイトル
				MB_OK						//ボタン
			);

			return FALSE;
		}

		PlayBGM.playType = DX_PLAYTYPE_LOOP;
		PlayBGM.Volume = 255;

		//音楽の画像を読み込み
		strcpyDx(EndBGM.path, ".\\Audio\\罪の後悔_2.mp3");		//パスのコピー
		EndBGM.handle = LoadSoundMem(EndBGM.path);

		//音楽が読み込めなかったときは、エラー(-1)が入る
		if (EndBGM.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//メインのウィンドウハンドル
				EndBGM.path,				//メッセージ本文
				"音楽読み込みエラー！",		//メッセージタイトル
				MB_OK						//ボタン
			);

			return FALSE;
		}

		PlayBGM.playType = DX_PLAYTYPE_LOOP;
		PlayBGM.Volume = 255;

		//効果音を読み込み
		strcpyDx(PlayerSE.path, ".\\Audio\\エネルギーをためる（ロックマン風）.mp3");		//パスのコピー
		PlayerSE.handle = LoadSoundMem(PlayerSE.path);

		//音楽が読み込めなかったときは、エラー(-1)が入る
		if (PlayerSE.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//メインのウィンドウハンドル
				PlayerSE.path,				//メッセージ本文
				"音楽読み込みエラー！",		//メッセージタイトル
				MB_OK						//ボタン
			);

			return FALSE;
		}

		EndBGM.playType = DX_PLAYTYPE_BACK;
		EndBGM.Volume = 255;
		
		return TRUE;
	}

	/// <summary>
	/// ゲームデータを初期化
	/// </summary>
	/// <param name=""></param>
	VOID GameInit(VOID)
	{
		//プレイヤーを初期化
		player.x = GAME_WIDTH / 2 - player.width / 2;		//中央寄せ
		player.y = GAME_HEIGHT / 2 - player.height / 2;		//中央寄せ
		player.speed = 500;
		player.IsDraw = TRUE;	//描画できる

			//当たり判定を更新する
		CollUpdatePlayer(&player);		//プレイヤーの当たり判定のアドレス

		//ゴールを初期化
		Goal.x = GAME_WIDTH - Goal.width;		//中央寄せ
		Goal.y = 0;		//中央寄せ
		Goal.speed = 500;
		Goal.IsDraw = TRUE;	//描画できる

		//当たり判定を更新する
		CollUpdate(&Goal);		//プレイヤーの当たり判定のアドレス
	}

/// <summary>
/// シーンを切り替える関数
/// </summary>
/// <param name="scene">シーン</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//シーンを切り替え
	IsFadeIn = FALSE;	//フェードインしない
	IsFadeOut = TRUE;	//フェードアウトする

	return;
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//処理
	TitleDraw();	//描画

	return;
}

/// <summary>
/// タイトル画面の処理
/// </summary>
VOID TitleProc(VOID)
{

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGMを止める
		StopSoundMem(TitleBGM.handle);

		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//ゲームの初期化
		GameInit();

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	//BGMが流れていないとき
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(TitleBGM.handle,TitleBGM.playType);
	}

	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
VOID TitleDraw(VOID)
{

	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//処理
	PlayDraw();	//描画

	return;
}

/// <summary>
/// プレイ画面の処理
/// </summary>
VOID PlayProc(VOID)
{
	/*
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}
	*/

	//BGMが流れていないとき
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	//プレイヤーの操作
	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		player.y -= player.speed * fps.DaltaTime;

		//動く時の効果音を追加
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		player.y += player.speed * fps.DaltaTime;

		//動く時の効果音を追加
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}

	}

	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		player.x -= player.speed * fps.DaltaTime;

		//動く時の効果音を追加
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		player.x += player.speed * fps.DaltaTime;

		//動く時の効果音を追加
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}

	//当たり判定を更新する
	CollUpdatePlayer(&player);

	//ゴールの当たり判定を更新する
	CollUpdate(&Goal);

	//プレイヤーがゴールに当たった時
	if (OnCollRect(player.coll, Goal.coll) == TRUE)
	{
		//BGMを止める
		StopSoundMem(PlayBGM.handle);

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
		
		//処理を強制終了
		return;
	}

	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{
	//背景動画を描画

	//もし、動画が再生されていないとき
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//再生する
		SeekMovieToGraph(playMovie.handle, 0);		//シークバーを最初に戻す
		PlayMovieToGraph(playMovie.handle);			//動画を再生
	}

	//動画を描画(画面に合わせて画像を引き伸ばす)
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);

	//プレイヤーを描画
	if (player.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(player.x, player.y, player.handle, TRUE);

		//デバッグの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	//ゴールを描画
	if (Goal.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(Goal.x, Goal.y, Goal.handle, TRUE);

		//デバッグの時は、当たり判定の領域を描画
		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();	//処理
	EndDraw();	//描画

	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//BGMを止める
		StopSoundMem(EndBGM.handle);

		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	//BGMが流れていないとき
	if (CheckSoundMem(EndBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(EndBGM.handle, EndBGM.playType);
	}

	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 画面の切り替え
/// </summary>
VOID Change(VOID)
{
	ChangeProc();
	ChangeDraw();

	return;
}

/// <summary>
/// 画面の切り替えの処理
/// </summary>
VOID ChangeProc(VOID)
{

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//カウンタを減らす
		}
		else
		{
			//フェードイン処理が終わった

			fadeInCnt = fadeInCntInit;	//カウンタを初期化
			IsFadeIn = FALSE;			//フェードイン処理終了
		}
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//カウンタを減らす
		}
		else
		{
			//フェードアウト処理が終わった

			fadeOutCnt = fadeOutCntInit;	//カウンタを初期化
			IsFadeOut = FALSE;				//フェードアウト処理終了
		}
	}

	//切り替え画面終了
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードインしていない、フェードアウトもしていないとき
		GameScene = NextGameScene;		//次のシーンに切り替え
		OldGameScene = GameScene;		//以前のゲームシーン更新
	}


	return;
}

/// <summary>
/// 画面の切り替えの描画
/// </summary>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();		//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();			//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();			//エンド画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// 当たり判定の領域判定(プレイヤー)
/// </summary>
/// <param name="chara">当たり判定の領域</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->x + 84;
	chara->coll.top = chara->y + 4;
	chara->coll.right = chara->x + chara->width - 85;
	chara->coll.bottom = chara->y + chara->height - 6;

	return;
}

/// <summary>
/// 当たり判定の領域判定
/// </summary>
/// <param name="chara">当たり判定の領域</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;
	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}

/// <summary>
/// 矩形と矩形の当たり判定
/// </summary>
/// <param name="a">矩形A</param>
/// <param name="b">矩形B</param>
/// <returns>当たったらTURE／当たらなかったらFALSE</returns>
BOOL OnCollRect(RECT a, RECT b)
{
	if(
		a.left < b.right &&
		a.right > b.left &&
		a.top < b.bottom &&
		a.bottom > b.top 
		)
	{
		//当たっているとき
		return TRUE;
	}
	else
	{
		//当たっていないとき
		return FALSE;
	}
}
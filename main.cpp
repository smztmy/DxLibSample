//ヘッダーファイルの読み込み
#include "DxLib.h"		//DxLibを使う時は必要

//マクロ定義
#define GAME_TITLE  "ゲームタイトル"	//ゲームタイトル
#define GAME_WIDTH  1280				//ゲーム画面の幅
#define GAME_HEIGHT 720					//ゲーム画面の高さ
#define GAME_COLOR  32					//ゲーム画面の色域

#define GAME_ICON_ID	333

// プログラムは WinMain から始まります
//Windowsのプログラミング方法 = (WinAPI)で動いている
//DxLibは、DirectXという、ゲームプログラミングを簡単に使える仕組み
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetMainWindowText("GAME_TITLE");					//ウィンドウのタイトル
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景の色
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルを読み込み


	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//無限ループ
	while (1)
	{
		if (CheckHitKeyAll() != 0)	//0の時、何かキーが押された
		{
			break;	//無限ループから抜ける
		}

		//メッセージを受け取り続ける
		if (ProcessMessage() != 0)	//-1のとき、エラーやウィンドウから閉じられる
		{
			break;	//無限ループを抜ける
		}

	}


	// ＤＸライブラリ使用の終了処理
	DxLib_End();				

	return 0;				// ソフトの終了 
}
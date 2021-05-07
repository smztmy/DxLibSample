//ヘッダーファイルの読み込み
#include "DxLib.h"		//DxLibを使う時は必要

//マクロ定義
#define GAME_TITLE  "ゲームタイトル"	//ゲームタイトル
#define GAME_WIDTH  1280				//ゲーム画面の幅
#define GAME_HEIGHT 720					//ゲーム画面の高さ
#define GAME_COLOR  32					//ゲーム画面の色域

#define GAME_ICON_ID	333				//ゲームのICOのID

#define GAME_WINDOW_BAR	0				//ウィンドウバーの種類

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

	//四角の位置を決める
	int X = GAME_WIDTH / 2;			//横の中心
	int Y = GAME_HEIGHT / 2;		//縦の中心

	//四角の大きさを決める
	int width = 32;			//幅
	int height = 32;		//高さ

	//円の半径を決める
	int radius = 25;

	//速度
	int Speed = 1;
	int XSpeed = Speed;
	int YSpeed = Speed;

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

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

		//画面を消去する
		if (ClearDrawScreen() != 0) { break; }

		//四角を描写
		//drawbox(
		//	x, y, x + width, y + height,
		//	getcolor(255, 0, 0),	//色を取得
		//	true					//塗りつぶし
		//);

		//円を描画
		DrawCircle(
			X, Y, radius,
			GetColor(0, 0, 0),
			FALSE, 5
		);

		DrawCircle(
			X, Y, radius,
			GetColor(255, 0, 0),
			TRUE
		);

		X += XSpeed;		//四角の位置を右にずらす

		//①四角を斜め右下
		Y += YSpeed;

		//②四角を画面の端に来たら、移動する向きを反転
		if (X - radius < 0 || X + radius > GAME_WIDTH)		//画面が横から出たとき
		{
			//+1 なら、-1する/ -1ならば、+1にする
			XSpeed = -XSpeed;	//移動する向きを反転

			//壁に当たったら速くなる
			if (XSpeed > 0) { XSpeed += 2; }
			else if (XSpeed < 0) { XSpeed -= 2; }
		}

		if (Y - radius < 0 || Y + radius > GAME_HEIGHT)		//画面が横から出たとき
		{
			//+1 なら、-1する/ -1ならば、+1にする
			YSpeed = -YSpeed;	//移動する向きを反転

			//壁に当たったら速くなる
			if (YSpeed > 0) { YSpeed += 2; }
			else if (YSpeed < 0) { YSpeed -= 2; }
		}
		
		ScreenFlip();	//ダブルバッファリングした画面を描画
	}


	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	return 0;				// ソフトの終了 
}
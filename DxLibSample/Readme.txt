SSBPLibをDXライブラリで使うときのサンプルです。


DXライブラリは
	C:\DxLib_VC\DxLib
にインストールしているものとして設定してます。

設定してる箇所は次の2箇所です。
	プロジェクトのプロパティ
	--> 構成プロパティ
	--> C/C++
	--> 全般
	--> 追加のインクルード ディレクトリ：	C:\DxLib_VC\DxLib;
	
	プロジェクトのプロパティ
	--> 構成プロパティ
	--> リンカー
	--> 全般
	--> 追加のライブラリ ディレクトリ：	C:\DxLib_VC\DxLib;


ビルドが通らない場合、
	SSBPLibプロジェクトのプロパティ
	--> 構成プロパティ
	--> C/C++
	--> コード生成
	--> ランタイム ライブラリ：	マルチスレッド デバッグ(/MTd)
など、試してみてください。



サンプルで使用しているデータは、
	http://www.webtech.co.jp/help/ja/spritestudio/download/tool_sample_download/
	--> サンプルデータのダウンロード
	--> キャラクターアニメーションテンプレート
	--> OPTPiX_SpriteStudio_Animationtemplate_20150325.zip
	これに入っている character_template_sample1/ 以下のデータを使ってます。
	
	ssbpファイルと、pngがあれば動きます。
	ひとまず次のように配置して実行してみてください。
	DxLibSample/QuickStart/character_template_sample1/character_template1.ssbp
	DxLibSample/QuickStart/character_template_sample1/character_2head.png
	DxLibSample/QuickStart/character_template_sample1/character_2head.png




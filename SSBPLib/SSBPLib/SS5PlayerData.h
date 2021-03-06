﻿#pragma once

namespace ss{

/*
 * ssbpに含まれるデータの定義
 * バイナリからオフセット指定でキャストするので、変数の並び順は変えないこと！
 */

/**
 * 型定義
 */
typedef short			ss_s16;
typedef unsigned short	ss_u16;
typedef unsigned int	ss_u32;
typedef int				ss_s32;
typedef int				ss_offset;		//ProjectDataの先頭からのオフセット値でアドレスを表現



/**
 * プロジェクト情報(ssbpのルート。ここからcellやanimeにアクセスする)
 */
struct ProjectData
{
	ss_u32		dataId;			//ssbpファイルデータ確認用のID
	ss_u32		version;		//ssbpファイルのバージョン
	ss_u32		flags;			//予備
	ss_offset	imageBaseDir;	//リソースパス、コンバート時の引数で設定する: (data+imageBaseDir) --> const char*
	ss_offset	cells;			//セル構造体へのポインタ（配列の先頭アドレス）: (data+cells) --> const Cell*
	ss_offset	animePacks;		//アニメパック構造体へのポインタ（配列の先頭アドレス）: (data+animePacks) --> const AnimePackData*
	ss_s16		numCells;		//セルの数
	ss_s16		numAnimePacks;	//アニメーションの数
};

/*
ProjectData
 +--Cell[num]
 |   +--CellMap
 |
 +--AnimePackData[num]
     +--PartData[num]
     +--AnimationData[num]
         +--AnimationInitialData
         +--frameData[num]
*/

/* Cell -------------------------------------------------*/
/**
 * セルマップ(アトラス化されたテクスチャの情報)
 */
struct CellMap{
	ss_offset	name;			//セルマップ名(ssce名): (data+name) --> const char*
	ss_offset	imagePath;		//テクスチャファイル名: (data+imagePath) --> const char*
	ss_s16		index;			//セルマップ番号
	ss_s16		reserved;		//予備
};


/**
 * セル(画像パーツと思えばOK。必ずどれかのセルマップに属する)
 */
struct Cell{
	ss_offset	name;			//セル名: (data+name) --> const char*
	ss_offset	cellMap;		//所属するセルマップ構造体へのポインタ: (data+cellMap) --> const CellMap*
	ss_s16		indexInCellMap;	//セル番号
	ss_s16		x;				//セル画像内の開始X座標
	ss_s16		y;				//セル画像内の開始Y座標
	ss_u16		width;			//セルの幅
	ss_u16		height;			//セルの高さ
	ss_s16		reserved;		//予備
};



/* Animation --------------------------------------------*/
/*
   hoge.ssae      <-- AnimePackData::name
    +-- attack    <-- AnimationData::name
    +-- wait
	+-- run
	...
*/


/**
 * アニメーションパック情報	//memo:モーションごとに違うパーツセットには出来ないらしい
 */
struct AnimePackData{
	ss_offset	name;			//アニメーションファイル名(ssae名): (data+name) --> const char*
	ss_offset	parts;			//パーツ構造体へのポインタ: (data+parts) --> const PartData*
	ss_offset	animations;		//アニメ（モーション）構造体へのポインタ: (data+animations) --> const AnimationData*
	ss_s16		numParts;		//パーツ数
	ss_s16		numAnimations;	//モーション数
};


/**
 * アニメ情報
 */
struct AnimationData{
	ss_offset	name;			//モーション名(attackとかwaitとか): (data+name) --> const char*
	ss_offset	defaultData;	//パーツの初期値（配列の先頭アドレス）:(data+defaultData) --> const AnimationInitialData*
	ss_offset	frameData;		//各フレームのパーツデータ（配列の先頭アドレス）:(data+frameData) --> const ss_s16*
	ss_offset	userData;		//ユーザーデータ（配列の先頭アドレス）:(data+userData) --> const ss_s16*
	ss_offset	labelData;		//ラベルデータ（配列の先頭アドレス）:(data+labelData) --> const ss_s16*
	ss_s16		numFrames;		//総フレーム
	ss_s16		fps;			//再生スピード
	ss_s16		labelNum;		//ラベルデータの数
};
//frameDataは、データへのアドレス(offset)の配列になってる
//データは、[パーツインデックス,フラグ,...]となっており、フラグを見て、初期値と異なるデータかどうかが判定できる
//...のところは、初期値と異なるデータが入っている(つまり、可変長データとなる)
//[frameNのときのパーツ数だけ、データが並んでいる] <--- frameData[N]
//  データに何が入っているかは旧SS5Player::setFrameが参考になる
//  PART_FLAG_...のenum順かと思いきやcolorblendとvertextransformは順序違かったりするので注意

/**
 * 各パーツの初期値
 *  各フレームのデータは、初期値と違うかどうかのフラグを持っている
 *  違うときはそのフレームのデータを読み、同じときはAnimationInitialDataの値を使う
 */
struct AnimationInitialData{
	ss_s16		index;			//パーツインデックス、SS上のフレームコントロールの順番
	ss_s16		dummy;			//バイナリデータ出力用のアライメントダミーデータ
	ss_u32		flags;			//非表示、X反転、Y反転フラグ
	ss_s16		cellIndex;		//セルインデックス <-- アニメーションとセルの紐付けはココ!!
	ss_s16		positionX;		//X座標初期値
	ss_s16		positionY;		//Y座標初期値
	ss_s16		positionZ;		//Z座標初期値
	ss_u16		opacity;		//透明度初期値
	float		anchorX;		//原点Xオフセット＋セルに設定された原点Xオフセット初期値
	float		anchorY;		//原点Yオフセット＋セルに設定された原点Yオフセット初期値
	float		rotationX;		//X回転初期値
	float		rotationY;		//Y回転初期値
	float		rotationZ;		//Z回転初期値
	float		scaleX;			//X拡大率初期値
	float		scaleY;			//Y拡大率初期値
	float		size_X;			//Xサイズ初期値
	float		size_Y;			//Yサイズ初期値
	float		uv_move_X;		//UV X移動初期値
	float		uv_move_Y;		//UV Y移動初期値
	float		uv_rotation;	//UV回転初期値
	float		uv_scale_X;		//UV Xスケール初期値
	float		uv_scale_Y;		//UV Yスケール初期値
	float		boundingRadius;	//当たり半径初期値
};

/**
 * パーツ アニメーションの最小単位っぽい
 */
struct PartData{
	ss_offset	name;			///(data+name) --> const char*
	ss_s16		index;			/// SS内のパーツインデックス
	ss_s16		parentIndex;	/// 親のパーツインデックス
	ss_s16		type;			/// パーツ種別
	ss_s16		boundsType;		/// 当たり判定種類
	ss_s16		alphaBlendType;	/// BlendType
	ss_offset	refname;		/// (data+refname) --> const char*　インスタンスとして配置されるアニメーション名
};


};	// namespace ss


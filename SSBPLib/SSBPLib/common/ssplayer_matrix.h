﻿#pragma once

#include <array>
#include "SSBPLib/SS5PlayerTypes.h"

namespace ss{

/**
 * 行列計算
 *	vec * matrix --> vec の形。vecは横ベクトル
 *	
 *	memo:4*3にして最適化する余地あり
 */
class SSMatrix{
public:
	SSMatrix();

	std::array<float, 16> _m;
	
	
	//各種行列に整える
	SSMatrix& setupTranslation(float x, float y, float z=0);
	SSMatrix& setupRotationX(float radian);
	SSMatrix& setupRotationY(float radian);
	SSMatrix& setupRotationZ(float radian);
	SSMatrix& setupScale(float x, float y, float z=1);
	SSMatrix& setupIdentity();
	
	//平行移動だけは簡単な演算で済むので別枠で定義しておく
	void setTranslation(float x, float y, float z=0);			//平行移動成分をxyzで置き換え
	void zeroTranslation();										//平行移動成分を0移動に置き換え
	void addTranslation(float x, float y, float z=0);			//平行移動させる
	void getTranslation(float *x, float *y, float *z) const;	//平行移動成分を取り出す
	void getTranslation(float *x, float *y) const;				//平行移動成分を取り出す

	//スケール値の抽出
	void getScale(float *x, float *y, float *z) const;

	//回転の抽出
	void getRotation(float *x, float *y, float *z) const; //ラジアンでかえります
	SSMatrix getRotationMatrix() const;		//回転成分だけ行列として取り出す

	SSMatrix operator*(const SSMatrix &o) const;
	SSMatrix& operator*=(const SSMatrix &o);
	
};


SSVertex3F operator*(const SSVertex3F &p, const SSMatrix &m);
SSVertex3F &operator*=(SSVertex3F &p, const SSMatrix &m);


} // namespace ss



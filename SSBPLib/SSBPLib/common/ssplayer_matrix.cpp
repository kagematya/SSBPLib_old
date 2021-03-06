﻿#include "ssplayer_matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include "SSBPLib/SS5PlayerTypes.h"


namespace ss{

SSMatrix::SSMatrix(){
	setupIdentity();
}


/* 各種行列に整える ----------*/
SSMatrix& SSMatrix::setupTranslation(float x, float y, float z){
	_m[0] = 1.0f;	_m[1] = 0.0f;	_m[2] = 0.0f;	_m[3] = 0.0f;
	_m[4] = 0.0f;	_m[5] = 1.0f;	_m[6] = 0.0f;	_m[7] = 0.0f;
	_m[8] = 0.0f;	_m[9] = 0.0f;	_m[10] = 1.0f;	_m[11] = 0.0f;
	_m[12] = x;		_m[13] = y;		_m[14] = z;		_m[15] = 1.0f;
	return *this;
}

SSMatrix& SSMatrix::setupRotationX(float radian){
	float s = sin(radian);
	float c = cos(radian);
	
	_m[0] = 1.0f;	_m[1] = 0.0f;	_m[2] = 0.0f;	_m[3] = 0.0f;
	_m[4] = 0.0f;	_m[5] = c;		_m[6] = s;		_m[7] = 0.0f;
	_m[8] = 0.0f;	_m[9] = -s;		_m[10] = c;		_m[11] = 0.0f;
	_m[12] = 0.0f;	_m[13] = 0.0f;	_m[14] = 0.0f;	_m[15] = 1.0f;
	return *this;
}

SSMatrix& SSMatrix::setupRotationY(float radian){
	float s = sin(radian);
	float c = cos(radian);

	_m[0] = c;		_m[1] = 0.0f;	_m[2] = -s;		_m[3] = 0.0f;
	_m[4] = 0.0f;	_m[5] = 1.0f;	_m[6] = 0.0f;	_m[7] = 0.0f;
	_m[8] = s;		_m[9] = 0.0f;	_m[10] = c;		_m[11] = 0.0f;
	_m[12] = 0.0f;	_m[13] = 0.0f;	_m[14] = 0.0f;	_m[15] = 1.0f;
	return *this;
}

SSMatrix& SSMatrix::setupRotationZ(float radian){
	float s = sin(radian);
	float c = cos(radian);

	_m[0] = c;		_m[1] = s;		_m[2] = 0.0f;	_m[3] = 0.0f;
	_m[4] = -s;		_m[5] = c;		_m[6] = 0.0f;	_m[7] = 0.0f;
	_m[8] = 0.0f;	_m[9] = 0.0f;	_m[10] = 1.0f;	_m[11] = 0.0f;
	_m[12] = 0.0f;	_m[13] = 0.0f;	_m[14] = 0.0f;	_m[15] = 1.0f;
	return *this;
}

SSMatrix& SSMatrix::setupScale(float x, float y, float z){
	_m[0] = x;		_m[1] = 0.0f;	_m[2] = 0.0f;	_m[3] = 0.0f;
	_m[4] = 0.0f;	_m[5] = y;		_m[6] = 0.0f;	_m[7] = 0.0f;
	_m[8] = 0.0f;	_m[9] = 0.0f;	_m[10] = z;		_m[11] = 0.0f;
	_m[12] = 0.0f;	_m[13] = 0.0f;	_m[14] = 0.0f;	_m[15] = 1.0f;
	return *this;
}
	
SSMatrix& SSMatrix::setupIdentity(){
	_m[0] = 1.0f;	_m[1] = 0.0f;	_m[2] = 0.0f;	_m[3] = 0.0f;
	_m[4] = 0.0f;	_m[5] = 1.0f;	_m[6] = 0.0f;	_m[7] = 0.0f;
	_m[8] = 0.0f;	_m[9] = 0.0f;	_m[10] = 1.0f;	_m[11] = 0.0f;
	_m[12] = 0.0f;	_m[13] = 0.0f;	_m[14] = 0.0f;	_m[15] = 1.0f;
	return *this;
}
	

/* 平行移動だけは簡単な演算で済むので別枠で定義しておく ----------*/
void SSMatrix::setTranslation(float x, float y, float z){			//平行移動成分をxyzで置き換え
	_m[12] = x;		_m[13] = y;		_m[14] = z;		//_m[15] = 1.0f;
}

void SSMatrix::zeroTranslation(){									//平行移動成分を0移動に置き換え
	setTranslation(0,0,0);
}

void SSMatrix::addTranslation(float x, float y, float z){			//平行移動させる
	//memo:作業内容はthis * setuptransration(x,y,z) なのだが、計算するとここの成分しか変わらない
	_m[12] += x;	_m[13] += y;	_m[14] = +z;	//_m[15] = 1.0f;
}

void SSMatrix::getTranslation(float *x, float *y, float *z) const{	//平行移動成分を取り出す
	*x = _m[12];	*y = _m[13];	*z = _m[14];
}
void SSMatrix::getTranslation(float *x, float *y) const{			//平行移動成分を取り出す
	*x = _m[12];	*y = _m[13];
}

/* スケール ------------------------------------------------------*/
void SSMatrix::getScale(float *x, float *y, float *z) const
{
	*x = sqrt( _m[0]*_m[0] + _m[1]*_m[1] + _m[2]*_m[2] );
	*y = sqrt( _m[4]*_m[4] + _m[5]*_m[5] + _m[6]*_m[6] );
	*z = sqrt( _m[8]*_m[8] + _m[9]*_m[9] + _m[10]*_m[10] );
}

/* 回転 ----------------------------------------------------------*/
void SSMatrix::getRotation(float *x, float *y, float *z) const
{
	static const float THRESHOLD = 0.001;
	//ZYXの順の回転行列のをオイラー角に戻す
	
	//回転行列だけにして考える
	SSMatrix rotMat = getRotationMatrix();
	float sin_y = rotMat._m[8];
	
	if(abs(sin_y - 1) < THRESHOLD){			//sin_y == 1
		*x = 0;
		*y = 3.141592653589793 / 2;
		*z = atan2(rotMat._m[1], rotMat._m[5]);
	}
	else if(abs(sin_y + 1) < THRESHOLD){	//sin_y == -1
		*x = 0;
		*y = -3.141592653589793 / 2;
		*z = atan2(rotMat._m[1], rotMat._m[5]);
	}
	else{
		*x = atan2(-rotMat._m[9], rotMat._m[10]);
		*y = asin(sin_y);
		*z = atan2(-rotMat._m[4], rotMat._m[0]);
	}
}

SSMatrix SSMatrix::getRotationMatrix() const
{
	/*
	  スケールS, 回転R, 平行移動T の行列はこうなってる
	  
	        SxR00, SxR01, SxR02, 0
	  mat = SyR10, SyR11, SyR12, 0
	        SzR20, SzR21, SzR22, 0
			Tx,    Ty,    Tz,    1
	  
	  要素を打ち消してR成分だけにする
	*/
	SSMatrix rotMat = *this;
	rotMat.setTranslation(0, 0, 0);
	
	float sx, sy, sz;
	rotMat.getScale(&sx, &sy, &sz);
	sx = (sx==0)? sx : 1.0f/sx;		//0除算回避。スケール0なら行が0,0,0になってるはず
	sy = (sy==0)? sy : 1.0f/sy;
	sz = (sz==0)? sz : 1.0f/sz;
	rotMat._m[0] *= sx;		rotMat._m[1] *= sx; 	rotMat._m[2] *= sx;
	rotMat._m[4] *= sy;		rotMat._m[5] *= sy; 	rotMat._m[6] *= sy;
	rotMat._m[8] *= sz;		rotMat._m[9] *= sz; 	rotMat._m[10] *= sz;

	return rotMat;
}


/* 演算子の定義 ----------*/
SSMatrix SSMatrix::operator*(const SSMatrix &o) const{
	SSMatrix tmp;

	tmp._m[0]  = _m[0]*o._m[0] + _m[1]*o._m[4] + _m[2]*o._m[8] + _m[3]*o._m[12];
	tmp._m[1]  = _m[0]*o._m[1] + _m[1]*o._m[5] + _m[2]*o._m[9] + _m[3]*o._m[13];
	tmp._m[2]  = _m[0]*o._m[2] + _m[1]*o._m[6] + _m[2]*o._m[10] + _m[3]*o._m[14];
	tmp._m[3]  = _m[0]*o._m[3] + _m[1]*o._m[7] + _m[2]*o._m[11] + _m[3]*o._m[15];

	tmp._m[4]  = _m[4]*o._m[0] + _m[5]*o._m[4] + _m[6]*o._m[8] + _m[7]*o._m[12];
	tmp._m[5]  = _m[4]*o._m[1] + _m[5]*o._m[5] + _m[6]*o._m[9] + _m[7]*o._m[13];
	tmp._m[6]  = _m[4]*o._m[2] + _m[5]*o._m[6] + _m[6]*o._m[10] + _m[7]*o._m[14];
	tmp._m[7]  = _m[4]*o._m[3] + _m[5]*o._m[7] + _m[6]*o._m[11] + _m[7]*o._m[15];
	
	tmp._m[8]  = _m[8]*o._m[0] + _m[9]*o._m[4] + _m[10]*o._m[8] + _m[11]*o._m[12];
	tmp._m[9]  = _m[8]*o._m[1] + _m[9]*o._m[5] + _m[10]*o._m[9] + _m[11]*o._m[13];
	tmp._m[10] = _m[8]*o._m[2] + _m[9]*o._m[6] + _m[10]*o._m[10] + _m[11]*o._m[14];
	tmp._m[11] = _m[8]*o._m[3] + _m[9]*o._m[7] + _m[10]*o._m[11] + _m[11]*o._m[15];
	//平行移動成分
	tmp._m[12] = _m[12]*o._m[0] + _m[13]*o._m[4] + _m[14]*o._m[8] + _m[15]*o._m[12];
	tmp._m[13] = _m[12]*o._m[1] + _m[13]*o._m[5] + _m[14]*o._m[9] + _m[15]*o._m[13];
	tmp._m[14] = _m[12]*o._m[2] + _m[13]*o._m[6] + _m[14]*o._m[10] + _m[15]*o._m[14];
	tmp._m[15] = _m[12]*o._m[3] + _m[13]*o._m[7] + _m[14]*o._m[11] + _m[15]*o._m[15];

	return tmp;
}

SSMatrix& SSMatrix::operator*=(const SSMatrix &o){
	*this = *this * o;
	return *this;
}



//頂点変換
SSVertex3F operator*(const SSVertex3F &p, const SSMatrix &m){

	//(x,y,z,w=1)*matrix の計算。実際にはwはないのでごまかしつつ計算
	return SSVertex3F(
		p.x*m._m[0] + p.y*m._m[4] + p.z*m._m[8] + m._m[12],
		p.x*m._m[1] + p.y*m._m[5] + p.z*m._m[9] + m._m[13],
		p.x*m._m[2] + p.y*m._m[6] + p.z*m._m[10] + m._m[14]
	);
}

SSVertex3F &operator*=(SSVertex3F &p, const SSMatrix &m){
	p = p * m;
	return p;
}



} //namespace ss

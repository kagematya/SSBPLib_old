﻿#pragma once

#include <assert.h>
#include <math.h>

namespace ss{
class DataArrayReader;

//素のままだと扱いにくいのでmemsetは諦めて便利関数追加してく


using TextureID = long;	//テクスチャの識別子


/**
 * カラー構造体
 */
typedef struct _SSColor4B{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	using uchar = unsigned char;
	_SSColor4B() :r(0), g(0), b(0), a(0){}
	_SSColor4B(uchar r_, uchar g_, uchar b_, uchar a_) :r(r_), g(g_), b(b_), a(a_){}
	
	//カラーの読み取り
	void readColor(DataArrayReader &reader);
	//カラーの読み取り(rateを考慮して読む)
	void readColorWithRate(DataArrayReader &reader);
	
	//unsigned longにパラメータを固める
	unsigned long pack() const;

} SSColor4B;


/**
 * 頂点座標構造体
 */
typedef struct _SSVertex3F{
	float x;
	float y;
	float z;

	_SSVertex3F() :x(0), y(0), z(0){}
	_SSVertex3F(float x_, float y_, float z_) : x(x_), y(y_), z(z_){}
	_SSVertex3F(const _SSVertex3F &o) : x(o.x), y(o.y), z(o.z){}

	_SSVertex3F& operator =(const _SSVertex3F &o){
		x=o.x;	y=o.y;	z=o.z;
		return *this;
	}
	bool operator ==(const _SSVertex3F &o) const{
		return (x==o.x && y==o.y && z==o.z);
	}
	bool operator !=(const _SSVertex3F &o) const{
			return !(*this == o);
	}

	const _SSVertex3F operator -() const{
		return _SSVertex3F(-x,-y,-z);
	}
	const _SSVertex3F operator +(const _SSVertex3F &o) const{
		return _SSVertex3F(x+o.x, y+o.y, z+o.z);
	}
	const _SSVertex3F operator -(const _SSVertex3F &o) const{
		return _SSVertex3F(x-o.x, y-o.y, z-o.z);
	}
	const float operator *(const _SSVertex3F &o) const{	//内積
		return x*o.x + y*o.y + z*o.z;
	}
	const _SSVertex3F operator *(float s) const{
		return _SSVertex3F(x*s, y*s, z*s);
	}
	const _SSVertex3F operator /(float s) const{
		assert(s);								//ゼロ除算チェック
		float oneOverS = 1.0f / s;
		return _SSVertex3F(x*oneOverS, y*oneOverS, z*oneOverS);
	}
	
	_SSVertex3F& operator +=(const _SSVertex3F &o){
		x += o.x;	y += o.y;	z += o.z;
		return *this;
	}
	_SSVertex3F &operator -=(const _SSVertex3F &o){
		x -= o.x;	y -= o.y;	z -= o.z;
		return *this;
	}
	_SSVertex3F &operator *=(float s){
		x*=s;	y*=s;	z*=s;
		return *this;
	}
	_SSVertex3F &operator /=(float s){
		assert(s);
		float oneOverS = 1.0f / s;
		x *= oneOverS;	y *= oneOverS;	z *= oneOverS;
		return *this;
	}
} SSVertex3F;

//! スカラー乗算の対称性のために定義しておく
inline const SSVertex3F operator *(float s, const SSVertex3F &o){
	return SSVertex3F(s*o.x, s*o.y, s*o.z);
}



/**
 * ４頂点座標構造体
 */
typedef struct _SSQuad3 {
	SSVertex3F        bl;
	SSVertex3F        br;
	SSVertex3F        tl;
	SSVertex3F        tr;

	//頂点オフセットの読み取り
	void readVertexTransform(DataArrayReader &reader);
} SSQuad3;


/**
 * UV構造体 ベクタの実装のコピペになっているが・・・
 */
typedef struct _SSTex2F {	
	float u;
	float v;

	_SSTex2F() :u(0), v(0){}
	_SSTex2F(float u_, float v_) :u(u_), v(v_){}
	_SSTex2F(const _SSTex2F& o) : u(o.u), v(o.v) {}
	
	_SSTex2F &operator =(const _SSTex2F &o){
		u = o.u;
		v = o.v;
		return *this;
	}
	bool operator ==(const _SSTex2F &o) const{
		return (u==o.u && v==o.v);
	}
	bool operator !=(const _SSTex2F &o) const{
		return (u!=o.u || v!=o.v);
	}
	
	const _SSTex2F operator -() const{
		return _SSTex2F(-u, -v);
	}
	const _SSTex2F operator +(const _SSTex2F &o) const{
		return _SSTex2F(u+o.u, v+o.v);
	}
	const _SSTex2F operator -(const _SSTex2F &o) const{
		return _SSTex2F(u-o.u, v-o.v);
	}
	const float operator *(const _SSTex2F &o) const{	//内積
		return u*o.u + v*o.v;
	}
	const _SSTex2F operator *(float s) const{
		return _SSTex2F(u*s, v*s);
	}
	const _SSTex2F operator /(float s) const{
		assert(s);								//ゼロ除算チェック
		float oneOverS = 1.0f / s;
		return _SSTex2F(u*oneOverS, v*oneOverS);
	}
	
	_SSTex2F &operator +=(const _SSTex2F &o){
		u += o.u;	v += o.v;
		return *this;
	}
	_SSTex2F &operator -=(const _SSTex2F &o){
		u -= o.u;	v -= o.v;
		return *this;
	}
	_SSTex2F &operator *=(float s){
		u *= s;		v *= s;
		return *this;
	}
	_SSTex2F &operator /=(float s){
		assert(s);
		float oneOverS = 1.0f / s;
		u *= oneOverS;	v *= oneOverS;
		return *this;
	}

		//! 回転させる
	 void rotate(float angleRadian){
		 float tu = u;
		 u = tu * cos(angleRadian) - v *sin(angleRadian);
		 v = tu * sin(angleRadian) + v * cos(angleRadian);
	 }
	 //! 基準点を中心として回転させる
	 void rotate(float angleRadian, const _SSTex2F &offset){
		 *this -= offset;
		 rotate(angleRadian);
		 *this += offset;
	 }
	
} SSTex2F;


/**
 * 頂点構造体
 */
typedef struct _ccV3F_C4B_T2F{
	SSVertex3F			vertices;	/// 座標
	SSColor4B			colors;		/// カラー
	SSTex2F				texCoords;	/// UV
} SSV3F_C4B_T2F;


/**
 * ４頂点構造体
 */
struct SSV3F_C4B_T2F_Quad{
	SSV3F_C4B_T2F    tl;
	SSV3F_C4B_T2F    bl;
	SSV3F_C4B_T2F    tr;
	SSV3F_C4B_T2F    br;

	//必要なものだけ追加しとく
	SSV3F_C4B_T2F_Quad& operator +=(const _SSQuad3 &o){	//4頂点の加算
		tl.vertices += o.tl;
		bl.vertices += o.bl;
		tr.vertices += o.tr;
		br.vertices += o.br;
		return *this;
	}

	//vertex各要素に演算を適用させる
	template<class F>
	void vertexForeach(F func){
		func(tl.vertices);
		func(tr.vertices);
		func(bl.vertices);
		func(br.vertices);
	}

	//uv各要素に演算を適用させる
	template<class F>
	void uvForeach(F func){
		func(tl.texCoords);
		func(tr.texCoords);
		func(bl.texCoords);
		func(br.texCoords);
	}

	//color各要素に演算を適用させる
	template<class F>
	void colorsForeach(F func){
		func(tl.colors);
		func(tr.colors);
		func(bl.colors);
		func(br.colors);
	}

};



};	// namespace ss


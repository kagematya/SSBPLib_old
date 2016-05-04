#pragma once


namespace ss{
class DataArrayReader;
class ToPointer;


/**
 * UserData
 */
struct UserData{
	
	enum {
		FLAG_INTEGER	= 1 << 0,
		FLAG_RECT		= 1 << 1,
		FLAG_POINT		= 1 << 2,
		FLAG_STRING		= 1 << 3
	};

	int			m_flags;		/// 含まれるデータの種類
	int			m_partIndex;	/// このデータのパーツインデックス
	int			m_integer;		/// 整数
	int			m_rect[4];		/// 矩形データ
	int			m_point[2];		/// 座標データ
	int			m_strSize;		/// 文字列のサイズ (byte count)
	const char*	m_str;			/// 文字列 (zero terminated)


	//readerを介してデータを読み取る
	void readData(DataArrayReader &reader, const ToPointer &ptr);

private:
	//ゼロクリア
	void init();

};


} //namespace ss

#pragma once

#include <assert.h>

namespace ss{

/**
 * 定数
 */
static const float ssPI = 3.14159265358979323846f;
static const float ss2PI = ssPI * 2;
#define DegreeToRadian(Degree) ((float)( Degree * ss2PI )/ 360.0f )
#define RadianToDegree(Radian) ((float)( Radian * 360.0f) / ss2PI)


#define SS_SAFE_DELETE(p)            do { if(p) { delete (p); (p) = 0; } } while(0)
#define SS_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = 0; } } while(0)


#ifdef _DEBUG
	#define SSLOG(...)       do {} while (0)
	#define SS_ASSERT(cond)    assert(cond)
	#define SS_ASSERT2(cond, msg) SS_ASSERT(cond)
	#define SSLOGERROR(format,...)  do {} while (0)
#else
	#define SSLOG(...)       do {} while (0)
	#define SS_ASSERT(cond)
	#define SS_ASSERT2(cond, msg) ((void)(cond))
	#define SSLOGERROR(format,...)  do {} while (0)
#endif



//配列のサイズを返す
template<class TYPE, size_t N>
size_t lengthof(const TYPE (&ar)[N]){
	return N;
}

//[minVal:maxVal]の範囲にする
template<class T>
T clamp(T val, T minVal, T maxVal){
	assert(minVal <= maxVal);
	return std::min(std::max(val, minVal), maxVal);
}

//[minVal:maxVal)の範囲でループさせる
template<class T>
T wrap(T val, T minVal, T maxVal){
	assert(minVal < maxVal);
	int n = (val - minVal) % (maxVal - minVal);
	return (n >= 0) ? (n + minVal) : (n+maxVal);
}

template<class T>
T fwrap(T val, T minVal, T maxVal){
	assert(minVal < maxVal);
    double n = fmod(val-minVal, maxVal-minVal);
    return (n>=0)? (n+minVal) : (n+maxVal);
}


} //namespace ss

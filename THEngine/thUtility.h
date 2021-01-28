#pragma once
#include "IDXDefines.h"

/*
** Common math constants
*/
#define TH_PI	3.14159265358979323846f
#define TH_PI_2	1.57079632679489661923f
#define TH_PI_4	0.785398163397448309616f
#define TH_1_PI	0.318309886183790671538f
#define TH_2_PI	0.636619772367581343076f

// 碰撞优化
#define INTERSECT_NEGATIVEY		0x10
#define INTERSECT_HORIZENTAL	0x30 

const float FLOAT_EPS = 0.001f;
// 定义一个2D向量
template<typename T>
struct  thVector2
{
public:
	// 构造
	thVector2():x(0),y(0){};
	thVector2(const T *f);
	thVector2(T x ,T y);
	thVector2(const thVector2& vec);

	// 操作符
	thVector2 operator+ (const thVector2& other) const;
	thVector2 operator- (const thVector2& other) const;
	thVector2& operator+= (const thVector2& other);
	thVector2& operator-= (const thVector2& other);

	thVector2 operator+ () const;
	thVector2 operator- () const;

	thVector2 operator* (T f);
	thVector2 operator/ (T f);
	thVector2& operator*= (T f);
	thVector2& operator/= (T f);

	bool operator== (const thVector2& other) const;
	bool operator!= (const thVector2& other) const;
	bool operator> (const thVector2& other) const;
	float operator^ (const thVector2& other) const;

	//清空
	void Clear();
	// 设置向量
	void SetVector(const thVector2 &vec);
	void SetVector(T x, T y);
	// 偏移
	void Offset(T x,T y);
	void Offset(T l);
	// 反向量
	void Inverse();
	// 向量长度的平方
	T LengthSq()const;
	// 向量长度
	T Length()const;
	// 单位化向量
	void Normalize();
	// 是否单位向量
	bool IsNormalized()const;
	// 点乘
	float Dot(const thVector2 &vec);
	// 夹角
	float Angle(const thVector2 &vec) const;
	// 旋转
	thVector2& Rotate(float a);
	// 是否空向量
	bool empty()const;

public:
	T x,y;
};

//// 定义一个3D向量
//typedef struct thVector3Flt : public D3DXVECTOR3
//{
//	thVector3Flt() {
//		x = y = z = 0.0f;
//	};
//	thVector3Flt(float _x, float _y,float _z) {
//		x = _x; y = _y;z = _z;
//	};
//	thVector3Flt( CONST D3DXVECTOR3 &v ) {
//		x = v.x; y = v.y; z = v.z;
//	};
//	//清空
//	void Clear();
//	// 设置向量
//	void SetVector(const thVector3Flt &vec);
//	void SetVector(float _x, float _y,float _z);
//	// 偏移
//	void Offset(float _x, float _y,float _z);
//	void Offset(float _l);
//	// 反向
//	void Inverse();
//	// 向量长度
//	float Length()const;
//	// 向量长度的平方
//	float LengthSq()const;
//	// 单位化向量
//	void Normalize();
//	// 是否单位向量
//	bool IsNormalized()const;
//	// 是否空向量
//	bool empty()const;
//
//}Vector3Flt;

// 定义一个rect
template<typename T>
struct  thVector4
{
public:
	// 构造
	thVector4():left(0),top(0),right(0),bottom(0){};
	thVector4(const T *f);
	thVector4(T l ,T t, T r ,T b);
	thVector4(const thVector4& vec);

	// 操作符
	thVector4 operator+ (const thVector4& other) const;
	thVector4 operator- (const thVector4& other) const;
	thVector4& operator+= (const thVector4& other);
	thVector4& operator-= (const thVector4& other);

	thVector4 operator+ () const;
	thVector4 operator- () const;

	thVector4 operator* (T f);
	thVector4 operator/ (T f);
	thVector4& operator*= (T f);
	thVector4& operator/= (T f);

	bool operator== (const thVector4& other) const;
	bool operator!= (const thVector4& other) const;

	//清空
	void Clear();
	// 是否空向量
	bool empty()const;
	// 设置向量
	void SetVector(const thVector4 &vec);
	// 设置向量
	void SetVector(T l, T t, T r, T b);
	// 偏移
	void Offset(T x,T y);
	void Offset(T l);
	// 测试点进入
	bool TestPoint(T x, T y) const;
	// 相交
	bool Intersect(const thVector4 *vec) const;
	// 获得左上点
	thVector2<T> GetLUp() const;
	// 获得右上点
	thVector2<T> GetRUp() const;
	// 获得左下点
	thVector2<T> GetLDown() const;
	// 获得右下点
	thVector2<T> GetRDown() const;
	// 获得宽
	T	 GetWidth() { return right - left; }
	// 获得高
	T	 GetHeight() { return bottom - top; }

public:
	T left,top,right,bottom;
};

//作用一个范围
template<typename T>
struct thRange
{
public:
	//构造函数
	thRange();

	//初始化
	void InitRange( T _PosX , T _PosY , T _Range );
	//设置位置
	void SetPos( T _PosX , T _PosY );
	//设置范围
	void SetRange( T _Range );
	//测试是否进入区域
	bool TestRange( T _PosX , T _PosY );
	//2个范围是否接触
	bool Intersect( thRange<T> &RangeTest );

	//返回范围
	T	 GetRange();
public:
	thVector2<T> m_Pos;
	T m_Range;	//范围
};

//函数
TH_INLINE
float VECTOR2_Dot(thVector2<float>& vec1 ,thVector2<float>& vec2)	//点乘
{
	return float(vec1.x*vec2.x + vec1.y*vec2.y);
}

TH_INLINE
float VECTOR2_Angle(thVector2<float>& vec1 ,thVector2<float>& vec2)	//向量夹角
{
	return (float)acosf(VECTOR2_Dot(vec1,vec2)/(vec1.Length()*vec2.Length()));
}

TH_INLINE
bool VECTOR2_Equal(float f1, float f2)	
{
	return (abs(f1 - f2) < 1e-4f); 
}

// 字节数据处理
// 数据加入不能太大,连续的内存分配可能是不够的
typedef	struct thByteStream /* Support small storage Try to less than < 2097152(2G) */
{
public:
	// 构造函数
	thByteStream();
	// 析构函数
	~thByteStream();
	// 根据所给数据创建一个对象
	thByteStream( const BYTE *pDat , size_t Size );
	thByteStream( const thByteStream *pByte );
	thByteStream( size_t Size );
	// 算术重载
	BYTE	 &operator[] ( const unsigned int Index);
	thByteStream   operator+  ( const BYTE& cbByte );
	thByteStream   operator+  ( const thByteStream& _thByteStream );
	thByteStream & operator+= ( const BYTE& cbByte );
	thByteStream & operator+= ( const thByteStream& _thByteStream  );
	thByteStream & operator=  ( const thByteStream & _thByteStream );
	// 是否为空
	bool	 IsEmpty() const;
	// 获取长度
	size_t	 GetLength() const;
	// 容器大小
	size_t	 Size() const;
	// 设置长度
	void	 Setlength( const size_t Size );
	// 设置数据
	void	 SetData( const void *pDat , size_t Size );
	void	 SetData( const thByteStream& T );
	// 附加数据
	void	 ApendData( const void *pDat , size_t Size );
	template< class _TYPE >
	void	 ApendData( const _TYPE *pType );
	// 获得数据
	const BYTE * GetData() const;
	BYTE*	 GetData();
	// 加上偏移
	BYTE*	 GetData( size_t Offset );
	// 获得一段数据 malloc分配 需要手动释放
	void*	 GetData( size_t Size, size_t Offset = 0 );
	// 获取指定数据类型
	template< class _TYPE >
	_TYPE *  GetData( size_t Offset = 0 ) const;

private:
	// 缓冲数据
	std::vector<BYTE> m_vcbData;

}*LPTHBYTE;

/// 时间控制器
/// ms 以 毫秒为单位
/// s  以 秒为单位
class thTimer /* timeGetTime() calculate */
{
public:
	thTimer(void):m_dwLastTime(0),
		m_bClose(true),m_iCurFPS(0),m_iFPS(0){};

	/// 开始计时
	void Begin(void);
	/// 结束
	void End(void);
	/// 重置
	void Reset(void);

	/// 是否已经开始计时
	bool IsBegin(void) const;
	/// 是否已经过去 time 时间
	bool IsPassTimeMs(DWORD time) const;
	bool IsPassTimes(DWORD time) const;

	/// 获取当前流失的时间
	DWORD GetCurrentTimeMs(void) const;
	/// 获得开始计时的时间
	DWORD GetStartTimeMs(void) const;
	/// 获得过去的时间
	DWORD GetLostTimeMs(void) const;

	float GetCurrentTimes(void) const;
	float GetStartTimes(void) const;
	float GetLostTimes(void) const;

	/// 游戏FPS计算
	/// 开始计算
	void	BeginFPS(void);
	/// 结束计算
	void	EndFPS(void);
	/// 获得FPS
	int		GetFPS(void);

private:
	DWORD m_dwLastTime;		/// 上次时间
	bool  m_bClose;			/// 关闭
	/// FPS
	int	  m_iCurFPS;
	int	  m_iFPS;
};

typedef thVector2<float>Vector2Flt;
typedef thVector2<int>	Vector2Int;
typedef thVector2<DWORD>Vector2Dwd;
typedef thVector4<float>Vector4Flt;
typedef thVector4<int>	Vector4Int;
typedef thVector4<DWORD>Vector4Dwd;
typedef thRange<float>	RangeFlt;
typedef thRange<int>	RangeInt;
typedef thRange<DWORD>	RangeDwd;

#include "thUtility.inl"

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

// ��ײ�Ż�
#define INTERSECT_NEGATIVEY		0x10
#define INTERSECT_HORIZENTAL	0x30 

const float FLOAT_EPS = 0.001f;
// ����һ��2D����
template<typename T>
struct  thVector2
{
public:
	// ����
	thVector2():x(0),y(0){};
	thVector2(const T *f);
	thVector2(T x ,T y);
	thVector2(const thVector2& vec);

	// ������
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

	//���
	void Clear();
	// ��������
	void SetVector(const thVector2 &vec);
	void SetVector(T x, T y);
	// ƫ��
	void Offset(T x,T y);
	void Offset(T l);
	// ������
	void Inverse();
	// �������ȵ�ƽ��
	T LengthSq()const;
	// ��������
	T Length()const;
	// ��λ������
	void Normalize();
	// �Ƿ�λ����
	bool IsNormalized()const;
	// ���
	float Dot(const thVector2 &vec);
	// �н�
	float Angle(const thVector2 &vec) const;
	// ��ת
	thVector2& Rotate(float a);
	// �Ƿ������
	bool empty()const;

public:
	T x,y;
};

//// ����һ��3D����
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
//	//���
//	void Clear();
//	// ��������
//	void SetVector(const thVector3Flt &vec);
//	void SetVector(float _x, float _y,float _z);
//	// ƫ��
//	void Offset(float _x, float _y,float _z);
//	void Offset(float _l);
//	// ����
//	void Inverse();
//	// ��������
//	float Length()const;
//	// �������ȵ�ƽ��
//	float LengthSq()const;
//	// ��λ������
//	void Normalize();
//	// �Ƿ�λ����
//	bool IsNormalized()const;
//	// �Ƿ������
//	bool empty()const;
//
//}Vector3Flt;

// ����һ��rect
template<typename T>
struct  thVector4
{
public:
	// ����
	thVector4():left(0),top(0),right(0),bottom(0){};
	thVector4(const T *f);
	thVector4(T l ,T t, T r ,T b);
	thVector4(const thVector4& vec);

	// ������
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

	//���
	void Clear();
	// �Ƿ������
	bool empty()const;
	// ��������
	void SetVector(const thVector4 &vec);
	// ��������
	void SetVector(T l, T t, T r, T b);
	// ƫ��
	void Offset(T x,T y);
	void Offset(T l);
	// ���Ե����
	bool TestPoint(T x, T y) const;
	// �ཻ
	bool Intersect(const thVector4 *vec) const;
	// ������ϵ�
	thVector2<T> GetLUp() const;
	// ������ϵ�
	thVector2<T> GetRUp() const;
	// ������µ�
	thVector2<T> GetLDown() const;
	// ������µ�
	thVector2<T> GetRDown() const;
	// ��ÿ�
	T	 GetWidth() { return right - left; }
	// ��ø�
	T	 GetHeight() { return bottom - top; }

public:
	T left,top,right,bottom;
};

//����һ����Χ
template<typename T>
struct thRange
{
public:
	//���캯��
	thRange();

	//��ʼ��
	void InitRange( T _PosX , T _PosY , T _Range );
	//����λ��
	void SetPos( T _PosX , T _PosY );
	//���÷�Χ
	void SetRange( T _Range );
	//�����Ƿ��������
	bool TestRange( T _PosX , T _PosY );
	//2����Χ�Ƿ�Ӵ�
	bool Intersect( thRange<T> &RangeTest );

	//���ط�Χ
	T	 GetRange();
public:
	thVector2<T> m_Pos;
	T m_Range;	//��Χ
};

//����
TH_INLINE
float VECTOR2_Dot(thVector2<float>& vec1 ,thVector2<float>& vec2)	//���
{
	return float(vec1.x*vec2.x + vec1.y*vec2.y);
}

TH_INLINE
float VECTOR2_Angle(thVector2<float>& vec1 ,thVector2<float>& vec2)	//�����н�
{
	return (float)acosf(VECTOR2_Dot(vec1,vec2)/(vec1.Length()*vec2.Length()));
}

TH_INLINE
bool VECTOR2_Equal(float f1, float f2)	
{
	return (abs(f1 - f2) < 1e-4f); 
}

// �ֽ����ݴ���
// ���ݼ��벻��̫��,�������ڴ��������ǲ�����
typedef	struct thByteStream /* Support small storage Try to less than < 2097152(2G) */
{
public:
	// ���캯��
	thByteStream();
	// ��������
	~thByteStream();
	// �����������ݴ���һ������
	thByteStream( const BYTE *pDat , size_t Size );
	thByteStream( const thByteStream *pByte );
	thByteStream( size_t Size );
	// ��������
	BYTE	 &operator[] ( const unsigned int Index);
	thByteStream   operator+  ( const BYTE& cbByte );
	thByteStream   operator+  ( const thByteStream& _thByteStream );
	thByteStream & operator+= ( const BYTE& cbByte );
	thByteStream & operator+= ( const thByteStream& _thByteStream  );
	thByteStream & operator=  ( const thByteStream & _thByteStream );
	// �Ƿ�Ϊ��
	bool	 IsEmpty() const;
	// ��ȡ����
	size_t	 GetLength() const;
	// ������С
	size_t	 Size() const;
	// ���ó���
	void	 Setlength( const size_t Size );
	// ��������
	void	 SetData( const void *pDat , size_t Size );
	void	 SetData( const thByteStream& T );
	// ��������
	void	 ApendData( const void *pDat , size_t Size );
	template< class _TYPE >
	void	 ApendData( const _TYPE *pType );
	// �������
	const BYTE * GetData() const;
	BYTE*	 GetData();
	// ����ƫ��
	BYTE*	 GetData( size_t Offset );
	// ���һ������ malloc���� ��Ҫ�ֶ��ͷ�
	void*	 GetData( size_t Size, size_t Offset = 0 );
	// ��ȡָ����������
	template< class _TYPE >
	_TYPE *  GetData( size_t Offset = 0 ) const;

private:
	// ��������
	std::vector<BYTE> m_vcbData;

}*LPTHBYTE;

/// ʱ�������
/// ms �� ����Ϊ��λ
/// s  �� ��Ϊ��λ
class thTimer /* timeGetTime() calculate */
{
public:
	thTimer(void):m_dwLastTime(0),
		m_bClose(true),m_iCurFPS(0),m_iFPS(0){};

	/// ��ʼ��ʱ
	void Begin(void);
	/// ����
	void End(void);
	/// ����
	void Reset(void);

	/// �Ƿ��Ѿ���ʼ��ʱ
	bool IsBegin(void) const;
	/// �Ƿ��Ѿ���ȥ time ʱ��
	bool IsPassTimeMs(DWORD time) const;
	bool IsPassTimes(DWORD time) const;

	/// ��ȡ��ǰ��ʧ��ʱ��
	DWORD GetCurrentTimeMs(void) const;
	/// ��ÿ�ʼ��ʱ��ʱ��
	DWORD GetStartTimeMs(void) const;
	/// ��ù�ȥ��ʱ��
	DWORD GetLostTimeMs(void) const;

	float GetCurrentTimes(void) const;
	float GetStartTimes(void) const;
	float GetLostTimes(void) const;

	/// ��ϷFPS����
	/// ��ʼ����
	void	BeginFPS(void);
	/// ��������
	void	EndFPS(void);
	/// ���FPS
	int		GetFPS(void);

private:
	DWORD m_dwLastTime;		/// �ϴ�ʱ��
	bool  m_bClose;			/// �ر�
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

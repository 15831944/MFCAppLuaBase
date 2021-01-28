
/// ===================================================================================
//Vector2
template<typename T>
TH_INLINE
thVector2<T>::thVector2( const T *f )
:x(f[0])
,y(f[1])
{
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T>::thVector2( const thVector2& vec )
:x(vec.x)
,y(vec.y)
{
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T>::thVector2( T x ,T y )
:x(x)
,y(y)
{
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T> thVector2<T>::operator+()const
{
	return *this;
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T> thVector2<T>::operator-()const
{
	return thVector2(-x,-y);
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T> thVector2<T>::operator+( const thVector2& other )const
{
	return thVector2(this->x+other.x ,this->y+other.y);
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T> thVector2<T>::operator-( const thVector2& other )const
{
	return thVector2(this->x-other.x ,this->y-other.y);
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T>& thVector2<T>::operator+=( const thVector2& other )
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T>& thVector2<T>::operator-=( const thVector2& other )
{
	this->x -= other.x;
	this->y -= other.y;
	return *this;
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T> thVector2<T>::operator*( T f )
{
	return thVector2(this->x*f ,this->y*f);
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T> thVector2<T>::operator/( T f )
{
	return thVector2(this->x/f ,this->y/f);
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T>& thVector2<T>::operator*=( T f )
{
	this->x *= f;
	this->y *= f;
	return *this;
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T>& thVector2<T>::operator/=( T f )
{
	this->x /= f;
	this->y /= f;
	return *this;
}

/// ===================================================================================
template<typename T>
TH_INLINE
bool thVector2<T>::operator==( const thVector2& other )const
{
	if ( (this->x - other.x) > -FLOAT_EPS 
		&& (this->x - other.x) < FLOAT_EPS 
		&& (this->y - other.y) > -FLOAT_EPS
		&& (this->y - other.y) < FLOAT_EPS )
	{
		return true;
	}
	return false;
}

/// ===================================================================================
template<typename T>
TH_INLINE
bool thVector2<T>::operator!=( const thVector2& other )const
{
	return !(*this==other);
}

/// ===================================================================================
template<typename T>
TH_INLINE
bool thVector2<T>::operator>( const thVector2& other ) const
{
	return (this->x > other.x || ((abs(this->x-other.x)< 1e-4f) && this->y > other.y));
}

/// ===================================================================================
template<typename T>
TH_INLINE
float thVector2<T>::operator^( const thVector2& other ) const
{
	return (this->x * other.y - this->y * other.x); 
}

/// ===================================================================================
template<typename T>
TH_INLINE
void thVector2<T>::Clear()	//清空
{
	x = 0;
	y = 0;
}

/// ===================================================================================
template<typename T>
TH_INLINE
void thVector2<T>::SetVector( const thVector2 &vec )	// 设置向量
{
	*this = vec;
}

/// ===================================================================================
template<typename T>
TH_INLINE
void thVector2<T>::SetVector( T x, T y )		// 设置向量
{
	this->x = x;
	this->y = y;
}

/// ===================================================================================
template<typename T>
void thVector2<T>::Offset( T x,T y )			// 偏移
{
	this->x += x;
	this->y += y;
}

/// ===================================================================================
template<typename T>
void thVector2<T>::Offset( T l )				// 偏移
{
	this->x += l;
	this->y += l;
}

/// ===================================================================================
template<typename T>
TH_INLINE
void thVector2<T>::Inverse()		// 反向量
{
	x = -x;
	y = -y;
}

/// ===================================================================================
template<typename T>
TH_INLINE
T thVector2<T>::LengthSq() const	// 向量长度的平方
{
	return x*x + y*y;
}

/// ===================================================================================
template<typename T>
TH_INLINE
T thVector2<T>::Length() const		// 向量长度
{
	return (T)sqrt(LengthSq());
}

/// ===================================================================================
template<typename T>
TH_INLINE
void thVector2<T>::Normalize()			// 单位化向量
{
	*this /= Length();
}

/// ===================================================================================
template<typename T>
TH_INLINE
bool thVector2<T>::IsNormalized() const	// 是否单位向量
{
	return Length() == 1.0f;
}

/// ===================================================================================
template<typename T>
TH_INLINE
float thVector2<T>::Dot( const thVector2 &vec )	// 点乘
{
	return float(x*vec.x + y*vec.y);
}

/// ===================================================================================
template<typename T>
TH_INLINE
float thVector2<T>::Angle( const thVector2 &vec ) const	// 夹角
{
	thVector2<T> s=*this, t=vec;

	s.Normalize(); t.Normalize();
	return acosf(s.Dot(&t)/(s.Length()*t.Length()));
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T>& thVector2<T>::Rotate( float a )	// 旋转
{
	thVector2<T> v;

	v.x=x*cosf(a) - y*sinf(a);
	v.y=x*sinf(a) + y*cosf(a);

	x=v.x; y=v.y;

	return this;
}

/// ===================================================================================
template<typename T>
TH_INLINE
bool thVector2<T>::empty() const
{
	if ( x > -FLOAT_EPS && x < FLOAT_EPS 
		&&  y > -FLOAT_EPS && y < FLOAT_EPS )
	{
		return true;
	}
	return false;
}

/// ===================================================================================
// Vector4
template<typename T>
TH_INLINE
thVector4<T>::thVector4( const T *f )
:left(f[0])
,top(f[1])
,right(f[2])
,bottom(f[3])
{
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector4<T>::thVector4( const thVector4& vec )
:left(vec.left)
,top(vec.top)
,right(vec.right)
,bottom(vec.bottom)
{
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector4<T>::thVector4( T l ,T t, T r ,T b )
:left(l)
,top(t)
,right(r)
,bottom(b)
{
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector4<T> thVector4<T>::operator+()const
{
	return *this;
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector4<T> thVector4<T>::operator-()const
{
	return thVector4(-left,-top,-right,-bottom);
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector4<T> thVector4<T>::operator+( const thVector4& other )const
{
	return thVector4(this->left+other.left ,this->top+other.top,
		this->right+other.right ,this->bottom+other.bottom);
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector4<T> thVector4<T>::operator-( const thVector4& other )const
{
	return thVector4(this->left-other.left ,this->top-other.top,
		this->right-other.right ,this->bottom-other.bottom);
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector4<T>& thVector4<T>::operator+=( const thVector4& other )
{
	this->left += other.left;
	this->top += other.top;
	this->right += other.right;
	this->bottom += other.bottom;
	return *this;
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector4<T>& thVector4<T>::operator-=( const thVector4& other )
{
	this->left -= other.left;
	this->top -= other.top;
	this->right -= other.right;
	this->bottom -= other.bottom;
	return *this;
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector4<T> thVector4<T>::operator*( T f )
{
	return thVector4(this->left*f ,this->top*f,
		this->right*f ,this->bottom*f);
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector4<T> thVector4<T>::operator/( T f )
{
	return Vector4(this->left/f ,this->top/f,
		this->right/f ,this->bottom/f);
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector4<T>& thVector4<T>::operator*=( T f )
{
	this->left *= f;
	this->top *= f;
	this->right *= f;
	this->bottom *= f;
	return *this;
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector4<T>& thVector4<T>::operator/=( T f )
{
	this->left /= f;
	this->top /= f;
	this->right /= f;
	this->bottom /= f;
	return *this;
}

/// ===================================================================================
template<typename T>
TH_INLINE
bool thVector4<T>::operator==( const thVector4& other )const
{
	if ( (this->left - other.left) > -FLOAT_EPS 
		&& (this->left - other.left) < FLOAT_EPS 
		&& (this->top - other.top) > -FLOAT_EPS
		&& (this->top - other.top) < FLOAT_EPS
		&& (this->right - other.right) > -FLOAT_EPS 
		&& (this->right - other.right) < FLOAT_EPS 
		&& (this->bottom - other.bottom) > -FLOAT_EPS
		&& (this->bottom - other.bottom) < FLOAT_EPS )
	{
		return true;
	}
	return false;
}

/// ===================================================================================
template<typename T>
TH_INLINE
bool thVector4<T>::operator!=( const thVector4& other )const
{
	return !(*this==other);
}

/// ===================================================================================
template<typename T>
TH_INLINE
void thVector4<T>::Clear()	//清空
{
	left = top = right = bottom = 0;
}

/// ===================================================================================
template<typename T>
TH_INLINE
void thVector4<T>::SetVector( const thVector4 &vec )	// 设置向量
{
	*this = vec;
}

/// ===================================================================================
template<typename T>
TH_INLINE
void thVector4<T>::SetVector( T l, T t, T r, T b )		// 设置向量
{
	this->left = l;
	this->top = t;
	this->right = r;
	this->bottom = b;
}

/// ===================================================================================
template<typename T>
TH_INLINE
void thVector4<T>::Offset( T x,T y )		// 偏移
{
	this->left += x;
	this->top  += y;
	this->right+= x;
	this->bottom+= y;
}

/// ===================================================================================
template<typename T>
TH_INLINE
void thVector4<T>::Offset( T l )			// 偏移
{
	this->left += l;
	this->top  += l;
	this->right+= l;
	this->bottom+= l;
}

/// ===================================================================================
template<typename T>
TH_INLINE
bool thVector4<T>::empty() const
{
	if ( left > -FLOAT_EPS && left < FLOAT_EPS 
		&&  top > -FLOAT_EPS && top < FLOAT_EPS 
		&& right > -FLOAT_EPS && right < FLOAT_EPS 
		&& bottom > -FLOAT_EPS && bottom < FLOAT_EPS )
	{
		return true;
	}
	return false;
}

/// ===================================================================================
template<typename T>
TH_INLINE
bool thVector4<T>::TestPoint( T x, T y ) const	 // 测试点进入
{
	if(x>left && x<right && y>top && y<bottom) return true;

	return false;
}

/// ===================================================================================
template<typename T>
TH_INLINE
bool thVector4<T>::Intersect( const thVector4 *vec ) const	// 相交
{
	if((T)fabs(left + right - vec->left - vec->right) < (right - left + vec->right - vec->left))
		if((T)fabs(top + bottom - vec->top - vec->bottom) < (bottom - top + vec->bottom - vec->top))
			return true;

	return false;
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T> thVector4<T>::GetLUp() const	// 获得左上点
{
	return thVector2<T>(left,top);
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T> thVector4<T>::GetRUp() const	// 获得右上点
{
	return thVector2<T>(right,top);
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T> thVector4<T>::GetLDown() const	// 获得左下点
{
	return thVector2<T>(left,bottom);
}

/// ===================================================================================
template<typename T>
TH_INLINE
thVector2<T> thVector4<T>::GetRDown() const	// 获得右下点
{
	return thVector2<T>(right,bottom);
}

/// ===================================================================================
// thByteStream
TH_INLINE thByteStream::thByteStream()
{
	m_vcbData.clear();
}

/// ===================================================================================
TH_INLINE thByteStream::thByteStream( const BYTE *pDat , size_t Size )
{
	SetData( pDat , Size );
}

/// ===================================================================================
TH_INLINE thByteStream::thByteStream( const thByteStream *pByte )
{
	SetData( pByte );
}

/// ===================================================================================
TH_INLINE thByteStream::thByteStream( size_t Size )
{
	assert( Size > 0 );
	m_vcbData.resize( Size );
}

/// ===================================================================================
TH_INLINE thByteStream::~thByteStream()
{
	m_vcbData.resize(0);
	m_vcbData.clear();
}

/// ===================================================================================
// According to the data to create an object
TH_INLINE BYTE & thByteStream::operator[]( const unsigned int Index )
{
	assert( Index < Size() );
	assert( Index >= 0 );
	return m_vcbData[Index];
}

/// ===================================================================================
TH_INLINE thByteStream thByteStream::operator+( const BYTE& cbByte )
{
	thByteStream T(*this);
	T.m_vcbData.push_back(cbByte);
	return T;
}

/// ===================================================================================
TH_INLINE thByteStream thByteStream::operator+( const thByteStream& _thByteStream )
{
	thByteStream T(*this);
	T += _thByteStream;
	return T;
}

/// ===================================================================================
TH_INLINE thByteStream & thByteStream::operator+=( const BYTE& cbByte )
{
	m_vcbData.push_back(cbByte);
	return *this;
}

/// ===================================================================================
TH_INLINE thByteStream & thByteStream::operator+=( const thByteStream& _thByteStream )
{
	assert( !_thByteStream.IsEmpty() );
	size_t size = _thByteStream.GetLength();
	thByteStream tempByte(_thByteStream);
	ApendData( tempByte.GetData() , size );
	return *this;
}

/// ===================================================================================
TH_INLINE thByteStream & thByteStream::operator=( const thByteStream & _thByteStream )
{
	assert( !_thByteStream.IsEmpty() );
	m_vcbData = _thByteStream.m_vcbData;
	return *this;
}

/// ===================================================================================
TH_INLINE bool thByteStream::IsEmpty() const
{
	// For null
	return m_vcbData.empty();
}

/// ===================================================================================
TH_INLINE size_t thByteStream::GetLength() const
{
	// Obtains the data length
	return m_vcbData.size()*sizeof(BYTE);
}

/// ===================================================================================
TH_INLINE size_t thByteStream::Size() const
{
	return m_vcbData.size();
}

/// ===================================================================================
TH_INLINE void thByteStream::Setlength( const size_t Size )
{
	assert( Size >= 0 );
	m_vcbData.resize( Size );
}

/// ===================================================================================
TH_INLINE void thByteStream::SetData( const void *pDat , size_t Size )
{
	assert( Size > 0 ); /* length > 0 */
	assert( pDat != NULL );
	m_vcbData.resize( Size );
	CopyMemory( &m_vcbData[0],pDat,Size );
}

/// ===================================================================================
TH_INLINE void thByteStream::SetData( const thByteStream& T )
{
	assert( T.Size() > 0 );
	m_vcbData = T.m_vcbData;
}

/// ===================================================================================
TH_INLINE void thByteStream::ApendData( const void *pDat , size_t Size )
{
	assert( Size > 0 );
	assert( pDat != NULL );
	// Size Try to less than < 2097152(2G)
	{
		size_t OldSize = GetLength();
		size_t NewSize = OldSize + Size;
		m_vcbData.resize( NewSize );
		CopyMemory( &m_vcbData[OldSize],pDat,Size );
	}
}

/// ===================================================================================
template< class _TYPE >
TH_INLINE void thByteStream::ApendData( const _TYPE *pType )
{
	assert( pType != NULL );
	ApendData( pType , sizeof(_TYPE) );
}

/// ===================================================================================
TH_INLINE const BYTE * thByteStream::GetData() const
{
	if( !IsEmpty() )
		return &m_vcbData[0];
	return NULL;
}

/// ===================================================================================
TH_INLINE BYTE* thByteStream::GetData()
{
	if ( IsEmpty() )
	{
		return NULL;
	}
	return &m_vcbData[0];
}

/// ===================================================================================
TH_INLINE BYTE* thByteStream::GetData( size_t Offset )
{
	return GetData<BYTE>(Offset);
}

/// ===================================================================================
TH_INLINE void* thByteStream::GetData( size_t Size, size_t Offset /*= 0 */ )
{
	if (IsEmpty()||Size==0) return NULL;
	void *_lpPtr  = malloc(Size);
	BYTE *_lpData = GetData()+Offset;
	memcpy(_lpPtr,_lpData,Size);
	return _lpPtr;
}

/// ===================================================================================
template< class _TYPE >
TH_INLINE _TYPE * thByteStream::GetData( size_t Offset /*= 0 */ ) const
{
	if( (Offset + sizeof( _TYPE )) > GetLength() ) /* data offset */
		return NULL;
	return (_TYPE*)(GetData() + Offset);
}

/// ===================================================================================
// Range
template<typename T>
TH_INLINE thRange<T>::thRange()
{
	m_Pos.x = m_Pos.y = m_Range = T(0);
}

/// ===================================================================================
template<typename T>
TH_INLINE void thRange<T>::InitRange( T _PosX , T _PosY , T _Range )
{
	m_Pos.x = _PosX;
	m_Pos.y = _PosY;
	m_Range= _Range;
}

/// ===================================================================================
template<typename T>
TH_INLINE void thRange<T>::SetPos( T _PosX , T _PosY )
{
	m_Pos.x = _PosX;
	m_Pos.y = _PosY;
}

/// ===================================================================================
template<typename T>
TH_INLINE void thRange<T>::SetRange( T _Range )
{
	m_Range = _Range;
}

/// ===================================================================================
template<typename T>
TH_INLINE bool thRange<T>::TestRange( T _PosX , T _PosY )
{
	return ( (T)sqrt(float((m_Pos.x -_PosX)*(m_Pos.x -_PosX) 
		+ (m_Pos.y -_PosY)*(m_Pos.y -_PosY))) < m_Range );
}

/// ===================================================================================
template<typename T>
TH_INLINE bool thRange<T>::Intersect( thRange<T> &RangeTest )
{
	thVector2 vTemp = thVector2(m_Pos.x,m_Pos.y) - thVector2(RangeTest.m_Pos.x,RangeTest.m_Pos.y);
	if ( vTemp.Length() <= (m_Range + RangeTest.m_Range) )
	{
		return true;
	}
	return false;
}

/// ===================================================================================
template<typename T>
TH_INLINE T thRange<T>::GetRange()
{
	return m_Range;
}

/// ===================================================================================
TH_INLINE void thTimer::Begin(void)
{
	if ( m_bClose )
	{
		// Began to record the time
		Reset();
		m_bClose = false;
	}
}

/// ===================================================================================
TH_INLINE void thTimer::End(void)
{
	// end time
	m_bClose = true;
	m_dwLastTime = 0;
}

/// ===================================================================================
TH_INLINE bool thTimer::IsPassTimeMs( DWORD time ) const
{
	// Will the past time
	return m_bClose ? false : (GetLostTimeMs() >= time);
}

/// ===================================================================================
TH_INLINE void thTimer::Reset( void )
{
	m_dwLastTime = timeGetTime();
}

/// ===================================================================================
TH_INLINE bool thTimer::IsBegin( void ) const
{
	return !m_bClose;
}

/// ===================================================================================
TH_INLINE bool thTimer::IsPassTimes( DWORD time ) const
{
	return IsPassTimeMs(time*1000);
}

/// ===================================================================================
TH_INLINE DWORD thTimer::GetCurrentTimeMs( void ) const
{
	return timeGetTime();
}

/// ===================================================================================
TH_INLINE DWORD thTimer::GetStartTimeMs( void ) const
{
	return m_dwLastTime;
}

/// ===================================================================================
TH_INLINE DWORD thTimer::GetLostTimeMs( void ) const
{
	return timeGetTime() - m_dwLastTime;
}

/// ===================================================================================
TH_INLINE float thTimer::GetCurrentTimes( void ) const
{
	return (float)timeGetTime()/1000.0f;
}

/// ===================================================================================
TH_INLINE float thTimer::GetStartTimes( void ) const
{
	return (float)m_dwLastTime/1000.0f;
}

/// ===================================================================================
TH_INLINE float thTimer::GetLostTimes( void ) const
{
	return (float)GetLostTimeMs()/1000.0f;
}

/// ===================================================================================
TH_INLINE void thTimer::BeginFPS( void )
{
	if (m_dwLastTime==0) m_dwLastTime = timeGetTime();
}

/// ===================================================================================
TH_INLINE void thTimer::EndFPS( void )
{
	DWORD dwCurTime = timeGetTime();
	if ( dwCurTime - m_dwLastTime < 1000 ) m_iFPS++;
	else
	{
		m_iCurFPS = m_iFPS;
		m_iFPS = 0;
		m_dwLastTime = dwCurTime;
	}
}

/// ===================================================================================
TH_INLINE int thTimer::GetFPS( void )
{
	return m_iCurFPS;
}

/// ===================================================================================

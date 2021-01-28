#include "pch.h"
#include "thHelper.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ShObjIdl.h>
#include <tlhelp32.h>

#include <Psapi.h>
#pragma comment(lib,"Psapi.lib")

//
//HWND BrotherWindowFromPoint(HWND hWndPoint,const POINT Point) 
//{ 
//	//检测兄弟窗口 
//	RECT rcPoint; 
//	RECT rcNow; 
//	HWND hWndBrother=hWndPoint;//GetWindow(hWndPoint,GW_HWNDFIRST); 
//	hWndPoint=NULL; 
//	do 
//	{ 
//		if(::IsWindowVisible(hWndBrother))//可见 
//		{ 
//			GetWindowRect(hWndBrother,&rcNow); 
//			if(PtInRect(&rcNow,Point)) 
//			{ 
//				//检验矩形嵌套情况 
//				if(hWndPoint==NULL) 
//				{ 
//					hWndPoint=hWndBrother; 
//					rcPoint=rcNow; 
//				} 
//				else if( 
//				((rcNow.bottom<rcPoint.bottom)&&(rcNow.bottom>rcPoint.top)&& 
//				(rcNow.left>rcPoint.left)&&(rcNow.left<rcPoint.right))//左下角 
//				||((rcNow.bottom<rcPoint.bottom)&&(rcNow.bottom>rcPoint.top)&& 
//				(rcNow.right>rcPoint.left)&&(rcNow.right<rcPoint.right))//右下角 
//				||((rcNow.top>rcPoint.top)&&(rcNow.top<rcPoint.bottom)&& 
//				(rcNow.left>rcPoint.left)&&(rcNow.left<rcPoint.right))//左上角 
//				||((rcNow.top>rcPoint.top)&&(rcNow.top<rcPoint.bottom) 
//				&&(rcNow.right>rcPoint.left)&&(rcNow.right<rcPoint.right))//右上角 
//				)
//				{ 
//					hWndPoint=hWndBrother; 
//					rcPoint=rcNow; 
//				} 
//			} 
//		} 
//	}
//	while(hWndBrother=GetWindow(hWndBrother,GW_HWNDNEXT)); 
//
//	return hWndPoint; 
//} 
//
//
//HWND MyWindowFromPoint(const POINT Point) 
//{ 
//	HWND hWndPoint=::WindowFromPoint(Point); 
//	if(hWndPoint) 
//	{ 
//		//宽度搜索兄弟窗口 
//		HWND hWndChild; 
//		if(!(GetWindowLong(hWndPoint,GWL_STYLE)&WS_CHILD))//顶层窗口 
//			return hWndPoint; 
//		//非顶层窗口,要进行兄弟查找. 
//		hWndPoint=BrotherWindowFromPoint(hWndPoint,Point); 
//		assert(hWndPoint); 
//		//深度搜索子窗口 
//		while(hWndChild=GetTopWindow(hWndPoint)) 
//		{ 
//			//宽度搜索兄弟子窗口 
//			if(NULL==(hWndChild=BrotherWindowFromPoint(hWndChild,Point))) 
//			break; 
//			hWndPoint=hWndChild; 
//		} //*/ 
//	} 
//	return hWndPoint; 
//}

thHelper _thHead;

#define in_rgbValues ((unsigned char*)in_data.Scan0)
#define out_rgbValues ((unsigned char*)out_data.Scan0)

#define TH_CLASS_MUTEX	_T("TH_IMG_EDIT_MUTEX")

//////////////////////////////////////////////////////////////////////////
// 功能补助函数
thHelper* thHelper::s_lpTH = NULL;
CString thHelper::s_strTemp;


CString thHelper::s_strAppPath;
CString thHelper::s_strLogFile;
CString thHelper::s_strIniFile;
char thHelper::s_szChar[BUF_SIZE_MAINMAX];
wchar_t thHelper::s_wChar[BUF_SIZE_MAINMAX];
thByteStream thHelper::s_tempByte;

// LayeredWindow thHelper::UpdateLayeredWindow = 0;
// IActiveDesktop* thHelper::s_lpDesktop = 0;
HICON thHelper::s_curPt = 0;
HANDLE thHelper::s_hMutex = 0;

std::vector<CString> thHelper::s_exitFile;

thHelper::thHelper( void )
{
	s_lpTH = this;
	_thHead.m_mapIni.clear();
	_thHead.m_mapAppHwnd.clear();
	_thHead.m_mapHwnd.clear();
	_thHead.m_mapHotKey.clear();
	_thHead.m_mapProcess.clear();
	_thHead.m_mapSound.clear();
	_thHead.m_mapDlg.clear();
}

void thHelper::Log_Create( const char *filename )
{
	if (!filename) return;
	s_strTemp = MakePath_ASCII(filename);
	s_strLogFile = MakePath_ASCII(filename);
	// 创建路径
	PathRemoveFileSpec(s_strTemp.GetBuffer());
	::CreateDirectoryA(s_strTemp,NULL);
	// 创建文件
	FILE *hf = fopen(s_strLogFile, "w");
	if(!hf) s_strLogFile = "";
	else fclose(hf);
	s_strTemp.ReleaseBuffer();
}

void thHelper::Log_Write( const char *szFormat, ... )
{
	if (s_strLogFile.IsEmpty()) return;
	FILE *hf = NULL;
	va_list ap;

	hf = fopen(s_strLogFile, "a");
	if(!hf) return;

	va_start(ap, szFormat);
	vfprintf(hf, szFormat, ap);
	va_end(ap);

	fprintf(hf, "\n");

	fclose(hf);
}

void thHelper::Log_WriteTime( const char *szFormat, ... )
{
	// 带日期时间
	SYSTEMTIME _time;
	::GetLocalTime(&_time);
	char szBuf[10240];
	va_list ap;
	va_start(ap, szFormat);
	vsprintf(szBuf, szFormat, ap);
	va_end(ap);
	CString strLog;

	strLog.Format("[%d-%d-%d--%d:%d:%d]	%s",
		_time.wYear,
		_time.wMonth,
		_time.wDay,
		_time.wHour,
		_time.wMinute,
		_time.wSecond,
		szBuf);
	Log_Write(strLog);
}

void thHelper::Log_Print(const char *szFormat, ...)
{
	char szBuf[10240];
	va_list ap;
	va_start(ap, szFormat);
	vsprintf(szBuf, szFormat, ap);
	va_end(ap);

	OutputDebugStringA(szBuf);
	OutputDebugStringA("\n");

	Log_Write(szBuf);
}

CString thHelper::MakePath_ASCII( const char *filename/*=0*/ )
{
	if ( s_strAppPath.IsEmpty() )
	{
		char szAppPath[260];
		GetModuleFileName(GetModuleHandle(NULL), szAppPath, sizeof(szAppPath));
		int i;
		for(i=strlen(szAppPath)-1; i>0; i--) if(szAppPath[i]=='\\') break;
		szAppPath[i+1]=0;
		s_strAppPath = szAppPath;
	}
	CString str;
	str = s_strAppPath;
	if ( filename )
	{
		str.Append(filename);
	}
	return str;
}

wchar_t* thHelper::MakePath_UNICODE( const char *filename/*=0*/ )
{
	const char *lpFile = MakePath_ASCII(filename);
	return ToUNICODE(lpFile);
}

CString thHelper::MakeTempPath( const char *filename/*=0*/ )
{
	char szPath[MAX_PATH];
	::GetTempPath(MAX_PATH,szPath);
	CString str = szPath;
	if ( filename )
	{
		str.Append(filename);
	}
	return str;
}

CString thHelper::AnalyPath( const char *Pathfile/*=0*/ )
{
	CString str = Pathfile;
	if ( !s_strAppPath.IsEmpty() ) 
	{
		str.Replace(s_strAppPath,"");
	}
	return str;
}

Bitmap* thHelper::LoadImgFromFile( const char *filename, DWORD size/*=0*/ )
{
	void *data = 0;
	data = thHelper::Resource_Load(filename,&size);
	if (!data) return NULL;

	IStream *stream = NULL;
	HGLOBAL global = GlobalAlloc( GMEM_MOVEABLE, size );
	if( NULL == global )
	{
		thHelper::Resource_Free(data);
		return NULL;
	}
	// 转换
	if( !mem_to_global( data, size, global ) )
	{
		GlobalFree( global );
		thHelper::Resource_Free(data);
		return NULL;
	}
	// 得到IStream从全局对象
	if( CreateStreamOnHGlobal( global, TRUE, &stream ) != S_OK )
	{
		GlobalFree( global );
		thHelper::Resource_Free(data);
		return NULL;
	}
	// 创建
	Bitmap *image = Bitmap::FromStream( stream, FALSE );
	//GlobalFree( global );
	stream->Release();
	thHelper::Resource_Free(data);
	return image;
}

Bitmap* thHelper::LoadImgFromMem(const char *data, DWORD size)
{
	if (!data) return NULL;

	IStream *stream = NULL;
	HGLOBAL global = GlobalAlloc(GMEM_MOVEABLE, size);
	if (NULL == global)
	{
		return NULL;
	}
	// 转换
	if (!mem_to_global(data, size, global))
	{
		GlobalFree(global);
		return NULL;
	}
	// 得到IStream从全局对象
	if (CreateStreamOnHGlobal(global, TRUE, &stream) != S_OK)
	{
		GlobalFree(global);
		return NULL;
	}
	// 创建
	Bitmap *image = Bitmap::FromStream(stream, FALSE);
	//GlobalFree( global );
	stream->Release();
	return image;
}

Bitmap* thHelper::LoadImgFromCxImg( const TCHAR * filename, DWORD imagetype/*=0*/ )
{
	if (!filename) return NULL;
	CString strExt = thHelper::GetFileExt(filename);
	if (strExt.CompareNoCase("png")==0
		||strExt.CompareNoCase("gif")==0
		||strExt.CompareNoCase("bmp")==0
		||strExt.CompareNoCase("jpg")==0
		||strExt.CompareNoCase("tif")==0)
	{
		Bitmap *pBitmap = Bitmap::FromFile(thHelper::ToUNICODE(filename));
		return pBitmap;
	}
	//CxImage xImg(filename,imagetype);
	//if (xImg.GetSize()<=0) 
	//{
	//	return NULL;
	//}

	// 转换到GDI+Bitmap
//	return Ximage2Bitmap(&xImg);
	return nullptr;
}

void thHelper::RGB2ARGB( Bitmap **pbm )
{
	if (/*HasAlpha(*pbm)*/(*pbm)->GetPixelFormat() == PixelFormat32bppARGB)
		return;

	Bitmap *pbmA = new Bitmap((*pbm)->GetWidth(), (*pbm)->GetHeight(), PixelFormat32bppARGB);
	if (!pbmA) return;

	Graphics *g = Graphics::FromImage(pbmA);
	g->DrawImage(*pbm, 0,0,(*pbm)->GetWidth(), (*pbm)->GetHeight());
	delete g;

	delete *pbm;
	*pbm = pbmA;
}

BOOL thHelper::Bitmap2Ximage( Bitmap *pBitmap, CxImage &xImage )
{
	//HBITMAP hBitmap;
	//pBitmap->GetHBITMAP(NULL, &hBitmap);
	//if (!hBitmap) return FALSE;

	//xImage.CreateFromHBITMAP(hBitmap);
	//if (!xImage.IsValid()) return FALSE;

	////设置ALPHA通道
	//if (pBitmap->GetPixelFormat() == PixelFormat32bppARGB)
	//{
	//	xImage.AlphaCreate();

	//	BitmapData bmpData;
	//	Rect rect(0,0,pBitmap->GetWidth(),pBitmap->GetHeight());
	//	pBitmap->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);
	//	int w = pBitmap->GetWidth();
	//	int h = pBitmap->GetHeight();
	//	BYTE* dst = (BYTE*)xImage.AlphaGetPointer();
	//	BYTE* src = (BYTE*)bmpData.Scan0;
	//	for(long y=0; y<h; y++){
	//		for(long x=0; x<w; x++){
	//			dst[(h-y-1)*w+x] = src[4*(y*w+x)+3];
	//		}
	//	}
	//	pBitmap->UnlockBits(&bmpData);
	//}

	return TRUE;

}

Bitmap* thHelper::Ximage2Bitmap( CxImage *pXimage )
{
	//if (!pXimage)
	//	return NULL;

	//BYTE *pDIB = (BYTE*)pXimage->GetDIB();
	//BITMAPINFOHEADER *pBmih = (BITMAPINFOHEADER*)pDIB;
	//int w = pBmih->biWidth;
	//int h = pBmih->biHeight;

	//Bitmap *pBitmap = new Bitmap(w, h, PixelFormat24bppRGB);
	//if (!pBitmap)
	//	return NULL;

	////复制RGB像素数据
	//BitmapData bmpData;
	//pBitmap->LockBits(&Rect(0,0,w,h), ImageLockModeWrite, PixelFormat24bppRGB, &bmpData);

	//if (pBmih->biSizeImage == bmpData.Stride * bmpData.Height) //likely
	//	memcpy(bmpData.Scan0, pDIB + sizeof(BITMAPINFOHEADER), pBmih->biSizeImage);
	//pBitmap->UnlockBits(&bmpData);

	////上下颠倒问题
	//pBitmap->RotateFlip(RotateNoneFlipY);

	////设置ALPHA通道
	//BYTE* src = (BYTE*)pXimage->AlphaGetPointer();
	//if (src)
	//{
	//	RGB2ARGB(&pBitmap);

	//	BitmapData bmpDataA;
	//	pBitmap->LockBits(&Rect(0,0,w,h), ImageLockModeWrite, PixelFormat32bppARGB, &bmpDataA);

	//	BYTE* dst = (BYTE*)bmpDataA.Scan0;
	//	for(long y=0; y<h; y++){
	//		for(long x=0; x<w; x++){
	//			dst[4*(y*w+x)+3] = src[(h-y-1)*w+x];
	//		}
	//	}

	//	pBitmap->UnlockBits(&bmpDataA);
	//}

	// return pBitmap;
	return nullptr;
}

UINT thHelper::GetFrameCount( Bitmap* pimage )
{
	UINT count = 0;
	count = pimage->GetFrameDimensionsCount();
	GUID *pDimensionIDs = (GUID*)new GUID[count];
	pimage->GetFrameDimensionsList(pDimensionIDs, count);
	WCHAR strGuid[39];
	StringFromGUID2(pDimensionIDs[0], strGuid, 39);
	UINT frameCount = pimage->GetFrameCount(&pDimensionIDs[0]); 
	//根据frameCount的值判断这个GIF文件是否有多个帧。
	TH_SAFE_DELETE_ARY(pDimensionIDs);
	return frameCount;
}

bool thHelper::GetEncoderCLSID( const WCHAR *format,CLSID *pClsid )
{
	ImageCodecInfo * pCodecInfo = NULL;
	UINT nNum = 0,nSize = 0;
	GetImageEncodersSize(&nNum,&nSize);
	if (nSize < 0)
	{
		return false;
	}
	pCodecInfo = new ImageCodecInfo[nSize];
	if (pCodecInfo == NULL)
	{
		return false;
	}
	GetImageEncoders(nNum,nSize,pCodecInfo);
	for (UINT i=0; i < nNum; ++i)
	{
		if (wcscmp(pCodecInfo[i].MimeType,format)==0)
		{
			*pClsid = pCodecInfo[i].Clsid;

			TH_SAFE_DELETE_ARY(pCodecInfo);
			return true;
		}
		else
		{
			continue;
		}
	}
	TH_SAFE_DELETE_ARY(pCodecInfo);
	return false;
}

//bool thHelper::SaveImg( const char* lpFile,Image *lpImg,int iImg/*=IMG_PNG*/ )
//{
//	if (!lpFile||!lpImg) return false;
//	// 解析图片格式
//	CString strFormat;
//	switch (iImg)
//	{
//	case IMG_BMP: strFormat.Format("image/bmp");
//		break;
//	case IMG_JPG: strFormat.Format("image/jpeg");
//		break;
//	case IMG_PNG: strFormat.Format("image/png");
//		break;
//	}
//	CLSID Clsid;
//	bool bOK = GetEncoderCLSID(CA2W(strFormat),&Clsid);
//	if (!bOK) return false;
//
//	lpImg->Save(CA2W(lpFile),&Clsid);
//	return true;
//}

bool thHelper::SaveImg( const char* lpFile,Bitmap *lpImg)
{
	if (!lpFile||!lpImg) return false;
	// 解析图片格式
	int iXImg = 0;
	CString strFMT = thHelper::GetFileExt(lpFile);
	//if (strFMT.CompareNoCase("tga")==0) iXImg = CXIMAGE_FORMAT_TGA;
	//else if (strFMT.CompareNoCase("ico")==0) iXImg = CXIMAGE_FORMAT_ICO;
	//if (iXImg)
	//{
	//	// 保存
	//	CxImage xImg;
	//	thHelper::Bitmap2Ximage(lpImg,xImg);
	//	xImg.Save(lpFile,iXImg);
	//	return true;
	//}
	CString strFormat;
	if (strFMT.CompareNoCase("bmp")==0) strFormat.Format("image/bmp");
	else if (strFMT.CompareNoCase("gif")==0) strFormat.Format("image/gif");
	else if (strFMT.CompareNoCase("png")==0) strFormat.Format("image/png");
	else if (strFMT.CompareNoCase("jpg")==0) strFormat.Format("image/jpeg");
	else if (strFMT.CompareNoCase("jpeg")==0) strFormat.Format("image/jpeg");
	else if (strFMT.CompareNoCase("ico")==0) strFormat.Format("image/ico");
	else if (strFMT.CompareNoCase("tif")==0) strFormat.Format("image/tiff");
	else if (strFMT.CompareNoCase("tiff")==0) strFormat.Format("image/tiff");

	CLSID Clsid;
	bool bOK = GetEncoderCLSID(CA2W(strFormat),&Clsid);
	if (!bOK) return false;

	lpImg->Save(CA2W(lpFile),&Clsid);

	return true;
}

CString thHelper::GetDlgPath( HWND hWnd )
{
	CString strTemp;
	TCHAR BASED_CODE szFilter[] = _T("PNG (*.png)|*.png|")
		_T("JPG (*.jpg;*.jpeg)|*.jpg;*.jpeg|BMP (*.bmp)|*.bmp|")
		_T("GIF (*.gif)|*.gif|TIF (*.tif;*.tiff)|*.tif;*.tiff|ICON (*.ico)|*.ico|")
		_T("TGA (*.tga)|*.tga||");
	CFileDialog dialg(false,"",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter,CWnd::FromHandle(hWnd));
	if ( dialg.DoModal() == IDOK )
	{
		return dialg.GetPathName();
	}
	return "";
}

CRect thHelper::ConstantRatioWH( long lNewW,long lNewH,long lSrcW,long lSrcH,BOOL bCenter )
{
	CRect rect(0,0,lNewW,lNewH);
	double dNewWidth = lNewW;
	double dNewHeight= lNewH;
	double dSrcWidth = lSrcW;
	double dSrcHeight= lSrcH;

	double dNewAspectRatio = dNewWidth/dNewHeight;
	double dSrcAspectRatio = dSrcWidth/dSrcHeight;

	if (dSrcAspectRatio > dNewAspectRatio)
	{
		int nNewHeight = (int)(dNewWidth/dSrcWidth*dSrcHeight);
		int nCenteringFactor = (dNewHeight - nNewHeight) / 2;
		rect.SetRect(0,nCenteringFactor,(int)dNewWidth,nNewHeight+nCenteringFactor);
	}
	else if (dSrcAspectRatio < dNewAspectRatio)
	{
		int nNewWidth = (int)(dNewHeight/dSrcHeight*dSrcWidth);
		int nCenteringFactor = (dNewWidth - nNewWidth) / 2;
		rect.SetRect(nCenteringFactor,0,nNewWidth + nCenteringFactor,(int)(dNewHeight));
	}
	return rect;
}

const char* thHelper::ToASCII( const wchar_t *pwIn )
{
	char szTemp[10240];
	int len = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pwIn, -1, 0, 0, NULL, NULL);
	::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pwIn, -1, szTemp, len, NULL, NULL);
	szTemp[len] = 0;
	s_strTemp = szTemp;
	return s_strTemp.GetString();
}

wchar_t* thHelper::ToUNICODE( const char *pszIn )
{
	int len = ::MultiByteToWideChar(CP_ACP,0,pszIn,-1,0,0);
	::MultiByteToWideChar(CP_ACP,0,pszIn,-1,s_wChar,len);
	s_wChar[len] = 0;
	return s_wChar;
}

const char* thHelper::toGB2312(const char *pszIn)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, pszIn, -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, pszIn, -1, s_wChar, len);
	s_wChar[len] = 0;

	char szTemp[10240];
	len = WideCharToMultiByte(CP_ACP, 0, s_wChar, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, s_wChar, -1, szTemp, len, NULL, NULL);
	szTemp[len] = 0;
	s_strTemp = szTemp;
	return s_strTemp.GetString();
}

const char* thHelper::sprintf_s(LPCSTR fmt, ...)
{
	char szBuf[10240];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(szBuf, fmt, ap);
	va_end(ap);
	s_strTemp = szBuf;
	return s_strTemp.GetString();
}

wchar_t* thHelper::sprintf_w( LPCSTR fmt,... )
{
	char szBuf[10240];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(szBuf, fmt, ap);
	va_end(ap);
	return ToUNICODE(szBuf);
}

int thHelper::strleng( LPSTR buf )
{
	return strlen(buf);
}

int thHelper::strleng( wchar_t* buf )
{
	return wcslen(buf);
}

int thHelper::CompLengFt( CString strBuf,int iFontSize )
{
	if (strBuf.IsEmpty()) return 0;
	int iSize = strBuf.GetLength();
	int iLeng = 0;
	for (int i=0;i<iSize;++i)
	{
		char ch = strBuf.GetAt(i);
		if (ch<0)
		{
			iLeng += iFontSize;
			i+=1;
			continue;
		}
		int iAdd = iFontSize-5;
		if (iAdd<0) iAdd = 0;
		iLeng += iAdd;
	}
	return iLeng;
}

bool thHelper::mem_to_global( const void *buf, size_t size,HGLOBAL global )
{
	void *dest = GlobalLock( global );
	if( NULL == dest )
	{
		return false;
	}
	memcpy( dest, buf, size );
	GlobalUnlock( global );
	return true;
}

bool thHelper::stream_to_mem( IStream *stream, void **outbuf, size_t *size )
{
	ULARGE_INTEGER ulnSize;
	LARGE_INTEGER lnOffset;
	lnOffset.QuadPart = 0;
	/* get the stream size */
	if( stream->Seek( lnOffset, STREAM_SEEK_END, &ulnSize ) != S_OK )
	{
		return false;
	}
	if( stream->Seek( lnOffset, STREAM_SEEK_SET, NULL ) != S_OK )
	{
		return false;
	}
	/* read it */
	*outbuf = malloc( (size_t)ulnSize.QuadPart );
	*size = (size_t) ulnSize.QuadPart;
	ULONG bytesRead;
	if( stream->Read( *outbuf, (ULONG)ulnSize.QuadPart, &bytesRead ) != S_OK )
	{
		free( *outbuf );
		return false;
	}
	return true;
}

int thHelper::CompareInt( int iLeft,int iRight )
{
	if (iLeft<iRight)
	{
		return -1;
	}
	else if (iLeft>iRight)
	{
		return 1;
	}
	return 0;
}

int thHelper::GetFileLength( const char *name )
{
	int fh, nbytes;

	fh = _open( name, _O_RDONLY );	//只读模式
	if( fh== -1 )
	{
		return -1;
	}

	nbytes=_filelength(fh);
	_close(fh);

	return nbytes;
}

bool thHelper::CheckFile( const char *name )
{
	//查找文件
	WIN32_FIND_DATA fdt;
	HANDLE hFile = FindFirstFile(name, &fdt);

	if( hFile == INVALID_HANDLE_VALUE )
		return false;
	else 
	{
		FindClose(hFile);
		return true;
	}
	return false;
}

const char* thHelper::GetFilePath( const char *filename )
{
	if (!filename) return NULL;
	CString strFile = filename;
	PathRemoveFileSpec(strFile.GetBuffer());
	memset(s_szChar, 0, sizeof(s_szChar));
	strncpy(s_szChar, strFile.GetString(), strFile.GetLength());
	strFile.ReleaseBuffer();
	return s_szChar;
	//int Len=s_strTemp.GetLength();
	//int i = 0;
	//for(i=Len-1; i>=0; --i)
	//{
	//	if( s_strTemp[i] == '\\' )			//从后向前找'\'
	//		break;
	//}
	//if (i>=0)
	//{
	//	// 截取
	//	CString strTemp = s_strTemp.Mid(0,i);
	//	s_strTemp = strTemp;
	//	s_strTemp.Append("\\");
	//}
	//return s_strTemp.GetBuffer();
}

const char* thHelper::GetFileName( const char* filename )
{
	if (!filename||filename[0]==0) return NULL;
	s_strTemp = filename;
	if (PathIsURL(filename)) return s_strTemp.GetString();
	char * szTemp = (char*)strrchr(filename,'\\');//找最后一个'\'
	if (szTemp)
	{
		szTemp++;									  //跳过'\'
		s_strTemp = szTemp;
	}
	memset(s_szChar, 0, sizeof(s_szChar));
	strncpy(s_szChar, s_strTemp.GetString(), s_strTemp.GetLength());
	return s_szChar;
}

CString thHelper::GetFileExt( const char* filename )
{
	s_strTemp = "";
	if (!filename) return s_strTemp;
	s_strTemp = filename;

	int iFd = 0;
	while (iFd != -1)
	{
		iFd = s_strTemp.Find(".");
		s_strTemp = s_strTemp.Mid(iFd+1); // 获取后缀
	}
	return s_strTemp;
}

CString thHelper::GetFileTitle( const char* filename )
{
	s_strTemp = "";
	if (!filename) return s_strTemp;
	CString strFile = filename;

	// 获取标题
	CString strExt = GetFileExt(filename);
	s_strTemp.Format(".%s",strExt);
	strFile.Replace(s_strTemp,"");
	s_strTemp = strFile;
	return s_strTemp;
}

bool thHelper::SortbyString( const CString& x, const CString& y )
{
	bool bCom = true;
	int iLeft = x.GetLength();
	int iRight= y.GetLength();
	if (iLeft<iRight)
	{
		bCom = true;
	}
	else if (iLeft>iRight)
	{
		bCom = false;
	}
	else
	{
		bCom = x < y;
	}
	return bCom;
}

bool thHelper::GetFileByPath( const char* path,FileMapPtr &mapFile,int iLv )
{
	//加载资源
	CFileFind find;
	CString strFile = path;
	CString strDirectory = strFile + _T("\\") + _T("\\*.*");
	strDirectory.Replace("\\\\","\\");

	BOOL IsFind = find.FindFile(strDirectory);
	if (!IsFind)
	{
		find.Close();
		return false;
	}
	while( IsFind )
	{
		IsFind=find.FindNextFile();

		// 如果是"." 则不扫描
		if(find.IsDots())
			continue;
		// 如果是是目录,跳过
		else if(find.IsDirectory())
		{
			continue;
		}
		// 文件
		else
		{   
			strFile = find.GetFileName();
			CString strTL = thHelper::GetFileExt(strFile);

			BOOL bFile = FALSE;
			switch (iLv)
			{
			case LV_ALL:	// 全部文件
				bFile = TRUE;
				break;
			case LV_IMG:	// 图片文件
				if (strTL.CompareNoCase("jpg")==0
					||strTL.CompareNoCase("png")==0
					||strTL.CompareNoCase("bmp")==0
					||strTL.CompareNoCase("tga")==0
					||strTL.CompareNoCase("gif")==0)
				{
					bFile = TRUE;
				}
				break;
			case LV_VIDEO:	// 视频文件
				{
					if (strTL.CompareNoCase("wmv")==0
						||strTL.CompareNoCase("avi")==0
						||strTL.CompareNoCase("rmvb")==0
						||strTL.CompareNoCase("flv")==0)
					{
						bFile = TRUE;
					}
				}
				break;
			case LV_SOUND:	// 音频文件
				{
					if (strTL.CompareNoCase("wav")==0
						||strTL.CompareNoCase("wma")==0
						||strTL.CompareNoCase("mp3")==0
						||strTL.CompareNoCase("mid")==0
						||strTL.CompareNoCase("ogg")==0)
					{
						bFile = TRUE;
					}
				}
				break;
			case LV_SWF:	// Flash文件
				{
					if (strTL.CompareNoCase("swf")==0)
					{
						bFile = TRUE;
					}
				}
				break;
			}
			if (bFile) mapFile.push_back(strFile);
		}
	}
	find.Close();
	if (mapFile.empty()) return false;
	
	std::sort(mapFile.begin(),mapFile.end(),SortbyString);
	return true;
}

bool thHelper::OpenFilePath( HWND hWnd, CString &strPath,CString &strFolder )
{
	strPath = "";
	strFolder = "";
	BROWSEINFO bi;
	char name[MAX_PATH];
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner = hWnd;
	bi.pszDisplayName = name;
	bi.lpszTitle = "请选择目录";
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if(idl == NULL)
		return false;
	CString strDirectoryPath;
	SHGetPathFromIDList(idl, strDirectoryPath.GetBuffer(MAX_PATH));
	strDirectoryPath.ReleaseBuffer();
	if(strDirectoryPath.IsEmpty())
		return false;
	if(strDirectoryPath.Right(1)!="\\")
		strDirectoryPath+="\\";

	strPath = strDirectoryPath;
	strFolder = name;
	return true;
}

CString thHelper::analyStringFind(const CString& str, const char* beginfd, const char* endfd, int& of /*= 0*/)
{
	int idx = str.Find(beginfd, of);
	if (idx == -1) return "";
	idx += strlen(beginfd);
	int end = str.Find(endfd, idx);
	if (end == -1)
	{
		return "";
	}
	of = end + strlen(endfd);
	return str.Mid(idx, end - idx);
}

std::string thHelper::analyStringFind(const std::string& str, const char* beginfd, const char* endfd, int& of)
{
	int idx = str.find(beginfd, of);
	if (idx == -1) return "";
	idx += strlen(beginfd);
	int end = str.find(endfd, idx);
	if (end == -1)
	{
		return "";
	}
	of = end + strlen(endfd);
	return str.substr(idx, end - idx);
}

std::string thHelper::analyString(std::string &str, const char* tp)
{
	int id = str.find(tp);
	if (id != -1)
	{
		std::string ret = str.substr(0, id);
		str = str.substr(id + strlen(tp));
		return ret;
	}
	auto ret = str;
	str = "";
	return ret;
}

CString thHelper::analyString(CString &str, const char *fmt)
{
	int iFind = str.Find(fmt);
	if (iFind != -1)
	{
		s_strTemp = str.Mid(0,iFind);
		str = str.Mid(iFind+strlen(fmt));
		return s_strTemp;
	}
	auto ret = str;
	str = "";
	return ret;
}

int thHelper::analyInt( CString &str, const char *fmt )
{
	auto lpString = thHelper::analyString(str,fmt);
	if (lpString)
	{
		return atoi(lpString);
	}
	return -1;
}

DWORD thHelper::analyDword(CString &str, const char *fmt)
{
	auto lpString = thHelper::analyString(str, fmt);
	if (lpString)
	{
		return atoll(lpString);
	}
	return -1;
}

int thHelper::analyInt(std::string &str, const char *tp)
{
	auto lpString = thHelper::analyString(str, tp);
	if (!lpString.empty())
	{
		return atoi(lpString.c_str());
	}
	return -1;
}

CString thHelper::analySign(CString &str, char *fmt, int &iMid)
{
	CString strTemp;
	strTemp.Format("%s[",fmt);
	int iFXG = str.Find(strTemp);
	if (iFXG!=-1)
	{
		s_strTemp = str.Mid(iFXG);
		iFXG = s_strTemp.Find("]");
		s_strTemp = str.Mid(0,iFXG);
		s_strTemp.Replace(strTemp,"");
		iMid = iFXG;
		return s_strTemp;
	}
	return "";
}

int thHelper::RegisterHotKey( HWND hWnd,int id,UINT fsMod1,UINT fsMod2,UINT vk,BOOL bSys/*=true*/ )
{
	if (bSys) 
	{
		BOOL bReist = ::RegisterHotKey(hWnd,id,fsMod1|fsMod2,vk);
		return bReist?0:2;
	}
	else
	{
		HotKeyMapPtr::iterator iter = _thHead.m_mapHotKey.find(id);
		if (iter != _thHead.m_mapHotKey.end())
		{
			return false;
		}
		iter = _thHead.m_mapHotKey.begin();
		while (iter != _thHead.m_mapHotKey.end())
		{
			if (iter->second.nGroupkey1==fsMod1
				&&iter->second.nGroupkey2==fsMod2
				&&iter->second.nHotkey==vk)
			{
				return 1;
			}
			++iter;
		}
		stHotKey hotKey;
		hotKey.hWnd = hWnd;
		hotKey.nGroupkey1 = fsMod1;
		hotKey.nGroupkey2 = fsMod2;
		hotKey.nHotkey	  = vk;
		_thHead.m_mapHotKey.insert(HotKeyMapPtr::value_type(id,hotKey));
	}
	return 0;
}

void thHelper::UnregisterHotKey( HWND hWnd,int id,BOOL bSys )
{
	if (bSys) ::UnregisterHotKey(hWnd,id);
	else
	{
		HotKeyMapPtr::iterator iter = _thHead.m_mapHotKey.find(id);
		if (iter != _thHead.m_mapHotKey.end())
		{
			_thHead.m_mapHotKey.erase(iter);
		}
	}
}

void thHelper::UnregisterHotKeyAll( void )
{
	_thHead.m_mapHotKey.clear();
}

void thHelper::RunHotKey( UINT vk )
{
	if (_thHead.m_mapHotKey.empty()) return;
	HotKeyMapPtr::iterator iter = _thHead.m_mapHotKey.begin();
	while (iter != _thHead.m_mapHotKey.end())
	{
		stHotKey *lpkey = &iter->second;
		UINT nGroupkey1 = lpkey->GetVKKey(lpkey->nGroupkey1);
		UINT nGroupkey2 = lpkey->GetVKKey(lpkey->nGroupkey2);
		UINT nHotKey = lpkey->nHotkey;
		
		BOOL bGroupKey1 = (GetAsyncKeyState(nGroupkey1) & 0x8000);
		BOOL bGroupKey2 = (GetAsyncKeyState(nGroupkey2) & 0x8000);
		BOOL bHotKey = (nHotKey == vk);
		if (nGroupkey2==0)
		{
			if (bGroupKey1&&bHotKey) ::SendMessage(lpkey->hWnd,WM_HOTKEY,iter->first,0);
		}
		else if (bGroupKey1&&bGroupKey2&&bHotKey)
		{
			::SendMessage(lpkey->hWnd,WM_HOTKEY,iter->first,0);
		}
		++iter;
	}
}

int thHelper::GetHotKey( CString &strKey )
{
	int iKey = -1;
	if (strKey.CompareNoCase("Ctrl")==0)
	{
		iKey = MOD_CONTROL;
	}
	else if (strKey.CompareNoCase("Alt")==0)
	{
		iKey = MOD_ALT;
	}
	else if (strKey.CompareNoCase("Shift")==0)
	{
		iKey = MOD_SHIFT;
	}
	else if (strKey.CompareNoCase("Win")==0)
	{
		iKey = MOD_WIN;
	}
	else if (strKey.CompareNoCase("F1")==0)
	{
		iKey = VK_F1;
	}
	else if (strKey.CompareNoCase("F2")==0)
	{
		iKey = VK_F2;
	}
	else if (strKey.CompareNoCase("F3")==0)
	{
		iKey = VK_F3;
	}
	else if (strKey.CompareNoCase("F4")==0)
	{
		iKey = VK_F4;
	}
	else if (strKey.CompareNoCase("F5")==0)
	{
		iKey = VK_F5;
	}
	else if (strKey.CompareNoCase("F6")==0)
	{
		iKey = VK_F6;
	}
	else if (strKey.CompareNoCase("F7")==0)
	{
		iKey = VK_F7;
	}
	else if (strKey.CompareNoCase("F8")==0)
	{
		iKey = VK_F8;
	}
	else if (strKey.CompareNoCase("F9")==0)
	{
		iKey = VK_F9;
	}
	else if (strKey.CompareNoCase("F10")==0)
	{
		iKey = VK_F10;
	}
	else if (strKey.CompareNoCase("F11")==0)
	{
		iKey = VK_F11;
	}
	else if (strKey.CompareNoCase("F12")==0)
	{
		iKey = VK_F12;
	}
	else if (strKey.CompareNoCase("A")==0)
	{
		iKey = VkKeyScan('a');
	}
	else if (strKey.CompareNoCase("B")==0)
	{
		iKey = VkKeyScan('b');
	}
	else if (strKey.CompareNoCase("C")==0)
	{
		iKey = VkKeyScan('c');
	}
	else if (strKey.CompareNoCase("D")==0)
	{
		iKey = VkKeyScan('d');
	}
	else if (strKey.CompareNoCase("E")==0)
	{
		iKey = VkKeyScan('e');
	}
	else if (strKey.CompareNoCase("F")==0)
	{
		iKey = VkKeyScan('f');
	}
	else if (strKey.CompareNoCase("G")==0)
	{
		iKey = VkKeyScan('g');
	}
	else if (strKey.CompareNoCase("H")==0)
	{
		iKey = VkKeyScan('h');
	}
	else if (strKey.CompareNoCase("I")==0)
	{
		iKey = VkKeyScan('i');
	}
	else if (strKey.CompareNoCase("J")==0)
	{
		iKey = VkKeyScan('j');
	}
	else if (strKey.CompareNoCase("K")==0)
	{
		iKey = VkKeyScan('k');
	}
	else if (strKey.CompareNoCase("L")==0)
	{
		iKey = VkKeyScan('l');
	}
	else if (strKey.CompareNoCase("M")==0)
	{
		iKey = VkKeyScan('m');
	}
	else if (strKey.CompareNoCase("N")==0)
	{
		iKey = VkKeyScan('n');
	}
	else if (strKey.CompareNoCase("O")==0)
	{
		iKey = VkKeyScan('o');
	}
	else if (strKey.CompareNoCase("P")==0)
	{
		iKey = VkKeyScan('p');
	}
	else if (strKey.CompareNoCase("Q")==0)
	{
		iKey = VkKeyScan('q');
	}
	else if (strKey.CompareNoCase("R")==0)
	{
		iKey = VkKeyScan('r');
	}
	else if (strKey.CompareNoCase("S")==0)
	{
		iKey = VkKeyScan('s');
	}
	else if (strKey.CompareNoCase("T")==0)
	{
		iKey = VkKeyScan('t');
	}
	else if (strKey.CompareNoCase("U")==0)
	{
		iKey = VkKeyScan('u');
	}
	else if (strKey.CompareNoCase("V")==0)
	{
		iKey = VkKeyScan('v');
	}
	else if (strKey.CompareNoCase("W")==0)
	{
		iKey = VkKeyScan('w');
	}
	else if (strKey.CompareNoCase("X")==0)
	{
		iKey = VkKeyScan('x');
	}
	else if (strKey.CompareNoCase("Y")==0)
	{
		iKey = VkKeyScan('y');
	}
	else if (strKey.CompareNoCase("Z")==0)
	{
		iKey = VkKeyScan('z');
	}
	else
	{
		CString strNum;
		for (int i=0;i<10; ++i)
		{
			strNum.Format("%d",i);
			if (strKey == strNum)
			{
				char tch = strNum.GetAt(0);
				iKey = VkKeyScan(tch);
				break;
			}
		}
	}
	return iKey;
}

const char* thHelper::GetHotKey( int vk )
{
	char szBuf[256];
	memset(szBuf,0,sizeof(szBuf));
	if (vk==MOD_CONTROL)
	{
		return "Ctrl";
	}
	else if (vk==MOD_ALT)
	{
		return "Alt";
	}
	else if (vk==MOD_SHIFT)
	{
		return "Shift";
	}
	else if (vk==MOD_WIN)
	{
		return "Win";
	}
	else if (vk==VK_F1)
	{
		return "F1";
	}
	else if (vk==VK_F2)
	{
		return "F2";
	}
	else if (vk==VK_F3)
	{
		return "F3";
	}
	else if (vk==VK_F4)
	{
		return "F4";
	}
	else if (vk==VK_F5)
	{
		return "F5";
	}
	else if (vk==VK_F6)
	{
		return "F6";
	}
	else if (vk==VK_F7)
	{
		return "F7";
	}
	else if (vk==VK_F8)
	{
		return "F8";
	}
	else if (vk==VK_F9)
	{
		return "F9";
	}
	else if (vk==VK_F10)
	{
		return "F10";
	}
	else if (vk==VK_F11)
	{
		return "F11";
	}
	else if (vk==VK_F12)
	{
		return "F12";
	}
	else if (vk>=65&&vk<=90)
	{
		szBuf[0] = vk+32;
		s_strTemp = szBuf;
		s_strTemp.MakeUpper();
		return s_strTemp.GetString();
	}
	else if (vk>=97&&vk<=122)
	{
		szBuf[0] = vk-32;
		s_strTemp = szBuf;
		s_strTemp.MakeUpper();
		return s_strTemp.GetString();
	}
	else if (vk>=48&&vk<=57)
	{
		s_strTemp.Format("%d",vk-48);
		return s_strTemp.GetString();
	}
	return NULL;
}

void thHelper::DrawString( HDC hDC,WCHAR *lpText,RectF &rect,Color col,WCHAR *lpFont,int iSize,INT style )
{
	Graphics graph(hDC);
	// gdi+渲染字体
	graph.SetSmoothingMode(SmoothingModeAntiAlias); //指定平滑（抗锯齿）
	////m_lpGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);//指定的高品质，双三次插值

	Gdiplus::FontFamily fontFamily(lpFont); //△字体，效果图为'微软雅黑'字体
	Gdiplus::StringFormat strFormat;
	Gdiplus::GraphicsPath path;
	path.AddString(lpText, -1, &fontFamily, style,(REAL)iSize,rect,&strFormat);
	//// 背影
	//Gdiplus::Pen pen(Gdiplus::Color(155, 215, 215,215),3);
	//m_lpGraphics->DrawPath(&pen,&path);

	SolidBrush solidBrush(col);
	graph.FillPath(&solidBrush,&path);
}

void thHelper::DrawImage(CWnd* self, int id, const CString& res)
{
	int cx, cy;
	CImage  image;
	CRect   rect;

	// 根据路径载入图片
	// image.LoadFromResource(theApp.m_hInstance, MAKEINTRESOURCE(resId));
	image.Load(thHelper::MakePath_ASCII(res));
	// 获取图片的宽 高度
	cx = image.GetWidth();
	cy = image.GetHeight();

	CWnd *pWnd = self->GetDlgItem(id);//获取控件句柄  

	//获取Picture Control控件的大小  
	pWnd->GetWindowRect(&rect);
	//将客户区选中到控件表示的矩形区域内  
	self->ScreenToClient(&rect);
	//窗口移动到控件表示的区域  
	pWnd->MoveWindow(rect.left, rect.top, cx, cy, TRUE);
	pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小  

	CDC *pDc = pWnd->GetDC();//获取picture的DC  

	image.Draw(pDc->m_hDC, rect);//将图片绘制到picture表示的区域内  

	pWnd->ReleaseDC(pDc);
}

HANDLE thHelper::LoadImage(LPCSTR name, UINT type)
{
	return ::LoadImage(theApp.m_hInstance , name , type , 0 , 0 , LR_LOADFROMFILE);
}

BOOL thHelper::ModifyDlgNumer( CWnd *lpWnd, bool bNumer )
{
	HWND hEdit = ::GetWindow(lpWnd->GetSafeHwnd(),GW_CHILD);   
	if(IsWindow(hEdit))   
	{   
		return lpWnd->ModifyStyle(hEdit,0,ES_NUMBER,0);   
	}   
	return FALSE;
}

void thHelper::ResetCtrlWidth( CComboBox *lpCtrl, int off )
{
	CString str;  
	int dx = 0;  
	CSize sz;  
	CDC *pDC = lpCtrl->GetDC();  
	CFont *pOldFont = pDC->SelectObject(lpCtrl->GetFont());  
	for (int i = 0; i < lpCtrl->GetCount(); i++)  
	{  
		lpCtrl->GetLBText(i, str);  
		sz = pDC->GetTextExtent(str);  

		if (sz.cx > dx)  
		{  
			dx = sz.cx;  
		}  

	}  
	lpCtrl->SetDroppedWidth(dx+ off);
	pDC->SelectObject(pOldFont);  
	lpCtrl->ReleaseDC(pDC); 
}

bool thHelper::CompTime( WORD wYear,WORD wMonth,WORD wDay )
{
	SYSTEMTIME _sysTime = thHelper::Time<SYSTEMTIME>(TM_SysTime);
	if (_sysTime.wYear==wYear
		&&_sysTime.wMonth==wMonth
		&&_sysTime.wDay==wDay)
	{
		return true;
	}
	return false;
}

bool thHelper::CompTime( WORD wHour,WORD wMinute )
{
	SYSTEMTIME _sysTime = thHelper::Time<SYSTEMTIME>(TM_SysTime);
	if (_sysTime.wHour==wHour
		&&_sysTime.wMinute==wMinute)
	{
		return true;
	}
	return false;
}

const char* thHelper::AnalyTime( int iDayOfWeek )
{
	s_strTemp = "一";
	switch (iDayOfWeek)
	{
	case 1:	s_strTemp = "六";
		break;
	case 2:	s_strTemp = "一";
		break;
	case 3:	s_strTemp = "二";
		break;
	case 4:	s_strTemp = "三";
		break;
	case 5:	s_strTemp = "四";
		break;
	case 6:	s_strTemp = "五";
		break;
	case 7:	s_strTemp = "日";
		break;
	}
	return s_strTemp.GetString();
}

Bitmap* thHelper::GrayImg( Bitmap *lpIn,int algo/*=GRAY_WEIGHTAVERAGE*/ )
{
	int width = lpIn->GetWidth();
	int height= lpIn->GetHeight();

	Bitmap *lpOut  = NULL;
	if (NULL == (lpOut = new Bitmap(width, height)))
	{
		return NULL;
	}

	BitmapData in_data;
	BitmapData out_data;

	lpIn->LockBits  (&Rect(0, 0, width, height), ImageLockModeRead , PixelFormat32bppARGB, &in_data);
	lpOut->LockBits(&Rect(0, 0, width, height), ImageLockModeWrite, PixelFormat32bppARGB, &out_data);

	unsigned numBytes = (unsigned)((width * height) << 2);
	int value = 0;

	for (unsigned i = 0; i < numBytes; i += 4)
	{
		switch (algo)
		{
		case GRAY_MAX:		// 最大值法
			value = in_rgbValues[i] > in_rgbValues[i + 1] ? in_rgbValues[i] : in_rgbValues[i + 1];
			value = value > in_rgbValues[i + 2] ? value : in_rgbValues[i + 2];
			break;
		case GRAY_AVERAGE: // 平均值法
			value = (int)((in_rgbValues[i] + in_rgbValues[i + 1] + in_rgbValues[i + 2]) / 3);
			break;
		case GRAY_WEIGHTAVERAGE: // 加权平均值法
			value = (int)(in_rgbValues[i] * 0.1 + in_rgbValues[i + 1] * 0.2 
				+ in_rgbValues[i + 2] * 0.7);
			break;
		default:
			break;
		}
		out_rgbValues[i] = (unsigned char)value;
		out_rgbValues[i + 1] = (unsigned char)value;
		out_rgbValues[i + 2] = (unsigned char)value;
		out_rgbValues[i + 3] = in_rgbValues[i + 3];
	}
	lpIn->UnlockBits(&in_data);
	lpOut->UnlockBits(&out_data);
	return lpOut;
}

void thHelper::HideTaskBar( BOOL bHide )
{
	int nCmdShow;
	HWND hWnd;
	LPARAM lParam;

	hWnd = FindWindow("Shell_TrayWnd",NULL);
	if(bHide == TRUE)
	{
		nCmdShow = SW_HIDE;
		lParam = ABS_AUTOHIDE | ABS_ALWAYSONTOP;
	}
	else
	{
		nCmdShow = SW_SHOW;
		lParam = ABS_ALWAYSONTOP;
	}

	ShowWindow(hWnd,nCmdShow);//隐藏任务栏


#ifndef ABM_SETSTATE 
#define ABM_SETSTATE 0x0000000a 
#endif 

	APPBARDATA apBar; 
	memset(&apBar, 0, sizeof(apBar)); 
	apBar.cbSize = sizeof(apBar); 
	apBar.hWnd = hWnd; 
	if(apBar.hWnd != NULL) 
	{ 
		apBar.lParam = lParam; 
		SHAppBarMessage(ABM_SETSTATE, &apBar); //设置任务栏自动隐藏
	} 
}

void thHelper::XorBorder( HWND hWnd )
{
	HPEN hPen,hOldPen; 
	CRect rect; //当前窗体区域 
	//HDC hdc=GetWindowDC(hWnd); 
	HDC hdc=GetWindowDC(0);
	GetWindowRect(hWnd,&rect); 
	//调整边框 
	//rect.bottom-=rect.top; 
	//rect.right-=rect.left; 
	//rect.left=rect.top=0; 
	//rect.left -= 1;
	//rect.top  -= 1;
	//rect.right+= 2;
	//rect.bottom+= 2;
	SetROP2(hdc,R2_NOTXORPEN); 
	hPen=CreatePen(PS_SOLID,4,RGB(0,0,0)); 
	hOldPen=(HPEN)::SelectObject(hdc,hPen); 
	//选择刷子为空，使矩形不填充内部 
	SelectObject(hdc,GetStockObject(NULL_BRUSH)); 
	//Rectangle(hdc,0,0,rect.Width(),rect.Height());
	Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);

	SelectObject(hdc,hOldPen); 
	DeleteObject(hPen); 
	ReleaseDC(0,hdc); 
}

bool thHelper::Ini_SetFile( const char *file )
{
	CString strFile = file;
	s_strIniFile = thHelper::MakePath_ASCII(strFile);
_OPEN:
	FILE *hf = fopen(s_strIniFile, "r");
	if(!hf) 
	{
		if (s_strIniFile.CompareNoCase(strFile)==0)
		{
			return false;
		}
		s_strIniFile = strFile;
		goto _OPEN;
	}
	else fclose(hf);
	return true;
}

bool thHelper::Ini_Check( const char *section, const char *name )
{
	char buf[256];
	if(!s_strIniFile.IsEmpty()) {
		GetPrivateProfileString(section, name, "CKNULL", buf, sizeof(buf), s_strIniFile);
		s_strTemp = buf;
		if (s_strTemp.CompareNoCase("CKNULL")==0) return false;
		return true;
	}
	return false;
}

void thHelper::Ini_Wirte( const char *szFormat, ... )
{
	if (s_strIniFile.IsEmpty()) return;
	FILE *hf = NULL;
	va_list ap;

	hf = fopen(s_strIniFile, "a");
	if(!hf) return;

	va_start(ap, szFormat);
	vfprintf(hf, szFormat, ap);
	va_end(ap);

	fclose(hf);
}

void thHelper::Ini_SetInt(const char *section, const char *name, int value)
{
	char buf[256];

	if(!s_strIniFile.IsEmpty()) {
		sprintf(buf,"%d",value);
		WritePrivateProfileString(section, name, buf, s_strIniFile);
	}
}


int thHelper::Ini_GetInt(const char *section, const char *name)
{
	char buf[256];

	if(!s_strIniFile.IsEmpty()) {
		if(GetPrivateProfileString(section, name, "", buf, sizeof(buf), s_strIniFile))
		{ return atoi(buf); }
		else { return 0; }
	}
	return 0;
}


void thHelper::Ini_SetFloat(const char *section, const char *name, float value)
{
	char buf[256];

	if(!s_strIniFile.IsEmpty()) {
		sprintf(buf,"%f",value);
		WritePrivateProfileString(section, name, buf, s_strIniFile);
	}
}


float thHelper::Ini_GetFloat(const char *section, const char *name)
{
	char buf[256];

	if(!s_strIniFile.IsEmpty()) {
		if(GetPrivateProfileString(section, name, "", buf, sizeof(buf), s_strIniFile))
		{ return (float)atof(buf); }
		else { return 0; }
	}
	return 0.0f;
}


void thHelper::Ini_SetString(const char *section, const char *name, const char *value)
{
	if(!s_strIniFile.IsEmpty()) WritePrivateProfileString(section, name, value, s_strIniFile);
}


const char* thHelper::Ini_GetString(const char *section, const char *name)
{
	char szIniString[1024];
	if(!s_strIniFile.IsEmpty()) GetPrivateProfileString(section, name, 0, szIniString, sizeof(szIniString), s_strIniFile);
	else return "";
	s_strTemp = szIniString;
	return s_strTemp.GetString();
}

void thHelper::Ini_SetVectorArry( const char *section, const char *name, IniMapPtr& mapIni )
{
	CString strNum;
	s_strTemp = "";
	for (int i=0; i<(int)mapIni.size(); ++i)
	{
		strNum.Format("%.2f,",mapIni[i]);
		s_strTemp.Append(strNum);
	}
	s_strTemp.Append("TOP");
	Ini_SetString(section,name,s_strTemp);
}

IniMapPtr& thHelper::Ini_GetVectorArry( const char *section, const char *name )
{
	CString strTemp = Ini_GetString(section,name);
	_thHead.m_mapIni.clear();
	float fTemp = 0;
	int   iIndex = 0;
	while (TRUE)
	{
		int iFind = strTemp.Find(",");
		int iMF = strTemp.GetLength();
		CString strNum = strTemp.Mid(0,iFind);
		fTemp = (float)atof(strNum.GetString());
		strTemp = strTemp.Mid(iFind+1);
		_thHead.m_mapIni.push_back(fTemp);
		if ( strTemp.CompareNoCase("TOP") == 0 )
		{
			break;
		}
		if (iIndex >= TH_FRAME_MAX)
		{
			_thHead.m_mapIni.clear();
			fTemp = 200;
			_thHead.m_mapIni.push_back(fTemp);
			break;
		}
	}
	return _thHead.m_mapIni;
}

void thHelper::Dlg_Push( int iDD,THDialog *dlg )
{
	//if (!dlg) return;
	//DlgMapPtr::iterator iter = s_lpTH->m_mapDlg.find(iDD);
	//if (iter != s_lpTH->m_mapDlg.end())
	//{
	//	if (!iter->second.bDelete) 
	//	{
	//		dlg->EtCancel();
	//		return;
	//	}
	//}

	//stDlgZJ dlgZJ;
	//dlgZJ.pdlg = dlg;
	//dlgZJ.bDelete = false;
	//s_lpTH->m_mapDlg[iDD] = dlgZJ;
	//dlg->SetThis(&s_lpTH->m_mapDlg[iDD].bDelete);
	////s_lpTH->m_mapDlg.insert(DlgMapPtr::value_type(iDD,dlg));
}

THDialog* thHelper::Dlg_Get( int iDD )
{
	//DlgMapPtr::iterator iter = s_lpTH->m_mapDlg.find(iDD);
	//if (iter != s_lpTH->m_mapDlg.end())
	//{
	//	return iter->second.pdlg;
	//}
	return NULL;
}

void thHelper::Dlg_Show( int iDD )
{
	//DlgMapPtr::iterator iter = s_lpTH->m_mapDlg.find(iDD);
	//if (iter != s_lpTH->m_mapDlg.end())
	//{
	//	if (!iter->second.bDelete)
	//	{
	//		THDialog *lpDlg = iter->second.pdlg;
	//		if (lpDlg) lpDlg->ShowDlg(true,false);
	//	}
	//}
}

void thHelper::Dlg_Release( int iDD )
{
	//THDialog *lpDlg = NULL;
	//DlgMapPtr::iterator iter = s_lpTH->m_mapDlg.begin();
	//if (iDD!=-1)
	//{
	//	iter = s_lpTH->m_mapDlg.find(iDD);
	//	if (iter != s_lpTH->m_mapDlg.end())
	//	{
	//		if (!iter->second.bDelete)
	//		{
	//			lpDlg = iter->second.pdlg;
	//			if (lpDlg) lpDlg->EtCancel();
	//			s_lpTH->m_mapDlg.erase(iter);
	//		}
	//	}
	//	return;
	//}
	//while (iter != s_lpTH->m_mapDlg.end())
	//{
	//	if (!iter->second.bDelete)
	//	{
	//		lpDlg = iter->second.pdlg;
	//		if (lpDlg) lpDlg->EtCancel();
	//	}
	//	++iter;
	//}
	//s_lpTH->m_mapDlg.clear();
}

THPopWindow* thHelper::Dlg_GifTs( int iDD,CWnd *lpParent )
{
	//THPopWindow *lpGif = (THPopWindow *)thHelper::Dlg_Get(iDD);
	//if (lpGif) lpGif->SetWindow(WD_LOAD,0,0);
	//else
	//{
	//	thHelper::Dlg_Release(iDD);
	//	lpGif = new THPopWindow;
	//	lpGif->Create(IDD_THIMGTOOL_DIALOG,lpParent);
	//	lpGif->Init();
	//	lpGif->SetWindow(WD_LOAD,0,0);
	//	lpGif->ShowDlg(true);
	//	thHelper::Dlg_Push(iDD,(THDialog*)lpGif);
	//}
	//return lpGif;
	return nullptr;
}

BOOL CALLBACK enumUserWindowsCB(HWND hwnd,LPARAM lParam) 
{ 
	long wflags = GetWindowLong(hwnd, GWL_STYLE); 
	if(!(wflags & WS_VISIBLE)) return TRUE; 

	HWND sndWnd; 
	if( !(sndWnd=FindWindowEx(hwnd, NULL, "SHELLDLL_DefView", NULL)) ) return TRUE; 

	HWND targetWnd; 
	if( !(targetWnd=FindWindowEx(sndWnd, NULL, "SysListView32", "FolderView")) ) return TRUE; 

	HWND* resultHwnd = (HWND*)lParam; 
	*resultHwnd = targetWnd; 

	return FALSE; 
}
//
//CSize thHelper::WindowWHRatio( long lW,long lH )
//{
//	CSize size;
//	for(int i = 2; i <= lW; i ++)
//	{
//		while((lW % i == 0) && (lH % i == 0))
//		{
//			lW = lW / i;
//			lH = lH / i;
//		}
//	}
//	size.cx = lW;
//	size.cy = lH;
//	return size;
//}

CSize thHelper::WindowScreenFmt(void)
{
	CSize size;
	HDC hdcScreen = ::GetDC(NULL); 
	size.cx = GetDeviceCaps(hdcScreen,HORZRES); 
	size.cy = GetDeviceCaps(hdcScreen,VERTRES); 
	DeleteObject(hdcScreen); 
	return size;
}

int thHelper::CursorState( int iCr/*=CR_MOVE*/ )
{
	int iCur = _thHead.m_iCursorSt;
	_thHead.m_iCursorSt = iCr;
	_thHead.m_thTimer.End();
	switch (_thHead.m_iCursorSt)
	{
	case CR_MOVE:
		break;
	case CR_STOP:
		_thHead.m_thTimer.Begin();
		break;
	case CR_INCTRL:
		break;
	}
	return iCur;
}

int thHelper::GetCursorState( void )
{
	return _thHead.m_iCursorSt;
}

bool thHelper::CursorSuspend( DWORD dwTime )
{
	if (_thHead.m_thTimer.IsPassTimeMs(dwTime))
	{
		_thHead.m_thTimer.End();
		return true;
	}
	return false;
}

bool thHelper::CursorForce( void )
{
	return _thHead.m_iCursorSt!=CR_STOP;
}

HWND thHelper::FindDesktopWnd( void )
{
	HWND resultHwnd = NULL; 
	::EnumWindows(enumUserWindowsCB, (LPARAM)&resultHwnd); 
	return resultHwnd; 
}

bool thHelper::ForegroundWindow( HWND hWnd )
{
	HWND hForeHwnd = ::GetForegroundWindow();
	return hForeHwnd == hWnd;
}

bool thHelper::ForegroundWindow( LPCTSTR strName )
{
	DWORD dwProcessId = thHelper::GetProcessId(strName);
	return thHelper::ForegroundWindow(dwProcessId);
}

bool thHelper::ForegroundWindow( DWORD dwProcessId )
{
	HWND hForeHwnd = ::GetForegroundWindow();
	thHelper::GetProcessHWND(dwProcessId);
	HwndMapPtr::iterator iter = _thHead.m_mapHwnd.begin();
	while (iter != _thHead.m_mapHwnd.end())
	{
		if (*iter==hForeHwnd) return true;
		++iter;
	}
	return false;
}

bool thHelper::BeginDesktop( void )
{
	//if (s_lpDesktop) return true;
	//HRESULT hr;
	//hr = CoCreateInstance( CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER,
	//	IID_IActiveDesktop, (void**)&s_lpDesktop);
	//if ( !SUCCEEDED(hr) )
	//	return false;

	return true;
}

bool thHelper::SetWallPaper( CString strPaper,DWORD dwStyle )
{
	bool bDelete = false;
	//HRESULT hr;
	//IActiveDesktop* pIAD = NULL;
	//if (!s_lpDesktop) 
	//{
	//	bDelete = true;
	//	thHelper::BeginDesktop();
	//}
	//pIAD = s_lpDesktop;

	////// 设置墙纸前先打开活动桌面开关
	////COMPONENTSOPT co = {0};
	////co.dwSize = sizeof(COMPONENTSOPT);
	////co.fEnableComponents = TRUE;
	////co.fActiveDesktop = TRUE;
	////hr = pIAD->SetDesktopItemOptions(&co, 0);
	////if ( !SUCCEEDED(hr) ) 
	////	return FALSE;

	////做以下宽字符的转换是因为SetWallpaper的参数是用的LPCWSTR
	//wchar_t *lpCStr = thHelper::ToUNICODE(strPaper);

	//// 设置墙纸
	//hr = pIAD->SetWallpaper(lpCStr, 0);
	//if ( !SUCCEEDED(hr) )
	//	return false;

	//// 设置墙纸风格
	//WALLPAPEROPT wp = {0};
	//wp.dwSize = sizeof(WALLPAPEROPT);
	//wp.dwStyle |= dwStyle;
	//hr = pIAD->SetWallpaperOptions(&wp,0);
	//if ( !SUCCEEDED(hr) )
	//	return false;

	//// 应用改变
	//hr = pIAD->ApplyChanges(AD_APPLY_ALL);
	//if ( !SUCCEEDED(hr) )
	//	return false;

	//if (bDelete) thHelper::EndDesktop();
	return true;
}

bool thHelper::EndDesktop( void )
{
	// 最后释放接口
//	TH_SAFE_RELEASE(s_lpDesktop);
	return true;
}

void thHelper::PegWindowDesk( HWND hWnd )
{
	// 父窗口
	HWND hWndDesk = thHelper::FindDesktopWnd();
	// 钉在桌面
//	SetWindowLong(hWnd,GWL_HWNDPARENT,(LONG)hWndDesk);
}

HwndMapPtr& thHelper::EnumWindows( LPCTSTR strName,bool bShow/*=true*/ )
{
	DWORD dwProcessId = thHelper::GetProcessId(strName);
	return thHelper::EnumWindows(dwProcessId,bShow);
}

HwndMapPtr& thHelper::EnumWindows( DWORD dwProcessId,bool bShow )
{
	_thHead.m_mapHwnd.clear();
	CString strClass,strTitle;
	char szTemp[256];

	HWND h = GetTopWindow(0);
	while (h)
	{
		DWORD pid = 0;
		DWORD dwTheardId = GetWindowThreadProcessId( h,&pid);

		if (dwTheardId != 0&&pid == dwProcessId)
		{
			::GetClassName(h,szTemp,256);			// 类名 
			strClass = szTemp;
			::GetWindowText(h,szTemp,256);			// 标题
			strTitle = szTemp;

			strClass.MakeUpper();
			BOOL bVisible = ::IsWindowVisible(h);
			if( strClass.CompareNoCase("Shell_TrayWnd") != 0  // 滤掉任务栏本身
				&& strClass.CompareNoCase("Progman") != 0		 // 滤掉桌面
				&& strClass.Find("360JS") == -1					 // 滤掉360度程序
				&& strClass.Find("360TRAY")==-1
				&& strClass.Find("360SAFE")==-1
				&& strClass.Find("Q360NETMONCLASS") == -1)
			{
				if (bShow&&bVisible) 
				{
					_thHead.m_mapHwnd.push_back(h);
				}
				if (!bShow&&!bVisible) _thHead.m_mapHwnd.push_back(h);
			}
		}
		h = GetNextWindow(h,GW_HWNDNEXT);
	}

	return _thHead.m_mapHwnd;
}

bool thHelper::AppShowWindow( int iCmd )
{
	if (iCmd==SW_HIDE)
	{
		// 记录需要隐藏的窗口
		_thHead.m_mapAppHwnd.clear();
		_thHead.m_mapAppHwnd = thHelper::GetProcessHWND("THImgEdit.exe");
	}
	if (_thHead.m_mapAppHwnd.empty()) return false;
	for (int i=_thHead.m_mapHwnd.size()-1;i>=0;--i)
	{
		thHelper::ShowWindow(_thHead.m_mapHwnd[i],iCmd);
	}
	if (iCmd!=SW_HIDE) _thHead.m_mapHwnd.clear();
	return true;
}

bool thHelper::RecordWindow( HWND hWnd )
{
	_thHead.m_mapHwndRec.clear();
	if (hWnd!=0) _thHead.m_mapHwndRec.push_back(hWnd);
	return true;
}

bool thHelper::RecordEmpty( void )
{
	return _thHead.m_mapHwndRec.empty();
}

bool thHelper::RecordMe( HWND hWnd )
{
	if (_thHead.m_mapHwndRec.empty()) return false;
	return _thHead.m_mapHwndRec[0]==hWnd;
}

bool thHelper::ShellExecute(const CString& open, const CString& param)
{
	if((DWORD)::ShellExecute(NULL, NULL, open, param, NULL, SW_SHOWMINIMIZED)>32)
	{
		// 获取Exe
		//char *lpExe = thHelper::GetFileName(open);
		//thHelper::ShowWindow(lpExe,SW_SHOW);
		return true;
	}
	else return false;
}

DWORD thHelper::ShellExecuteEx(const CString& open, const CString& param /*= ""*/)
{
	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = open;// "D:\\NetEase\\逆水寒\\program\\bin\\Release64\\GacRunnerNG.exe";
	ShExecInfo.lpParameters = param;// "42.186.115.17:23056@0[剑履山河] __RESERVE__={1}__CREATE_FACE__={0}__MD5__={}";
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOWMINIMIZED;
	ShExecInfo.hInstApp = NULL;
	BOOL ret = ::ShellExecuteEx(&ShExecInfo);
	if (!ret)
	{
		CloseHandle(ShExecInfo.hProcess);
		return 0;
	}
	return DWORD(ShExecInfo.hProcess);
}

bool thHelper::ShowWindow( LPCTSTR strName,int iCmd )
{
	// 获取窗口
	HwndMapPtr mapHwnd = thHelper::GetProcessHWND(strName);
	if (!mapHwnd.empty())
	{
		HwndMapPtr::iterator iter = mapHwnd.begin();
		while (iter != mapHwnd.end())
		{
			HWND hWnd = *iter;
			char szClassName[256];
			GetClassName(hWnd,szClassName,256);
			GetWindowText(hWnd,szClassName,256);
			thHelper::ShowWindow(hWnd,iCmd);
			++iter;
		}
		return true;
	}
	return false;
}

bool thHelper::ShowWindow( HWND hWnd,int iCmd )
{
	// 显示窗口 提前
	::ShowWindow(hWnd,iCmd);
	::BringWindowToTop(hWnd); 
	::SetForegroundWindow(hWnd);
	::SetFocus(hWnd);
	return true;
}

void thHelper::AppInit( void )
{
	_thHead.m_thTimer.End();
	_thHead.m_mapSound.clear();
	_thHead.m_mapAppHwnd.clear();
	_thHead.m_mapHwnd.clear();
	_thHead.m_mapIni.clear();
	_thHead.m_mapProcess.clear();
	_thHead.m_mapDlg.clear();
	_thHead.m_mapHwndRec.clear();
	thHelper::CursorState(CR_MOVE);

	// 声效
	//_thHead.m_SMManager.Initialize(theApp.m_pMainWnd->m_hWnd);
	//_thHead.m_SMManager.Input_Resource_Load(Resource_Load);
	//_thHead.m_SMManager.Input_Resource_Free(Resource_Free);
	//_thHead.m_SMManager.Input_Log_Write(Log_Write);

	// 报时程序
}

bool thHelper::IsAppExist( void )
{
	// 打开互斥量判断
	HANDLE hMutex = ::OpenMutex(MUTEX_ALL_ACCESS,FALSE,TH_CLASS_MUTEX);
	if ( hMutex != NULL )
	{
		return false;
	}
	// 创建互斥量
	s_hMutex = ::CreateMutex(NULL,TRUE,TH_CLASS_MUTEX);
	return true;
}

void thHelper::AppRun(int delay)
{
	// 鼠标操作
	CPoint pt;
	GetCursorPos(&pt);
	if (_thHead.m_ptCurPos!=pt)
	{
		_thHead.m_ptCurPos = pt;
		thHelper::CursorState(CR_MOVE);
	}
	else if (_thHead.m_iCursorSt!=CR_STOP
		&&_thHead.m_iCursorSt!=CR_INCTRL)
	{
		thHelper::CursorState(CR_STOP);
	}
	HttpClient::run();
	// 计时器
	if (!_thHead.m_vCallTime.empty())
	{
		_thHead.m_vCallTimeC.clear();
		auto iter = _thHead.m_vCallTime.begin();
		while (iter != _thHead.m_vCallTime.end())
		{
			iter->time -= delay;
			if (iter->time <= 0)
			{
				// iter->call();
				_thHead.m_vCallTimeC.push_back(iter->call);
				iter = _thHead.m_vCallTime.erase(iter);
				continue;
			}
			++iter;
		}
		for (auto &v: _thHead.m_vCallTimeC)
		{
			v();
		}
	}

	// 音乐
	//SoundMapPtr::iterator iter = _thHead.m_mapSound.begin();
	//while (iter != _thHead.m_mapSound.end())
	//{
	//	if (_thHead.m_SMManager.Channel_IsStopped(iter->second.chn))
	//	{
	//		iter->second.thTime.Begin();
	//	}
	//	if (iter->second.thTime.IsPassTimeMs(500))
	//	{
	//		iter->second.thTime.End();
	//		_thHead.m_SMManager.Channel_Stop(iter->second.chn);
	//		_thHead.m_SMManager.Sound_Free(iter->second.chn);
	//		iter = _thHead.m_mapSound.erase(iter);
	//		continue;
	//	}
	//	++iter;
	//}
	//// 报时
	//_thHead.m_TimeBS.Run();
	//// Dlg 组件
	//DlgMapPtr::iterator iterDlg = s_lpTH->m_mapDlg.begin();
	//while (iterDlg != s_lpTH->m_mapDlg.end())
	//{
	//	if (iterDlg->second.bDelete)
	//	{
	//		iterDlg = s_lpTH->m_mapDlg.erase(iterDlg);
	//		continue;
	//	}
	//	if (iterDlg->second.pdlg)
	//	{
	//		iterDlg->second.pdlg->Run();
	//	}
	//	++iterDlg;
	//}
}

void thHelper::AppExit( void )
{
	//互斥量
	if(s_hMutex)
	{
		::ReleaseMutex(s_hMutex);
		::CloseHandle(s_hMutex);
		s_hMutex = NULL;
	}
//	_thHead.m_SMManager.Release();
	thHelper::Dlg_Release(-1);
	thHelper::EndDesktop();

	exitTempFile();
}

HwndMapPtr& thHelper::GetProcessHWND( LPCTSTR strName )
{
	//DWORD ProcId = GetThreadId(strName);
	//
	//s_mapHwnd.clear();
	//EnumThreadWindows(ProcId,EnEnumThreadWndProc,(LPARAM)&s_mapHwnd);
	return thHelper::EnumWindows(strName);
}

HwndMapPtr& thHelper::GetProcessHWND( DWORD dwProcessId )
{
	//DWORD ProcId = GetThreadId(dwProcessId);

	//s_mapHwnd.clear();
	//EnumThreadWindows(ProcId,EnEnumThreadWndProc,(LPARAM)&s_mapHwnd);
	return thHelper::EnumWindows(dwProcessId);
}

HWND thHelper::GetHwndByTitleID( const char*lpTitile,DWORD dwProcessId )
{
	GetProcessHWND(dwProcessId);
	HwndMapPtr::iterator iter = _thHead.m_mapHwnd.begin();
	while (iter != _thHead.m_mapHwnd.end())
	{
		char szTitle[256] = {0};
		::GetWindowText(*iter,szTitle,256);
		if (strcmp(szTitle,lpTitile)==0)
		{
			return *iter;
		}
		++iter;
	}
	return 0;
}

DWORD thHelper::GetThreadId( LPCTSTR strName )
{
	if(strName==NULL)
	{
		return 0;
	}
	return GetThreadId(GetProcessId(strName));
}

DWORD thHelper::GetThreadId( DWORD dwProcessId )
{
	if(dwProcessId==0) return 0;

	HANDLE hThread=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
	BOOL bHave = FALSE;
	THREADENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	TCHAR buf[100] = {0};
	if(hThread)
	{
		bHave=Thread32First(hThread,&pe32);
		while(bHave)
		{
			if(pe32.th32OwnerProcessID==dwProcessId)//第一个一般就是主线程ID.
			{
				 ::CloseHandle(hThread);
				return pe32.th32ThreadID;
			}
			bHave=Thread32Next(hThread,&pe32);
		}
	}
	 ::CloseHandle(hThread);
	return 0;
}

DWORD thHelper::GetProcessId( LPCTSTR strName )
{
	ProcessMapPtr::iterator iter = _thHead.m_mapProcess.find(strName);
	if (iter != _thHead.m_mapProcess.end()) 
	{
		return iter->second.th32ProcessID;
	}
	PROCESSENTRY32 pe32 = { sizeof(pe32) };
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(Process32First(hSnapshot, &pe32))
	{
		do
		{
			if( strcmp(strName,pe32.szExeFile) == 0 ) 
			{ 
				CloseHandle(hSnapshot);
				return pe32.th32ProcessID;
			} 
		}
		while(Process32Next(hSnapshot, &pe32));
	}
	CloseHandle(hSnapshot);
	return 0;
}

const char* thHelper::GetFileExe( HWND hWnd )
{
	DWORD dwProcessId = 0;
	::GetWindowThreadProcessId(hWnd,&dwProcessId);
	return GetFileExe(dwProcessId);
}

const char* thHelper::GetFileExe( DWORD dwProcessId )
{
	if (dwProcessId==0) return NULL;
	TCHAR szBuffe[MAX_PATH];
	HANDLE open = OpenProcess(PROCESS_VM_READ|PROCESS_QUERY_INFORMATION,false,dwProcessId);
	if (open==0)
	{
		// 提升权限
		thHelper::AdjustPrivileges(SE_SHUTDOWN_NAME);
		open = OpenProcess(PROCESS_VM_READ|PROCESS_QUERY_INFORMATION,false,dwProcessId);
	}
	GetModuleFileNameEx(open,NULL,szBuffe,MAX_PATH);
	CloseHandle(open);
	s_strTemp = szBuffe;
	// 判断是否是有效的路径
	if (!thHelper::CheckFile(s_strTemp))
	{
		return NULL;
	}
	return s_strTemp.GetString();
}

ProcessMapPtr& thHelper::BeginProcess( void )
{
	stProcess process;
	PROCESSENTRY32 pe32 = { sizeof(pe32) };
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(Process32First(hSnapshot, &pe32))
	{
		do
		{
			s_strTemp = GetFileExe(pe32.th32ProcessID);
			if (!s_strTemp.IsEmpty())
			{
				CString strProcess;
				strProcess = s_strTemp;
				strProcess.MakeUpper();
				HMODULE hModule = (HMODULE)0x00400000;
				if ( strProcess.Find("SYSTEM32")==-1
					&& strProcess.Find("360JS")==-1
					&& strProcess.Find("360TRAY")==-1
					&& strProcess.Find("360SAFE")==-1
					&& strProcess.Find("Q360NETMONCLASS")==-1
					&& strProcess.Find("EXPLORER")==-1)
				{
					// 不是系统进程
					process.th32ProcessID = pe32.th32ProcessID;
					process.mapHwnd = GetProcessHWND(pe32.th32ProcessID);
					memset(process.szExePath,0,sizeof(process.szExePath));
					CopyMemory(process.szExePath,s_strTemp,s_strTemp.GetLength());
					s_strTemp = thHelper::GetFileTitle(pe32.szExeFile);
					_thHead.m_mapProcess.insert(ProcessMapPtr::value_type(s_strTemp,process));
				}
			}
		}
		while(Process32Next(hSnapshot, &pe32));
	}
	CloseHandle(hSnapshot);
	return _thHead.m_mapProcess;
}

void thHelper::EndProcess( void )
{
	_thHead.m_mapProcess.clear();
}

HwndMapPtr& thHelper::BeginHwnd( bool bFilter )
{
	//EnumWindows(EnEnumThreadWndProc,(LPARAM)&s_mapHwnd);
	CString strClass,strTitle;
	char szTemp[256];

	HWND hAll = ::GetDesktopWindow(); 
	HWND hwnd = ::GetNextWindow(hAll, GW_CHILD);
	while (hwnd != NULL)
	{
		::GetClassName(hwnd,szTemp,256);			// 类名 
		strClass = szTemp;
		::GetWindowText(hwnd,szTemp,256);			// 标题
		strTitle = szTemp;

		strClass.MakeUpper();
		BOOL bVisible = ::IsWindowVisible(hwnd);
		if(bVisible)
		{
			if (bFilter
				&& strClass.CompareNoCase("Shell_TrayWnd") != 0  // 滤掉任务栏本身
				&& strClass.CompareNoCase("Progman") != 0		 // 滤掉桌面
				&& strClass.Find("360JS") == -1					 // 滤掉360度程序
				&& strClass.Find("360TRAY") == -1
				&& strClass.Find("360SAFE") == -1
				&& strClass.Find("Q360NETMONCLASS") == -1)
			{
				_thHead.m_mapHwnd.push_back(hwnd);
			}
			else if (!bFilter)
			{
				_thHead.m_mapHwnd.push_back(hwnd);
			}
		}
		hwnd = ::GetNextWindow(hwnd, GW_HWNDNEXT);
	}

	return _thHead.m_mapHwnd;
}

void thHelper::EndHwnd( void )
{
	_thHead.m_mapHwnd.clear();
}

bool thHelper::IsProcessExist( LPCTSTR strName )
{
	if (!_thHead.m_mapProcess.empty()) 
	{
		ProcessMapPtr::iterator iter = _thHead.m_mapProcess.find(strName);
		if (iter != _thHead.m_mapProcess.end()) return true;
		return false;
	}

	PROCESSENTRY32 pe32 = { sizeof(pe32) };
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(Process32First(hSnapshot, &pe32))
	{
		do
		{
			if( strcmp(strName,pe32.szExeFile) == 0 ) 
			{ 
				CloseHandle(hSnapshot);
				return true;
			} 
		}
		while(Process32Next(hSnapshot, &pe32));
	}
	CloseHandle(hSnapshot);
	return false;
}

bool thHelper::IsProcessExist( DWORD dwProcessId )
{
	if (!_thHead.m_mapProcess.empty())
	{
		ProcessMapPtr::iterator iter = _thHead.m_mapProcess.begin();
		while (iter != _thHead.m_mapProcess.end()) 
		{
			if (iter->second.th32ProcessID==dwProcessId)
			{
				return true;
			}
			++iter;
		}
		return false;
	}

	PROCESSENTRY32 pe32 = { sizeof(pe32) };
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(Process32First(hSnapshot, &pe32))
	{
		do
		{
			if( dwProcessId == pe32.th32ProcessID ) 
			{ 
				CloseHandle(hSnapshot);
				return true;
			} 
		}
		while(Process32Next(hSnapshot, &pe32));
	}
	CloseHandle(hSnapshot);

	return false;
}

bool thHelper::ExitProcess( LPCTSTR strName )
{
	thHelper::AdjustPrivileges(SE_DEBUG_NAME);
	if (!_thHead.m_mapProcess.empty()) 
	{
		ProcessMapPtr::iterator iter = _thHead.m_mapProcess.find(strName);
		if (iter != _thHead.m_mapProcess.end()) 
		{
			HANDLE handle=OpenProcess(PROCESS_TERMINATE,FALSE,iter->second.th32ProcessID); 
			//结束进程 
			TerminateProcess(handle,0); 
			return true;
		}
		return false;
	}

	PROCESSENTRY32 pe32 = { sizeof(pe32) };
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(Process32First(hSnapshot, &pe32))
	{
		do
		{
			if( strcmp(strName,pe32.szExeFile) == 0 ) 
			{ 
				HANDLE handle=OpenProcess(PROCESS_TERMINATE,FALSE,pe32.th32ProcessID); 
				int iErr = GetLastError();
				//结束进程 
				TerminateProcess(handle,0); 
				CloseHandle(hSnapshot); 
				return true;
			} 
		}
		while(Process32Next(hSnapshot, &pe32));
	}
	CloseHandle(hSnapshot);
	return false;
}

bool thHelper::ExitProcess( DWORD dwProcessId )
{
	if (dwProcessId)
	{
		HANDLE handle=OpenProcess(PROCESS_TERMINATE,FALSE,dwProcessId); 
		//结束进程 
		TerminateProcess(handle,0); 
		return true;
	}
	return false;
}

bool thHelper::ExitProcess( HWND hWnd )
{
	DWORD dwProcessId = 0;
	::GetWindowThreadProcessId(hWnd,&dwProcessId);
	if (dwProcessId==0) return false;
	return ExitProcess(dwProcessId);
}

void thHelper::ThreadDelay(int delay)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}

void thHelper::ThreadDelayRandom(int max)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(thHelper::random_int(30, max)));
}

bool thHelper::IsWindowMe( HWND hWnd )
{
	DWORD dwCurrent = GetCurrentProcessId();
	DWORD dwHwndId = 0;
	::GetWindowThreadProcessId(hWnd,&dwHwndId);
	return dwCurrent==dwHwndId;
}

bool thHelper::AdjustPrivileges( LPCSTR lpNameSE )
{
	// 取得当前进程的[Token](标识)句柄
	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		if (GetLastError()==ERROR_CALL_NOT_IMPLEMENTED) return true;
		else return false;
	}

	// 取得关闭系统的[LUID](本地唯一的标识符)值
	TOKEN_PRIVILEGES tokenPrivilege;
	ZeroMemory(&tokenPrivilege, sizeof(tokenPrivilege));
	if (!::LookupPrivilegeValue(NULL, lpNameSE/*SE_SHUTDOWN_NAME*/, &tokenPrivilege.Privileges[0].Luid))
	{
		CloseHandle(hToken);
		return false;
	}

	// 设置特权数组的元素个数
	tokenPrivilege.PrivilegeCount = 1;

	// 设置[LUID]的属性值
	tokenPrivilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// 为当前进程取得DEBUG权限
	if (!::AdjustTokenPrivileges(hToken, FALSE, &tokenPrivilege, 0, NULL, NULL))
	{
		CloseHandle(hToken);
		return false;
	}

	return true;
}

bool thHelper::ExitWindow( int iEt )
{
	// 提升获取权限
	thHelper::AdjustPrivileges(SE_SHUTDOWN_NAME);

	BOOL bOK = FALSE;
	// 执行命令
	switch (iEt)
	{
		// 正常关机
	case ET_SHUTDOWN: bOK = ExitWindowsEx(EWX_SHUTDOWN,0);
		break;
		// 强制关机
	case ET_POWEROFF: 
		{
			// 关机
			bOK = ExitWindowsEx(EWX_POWEROFF,0);
			// 强制关闭运行中的进程
			thHelper::BeginProcess();
			ProcessMapPtr::iterator iter = _thHead.m_mapProcess.begin();
			while (iter != _thHead.m_mapProcess.end())
			{
				HANDLE handle=OpenProcess(PROCESS_TERMINATE,FALSE,iter->second.th32ProcessID); 
				//结束进程 
				TerminateProcess(handle,0);
				++iter;
			}
			thHelper::EndProcess();
		}
		break;
		// 重启
	case ET_REBOOT: bOK = ExitWindowsEx(EWX_REBOOT,0);
		break;
		// 注销
	case ET_LOGOFF: bOK = ExitWindowsEx(EWX_LOGOFF,0);
		break;
		// 待机
	case ET_IDLE: bOK = SetSystemPowerState(TRUE,FALSE);
		break;
		// 休眠
	case ET_OFF: bOK = SetSystemPowerState(FALSE,TRUE);
		break;
		// 锁屏
	case ET_SUOPING: bOK = LockWorkStation();
		break;
		// 关闭显示器
	case ET_MONITORPOWER:
		{
			bOK = SendMessage(theApp.m_pMainWnd->m_hWnd,WM_SYSCOMMAND,SC_MONITORPOWER,2);
		}
		break;
		// 启动屏保
	case ET_SCREENSAVE:
		{
			bOK = SendMessage(theApp.m_pMainWnd->m_hWnd,WM_SYSCOMMAND,SC_SCREENSAVE,0);
		}
		break;
	}
	return bOK == TRUE;
}

Image* thHelper::PrintScreen(HWND hWnd,bool bDesktop,bool bJQ,const char* lpFile)
{
	if (bDesktop) hWnd = GetDesktopWindow();
	// 获取窗口信息
	WINDOWPLACEMENT wcmd;
	GetWindowPlacement(hWnd,&wcmd);
	LONG winLong = GetWindowLong(hWnd, GWL_EXSTYLE);
	BOOL bIconic = IsIconic(hWnd);
	// 截图特殊处理 以便能截取到最下化窗口的图像
	// 获取窗口Z序
	//GetWindow()
	// 前台窗口
	HWND hForeHwnd = GetForegroundWindow();
	if (bJQ&&hForeHwnd!=hWnd)
	{
		// 加强截图
		SetWindowLong(hWnd, GWL_EXSTYLE, winLong|WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd, 0, 1, LWA_ALPHA);

		ShowWindow(hWnd, SW_RESTORE);
		SendMessage(hWnd, WM_PAINT, 0, 0);
	}
	// 获取大小
	CRect rect;
	GetWindowRect(hWnd, &rect);
	// 截取图像
	HDC hDC = GetWindowDC(hWnd);
	HDC memDC = CreateCompatibleDC(hDC); // 绘图内存DCd
	HBITMAP hBitMap = CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
	SelectObject(memDC,hBitMap);

	if (bDesktop||hForeHwnd==hWnd) BitBlt(memDC, 0, 0, rect.Width(), rect.Height(), hDC, 0, 0, SRCCOPY);
	//{
	//	bool bOK = PaintDesktop(memDC);
	//}
	else PrintWindow(hWnd,memDC,0);
	Bitmap *lpImg = Bitmap::FromHBITMAP(hBitMap,0);
	if (lpFile)
	{
		// 保存
		thHelper::SaveImg(lpFile,lpImg);
		TH_SAFE_DELETE(lpImg);
	}
	// 释放资源
	//TH_SAFE_DELETE(lpImg);
	DeleteObject(hBitMap);
	DeleteDC(memDC);
	ReleaseDC(hWnd,hDC);

	// 还原窗口状态
	if (bJQ&&hForeHwnd!=hWnd)
	{
		int iCmd = SW_RESTORE;
		if (bIconic) iCmd = SW_MINIMIZE;
		ShowWindow(hWnd,iCmd);
		SetWindowLong(hWnd, GWL_EXSTYLE, winLong);
		//SetWindowPlacement(hWnd,&wcmd);
	}
	return lpImg;
}

CString thHelper::GetExePathByReg(const char* valueName)
{
	HKEY hKEY;
	LPCTSTR subKey = _T("Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\MuiCache");//主键值
	// open
	long ret0 = RegOpenKeyEx(HKEY_CURRENT_USER, subKey, NULL, KEY_READ, &hKEY);//打开主键
	if (ret0 != ERROR_SUCCESS)//如果无法打开hKEY,则中止程序的执行 ?
	{
		return "";
	}

	DWORD dwPortName = 0;
	DWORD dwComName = 0;

	TCHAR cPortName[MAX_PATH];
	TCHAR cComName[MAX_PATH];
	DWORD dwIndex = 0;
	CString strComName = _T("");

	while (true)
	{
		ZeroMemory(cPortName, MAX_PATH);
		ZeroMemory(cComName, MAX_PATH);

		dwPortName = MAX_PATH;
		dwComName = MAX_PATH;
		if (::RegEnumValue(hKEY, dwIndex, cPortName, &dwPortName, NULL, NULL, (LPBYTE)cComName, &dwComName) == ERROR_NO_MORE_ITEMS)
		{
			break;
		}

		if (strcmp(cComName, valueName) == 0)
		{
			strComName = cPortName;
			break;
		}

		dwIndex++;
	}

	RegCloseKey(hKEY);// 程序结束前要关闭已经打开的 hKEY。 
	return strComName;
}

CString thHelper::Base64EncodeStr( LPCTSTR lpszSrc )
{
	ASSERT(lpszSrc != NULL && AfxIsValidString(lpszSrc));
	const char BASE64_ENCODE_TABLE[64] = {
		65,  66,  67,  68,  69,  70,  71,  72,  // 00 - 07
		73,  74,  75,  76,  77,  78,  79,  80,  // 08 - 15
		81,  82,  83,  84,  85,  86,  87,  88,  // 16 - 23
		89,  90,  97,  98,  99, 100, 101, 102,  // 24 - 31
		103, 104, 105, 106, 107, 108, 109, 110,  // 32 - 39
		111, 112, 113, 114, 115, 116, 117, 118,  // 40 - 47
		119, 120, 121, 122,  48,  49,  50,  51,  // 48 - 55
		52,  53,  54,  55,  56,  57,  43,  47 };// 56 - 63

	unsigned int iTest;

	LPCTSTR pInBuffer=lpszSrc;

	int nSize = (int)_tcslen(lpszSrc);
	char* pOutBuffer=new char[nSize/3*4+5];
	ZeroMemory(pOutBuffer,nSize/3*4+5);

	for(UINT i=0;i<_tcslen(lpszSrc) / 3;i++)
	{
		iTest = (unsigned char) *pInBuffer++;
		iTest = iTest << 8;

		iTest = iTest | (unsigned char) *pInBuffer++;
		iTest = iTest << 8;

		iTest = iTest | (unsigned char) *pInBuffer++;

		//以4 byte倒序写入输出缓冲
		pOutBuffer[3] = BASE64_ENCODE_TABLE[iTest & 0x3F];
		iTest = iTest >> 6;
		pOutBuffer[2] = BASE64_ENCODE_TABLE[iTest & 0x3F];
		iTest = iTest >> 6;
		pOutBuffer[1] = BASE64_ENCODE_TABLE[iTest & 0x3F];
		iTest = iTest >> 6;
		pOutBuffer[0] = BASE64_ENCODE_TABLE[iTest];
		pOutBuffer+=4;
	}

	//设置尾部
	switch (_tcslen(lpszSrc) % 3)
	{
	case 0:
		break;
	case 1:
		iTest = (unsigned char) *pInBuffer;
		iTest = iTest << 4;
		pOutBuffer[1] = BASE64_ENCODE_TABLE[iTest & 0x3F];
		iTest = iTest >> 6;
		pOutBuffer[0] = BASE64_ENCODE_TABLE[iTest];
		pOutBuffer[2] = '='; //用'='也就是64码填充剩余部分
		pOutBuffer[3] = '=';
		break;
	case 2:
		iTest = (unsigned char) *pInBuffer++;
		iTest = iTest << 8;
		iTest = iTest | (unsigned char) *pInBuffer;
		iTest = iTest << 2;
		pOutBuffer[2] = BASE64_ENCODE_TABLE[iTest & 0x3F];
		iTest = iTest >> 6;
		pOutBuffer[1] = BASE64_ENCODE_TABLE[iTest & 0x3F];
		iTest = iTest >> 6;
		pOutBuffer[0] = BASE64_ENCODE_TABLE[iTest];
		pOutBuffer[3] = '='; // Fill remaining byte.
		break;
	}
	pOutBuffer-=nSize/3*4;
	CString strEncode=pOutBuffer;
	delete [] pOutBuffer;
	pOutBuffer=NULL;
	return strEncode;
}

CString thHelper::Base64DecodeStr( LPCTSTR lpszSrc )
{
	ASSERT(lpszSrc != NULL && AfxIsValidString(lpszSrc));
	const unsigned int BASE64_DECODE_TABLE[256] = {
		255, 255, 255, 255, 255, 255, 255, 255, //  00 -  07
		255, 255, 255, 255, 255, 255, 255, 255, //  08 -  15
		255, 255, 255, 255, 255, 255, 255, 255, //  16 -  23
		255, 255, 255, 255, 255, 255, 255, 255, //  24 -  31
		255, 255, 255, 255, 255, 255, 255, 255, //  32 -  39
		255, 255, 255,  62, 255, 255, 255,  63, //  40 -  47
		52,  53,  54,  55,  56,  57,  58,  59, //  48 -  55
		60,  61, 255, 255, 255, 255, 255, 255, //  56 -  63
		255,   0,   1,   2,   3,   4,   5,   6, //  64 -  71
		7,   8,   9,  10,  11,  12,  13,  14, //  72 -  79
		15,  16,  17,  18,  19,  20,  21,  22, //  80 -  87
		23,  24,  25, 255, 255, 255, 255, 255, //  88 -  95
		255,  26,  27,  28,  29,  30,  31,  32, //  96 - 103
		33,  34,  35,  36,  37,  38,  39,  40, // 104 - 111
		41,  42,  43,  44,  45,  46,  47,  48, // 112 - 119
		49,  50,  51, 255, 255, 255, 255, 255, // 120 - 127
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255 };

	const int nSrcCount=(int)_tcslen(lpszSrc);
	int nSize=nSrcCount/4*3;
	if(lpszSrc[nSrcCount-1]=='=')
		nSize--;
	if(lpszSrc[nSrcCount-2]=='=')
		nSize--;
	char* pOutBuffer=new char[nSize+3];
	ZeroMemory(pOutBuffer,nSize+3);
	LPCTSTR pInBuffer=lpszSrc;
	UINT iTest,iPack;
	for(int i=0;i<nSize/3 ;i++)
	{
		for(int j=0;j<4;j++)
		{
			iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
			//InPtr++;
			if (iTest == 0xFF) 
			{
				j--;
				continue; //读到255非法字符
			}
			iPack = iPack << 6 ;
			iPack = iPack | iTest ;
		}
		pOutBuffer[2] = iPack;
		iPack = iPack >> 8;
		pOutBuffer[1] = iPack;
		iPack = iPack >> 8;
		pOutBuffer[0] = iPack;
		//准备写入后3位
		pOutBuffer+= 3; iPack = 0;

	}
	switch(nSize%3)
	{
	case 1:
		iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
		if (iTest != 0xFF)
		{
			iPack = iPack << 6 ;
			iPack = iPack | iTest ;
		}
		iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
		if (iTest != 0xFF)
		{
			iPack = iPack << 6 ;
			iPack = iPack | iTest ;
		}
		iPack = iPack >> 4;
		pOutBuffer[0] = iPack;
		pOutBuffer++;
		break;
	case 2:
		iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
		if (iTest != 0xFF)
		{
			iPack = iPack << 6 ;
			iPack = iPack | iTest ;
		}
		iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
		if (iTest != 0xFF)
		{
			iPack = iPack << 6 ;
			iPack = iPack | iTest ;
		}
		iTest = BASE64_DECODE_TABLE[*pInBuffer++]; // Read from InputBuffer.
		if (iTest != 0xFF)
		{
			iPack = iPack << 6 ;
			iPack = iPack | iTest ;
		}
		iPack = iPack >> 2;
		pOutBuffer[1] = iPack;
		iPack = iPack >> 8;
		pOutBuffer[0] = iPack;
		pOutBuffer+=2;
		break;
	default:
		break;
	}
	pOutBuffer-=nSize;
	CString strDecode=pOutBuffer;
	delete pOutBuffer;
	return strDecode;
}

static inline bool isData(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}
CString thHelper::EncodeString(const CString& str, const CString& key)
{
	std::string dataChars = key;// "coco3/NRVZdBFJuy26+imqtxhlpWaeCGKPTXbfDHLsw0gkovz37/fsString";
	s_strTemp = "";
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	char* bytes_to_encode = (char*)str.GetString();
	int in_len = str.GetLength();
	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (int x = 0; (x < 4); x++)
				s_strTemp.AppendChar(key[char_array_4[x]]);
			i = 0;
		}
	}
	if (i)
	{
		for (j = i; j < 3; j++)
		{
			char_array_3[j] = '\0';
		}
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;
		for (j = 0; j < i + 1; j++)
		{
			s_strTemp.AppendChar(key[char_array_4[j]]);
		}
		while ((i++ < 3))
		{
			s_strTemp.AppendChar('=');
		}
	}
	return s_strTemp;
}

CString thHelper::DecodeString(const CString& str, const CString& key)
{
	std::string dataChars = key;// "coco3/NRVZdBFJuy26+imqtxhlpWaeCGKPTXbfDHLsw0gkovz37/fsString";
	int in_len = str.GetLength();
	s_strTemp = "";
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	while (in_len-- && (str[in_] != '=') && isData(str[in_]))
	{
		char_array_4[i++] = str[in_];
		in_++;
		if (i == 4)
		{
			for (i = 0; i < 4; i++)
			{
				char_array_4[i] = (unsigned char)dataChars.find(char_array_4[i]);
			}
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
			for (i = 0; i < 3; i++)
				s_strTemp.AppendChar(char_array_3[i]);
			i = 0;
		}
	}
	if (i)
	{
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;
		for (j = 0; j < 4; j++)
			char_array_4[j] = (unsigned char)dataChars.find(char_array_4[j]);
		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++)
		{
			s_strTemp.AppendChar(char_array_3[j]);
		}
	}
	return s_strTemp;
}

#include "lua/xxtea/xxtea.h"

CString thHelper::XxteaEncodeStr(const CString& str, const CString& key)
{
	xxtea_long retlen = 0;
	auto retstr = xxtea_encrypt((unsigned char *)str.GetString(), str.GetLength(), (unsigned char *)key.GetString(), key.GetLength(),&retlen);
	s_strTemp.SetString((const char *)retstr, retlen);
	free(retstr);
	return s_strTemp;
}

CString thHelper::XxteaDecodeStr(const CString& str, const CString& key)
{
	xxtea_long retlen = 0;
	auto retstr = xxtea_decrypt((unsigned char *)str.GetString(), str.GetLength(), (unsigned char *)key.GetString(), key.GetLength(), &retlen);
	s_strTemp.SetString((const char *)retstr, retlen);
	free(retstr);
	return s_strTemp;
}

void thHelper::XxteaEncodeByte(thByteStream& sbyte, const CString& key)
{
	xxtea_long retlen = 0;
	auto retstr = xxtea_encrypt((unsigned char *)sbyte.GetData(), sbyte.GetLength(), (unsigned char *)key.GetString(), key.GetLength(), &retlen);
	if (retlen > 0)
	{
		sbyte.SetData(retstr, retlen);
	}
	else
	{
		sbyte.Setlength(retlen);
	}
	free(retstr);
}

void thHelper::XxteaDecodeByte(thByteStream& sbyte, const CString& key)
{
	xxtea_long retlen = 0;
	auto retstr = xxtea_decrypt((unsigned char *)sbyte.GetData(), sbyte.GetLength(), (unsigned char *)key.GetString(), key.GetLength(), &retlen);
	if (retlen > 0)
	{
		sbyte.SetData(retstr, retlen);
	}
	else
	{
		sbyte.Setlength(retlen);
	}
	free(retstr);
}

CString thHelper::TimeFormat( float fPos,float fLeng )
{
	CString strFMT;
	int hrs, mins, secs;

	fPos += 0.49f;
	hrs  =  (int)fPos / 3600;
	mins = ((int)fPos % 3600) / 60;
	secs = ((int)fPos % 3600) % 60;
	strFMT.Format("%02d:%02d",mins,secs);
	if (hrs!=0) strFMT.Format("%02d:%02d:%02d",hrs,mins,secs);
	s_strTemp = strFMT;
	s_strTemp.Append("/");

	fLeng += 0.49f;
	hrs  =  (int)fLeng / 3600;
	mins = ((int)fLeng % 3600) / 60;
	secs = ((int)fLeng % 3600) % 60;
	strFMT.Format("%02d:%02d",mins,secs);
	if (hrs!=0) strFMT.Format("%02d:%02d:%02d",hrs,mins,secs);
	s_strTemp.Append(strFMT);

	return s_strTemp;
}

void* thHelper::Resource_Load( const char *filename, DWORD *size/*=0*/ )
{
	void *ptr;
	HANDLE hF;
	DWORD dwSize = 0;

	hF = ::CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
	if(hF == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	dwSize = GetFileSize(hF, NULL);
	ptr = malloc(dwSize);
	if(!ptr)
	{
		CloseHandle(hF);
		return 0;
	}
	if(ReadFile(hF, ptr, dwSize, &dwSize, NULL ) == 0)
	{
		CloseHandle(hF);
		free(ptr);
		return 0;
	}

	CloseHandle(hF);
	if(size) *size=dwSize;
	return ptr;
}

void thHelper::Resource_Free( void *res )
{
	if (res) free(res);
}

thByteStream* thHelper::ResourceByte_Load( const char *filename, DWORD *size/*=0*/ )
{
	DWORD dwSize = 0;
	BYTE *lpData = (BYTE*)thHelper::Resource_Load(filename,&dwSize);
	if (lpData)
	{
		thByteStream *lpByte = new thByteStream;
		lpByte->Setlength(dwSize);
		lpByte->SetData(lpData,dwSize);
		thHelper::Resource_Free(lpData);
		if (size) *size = dwSize;
		return lpByte;
	}
	return NULL;
}

void thHelper::ResourceByte_Free( thByteStream *res )
{
	if (res)
	{
		res->Setlength(0);
		TH_SAFE_DELETE(res);
	}
}

CString thHelper::ResourceString_Load(const char *filename, DWORD *size /*= 0*/)
{
	DWORD dwSize = 0;
	char *lpData = (char*)thHelper::Resource_Load(filename, &dwSize);
	if (lpData)
	{
		CString str;
		str.SetString(lpData, dwSize);
		thHelper::Resource_Free(lpData);
		if (size) *size = dwSize;
		return std::move(str);
	}
	return "";
}

void thHelper::CreateDirectory(const char *path)
{
	s_strTemp = path;
	// 创建路径
//	PathRemoveFileSpec(s_strTemp.GetBuffer());
	::CreateDirectory(s_strTemp, NULL);
//	s_strTemp.ReleaseBuffer();
}

bool thHelper::CreateFile( const char *filename )
{
	FILE *fp = fopen(filename,"w");
	if (fp) 
	{
		fclose(fp);
		return true;
	}
	return false;
}

void thHelper::ByteToFile( const char *filename,thByteStream *lpData,const char * _Mode )
{
	FILE *fp = fopen(filename,_Mode);
	if (fp)
	{
		fwrite(lpData->GetData(),lpData->GetLength(),sizeof(BYTE),fp);
		fclose(fp);
	}
}

void thHelper::Sound_Play( const char* file )
{
	Sound_PlayEx(file);
}

void thHelper::Sound_PlayEx( const char* file, int volume/*=100*/, int pan/*=0*/, float pitch/*=1.0f*/, bool loop/*=false*/ )
{
	//SoundMapPtr::iterator iter = _thHead.m_mapSound.find(file);
	//if (iter != _thHead.m_mapSound.end())
	//{
	//	return;
	//}
	//stSound stSd;
	//HSOUND sound = _thHead.m_SMManager.Sound_Load(file);
	//stSd.chn = _thHead.m_SMManager.Sound_PlayEx(sound,volume,pan,pitch,loop);
	//stSd.sound = sound;
	//stSd.thTime.End();
	//_thHead.m_mapSound.insert(SoundMapPtr::value_type(file,stSd));
}

bool thHelper::Sound_IsPlaying( const char* file )
{
	//SoundMapPtr::iterator iter = _thHead.m_mapSound.find(file);
	//if (iter != _thHead.m_mapSound.end())
	//{
	//	return _thHead.m_SMManager.Channel_IsPlaying(iter->second.chn);
	//}
	return false;
}

bool thHelper::Sound_IsStopped( const char* file )
{
	//SoundMapPtr::iterator iter = _thHead.m_mapSound.find(file);
	//if (iter != _thHead.m_mapSound.end())
	//{
	//	return _thHead.m_SMManager.Channel_IsStopped(iter->second.chn);
	//}
	return false;
}

void thHelper::Sound_Resume( const char* file )
{
	//SoundMapPtr::iterator iter = _thHead.m_mapSound.find(file);
	//if (iter != _thHead.m_mapSound.end())
	//{
	//	_thHead.m_SMManager.Channel_Resume(iter->second.chn);
	//}
}

void thHelper::Sound_Pause( const char* file )
{
	//SoundMapPtr::iterator iter = _thHead.m_mapSound.find(file);
	//if (iter != _thHead.m_mapSound.end())
	//{
	//	_thHead.m_SMManager.Channel_Pause(iter->second.chn);
	//}
}

void thHelper::Sound_Stop( const char* file )
{
	//SoundMapPtr::iterator iter = _thHead.m_mapSound.find(file);
	//if (iter != _thHead.m_mapSound.end())
	//{
	//	_thHead.m_SMManager.Channel_Stop(iter->second.chn);
	//	_thHead.m_SMManager.Sound_Free(iter->second.sound);
	//	_thHead.m_mapSound.erase(iter);
	//}
}

float thHelper::Sound_GetLength( const char* file )
{
	//SoundMapPtr::iterator iter = _thHead.m_mapSound.find(file);
	//if (iter != _thHead.m_mapSound.end())
	//{
	//	return _thHead.m_SMManager.Channel_GetLength(iter->second.chn);
	//}
	return 0.0f;
}

float thHelper::Sound_GetPos( const char* file )
{
	//SoundMapPtr::iterator iter = _thHead.m_mapSound.find(file);
	//if (iter != _thHead.m_mapSound.end())
	//{
	//	return _thHead.m_SMManager.Channel_GetPos(iter->second.chn);
	//}
	return 0.0f;
}

void thHelper::Sound_SetVolume( const char* file, int volume )
{
	//SoundMapPtr::iterator iter = _thHead.m_mapSound.find(file);
	//if (iter != _thHead.m_mapSound.end())
	//{
	//	_thHead.m_SMManager.Channel_SetVolume(iter->second.chn,volume);
	//}
}

void thHelper::Sound_SetPos( const char* file, float fSeconds )
{
	//SoundMapPtr::iterator iter = _thHead.m_mapSound.find(file);
	//if (iter != _thHead.m_mapSound.end())
	//{
	//	_thHead.m_SMManager.Channel_SetPos(iter->second.chn,fSeconds);
	//}
}

void thHelper::timeCall(int time, const std::function<void(void)>& call)
{
	s_lpTH->m_vCallTime.push_back(thDEC_timeCall(time,call));
}

#include <random>
#include <cstdlib>
class RandomHelper
{
public:
	template<typename T>
	static inline T random_real(T min, T max) {
		std::uniform_real_distribution<T> dist(min, max);
		auto &mt = RandomHelper::getEngine();
		return dist(mt);
	}

	template<typename T>
	static inline T random_int(T min, T max) {
		std::uniform_int_distribution<> dist(min, max);
		auto &mt = RandomHelper::getEngine();
		return dist(mt);
	}
private:
	static std::mt19937 &getEngine()
	{
		static std::random_device seed_gen;
		static std::mt19937 engine(seed_gen());
		return engine;
	}
};

int thHelper::random_int(int min, int max)
{
	return RandomHelper::random_int(min, max);
}

float thHelper::random_float(float min, float max)
{
	return RandomHelper::random_real(min, max);
}

thByteStream& thHelper::exeResToByte(int resId, const CString& resType)
{
	auto hInstance = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInstance, MAKEINTRESOURCE(resId), resType);
	ASSERT(hRsrc != NULL);
	s_tempByte.Setlength(0);
	if (NULL == hRsrc)
	{
		return s_tempByte;
	}

	DWORD dwSize = ::SizeofResource(hInstance, hRsrc);
	LPBYTE lpRsrc = (LPBYTE)::LoadResource(hInstance, hRsrc);
	ASSERT(lpRsrc != NULL);
	if (NULL == hRsrc)
	{
		return s_tempByte;
	}

	//// 后面采用流加载的方式使用到了CreateStreamOnHGlobal，它需要使用HGLOBAL内存  
	//HGLOBAL hMem = ::GlobalAlloc(GMEM_FIXED, dwSize);
	//if (NULL == hMem)
	//{
	//	::FreeResource(lpRsrc);
	//	return NULL;
	//}

	//LPBYTE pMem = (LPBYTE)::GlobalLock(hMem);
	//if (NULL == pMem)
	//{
	//	::GlobalUnlock(hMem);
	//	::GlobalFree(hMem);
	//	::FreeResource(lpRsrc);
	//	return NULL;
	//}

	//memcpy(pMem, lpRsrc, dwSize);
	//IStream * pStream = NULL;
	//HRESULT hr = ::CreateStreamOnHGlobal(hMem, FALSE, &pStream);

	//// 释放资源  
	//::GlobalUnlock(hMem);
	//::GlobalFree(hMem);
	s_tempByte.SetData(lpRsrc, dwSize);
	::FreeResource(lpRsrc);

	return s_tempByte;
}

void thHelper::exeResToFile(int resId, const CString& resType, const CString& file)
{
	// 导出system.zip  
	HRSRC hrSrcSys = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(resId), resType);
	HGLOBAL hGlobalSys = LoadResource(AfxGetResourceHandle(), hrSrcSys);
	LPVOID lpGlobalSys = LockResource(hGlobalSys);

	int ret = 0;
	CFile cfile;
	if (ret = cfile.Open(file, CFile::modeCreate | CFile::modeWrite))
	{
		cfile.Write(lpGlobalSys, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrcSys));
		cfile.Close();
	}

	::UnlockResource(hGlobalSys);
	::FreeResource(hGlobalSys);
	return;
}

void thHelper::pushExitTempFile(const CString& file)
{
	s_exitFile.emplace_back(file);
}

void thHelper::exitTempFile(void)
{
	for (auto&v: s_exitFile)
	{
		DeleteFileA(v);
	}
	s_exitFile.clear();
}

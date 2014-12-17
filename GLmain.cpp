#pragma once
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"glew32.lib")
#pragma comment (lib,"glew32mx.lib")
#pragma comment (lib,"glew32mxs.lib")
#pragma comment (lib,"glew32s.lib")
#pragma comment (lib,"FreeImage.lib")

#include "GL/glew.h"
#include "GL/wglew.h"
#include <stdio.h>
#include <fstream>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stdafx.h"
#include "shaders.h"
#include "FreeImage/FreeImage.h"
#include "image.h"
#include "OpenFileDialog.h"
#include "Resource.h"
#include "texture.h"



HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
HGLRC					hRC = NULL;
WORD xPosDown, yPosDown, nSize;
WORD xPosMove, yPosMove;
FLOAT deltaAngle = 0.0f;
FLOAT deltaMove = 0;
FLOAT angle=0.0;
FLOAT lx=0.0f, lz=0.0f;
bool mStartMes = false;
FLOAT mOrbitX;
FLOAT mOrbitY;
bool mMouseMove;
FIBITMAP *fibmp_img;
HBITMAP hBitmap;
TCHAR * mImagePath;
COLORREF * lpPixels;
BITMAPINFO bmpInfo;
COpenFileDialog mFileDialog; 
CTexture Tex;
bool flag = false;


HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitCompatibleContext();
HRESULT InitContext();
void mouseMove(int x, int y);
void InitGeometry();
void CleanupContext();
void LoadSelectedImage();
void Render();
void GenerateHMap();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );





int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

	if( FAILED( InitCompatibleContext() ) )
		return 0;

	if( FAILED( InitContext() ) )
    {
        CleanupContext();
        return 0;
    }


	InitGeometry();
	

    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
		 if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		  else
			{
				Render();
			}
    }

    CleanupContext();

    return ( int )msg.wParam;
}

void czCreateMenu(HWND hwnd)
{
            HMENU MainMenu = CreateMenu();
            HMENU hPopupMenu = CreatePopupMenu();
 
            AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu, L"&Файл"); 
            {
                AppendMenu(hPopupMenu, MF_STRING, 1008, L"Новый");
                AppendMenu(hPopupMenu, MF_STRING, 1000, L"Сохранить");
                AppendMenu(hPopupMenu, MF_STRING, 1000, L"Сохранить как...");
                AppendMenu(hPopupMenu, MF_STRING, 1000, L"Авторизация");
                AppendMenu(hPopupMenu, MF_STRING | MF_CHECKED, 1000, L"Печать");
                AppendMenu(hPopupMenu, MF_STRING, 1000, L"Выход");
            }
 
            AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&Правка");  
            AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&Окна");    
            AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&Настройки");
            AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, L"&?");   
 
            
            
            SetMenu(hwnd, MainMenu);
}


HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0; 
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"GLProject";
    wcex.hIconSm = 0; 
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 1200, 1800 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow(	L"GLProject",
							L"GLProgram",
							WS_OVERLAPPEDWINDOW,
      0, 0, DEFAULTWINDOWH + DEFAULTTOOLSW, DEFAULTWINDOWH, NULL, NULL, hInstance, NULL );
	czCreateMenu(g_hWnd);
    if( !g_hWnd )
        return E_FAIL;
    ShowWindow( g_hWnd, nCmdShow );
	mStartMes = true;


    return S_OK;
}


LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;
		case WM_LBUTTONDOWN:
		{
		  // Сохраняем координаты курсора мыши
		  xPosDown   = LOWORD(lParam);
		  yPosDown   = HIWORD(lParam); 

		  mMouseMove = true;
        }

		case WM_MOUSEMOVE:
			{
				xPosMove   = LOWORD(lParam);
				yPosMove   = HIWORD(lParam);


			if(mMouseMove)
				mouseMove(xPosMove -xPosDown , yPosMove - yPosDown );
			xPosDown = xPosMove;
			yPosDown = yPosMove;
				break;
			}

		case WM_LBUTTONUP:
			mMouseMove = FALSE;
		break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

		case WM_COMMAND:
			flag =true;
			InitContext();
			InitGeometry() ;
			break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

LRESULT CALLBACK WndProc2( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;
	
        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }
    return 0;
}


HRESULT InitCompatibleContext()
{
	int iMajorVersion=0;
	int iMinorVersion=0;

	HDC hDC = GetDC(g_hWnd);
	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA; 
	pfd.cColorBits = 32;		    
	pfd.cDepthBits = 24;            
	pfd.iLayerType = PFD_MAIN_PLANE;
 
	int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (iPixelFormat == 0) return false;

	if(!SetPixelFormat(hDC, iPixelFormat, &pfd)) return false;


	HGLRC hRCCompatible = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRCCompatible);

	bool bResult = true;

	if(glewInit() != GLEW_OK)
	{
			MessageBoxA(g_hWnd, "Couldn't initialize GLEW!", "Fatal Error", MB_ICONERROR);
			bResult = false;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRCCompatible);

	return bResult;
}

HRESULT InitContext()
{
	int iMajorVersion=3;
	int iMinorVersion=0;

	HDC hDC = GetDC(g_hWnd);

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd,sizeof(pfd)); 

	bool bError=false;
	

	if(WGLEW_ARB_create_context && WGLEW_ARB_pixel_format)
	{
		const int iPixelFormatAttribList[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, 
			WGL_COLOR_BITS_ARB, 32,			       
			WGL_DEPTH_BITS_ARB, 24,             
			WGL_STENCIL_BITS_ARB, 8,
			0 // End of attributes list
		};
		int iContextAttribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, iMajorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, iMinorVersion,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0 // End of attributes list
		};

		int iPixelFormat, iNumFormats;
		wglChoosePixelFormatARB(hDC, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats);

		// PFD seems to be only redundant parameter now
		if(!SetPixelFormat(hDC, iPixelFormat, &pfd))return false; 
    


		hRC = wglCreateContextAttribsARB(hDC, 0, iContextAttribs);
		// If everything went OK
		if(hRC) wglMakeCurrent(hDC, hRC);
		else bError = true;
	}
	else bError = true;
	
	if(bError)
	{
		// Generate error messages
		char sErrorMessage[255], sErrorTitle[255];
		sprintf(sErrorMessage, "OpenGL %d.%d is not supported! Please download latest GPU drivers!", iMajorVersion, iMinorVersion);
		sprintf(sErrorTitle, "OpenGL %d.%d Not Supported", iMajorVersion, iMinorVersion);
		MessageBoxA(g_hWnd, sErrorMessage, sErrorTitle, MB_ICONINFORMATION);
		return false;
	}
   return S_OK;
}
BITMAPFILEHEADER1 header ;
BITMAPINFOHEADER2 bmiHeader;

 int u = 0;
 int v = 0;
 float**hmap ;
 int IndicesCount =0;

 void mouseMove(int x, int y) { 	
	 mOrbitX+=x;
	 mOrbitY+=y;

}
UINT uiVBO[4]; 
UINT uiVAO[1]; 

 void LoadImageDefault (){
	hBitmap = (HBITMAP)LoadImage(NULL, L"Demo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
 }

 void LoadSelectedImage (){
	if (mFileDialog.ShowDialog(g_hWnd))
    {   
		glDeleteBuffers(4, uiVBO);
		glDeleteVertexArrays(2, uiVAO);
		hBitmap = (HBITMAP)LoadImage(NULL, mFileDialog.GetFileName(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	}
 }

void GenerateHMap()
{
	if(!flag)
		LoadImageDefault ();
	else
		LoadSelectedImage ();
    HDC hdcSrc = CreateCompatibleDC(NULL);
    HDC hdcDst = CreateCompatibleDC(NULL);

    BITMAP bm;
    GetObject(hBitmap, sizeof(bm), &bm);

    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = bm.bmWidth;
    bmpInfo.bmiHeader.biHeight = -bm.bmHeight;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;        
    bmpInfo.bmiHeader.biSizeImage = 0;
 
	u =  bm.bmWidth;
	v=  bm.bmHeight;

	IndicesCount= (u-1)*(v-1)*6;
    hmap = new float*[u];
	lpPixels = new COLORREF [u*v];
    GetDIBits( hdcSrc , hBitmap , 0 , v , lpPixels , &bmpInfo , DIB_RGB_COLORS );
 
	for (int count = 0; count < u; count++)
        hmap[count] = new float [v];

    for (int i = 0; i < u; i++) {
        for (int j = 0; j < v; j++) {
			int pos = lpPixels[ (i*v) + j];
			hmap[i][j] = ((float)(( GetBValue(pos) + GetGValue(pos) + GetRValue(pos) ) / 3.0f) / 255.0f)/5;
        }
    }
}

void GenerateLandscape()
{
	class VertexSruct
	{
		public:
			glm::vec3 *pos;
			glm::vec3 *normal;
			glm::vec2 *tex;
		public:
			VertexSruct(int u, int v)
			{
				pos = new glm::vec3[u*v];
				normal = new glm::vec3[u*v];
			    tex = new glm::vec2[u*v];
			}

			~VertexSruct()
			{
			}
	};

	GenerateHMap();
	VertexSruct *vertices = new VertexSruct(u,v);

	unsigned int* indices = new unsigned int[IndicesCount];

	for (int i=0; i<u; i++)
	for (int j=0; j<v; j++)
	{
		float x=(float)i/(float)u-0.5f;
		float z=(float)j/(float)v-0.5f;
		vertices->pos[j*u+i]=glm::vec3(x,hmap[i][j],z) * 7.5f;
		vertices->normal[j*u+i]=glm::vec3(0,1,0);
		vertices->tex[j*u+i]=glm::vec2(x + 0.5f, 1.0f - ((z * (FLOAT)u / (FLOAT)v) + 0.5f));

	}
	for (int i=0; i<(u-1); i++)
		for (int j=0; j<(v-1); j++)
		{
			unsigned int indexa=j*(u-1)+i;
			unsigned int indexb=j*u+i;
			indices[indexa*6+0]=indexb;
			indices[indexa*6+1]=indexb+1+u;
			indices[indexa*6+2]=indexb+1;

			indices[indexa*6+3]=indexb;
			indices[indexa*6+4]=indexb+u;
			indices[indexa*6+5]=indexb+u+1;
		}

   int verticesCount=u*v;
   int indicesCount=IndicesCount;

   glDeleteBuffers(4, uiVBO);
   glDeleteVertexArrays(2, uiVAO);
   glGenVertexArrays(2, uiVAO); 
   glGenBuffers(4, uiVBO);		

   glBindVertexArray(uiVAO[0]); 
   glBindBufferARB(GL_ARRAY_BUFFER, uiVBO[0]); 
   glBufferDataARB(GL_ARRAY_BUFFER, 3*verticesCount*sizeof(float), vertices->pos, GL_STATIC_DRAW); 
   glEnableVertexAttribArrayARB(0); 
   glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 

   glBindBufferARB(GL_ARRAY_BUFFER, uiVBO[1]); 
   glBufferDataARB(GL_ARRAY_BUFFER, 3*verticesCount*sizeof(float), vertices->normal, GL_STATIC_DRAW); 
   glEnableVertexAttribArrayARB(1); 
   glVertexAttribPointerARB(1, 3, GL_FLOAT, GL_FALSE, 0, 0); 

   glBindBufferARB(GL_ARRAY_BUFFER, uiVBO[2]); 
   glBufferDataARB(GL_ARRAY_BUFFER, 2*verticesCount*sizeof(float), vertices->tex, GL_STATIC_DRAW); 
   glEnableVertexAttribArrayARB(2); 
   glVertexAttribPointerARB(2, 2, GL_FLOAT, GL_FALSE, 0, 0); 

   glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, uiVBO[3]); 
   glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, indicesCount*sizeof(unsigned int), indices, GL_STATIC_DRAW); 
}

CShader shVertex, shFragment; 
CShaderProgram spMain; 


void InitGeometry() 
{ 
   GenerateLandscape();

   Tex.loadTexture2D("texdiag.jpg", true);
   Tex.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);

   shVertex.loadShader("data\\shaders\\shader.vert", GL_VERTEX_SHADER); 
   shFragment.loadShader("data\\shaders\\shader.frag", GL_FRAGMENT_SHADER); 

   spMain.createProgram(); 
   spMain.addShaderToProgram(&shVertex); 
   spMain.addShaderToProgram(&shFragment); 
   spMain.linkProgram(); 
   spMain.useProgram(); 

   wglSwapIntervalEXT(1);

   glEnable(GL_DEPTH_TEST);
   glClearDepth(1.0f);
}   


void Render()
{
	HDC hDC = GetDC(g_hWnd);

	glClearColor(0.0f, 0.9f, 0.5f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec3   Eye( 5.0f, 3.0f, 3.0f );
	glm::mat4	mWorld;
	glm::mat4	mView;
	glm::mat4	mProjection;

	mWorld =  glm::translate(0.0f, 0.0f, 0.0f);
	mWorld = mWorld * glm::rotate(mOrbitX, 0.0f, 1.0f, 0.0f); 
	mWorld = mWorld * glm::rotate(mOrbitY, 1.0f, 0.0f, 0.0f);
	mView=		glm::lookAt(Eye,glm::vec3(0,0,1),glm::vec3(0.0f,1.0f,0.0f));
	mProjection=glm::perspectiveFov(90.0f,533.0f,400.0f,0.001f,1000.0f);

	int iWorld=	glGetUniformLocation(spMain.getProgramID(),"mWorld");
	int iView=	glGetUniformLocation(spMain.getProgramID(),"mView");
	int iProjection=	glGetUniformLocation(spMain.getProgramID(),"mProjection");

	glUniformMatrix4fv(iWorld,1,GL_FALSE,glm::value_ptr(mWorld));
	glUniformMatrix4fv(iView,1,GL_FALSE,glm::value_ptr(mView));
	glUniformMatrix4fv(iProjection,1,GL_FALSE,glm::value_ptr(mProjection));

	glm::vec3 LightPos[1];
	glm::vec3 LightColor[1];
	LightPos[0]=glm::vec3(-1.0f,1.0f,1.0f);
	LightColor[0]=glm::vec3(0,1,1);
	int iLightPos=	glGetUniformLocation(spMain.getProgramID(),"vLightPos");
	int iLightColor=glGetUniformLocation(spMain.getProgramID(),"vLightColor");

	glUniform3fv(iLightPos,1,  (GLfloat*)LightPos);
	glUniform3fv(iLightColor,1,(GLfloat*)LightColor);

	glBindVertexArray(uiVAO[0]);
	glUniformMatrix4fv(iWorld,1,GL_FALSE,glm::value_ptr(mWorld));

	glDrawElements(GL_TRIANGLES,IndicesCount,GL_UNSIGNED_INT,0);
	SwapBuffers(hDC);
	DeleteObject(hDC);
}

void CleanupContext()
{
	Tex.releaseTexture();
	spMain.deleteProgram();
	shVertex.deleteShader();
	shFragment.deleteShader();

	HDC hDC = GetDC(g_hWnd);
	wglMakeCurrent(NULL, NULL);
	ReleaseDC(g_hWnd, hDC);

	g_hWnd = NULL;
}



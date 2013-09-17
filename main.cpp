#include "DXUT.h"
#include "DXUT\Optional\DXUTcamera.h"
#include "DXUT\Optional\SDKmisc.h"
#include "DXUT\Optional\DXUTgui.h"

#include "MultiTexturePresenter.h"
#include "SpinningFirework.h"
#include "GlowEffect.h"
#include "MotionBlurEffect.h"

#define SUB_TEXTUREWIDTH 1024
#define SUB_TEXTUREHEIGHT 768

//--------------------------------------------------------------------------------------
//Global Variables
//--------------------------------------------------------------------------------------
MultiTexturePresenter						MultiTexture = MultiTexturePresenter(1,true,SUB_TEXTUREWIDTH,SUB_TEXTUREHEIGHT);
SpinningFirework							SpinFirework = SpinningFirework(SUB_TEXTUREWIDTH,SUB_TEXTUREHEIGHT);
GlowEffect									PostEffect_Glow = GlowEffect(SUB_TEXTUREWIDTH,SUB_TEXTUREHEIGHT);
MotionBlurEffect							PostEffect_Blur = MotionBlurEffect(SUB_TEXTUREWIDTH,SUB_TEXTUREHEIGHT);
//--------------------------------------------------------------------------------------
//Initialization
//--------------------------------------------------------------------------------------
HRESULT Initial()
{ 
	HRESULT hr = S_OK;
	V_RETURN( MultiTexture.Initial() );
	V_RETURN( SpinFirework.Initial() );
	V_RETURN( PostEffect_Glow.Initial() );
	V_RETURN( PostEffect_Blur.Initial() );
	return hr;
}

//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
									   DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	return true;
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D11 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	MultiTexture.ModifyDeviceSettings( pDeviceSettings );
	return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
									  void* pUserContext )
{
	HRESULT hr = S_OK;
	V_RETURN( SpinFirework.CreateResource( pd3dDevice ));
	V_RETURN( PostEffect_Glow.CreateResource( pd3dDevice, SpinFirework.m_pOutputTextureRV, &SpinFirework.m_Camera ));
	V_RETURN( PostEffect_Blur.CreateResource( pd3dDevice, PostEffect_Glow.m_pOutputTextureSRV ));
	V_RETURN( MultiTexture.CreateResource( pd3dDevice, PostEffect_Blur.m_pOutputTextureSRV ));

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
										  const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	MultiTexture.Resize();
	SpinFirework.Resize();
	//PostEffect_Glow.Resize();

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	SpinFirework.Update( fElapsedTime );
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
								  double fTime, float fElapsedTime, void* pUserContext )
{
	SpinFirework.Render( pd3dImmediateContext, fTime, fElapsedTime );
	PostEffect_Glow.Render( pd3dImmediateContext );
	PostEffect_Blur.Render( pd3dImmediateContext );
	MultiTexture.Render( pd3dImmediateContext );

}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
{
	MultiTexture.Release();
	PostEffect_Blur.Release();
	PostEffect_Glow.Release();
	SpinFirework.Release();
}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
						  bool* pbNoFurtherProcessing, void* pUserContext )
{
	SpinFirework.HandleMessages( hWnd, uMsg, wParam, lParam );
	PostEffect_Glow.HandleMessages( hWnd, uMsg, wParam, lParam );
	PostEffect_Blur.HandleMessages( hWnd, uMsg, wParam, lParam );
	return 0;
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Handle mouse button presses
//--------------------------------------------------------------------------------------
void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
					   bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
					   int xPos, int yPos, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Call if device was removed.  Return true to find a new device, false to quit
//--------------------------------------------------------------------------------------
bool CALLBACK OnDeviceRemoved( void* pUserContext )
{
	return true;
}


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// DXUT will create and use the best device (either D3D9 or D3D11) 
	// that is available on the system depending on which D3D callbacks are set below

	// Set general DXUT callbacks
	DXUTSetCallbackFrameMove( OnFrameMove );
	DXUTSetCallbackKeyboard( OnKeyboard );
	DXUTSetCallbackMouse( OnMouse );
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
	DXUTSetCallbackDeviceRemoved( OnDeviceRemoved );


	// Set the D3D11 DXUT callbacks. Remove these sets if the app doesn't need to support D3D11
	DXUTSetCallbackD3D11DeviceAcceptable( IsD3D11DeviceAcceptable );
	DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice );
	DXUTSetCallbackD3D11SwapChainResized( OnD3D11ResizedSwapChain );
	DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );
	DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11ReleasingSwapChain );
	DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );

	// Perform any application-level initialization here

	DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
	DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
	DXUTCreateWindow( L"Particles System" );

	// Only require 10-level hardware
	DXUTCreateDevice( D3D_FEATURE_LEVEL_11_0, true, 1024, 768 );
	DXUTMainLoop(); // Enter into the DXUT ren  der loop

	// Perform any application-level cleanup here

	return DXUTGetExitCode();
}



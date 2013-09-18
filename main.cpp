#include "DXUT.h"
#include "DXUT\Optional\DXUTcamera.h"
#include "DXUT\Optional\SDKmisc.h"
#include "DXUT\Optional\DXUTgui.h"

#include "MultiTexturePresenter.h"
#include "SpinningFirework.h"
#include "GlowEffect.h"
#include "MotionBlurEffect.h"

#define SUB_TEXTUREWIDTH 1280
#define SUB_TEXTUREHEIGHT 800

//--------------------------------------------------------------------------------------
//Global Variables
//--------------------------------------------------------------------------------------
MultiTexturePresenter						MultiTexture = MultiTexturePresenter(1,true,SUB_TEXTUREWIDTH,SUB_TEXTUREHEIGHT);
SpinningFirework							SpinFirework = SpinningFirework(SUB_TEXTUREWIDTH,SUB_TEXTUREHEIGHT);
GlowEffect									PostEffect_Glow = GlowEffect(SUB_TEXTUREWIDTH,SUB_TEXTUREHEIGHT);
MotionBlurEffect							PostEffect_Blur = MotionBlurEffect(SUB_TEXTUREWIDTH,SUB_TEXTUREHEIGHT);

CDXUTDialogResourceManager					DialogResourceManager; 
CDXUTDialog									UI;

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------

#define IDC_GLOWFACTOR_STATIC		1
#define IDC_GLOWFACTOR_SLIDER		2
#define IDC_GLOWBLENDFACTOR_STATIC	3
#define IDC_GLOWBLENDFACTOR_SLIDER	4
#define IDC_BLURFACTOR_STATIC		5
#define IDC_BLURFACTOR_SLIDER		6

#define IDC_FIREINTERVAL_STATIC		7
#define IDC_FIREINTERVAL_SLIDER		8
#define IDC_NUM_FLY1_STATIC			9
#define IDC_NUM_FLY1_SLIDER			10
#define IDC_MAX_SUBDETONATE_STATIC	11
#define IDC_MAX_SUBDETONATE_SLIDER	12
#define IDC_DETONATE_LIFE_STATIC	13
#define IDC_DETONATE_LIFE_SLIDER	14
#define IDC_FIREFLY_LIFE_STATIC		15
#define IDC_FIREFLY_LIFE_SLIDER		16
#define IDC_SDETONATE_LIFE_STATIC	17
#define IDC_SDETONATE_LIFE_SLIDER	18
#define IDC_FIREFLY2_LIFE_STATIC	19
#define IDC_FIREFLY2_LIFE_SLIDER	20


void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

//--------------------------------------------------------------------------------------
//Initialization
//--------------------------------------------------------------------------------------
HRESULT Initial()
{ 
	HRESULT hr = S_OK;

	UI.Init( &DialogResourceManager );
	//UI.SetFont
	UI.SetCallback( OnGUIEvent ); int iY = 10;

	UI.SetFont( 1, L"Comic Sans MS", 400, 400 );
	UI.SetFont( 2, L"Courier New", 16, FW_NORMAL );

	WCHAR sz[100];
	iY += 24;
	swprintf_s( sz, 100, L"Glow factor: %0.2f", PostEffect_Glow.m_CBperResize.glow_factor );
	UI.AddStatic( IDC_GLOWFACTOR_STATIC, sz, 0, iY, 170, 23 );
	UI.AddSlider( IDC_GLOWFACTOR_SLIDER, 0, iY += 26, 170, 23, 0, 1000, 450 );

	swprintf_s( sz, 100, L"Blend factor: %0.2f", PostEffect_Glow.m_CBperResize.blend_factor );
	UI.AddStatic( IDC_GLOWBLENDFACTOR_STATIC, sz, 0,  iY += 26,170, 23 );
	UI.AddSlider( IDC_GLOWBLENDFACTOR_SLIDER, 0, iY += 26, 170, 23, 0, 300, 100 );

	swprintf_s( sz, 100, L"Blur factor: %0.2f", PostEffect_Blur.m_CBperResize.blur_factor );
	UI.AddStatic( IDC_BLURFACTOR_STATIC, sz, 0,  iY += 26,170, 23 );
	UI.AddSlider( IDC_BLURFACTOR_SLIDER, 0, iY += 26, 170, 23, 800, 1000, 970 );


	swprintf_s( sz, 100, L"Fire Interval: %0.2f", SpinFirework.m_CBallInOne.fFireInterval );
	UI.AddStatic( IDC_FIREINTERVAL_STATIC, sz, 0,  iY += 26,170, 23 );
	UI.AddSlider( IDC_FIREINTERVAL_SLIDER, 0, iY += 26, 170, 23, 5, 500, 80 );

	swprintf_s( sz, 100, L"Num of Firefly: %i", SpinFirework.m_CBallInOne.iNumFirefly1s );
	UI.AddStatic( IDC_NUM_FLY1_STATIC, sz, 0,  iY += 26,170, 23 );
	UI.AddSlider( IDC_NUM_FLY1_SLIDER, 0, iY += 26, 170, 23, 1, 93, 93 );
	
	swprintf_s( sz, 100, L"Max SubDetonate: %0.2f", SpinFirework.m_CBallInOne.fMaxSubDetonates );
	UI.AddStatic( IDC_MAX_SUBDETONATE_STATIC, sz, 0,  iY += 26,170, 23 );
	UI.AddSlider( IDC_MAX_SUBDETONATE_SLIDER, 0, iY += 26, 170, 23, 1, 93, 40 );

	swprintf_s( sz, 100, L"Detonate Life: %0.2fs", SpinFirework.m_CBallInOne.fDetonateLife );
	UI.AddStatic( IDC_DETONATE_LIFE_STATIC, sz, 0,  iY += 26,170, 23 );
	UI.AddSlider( IDC_DETONATE_LIFE_SLIDER, 0, iY += 26, 170, 23, 10, 100, 35 );

	swprintf_s( sz, 100, L"Firefly Life: %0.2fs", SpinFirework.m_CBallInOne.fFirefly1Life );
	UI.AddStatic( IDC_FIREFLY_LIFE_STATIC, sz, 0,  iY += 26,170, 23 );
	UI.AddSlider( IDC_FIREFLY_LIFE_SLIDER, 0, iY += 26, 170, 23, 10, 100, 35 );

	swprintf_s( sz, 100, L"SubDetonate Life: %0.2fs", SpinFirework.m_CBallInOne.fSubDetonateLife );
	UI.AddStatic( IDC_SDETONATE_LIFE_STATIC, sz, 0,  iY += 26,170, 23 );
	UI.AddSlider( IDC_SDETONATE_LIFE_SLIDER, 0, iY += 26, 170, 23, 10, 60, 15 );

	swprintf_s( sz, 100, L"Firefly2 Life: %0.2fs", SpinFirework.m_CBallInOne.fFirefly2Life );
	UI.AddStatic( IDC_FIREFLY2_LIFE_STATIC, sz, 0,  iY += 26,170, 23 );
	UI.AddSlider( IDC_FIREFLY2_LIFE_SLIDER, 0, iY += 26, 170, 23, 10, 60, 25 );



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
	ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();
	V_RETURN( DialogResourceManager.OnD3D11CreateDevice( pd3dDevice, pd3dImmediateContext));

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
	HRESULT hr;
	V_RETURN( DialogResourceManager.OnD3D11ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ));
	UI.SetLocation( pBackBufferSurfaceDesc->Width - 180, 0 );
	UI.SetSize( 180, 600 );

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

	DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR2, L"UI" );
	UI.OnRender( fElapsedTime );
	DXUT_EndPerfEvent();
}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
{

	DialogResourceManager.OnD3D11ReleasingSwapChain();
}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
{
	DialogResourceManager.OnD3D11DestroyDevice();
	DXUTGetGlobalResourceCache().OnDestroyDevice();
	MultiTexture.Release();
	PostEffect_Blur.Release();
	PostEffect_Glow.Release();
	SpinFirework.Release();
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
						  bool* pbNoFurtherProcessing, void* pUserContext )
{
	 // Pass messages to dialog resource manager calls so GUI state is updated correctly
	*pbNoFurtherProcessing = DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;

	 // Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = UI.MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;

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
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
		case IDC_GLOWFACTOR_SLIDER:
			{
				WCHAR sz[100];
				float glowFactor= ( float )( UI.GetSlider( IDC_GLOWFACTOR_SLIDER )->GetValue() * 0.01f );
				swprintf_s( sz, 100, L"Glow Factor: %0.2f", glowFactor);
				PostEffect_Glow.m_CBperResize.glow_factor = glowFactor;
				UI.GetStatic( IDC_GLOWFACTOR_STATIC )->SetText( sz );
				break;
			}

		case IDC_BLURFACTOR_SLIDER:
			{
				WCHAR sz[100];
				float blurFactor= ( float )( UI.GetSlider( IDC_BLURFACTOR_SLIDER )->GetValue() * 0.001f );
				swprintf_s( sz, 100, L"Blur Factor: %0.4f", blurFactor);
				PostEffect_Blur.m_CBperResize.blur_factor = blurFactor;
				UI.GetStatic( IDC_BLURFACTOR_STATIC )->SetText( sz );
				break;
			}

		case IDC_GLOWBLENDFACTOR_SLIDER:
			{
				WCHAR sz[100];
				float blendFactor= ( float )( UI.GetSlider( IDC_GLOWBLENDFACTOR_SLIDER )->GetValue() * 0.01f );
				swprintf_s( sz, 100, L"Blend Factor: %0.2f", blendFactor);
				PostEffect_Glow.m_CBperResize.blend_factor = blendFactor;
				UI.GetStatic( IDC_GLOWBLENDFACTOR_STATIC )->SetText( sz );
				break;
			}

		case IDC_FIREINTERVAL_SLIDER:
			{
				WCHAR sz[100];
				float fireInterval= ( float )( UI.GetSlider( IDC_FIREINTERVAL_SLIDER )->GetValue() * 0.01f );
				swprintf_s( sz, 100, L"Fire Interval: %0.2f", fireInterval);
				SpinFirework.m_CBallInOne.fFireInterval = fireInterval;
				UI.GetStatic( IDC_FIREINTERVAL_STATIC )->SetText( sz );
				break;
			}

		case IDC_NUM_FLY1_SLIDER:
			{
				WCHAR sz[100];
				int numOfFirefly= UI.GetSlider( IDC_NUM_FLY1_SLIDER )->GetValue();
				swprintf_s( sz, 100, L"Num Firefly: %i", numOfFirefly);
				SpinFirework.m_CBallInOne.iNumFirefly1s = numOfFirefly;
				UI.GetStatic( IDC_NUM_FLY1_STATIC )->SetText( sz );
				break;
			}

		case IDC_MAX_SUBDETONATE_SLIDER:
			{
				WCHAR sz[100];
				float maxSubdetonate= ( float )( UI.GetSlider( IDC_MAX_SUBDETONATE_SLIDER )->GetValue() );
				swprintf_s( sz, 100, L"Max SubDetonate: %0.2f", maxSubdetonate);
				SpinFirework.m_CBallInOne.fMaxSubDetonates = maxSubdetonate;
				UI.GetStatic( IDC_MAX_SUBDETONATE_STATIC )->SetText( sz );
				break;
			}

		case IDC_DETONATE_LIFE_SLIDER:
			{
				WCHAR sz[100];
				float lifeOfDetonate= ( float )( UI.GetSlider( IDC_DETONATE_LIFE_SLIDER )->GetValue() * 0.1f );
				swprintf_s( sz, 100, L"Detonate Life: %0.2fs", lifeOfDetonate);
				SpinFirework.m_CBallInOne.fDetonateLife = lifeOfDetonate;
				UI.GetStatic( IDC_DETONATE_LIFE_STATIC )->SetText( sz );
				break;
			}

		case IDC_FIREFLY_LIFE_SLIDER:
			{
				WCHAR sz[100];
				float lifeOfFirefly= ( float )( UI.GetSlider( IDC_FIREFLY_LIFE_SLIDER )->GetValue() *0.1f );
				swprintf_s( sz, 100, L"Firefly Life: %0.2fs", lifeOfFirefly);
				SpinFirework.m_CBallInOne.fFirefly1Life = lifeOfFirefly;
				UI.GetStatic( IDC_FIREFLY_LIFE_STATIC )->SetText( sz );
				break;
			}

		case IDC_FIREFLY2_LIFE_SLIDER:
			{
				WCHAR sz[100];
				float lifeOfFirefly= ( float )( UI.GetSlider( IDC_FIREFLY2_LIFE_SLIDER )->GetValue() *0.1f );
				swprintf_s( sz, 100, L"Firefly2 Life: %0.2fs", lifeOfFirefly);
				SpinFirework.m_CBallInOne.fFirefly2Life = lifeOfFirefly;
				UI.GetStatic( IDC_FIREFLY2_LIFE_STATIC )->SetText( sz );
				break;
			}

		case IDC_SDETONATE_LIFE_SLIDER:
			{
				WCHAR sz[100];
				float lifeOfSubDetonate= ( float )( UI.GetSlider( IDC_SDETONATE_LIFE_SLIDER )->GetValue() *0.1f );
				swprintf_s( sz, 100, L"SubDetonate Life: %0.2fs", lifeOfSubDetonate);
				SpinFirework.m_CBallInOne.fSubDetonateLife = lifeOfSubDetonate;
				UI.GetStatic( IDC_SDETONATE_LIFE_STATIC )->SetText( sz );
				break;
			}
	}

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
	
	Initial();
	
	DXUTCreateWindow( L"Particles System" );

	// Only require 10-level hardware
	DXUTCreateDevice( D3D_FEATURE_LEVEL_11_0, true, 1024, 768 );
	DXUTMainLoop(); // Enter into the DXUT ren  der loop

	// Perform any application-level cleanup here

	return DXUTGetExitCode();
}



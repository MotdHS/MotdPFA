/*************************************************************************************************
*
* File: PianoFromAbove.cpp
*
* Description: Main entry point for Piano From Above.
*              Creates windows and enters the GUI and game loops
*
* Copyright (c) 2010 Brian Pantano. All rights reserved.
*
*************************************************************************************************/
#include <Windows.h>
#include <CommCtrl.h>
#include <ctime>

#include "MainProcs.h"
#include "resource.h"

#include "Config.h"
#include "GameState.h"
#include "Renderer.h"
#include "Misc.h"

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, INT nCmdShow );
DWORD WINAPI GameThread( LPVOID lpParameter );

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;
HWND g_hWndBar = NULL;
HWND g_hWndLibDlg = NULL;
HWND g_hWndGfx = NULL;
TSQueue< MSG > g_MsgQueue; // Producer/consumer to hold events for our game thread

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, INT nCmdShow )
{
    /*
    rl::InitWindow(800, 600, "Meows");
    while (!rl::WindowShouldClose()) {
        rl::BeginDrawing();
        rl::ClearBackground(rl::BLACK);
        rl::DrawText("meows", 10, 10, 20, rl::WHITE);
        rl::DrawTextPro(rl::GetFontDefault(), "Please Work!!!!!!!",
            { 50, 50 }, { 0, 0 }, 0, 20, 2, rl::WHITE);
        rl::DrawRectangle(250, 250, 50, 50, rl::GREEN);
        rl::EndDrawing();
    }
    rl::CloseWindow();
    exit(0);
    */
    g_hInstance = hInstance;
    srand( ( unsigned )time( NULL ) );

    // Ensure that the common control DLL is loaded. 
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof( INITCOMMONCONTROLSEX );
    icex.dwICC  = ICC_WIN95_CLASSES | ICC_COOL_CLASSES | ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex); 

    // Initialize COM. For the SH* functions
    HRESULT hr = CoInitialize( NULL );
    if ( FAILED( hr ) ) return 1;

    // Register the window class
    WNDCLASSEX wc;
    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0L;
    wc.cbWndExtra = 0L;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_PFAICON ) );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    // Window is only a container... never seen, thus null brush
    wc.hbrBackground = NULL; //( HBRUSH )GetStockObject( NULL_BRUSH );
    wc.lpszMenuName = MAKEINTRESOURCE( IDM_MAINMENU );
    wc.lpszClassName = CLASSNAME;
    wc.hIconSm = NULL;
    if ( !RegisterClassEx( &wc ) )
        return 1;

    // Register the graphics window class
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = GfxProc;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = GFXCLASSNAME;
    if ( !RegisterClassEx( &wc ) )
        return 1;

    // Register the position control window class
    wc.style = 0;
    wc.lpfnWndProc = PosnProc;
    wc.lpszClassName = POSNCLASSNAME;
    if ( !RegisterClassEx( &wc ) )
        return 1;

    // In addition to getting settings, triggers loading of saved config
    Config &config = Config::GetConfig();
    ViewSettings &cView = config.GetViewSettings();
    PlaybackSettings &cPlayback = config.GetPlaybackSettings();

    // Create the application window
    g_hWnd = CreateWindowEx( 0, CLASSNAME, CLASSNAME, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, cView.GetMainLeft(), cView.GetMainTop(),
                             cView.GetMainWidth(), cView.GetMainHeight(), NULL, NULL, wc.hInstance, NULL );
    if ( !g_hWnd ) return 1;

    // Creation order (z-order) matters big time for full screen

    // Create the controls rebar
    g_hWndBar = CreateRebar( g_hWnd );
    if ( !g_hWndBar ) return 1;

    // Create the library window
    g_hWndLibDlg = CreateDialog( hInstance, MAKEINTRESOURCE( IDD_LIBDLG ), g_hWnd, LibDlgProc );
    if ( !g_hWndLibDlg ) return 1;
    SetWindowLongPtr( g_hWndLibDlg, GWL_EXSTYLE,
                      GetWindowLongPtr( g_hWndLibDlg, GWL_EXSTYLE ) | WS_EX_CONTROLPARENT );

    // Create the graphics window
    g_hWndGfx = CreateWindowEx( 0, GFXCLASSNAME, NULL, WS_CHILD | WS_TABSTOP | WS_CLIPSIBLINGS,
                                0, 0, 800, 600, g_hWnd, NULL, wc.hInstance, NULL );
    if ( !g_hWndGfx ) return 1;

    HACCEL hAccel = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDA_MAINMENU ) );
    if ( !hAccel ) return 1;

    // Get the game going
    HANDLE hThread = CreateThread( NULL, 0, GameThread, new SplashScreen( NULL, NULL ), 0, NULL );
    if ( !hThread ) return 1;

    // Set up GUI and show
    SetPlayMode( GameState::Splash );
    SetOnTop( cView.GetOnTop() );
    ShowControls( cView.GetControls() );
    ShowLibrary( cView.GetLibrary() );
    ShowWindow( g_hWndGfx, SW_SHOW );
    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );
    SetFocus( g_hWndGfx );
    cPlayback.SetPaused( false, false );

    // Enter the message loop
    MSG msg = { 0 };
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        if( !TranslateAccelerator( g_hWnd, hAccel, &msg ) &&
            !IsDialogMessage( g_hWnd, &msg ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }

    // Signal the game thread to exit and wait for it
    g_MsgQueue.ForcePush( msg );
    WaitForSingleObject( hThread, INFINITE );

    // Save settings
    config.SaveConfigValues();

    // Clean up
    UnregisterClass( CLASSNAME, wc.hInstance );
    CoUninitialize();
    return 0;
}

DWORD WINAPI GameThread( LPVOID lpParameter )
{
    if ( !g_hWndGfx ) return 0;

    // Initialize Direct3D
    Renderer *pRenderer = new D3D9Renderer();
    if( FAILED( pRenderer->Init( g_hWndGfx, Config::GetConfig().GetVideoSettings().bLimitFPS ) ) )
    {
        MessageBox( g_hWnd, TEXT( "Fatal error initializing Direct3D. Is DirectX 9 installed properly?" ), TEXT( "Error" ), MB_OK | MB_ICONEXCLAMATION );
        PostMessage( g_hWnd, WM_QUIT, 1, 0 );
        return 1;
    }

    // Create the game object
    GameState *pGameState = reinterpret_cast< GameState* >( lpParameter );
    rl::InitWindow(800, 600, "Piano From Above but raylib");
    HWND g_hWnd_rl = (HWND)rl::GetWindowHandle();
    rl::SetWindowState(rl::ConfigFlags::FLAG_WINDOW_UNDECORATED | rl::ConfigFlags::FLAG_WINDOW_MOUSE_PASSTHROUGH);
    rl::SetWindowPosition(0, 0);
    SetParent(g_hWnd_rl, g_hWndGfx);
    SetWindowLongPtr(g_hWnd_rl, GWL_STYLE, GetWindowLongPtr(g_hWnd_rl, GWL_STYLE) | WS_CHILD);

    pGameState->SetHWnd( g_hWndGfx );
    pGameState->SetRenderer( pRenderer );
    pGameState->Init();
    GameState::GameError ge;

    // Event, logic, render...
    MSG msg = { 0 };
    while( msg.message != WM_QUIT )
    {
        while ( g_MsgQueue.Pop( msg ) )
            pGameState->MsgProc( msg.hwnd, msg.message, msg.wParam, msg.lParam );

        if ( ( ge = GameState::ChangeState( pGameState->NextState(), &pGameState ) ) != GameState::Success )
            PostMessage( g_hWnd, WM_COMMAND, ID_GAMEERROR, ge );
        pGameState->Logic();
        rl::BeginDrawing();
        pGameState->Render();
        //rl::DrawFPS(10, 10);
        rl::EndDrawing();
    }

    delete pGameState;
    delete pRenderer;
    rl::CloseWindow();

    return 0;
}
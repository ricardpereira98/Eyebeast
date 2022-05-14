
/* wxTiny.cpp version 5.0
   Artur Miguel Dias, 2022
   Dept. de Informatica, FCT, Universidade Nova de Lisboa.

   wxTiny is a small framework that supports simple 2D arcade games.
   It has been created for pedagogical purposes.

   wxTiny works on Windows, Linux and MacOS X and is compatible with C and C++.
   wxTiny runs on top of the (wonderful) wxWidgets library.
*/

#include <wx/wx.h>
#include <math.h>
#include "wxTiny.h"

#define wxVersion	(wxMAJOR_VERSION * 10 + wxMINOR_VERSION)

#ifdef __WXMAC__
//	#include <ApplicationServices/ApplicationServices.h>
#endif

using namespace std;

// ----------------------------------------------------------------------------
// Strings
// ----------------------------------------------------------------------------

static wxMBConv *conv;

static void SetupCharConversion() {
	if( strlen("Ã¡") > 1 )
		conv = &wxConvUTF8;
	else
		conv = &wxConvISO8859_1;
}

#define wxStr(s)	wxString(s, *conv)

static char *cStr(wxString s) {
	static char c[1024];
#if _WIN32
	strcpy(c, s.c_str());
#else
	strcpy(c, s.char_str(*conv));
#endif
	return c;
}


// ----------------------------------------------------------------------------
// Console
// ----------------------------------------------------------------------------

#if _WIN32
#ifndef _WIN32_WINNT
#  define _WIN32_WINNT 0x0500
#endif
#include <windows.h>

static HWND console = 0;

static void CreateConsole()
{
	if( AllocConsole() ) {
		console = GetConsoleWindow();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}
}

#if 0
static void Interpreter()
{
	::SetForegroundWindow(console);
	int x;
	do {
		fflush(stdin);
		printf("> ");
		scanf("%d", &x);
		printf("< %d\n", x);
	} while( x != 0 );
}

static void DeleteConsole()
{
	FreeConsole();
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
}
#endif

#else
void CreateConsole()
{
	// Nada
}

#if 0
static void Interpreter()
{
	// Nada
}

static void DeleteConsole()
{
	// Nada
}
#endif

#endif


// ----------------------------------------------------------------------------
// Images
// ----------------------------------------------------------------------------

static wxClientDC *curr_dc = 0;

static const int MAX_BITMAPS = 100;

static wxBitmap *bitMaps[MAX_BITMAPS];
static int n_bitMaps = 0;

int tyCreateImage(tyImage xpm) {
	if( n_bitMaps < MAX_BITMAPS ) {
		bitMaps[n_bitMaps] = new wxBitmap(xpm);
		return n_bitMaps++;
	}
	else {
		tyFatalError((tyStr)"Too many images created.\nBye bye!");
		return 0;
	}
}

int tyCreateImage2(tyImage xpm, tyConstStr color) {
	const char *save = 0, *pos = 0;
	char str[256];
	int i, n, savei;
	sscanf(xpm[0], "%*d %*d %d", &n);
	for(i = 1 ; i <= n; i++) {
		if( (pos = strstr(xpm[i], "$1")) != 0 ) {
			strncpy(str, xpm[i], pos-xpm[i]);
			str[pos-xpm[i]] = '\0';
			strcat(str, color);
			strcat(str, pos+2);
			save = xpm[i];
			savei = i;
			xpm[i] = str;
		}
	}
	int res = tyCreateImage(xpm);
	if( save != 0 )
		xpm[savei] = save;
	return res;
}

void tyDrawImage(int image, int x, int y) {
	if( curr_dc != 0 )
		curr_dc->DrawBitmap(*bitMaps[image], x, y, false);
}

#if !_WIN32
/* XPM */
static tyImage mondrian_xpm = {
/* columns rows colors chars-per-pixel */
"32 32 6 1",
"  c Black",
". c Blue",
"X c #00bf00",
"o c Red",
"O c Yellow",
"+ c Gray100",
/* pixels */
"                                ",
" oooooo +++++++++++++++++++++++ ",
" oooooo +++++++++++++++++++++++ ",
" oooooo +++++++++++++++++++++++ ",
" oooooo +++++++++++++++++++++++ ",
" oooooo +++++++++++++++++++++++ ",
" oooooo +++++++++++++++++++++++ ",
" oooooo +++++++++++++++++++++++ ",
"                                ",
" ++++++ ++++++++++++++++++ .... ",
" ++++++ ++++++++++++++++++ .... ",
" ++++++ ++++++++++++++++++ .... ",
" ++++++ ++++++++++++++++++ .... ",
" ++++++ ++++++++++++++++++ .... ",
" ++++++ ++++++++++++++++++      ",
" ++++++ ++++++++++++++++++ ++++ ",
" ++++++ ++++++++++++++++++ ++++ ",
" ++++++ ++++++++++++++++++ ++++ ",
" ++++++ ++++++++++++++++++ ++++ ",
" ++++++ ++++++++++++++++++ ++++ ",
" ++++++ ++++++++++++++++++ ++++ ",
" ++++++ ++++++++++++++++++ ++++ ",
" ++++++ ++++++++++++++++++ ++++ ",
" ++++++ ++++++++++++++++++ ++++ ",
" ++++++                    ++++ ",
" ++++++ OOOOOOOOOOOO XXXXX ++++ ",
" ++++++ OOOOOOOOOOOO XXXXX ++++ ",
" ++++++ OOOOOOOOOOOO XXXXX ++++ ",
" ++++++ OOOOOOOOOOOO XXXXX ++++ ",
" ++++++ OOOOOOOOOOOO XXXXX ++++ ",
" ++++++ OOOOOOOOOOOO XXXXX ++++ ",
"                                "
};
#endif

// ----------------------------------------------------------------------------
// Classes GameApp, GameFrame & GamePanel
// ----------------------------------------------------------------------------

class GameApp;
class GameFrame;
class GamePanel;

static GameFrame *gameFrame = 0;

class GameApp : public wxApp {
public:
	virtual bool OnInit();
	virtual int OnExit();
};

class GameFrame : public wxFrame {
  public:
	GameFrame();
	void Start();
	void Paused(bool paused);
	void SetSpeed(int speed);
	GamePanel *GetBoard() { return board; }
  protected:
  // event handlers (these functions should _not_ be virtual)
	void OnMenu(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
  private:
	GamePanel *board;
	int menuId;
	void BuildMenuBar(wxMenuBar *menuBar);
	void BuildMenuBarMore(wxMenuBar *menuBar);
	bool HandleDirectMenu(wxString m);
	DECLARE_EVENT_TABLE()
};

class GamePanel: public wxPanel {
  public:
	GamePanel(GameFrame *parent);
	void Paused(bool paused);
	void SetSpeed(int speed);
	void Boost(int b);
	void ResetKey();
	int GetDX() { return dx ; }
	int GetDY() { return dy ; }
	int GetKey() { return key ; }
  protected:
  // event handlers (these functions should _not_ be virtual)
	void OnPaint(wxPaintEvent& event);
  //void OnMouseMove(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	void OnKillFocus(wxFocusEvent& event);
  private:
	GameFrame *owner;
	bool hasFocus;
	int key, dx, dy;
	wxTimer timer;
	bool paused;
	int rythm;
	DECLARE_EVENT_TABLE()
};


// ----------------------------------------------------------------------------
// Class GameApp implementation
// ----------------------------------------------------------------------------

IMPLEMENT_APP(GameApp)

bool GameApp::OnInit()
{
	SetupCharConversion();
	CreateConsole();
	gameFrame = new GameFrame();
	gameFrame->Start();
	SetTopWindow(gameFrame);
	return true;
}

int GameApp::OnExit() {
	return wxApp::OnExit();
}


// ----------------------------------------------------------------------------
// Class GameFrame implementation
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(GameFrame, wxFrame)
	EVT_MENU( 1, GameFrame::OnMenu)
	EVT_MENU( 2, GameFrame::OnMenu)
	EVT_MENU( 3, GameFrame::OnMenu)
	EVT_MENU( 4, GameFrame::OnMenu)
	EVT_MENU( 5, GameFrame::OnMenu)
	EVT_MENU( 6, GameFrame::OnMenu)
	EVT_MENU( 7, GameFrame::OnMenu)
	EVT_MENU( 8, GameFrame::OnMenu)
	EVT_MENU( 9, GameFrame::OnMenu)
	EVT_MENU(10, GameFrame::OnMenu)
	EVT_MENU(11, GameFrame::OnMenu)
	EVT_MENU(12, GameFrame::OnMenu)
	EVT_MENU(13, GameFrame::OnMenu)
	EVT_MENU(14, GameFrame::OnMenu)
	EVT_MENU(15, GameFrame::OnMenu)
	EVT_MENU(16, GameFrame::OnMenu)
	EVT_MENU(17, GameFrame::OnMenu)
	EVT_MENU(18, GameFrame::OnMenu)
	EVT_MENU(19, GameFrame::OnMenu)
	EVT_MENU(20, GameFrame::OnMenu)
	EVT_CLOSE(GameFrame::OnClose)
END_EVENT_TABLE()

GameFrame::GameFrame()
		: wxFrame(0, wxID_ANY, wxT(""),
					wxPoint(80, 30), wxDefaultSize,
					wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
#ifdef __WXMAC__
//	ProcessSerialNumber PSN;
//	GetCurrentProcess(&PSN);
//	TransformProcessType(&PSN,kProcessTransformToForegroundApplication);
#endif

	SetIcon(wxICON(mondrian));
	board = new GamePanel(this);
	board->SetBackgroundColour(*wxWHITE);
	board->SetFocus();
	menuId = 1;
}

void GameFrame::BuildMenuBar(wxMenuBar *menuBar)
{
	wxMenu *menu = 0;
	wxString menuName;
	char *s = strtok(cStr(wxStr(tyMenus())), ":");
	do {
		switch( s[0] ) {
			case '+':
				if( menu != 0 )
					menuBar->Append(menu, menuName);
				menu = new wxMenu;
				menuName = wxStr(s+1);
				break;
			case '-':
				menu->AppendSeparator();
				break;
			default:
				menu->Append(menuId++, wxStr(s));
				break;
		}
	}
	while( (s = strtok(0, ":")) != 0 );
	if( menu != 0 )
		menuBar->Append(menu, menuName);
}

void GameFrame::BuildMenuBarMore(wxMenuBar *menuBar)
{
#if 0
	wxMenu *menu = new wxMenu;
	menuBar->Append(menu, wxStr("Speed"));
	menu->Append(menuId++, wxStr("&1\tCtrl-1"));
	menu->Append(menuId++, wxStr("&2\tCtrl-2"));
	menu->Append(menuId++, wxStr("&3\tCtrl-3"));
	menu->Append(menuId++, wxStr("&4\tCtrl-4"));
	menu->Append(menuId++, wxStr("&5\tCtrl-5"));
	menu->Append(menuId++, wxStr("&6\tCtrl-6"));
	menu->Append(menuId++, wxStr("&7\tCtrl-7"));
	menu->Append(menuId++, wxStr("&8\tCtrl-8"));
	menu->Append(menuId++, wxStr("&9\tCtrl-9"));
#endif
}

bool GameFrame::HandleDirectMenu(wxString m)
{
	return false;
	if( m.length() == 1 ) {
		switch(m.GetChar(0) - '0') {
			case 1: board->Boost(100); SetSpeed(1); break;
			case 2: board->Boost(100); SetSpeed(4); break;
			case 3: board->Boost(100); SetSpeed(7); break;
			case 4: board->Boost(80); SetSpeed(7); break;
			case 5: board->Boost(60); SetSpeed(7); break;
			case 6: board->Boost(40); SetSpeed(7); break;
			case 7: board->Boost(20); SetSpeed(7); break;
			case 8: board->Boost(10); SetSpeed(7); break;
			case 9: board->Boost(5); SetSpeed(7); break;
		}
		return true;
	}
	else
		return false;
}

void GameFrame::Start()
{
	// create the menu bar
	wxMenuBar *menuBar = new wxMenuBar;
	BuildMenuBar(menuBar);
	BuildMenuBarMore(menuBar);
	SetMenuBar(menuBar);

	// title and status bar
	SetTitle(wxStr(tyAppName()));
	CreateStatusBar(tyStatusItems());

	/* must be last */
	SetClientSize(tyWidth(), tyHeight());
	tyHandleStart();
	Show(true);
}

void GameFrame::Paused(bool paused)
{
	board->Paused(paused);
}

void GameFrame::SetSpeed(int speed)
{
	board->SetSpeed(speed);
}

void GameFrame::OnMenu(wxCommandEvent& event)
{
	unsigned int i;
	wxString s = GetMenuBar()->GetLabel(event.GetId());
	wxString m = wxT("");
	for( i = 0 ; i < s.size() && s[i] != '\t' ; i++ )
		if( s[i] != '_' && s[i] != '&' )
			m += s[i];
	if( !HandleDirectMenu(m) ) {
		tyHandleMenuCommand(cStr(m));
		board->Refresh();
	}
}

void GameFrame::OnClose(wxCloseEvent& WXUNUSED(event))
{
	tyHandleMenuCommand((tyStr)"Quit");
}

void tyPaused(bool paused)
{
	if( gameFrame != 0 )
		gameFrame->Paused(paused);
}

void tySetSpeed(int speed) {
	if( gameFrame != 0 )
		gameFrame->SetSpeed(speed);
}

void tySetStatusText(int pos, tyConstStr txt)
{
	if( 0 <= pos && pos < tyStatusItems() && gameFrame != 0 )
		gameFrame->SetStatusText(wxStr(txt), pos);
}

void tyQuit()
{
	if( gameFrame != 0 )
		gameFrame->Destroy();
	exit(0);
}


// ----------------------------------------------------------------------------
// GamePanel
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(GamePanel, wxPanel)
	EVT_PAINT(GamePanel::OnPaint)
//	EVT_MOTION(GamePanel::OnMouseMove)
	EVT_KEY_DOWN(GamePanel::OnKeyDown)
	EVT_KEY_UP(GamePanel::OnKeyUp)
	EVT_TIMER(wxID_ANY, GamePanel::OnTimer)
	EVT_SET_FOCUS(GamePanel::OnSetFocus)
	EVT_KILL_FOCUS(GamePanel::OnKillFocus)
END_EVENT_TABLE()

GamePanel::GamePanel(GameFrame *parent):
	wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE),
	timer(this)
{
	owner = parent;
	hasFocus = false;
	timer.Start(25);
	ResetKey();
	Paused(false);
	SetSpeed(5);
}

void GamePanel::OnPaint(wxPaintEvent &WXUNUSED(event))
{
	wxPaintDC dc(this);
	PrepareDC(dc);
	dc.SetPen(*wxWHITE_PEN);
	dc.Clear();
	wxClientDC *save_dc = curr_dc;
	curr_dc = &dc;
	tyHandleRedraw();
	curr_dc = save_dc;
}

/*
void GamePanel::OnMouseMove(wxMouseEvent &event)
{
	wxClientDC dc(this);
	PrepareDC(dc);
	dc.SetPen(*wxWHITE_PEN);
	wxPoint pos = event.GetPosition();
	long x = dc.DeviceToLogicalX( pos.x );
	long y = dc.DeviceToLogicalY( pos.y );
	wxString str;
	str.Printf( wxT("x=%d, y=%d"), (int)x, (int)y );
	owner->SetStatusText(str, 0);
}
*/

void GamePanel::ResetKey() {
	key = dx = dy = 0;
}

void GamePanel::OnKeyDown(wxKeyEvent& event) {
	int k = event.GetKeyCode();
	key = 0;
	switch ( k )
	{
		case WXK_LEFT:	case 'O': case 'J': dx = -1; dy =  0; break;
		case WXK_UP:	case 'Q': case 'I': dx =  0; dy = -1; break;
		case WXK_RIGHT: case 'P': case 'L': dx =  1; dy =  0; break;
		case WXK_DOWN:	case 'A': case 'K': dx =  0; dy =  1;  break;
		default: key= (' '<=k && k<='~') ? k : 0; dx = 0; dy = 0; break;
	}
	event.Skip();
}

void GamePanel::OnKeyUp(wxKeyEvent& event){
#if 1
	int k = event.GetKeyCode();
	key = 0;
	switch ( k )
	{
		case WXK_LEFT:	case 'O': case 'J': if(dx==-1) dx = 0; break;
		case WXK_UP:	case 'Q': case 'I': if(dy==-1) dy = 0; break;
		case WXK_RIGHT: case 'P': case 'L': if(dx== 1) dx = 0; break;
		case WXK_DOWN:	case 'A': case 'K': if(dy== 1) dy = 0; break;
	}
#else
	ResetKey();
#endif
	event.Skip();
}

int tyKeyDeltaX() {
	return gameFrame != 0 ? gameFrame->GetBoard()->GetDX() : 0;
}

int tyKeyDeltaY() {
	return gameFrame != 0 ? gameFrame->GetBoard()->GetDY() : 0;
}

char tyKey() {
	return gameFrame != 0 ? gameFrame->GetBoard()->GetKey() : 0;
}

void tyResetKey() {
	if( gameFrame != 0 )
		gameFrame->GetBoard()->ResetKey();
}

void GamePanel::OnTimer(wxTimerEvent& WXUNUSED(event))
{
	static int count = 0;
	if( hasFocus && !paused ) {
		wxClientDC dc(this);
		PrepareDC(dc);
		dc.SetPen(*wxWHITE_PEN);
		wxClientDC *save_dc = curr_dc;
		curr_dc = &dc;
		if( ++count >= rythm ) {
			tyHandleTime();
			count = 0;
		}
		curr_dc = save_dc;
	}
}

void GamePanel::Paused(bool paused)
{
	this->paused = paused;
}

void GamePanel::SetSpeed(int speed)
{
	if( speed < 0 )
		speed = 0;
	else if( speed > TY_MAX_SPEED )
		speed = TY_MAX_SPEED;
	this->rythm = TY_MAX_SPEED + 1 - speed;
 }

void GamePanel::Boost(int b)
{
	timer.Start(b);
}

void GamePanel::OnSetFocus(wxFocusEvent& WXUNUSED(event))
{
	hasFocus = true;
}

void GamePanel::OnKillFocus(wxFocusEvent& WXUNUSED(event))
{
	hasFocus = false;
}


// ----------------------------------------------------------------------------
// TIME
// ----------------------------------------------------------------------------

static long long baseSeconds = wxGetLocalTime();

void tySecondsSetZero() {
	baseSeconds = wxGetLocalTime();
}

int tySeconds() {
	return wxGetLocalTime() - baseSeconds;
}


// ----------------------------------------------------------------------------
// DIALOGS
// ----------------------------------------------------------------------------

#define MAX_MESSAGE		1024

void tyAlertDialog(tyConstStr title, tyConstStr fmt, ...) {
	char message[MAX_MESSAGE];
	va_list v;
	va_start(v, fmt);
	vsnprintf(message, MAX_MESSAGE, fmt, v);
	va_end(v) ;
	wxMessageDialog *d =
		new wxMessageDialog(0, wxStr(message),
							wxStr(title), wxOK | wxICON_INFORMATION);
	d->ShowModal();
	d->Destroy();
	if( gameFrame != 0 )
		gameFrame->Refresh();
	tyResetKey();
}

void tyError(tyConstStr fmt, ...)
{
	char message[MAX_MESSAGE];
	va_list v;
	va_start(v, fmt);
	vsnprintf(message, MAX_MESSAGE, fmt, v);
	va_end(v) ;
	tyAlertDialog((tyStr)"Error", message);
	tyResetKey();
}

void tyFatalError(tyConstStr fmt, ...)
{
	char message[MAX_MESSAGE];
	va_list v;
	va_start(v, fmt);
	vsnprintf(message, MAX_MESSAGE, fmt, v);
	va_end(v) ;
	tyAlertDialog((tyStr)"Fatal Error", message);
	tyQuit();
}

tyStr tyQueryDialog(tyConstStr title, tyConstStr question) {
	wxTextEntryDialog *d =
		new wxTextEntryDialog(
				0, wxStr(question),
				wxStr(title), wxT(""),
				wxOK | wxCENTRE);
	tyResetKey();
	return d->ShowModal() == wxID_OK ? cStr(d->GetValue()) : 0;
}


tyStr tyOpenDialog()
{
	wxFileDialog *d = new wxFileDialog(
			gameFrame, wxT("Open File"), wxT(""), wxT(""),
			wxT("Text files (*.txt)|*.txt|All files (*)|*"),
#if wxVersion > 28
			wxFD_OPEN
#else
			wxOPEN
#endif
	);
	tyResetKey();
	return d->ShowModal() == wxID_OK ? cStr(d->GetPath()) : 0;
}

tyStr tySaveDialog()
{
	wxFileDialog *d = new wxFileDialog(
			gameFrame, wxT("Save File As"), wxT(""), wxT(""),
			wxT("Text files (*.txt)|*.txt|All files (*)|*"),
#if wxVersion > 28
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT
#else
			wxSAVE | wxOVERWRITE_PROMPT
#endif
	);
	tyResetKey();
	return d->ShowModal() == wxID_OK ? cStr(d->GetPath()) : 0;
}


// ----------------------------------------------------------------------------
// Numeric functions
// ----------------------------------------------------------------------------

int tyDistance(int x1, int y1, int x2, int y2)
{
	int distx = abs(x1 - x2);
	int disty = abs(y1 - y2);
	return (int)ceil(sqrt(distx*distx + disty*disty));
}

int tyDistanceCircular(int x1, int y1, int x2, int y2, int width, int height)
{
	int distx = abs(x1 - x2);
	int disty = abs(y1 - y2);
	if( distx > width / 2.0 ) distx = width - distx;
	if( disty > height / 2.0 ) disty = height - disty;
	return (int)ceil(sqrt(distx*distx + disty*disty));
}

int tyRand(int n)
{
	static bool initialized = false;
	if( !initialized ) {
		srand(time(0));
		initialized = true;
	}
	return rand() % n;
}

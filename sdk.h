#pragma once
#include "Includes.h"
#define perTick 0.015625f

struct Vector3 {
	float x;
	float y;
	float z;
};

struct QAngle {
	float pitch;
	float yaw;
	float roll;
};

typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);

void* GetInterface(const char* dllname, const char* interfacename)
{
	tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(dllname), "CreateInterface");
	int returnCode = 0;
	void* ginterface = CreateInterface(interfacename, &returnCode);
	return ginterface;
}

float clamp89(float a) {
	if (a < -89.f) {
		a = -89.f;
	}
	if (a > 89.f) {
		a = 89.f;
	}
	return a;
}
float clamp180(float a) {
	while (a < -180.f) {
		a += 360.f;
	}
	while (a > 180.f) {
		a -= 360.f;
	}
	return a;
}

float clamp450(float a) {
	if (a < -450.f) {
		a = -450.f;
	}
	if (a > 450.f) {
		a = 450.f;
	}
	return a;
}

enum WeaponIndex
{
	WEAPON_NONE,
	WEAPON_DEAGLE = 1,
	WEAPON_DUAL = 2,
	WEAPON_FIVE7 = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALIL = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250_CZ75 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG553 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFEGG = 41,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKE = 45,
	WEAPON_T_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_CT_MOLOTOV = 48,
	WEAPON_C4 = 49,
	WEAPON_MAX
};
enum {
	IN_ATTACK = (1 << 0),
	IN_JUMP = (1 << 1),
	IN_DUCK = (1 << 2),
	IN_FORWARD = (1 << 3),
	IN_BACK = (1 << 4),
	IN_USE = (1 << 5),
	IN_CANCEL = (1 << 6),
	IN_LEFT = (1 << 7),
	IN_RIGHT = (1 << 8),
	IN_MOVELEFT = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_ATTACK2 = (1 << 11),
	IN_RUN = (1 << 12),
	IN_RELOAD = (1 << 13),
	IN_ALT1 = (1 << 14),
	IN_ALT2 = (1 << 15),
	IN_SCORE = (1 << 16),
	IN_SPEED = (1 << 17),
	IN_WALK = (1 << 18),
	IN_ZOOM = (1 << 19),
	IN_WEAPON1 = (1 << 20),
	IN_WEAPON2 = (1 << 21),
	IN_BULLRUSH = (1 << 22),
	IN_GRENADE1 = (1 << 23),
	IN_GRENADE2 = (1 << 24),
	IN_LOOKSPIN = (1 << 25),
};



struct CUserCmd {
	void* vmt;
	int commandNumber;
	int tickCount;
	QAngle viewangles;
	Vector3 aimdirection;
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	BYTE impulse;
	int weaponselect;
	int weaponsubtype;
	int randomSeed;
	short mousedx;
	short mousedy;
	bool hasbeenpredicted;
};

class CVerifiedUserCmd
{
public:
	CUserCmd m_cmd;
	unsigned long m_crc;
};

CUserCmd* UserCmd;
CVerifiedUserCmd* VUserCmd;

class CViewSetup
{
public:
	int			iX;
	int			iUnscaledX;
	int			iY;
	int			iUnscaledY;
	int			iWidth;
	int			iUnscaledWidth;
	int			iHeight;
	int			iUnscaledHeight;
	bool		bOrtho;
	float		flOrthoLeft;
	float		flOrthoTop;
	float		flOrthoRight;
	float		flOrthoBottom;
	byte		pad0[0x7C];
	float		flFOV;
	float		flViewModelFOV;
	Vector3		vecOrigin;
	QAngle		angView;
	float		flNearZ;
	float		flFarZ;
	float		flNearViewmodelZ;
	float		flFarViewmodelZ;
	float		flAspectRatio;
	float		flNearBlurDepth;
	float		flNearFocusDepth;
	float		flFarFocusDepth;
	float		flFarBlurDepth;
	float		flNearBlurRadius;
	float		flFarBlurRadius;
	float		flDoFQuality;
	int			nMotionBlurMode;
	float		flShutterTime;
	Vector3		vecShutterOpenPosition;
	QAngle		vecShutterOpenAngles;
	Vector3		vecShutterClosePosition;
	QAngle		vecShutterCloseAngles;
	float		flOffCenterTop;
	float		flOffCenterBottom;
	float		flOffCenterLeft;
	float		flOffCenterRight;
	bool		bOffCenter : 1;
	bool		bRenderToSubrectOfLargerScreen : 1;
	bool		bDoBloomAndToneMapping : 1;
	bool		bDoDepthOfField : 1;
	bool		bHDRTarget : 1;
	bool		bDrawWorldNormal : 1;
	bool		bCullFontFaces : 1;
	bool		bCacheFullSceneState : 1;
	bool		bCSMView : 1;
};

class CInput
{
public:
	char                pad_0x00[0xC1];
	//bool                m_trackir_available;
	//bool                m_mouse_initiated;
	//bool                m_mouse_active;
	//bool                m_fJoystickAdvancedInit;
	//char                pad_0x08[0x2C];
	//void*					m_pKeys;
	//char                pad_0x38[0x70]; //was 0x6C
	//bool                m_fCameraInterceptingMouse;

	//bool                m_fCameraMovingWithMouse;

	//bool                m_fCameraDistanceMove;

	bool                m_fCameraInThirdPerson;
	bool				m_fAtNuts;
	Vector3             m_vecCameraOffset;
	int                 m_nCameraOldX;
	int                 m_nCameraOldY;
	int                 m_nCameraX;
	int                 m_nCameraY;
	bool                m_CameraIsOrthographic;
	Vector3             m_angPreviousViewAngles;
	Vector3             m_angPreviousViewAnglesTilt;
	float               m_flLastForwardMove;
	int                 m_nClearInputState;
	char                pad_0xE4[0x8];
	CUserCmd*			m_pCommands;
	CVerifiedUserCmd*	m_pVerifiedCommands;
	inline CUserCmd*		GetUserCmd(int sequence_number);
	inline CUserCmd*		GetUserCmd(int nSlot, int sequence_number);
	inline CVerifiedUserCmd* GetVerifiedCmd(int sequence_number);
};

using tCreateMove = uintptr_t(__fastcall*)(void*,void*,float,CUserCmd*);
tCreateMove fCreateMove;
using tOverrideView = void(__fastcall*)(void*,void*,CViewSetup*);
tOverrideView fOverrideView;

class CGlobals
{
public:
	CGlobals(bool bIsClient);
	// This can be used to filter debug output or to catch the client or server in the act.
	bool IsClient() const;
	inline bool IsRemoteClient() const;
	// for encoding m_flSimulationTime, m_flAnimTime
	int GetNetworkBase(int nTick, int nEntity);
public:
	// Absolute time (per frame still - Use Plat_FloatTime() for a high precision real time 
	//  perf clock, but not that it doesn't obey host_timescale/host_framerate)
	float			realtime;
	// Absolute frame counter - continues to increase even if game is paused
	int				framecount;
	// Non-paused frametime
	float			absoluteframetime;
	float			absoluteframestarttimestddev;
	float			curtime;
	// Time spent on last server or client frame (has nothing to do with think intervals)
	float			frametime;
	// current maxplayers setting
	int				maxClients;
	// Simulation ticks - does not increase when game is paused
	int				tickcount;
	// Simulation tick interval
	float			interval_per_tick;
	// interpolation amount ( client-only ) based on fraction of next tick which has elapsed
	float			interpolation_amount;
	int				simTicksThisFrame;
	int				network_protocol;
	// current saverestore data
	int*			pSaveData;
private:
	// Set to true in client code.
	bool			m_bClient;
public:
	// true if we are a remote clinet (needs prediction & interpolation - server not on this machine) as opposed to split-screen or local
	bool			m_bRemoteClient;
private:
	// 100 (i.e., tickcount is rounded down to this base and then the "delta" from this base is networked
	int				nTimestampNetworkingBase;
	// 32 (entindex() % nTimestampRandomizeWindow ) is subtracted from gpGlobals->tickcount to set the networking basis, prevents
	//  all of the entities from forcing a new PackedEntity on the same tick (i.e., prevents them from getting lockstepped on this)
	int				nTimestampRandomizeWindow;
};


class ITexture;

namespace vgui
{
	class Vec2 {
	public:
		float x;
		float y;
		Vec2(float ix, float iy) {
			x = ix;
			y = iy;
		}
	};

	class IImage;
	class Image;
	class Point;
	struct Color {
		int r, g, b, a;
	};
	// handles
	typedef unsigned long HCursor;
	typedef unsigned long HTexture;
	typedef unsigned long HFont;

	enum FontDrawType_t
	{
		// Use the "additive" value from the scheme file
		FONT_DRAW_DEFAULT = 0,

		// Overrides
		FONT_DRAW_NONADDITIVE,
		FONT_DRAW_ADDITIVE,

		FONT_DRAW_TYPE_COUNT = 2,
	};
	class Vertex_t;

	// Refactor these two
	struct CharRenderInfo
	{
		// Text pos
		int                x, y;
		// Top left and bottom right
		// This is now a pointer to an array maintained by the surface, to avoid copying the data on the 360
		Vertex_t* verts;
		int                textureId;
		int                abcA;
		int                abcB;
		int                abcC;
		int                fontTall;
		HFont            currentFont;
		// In:
		FontDrawType_t    drawType;
		wchar_t            ch;

		// Out
		bool            valid;
		// In/Out (true by default)
		bool            shouldclip;
	};


	struct IntRect
	{
		int x0;
		int y0;
		int x1;
		int y1;
	};
}

enum VGuiPanel_t // layers
{
	PANEL_ROOT = 0,
	PANEL_GAMEUIDLL,  // the console, game menu
	PANEL_CLIENTDLL,
	PANEL_TOOLS,
	PANEL_INGAMESCREENS,
	PANEL_GAMEDLL,
	PANEL_CLIENTDLL_TOOLS,
	PANEL_GAMEUIBACKGROUND, // the console background, shows under all other stuff in 3d engine view
	PANEL_TRANSITIONEFFECT,
	PANEL_STEAMOVERLAY,
};

enum PaintMode_t
{
	PAINT_UIPANELS = (1 << 0),
	PAINT_INGAMEPANELS = (1 << 1),
	PAINT_CURSOR = (1 << 2), // software cursor, if appropriate
};

class ISurface
{
public:

	void SetDrawColor(int r, int g, int b, int a = 255)
	{
		using original_fn = void(__thiscall*)(void*, int r, int g, int b, int a);
		return (*(original_fn**)this)[15](this, r, g, b, a = 255);
	}
	void SetDrawColor(vgui::Color color)
	{
		using original_fn = void(__thiscall*)(void*, vgui::Color color);
		return (*(original_fn**)this)[15](this, color);
	}
	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		using original_fn = void(__thiscall*)(void*, int x0, int y0, int x1, int y1);
		return (*(original_fn**)this)[18](this, x0, y0, x1, y1);
	}
	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		using original_fn = void(__thiscall*)(void*, int x0, int y0, int x1, int y1);
		return (*(original_fn**)this)[16](this, x0, y0, x1, y1);
	}
	void DrawLine(int x0, int y0, int x1, int y1)
	{
		using original_fn = void(__thiscall*)(void*, int x0, int y0, int x1, int y1);
		return (*(original_fn**)this)[19](this, x0, y0, x1, y1);
	}
	void DrawOutlinedCircle(int x, int y, int r, int seg)
	{
		using original_fn = void(__thiscall*)(void*, int x, int y, int r, int seg);
		return (*(original_fn**)this)[103](this, x, y, r, seg);
	}
	void SetTextFont(vgui::HFont font)
	{
		using original_fn = void(__thiscall*)(void*, vgui::HFont font);
		return (*(original_fn**)this)[23](this, font);
	}
	void SetTextColor(int r, int g, int b, int a)
	{
		using original_fn = void(__thiscall*)(void*, int r, int g, int b, int a);
		return (*(original_fn**)this)[25](this, r, g, b, a);
	}
	void SetTextPosition(int x, int y)
	{
		using original_fn = void(__thiscall*)(void*, int x, int y);
		return (*(original_fn**)this)[26](this, x, y);
	}
	vgui::HFont sCreateFont()
	{
		using original_fn = vgui::HFont(__thiscall*)(void*);
		return (*(original_fn**)this)[71](this);
	}
	bool SetFontGlyphSet(vgui::HFont font, const char* fontName, int size, int weight, int blur, int scanlines, int flags, int rangeMin = 0, int rangeMax = 0)
	{
		using original_fn = bool(__thiscall*)(void*, vgui::HFont font, const char* fontName, int size, int weight, int blur, int scanlines, int flags, int rangeMin, int rangeMax);
		return (*(original_fn**)this)[72](this, font, fontName, size, weight, blur, scanlines, flags, rangeMin = 0, rangeMax = 0);
	}
	void PrintText(const wchar_t* text, int length, int drawType = 0)
	{
		using original_fn = void(__thiscall*)(void*, const wchar_t* text, int length, int drawType);
		return (*(original_fn**)this)[28](this, text, length, drawType = 0);
	}
	void UnlockCursor()
	{
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn**)this)[66](this);
	}
	void LockCursor() 
	{
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn**)this)[67](this);
	}
	void SurfaceGetCursorPos(int& x, int& y) 
	{
		using original_fn = void(__thiscall*)(void*, int&, int&);
		return (*(original_fn**)this)[96](this, x, y);
	}
	void StartDrawing() {
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn**)this)[83](this);
	}
	void FinishDrawing() {
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn**)this)[85](this);
	}
	enum ETextureFormat
	{
		eTextureFormat_RGBA,
		eTextureFormat_BGRA,
		eTextureFormat_BGRA_Opaque, // bgra format but alpha is always 255, CEF does this, we can use this fact for better perf on win32 gdi
	};

	// returns true if the surface supports minimize & maximize capabilities
	enum SurfaceFeature_e
	{
		ANTIALIASED_FONTS = 1,
		DROPSHADOW_FONTS = 2,
		ESCAPE_KEY = 3,
		OPENING_NEW_HTML_WINDOWS = 4,
		FRAME_MINIMIZE_MAXIMIZE = 5,
		OUTLINE_FONTS = 6,
		DIRECT_HWND_RENDER = 7,
	};

	// fonts
	// creates an empty handle to a vgui font.  windows fonts can be add to this via SetFontGlyphSet().

	// adds to the font
	enum EFontFlags
	{
		FONTFLAG_NONE,
		FONTFLAG_ITALIC = 0x001,
		FONTFLAG_UNDERLINE = 0x002,
		FONTFLAG_STRIKEOUT = 0x004,
		FONTFLAG_SYMBOL = 0x008,
		FONTFLAG_ANTIALIAS = 0x010,
		FONTFLAG_GAUSSIANBLUR = 0x020,
		FONTFLAG_ROTARY = 0x040,
		FONTFLAG_DROPSHADOW = 0x080,
		FONTFLAG_ADDITIVE = 0x100,
		FONTFLAG_OUTLINE = 0x200,
		FONTFLAG_CUSTOM = 0x400,        // custom generated font - never fall back to asian compatibility mode
		FONTFLAG_BITMAP = 0x800,        // compiled bitmap font - no fallbacks
	};
};

using tLockCursor = void(__fastcall*)(void*, void*);
tLockCursor fLockCursor;

class IEngineVGui {
	void Paint(PaintMode_t mode) {
		using original_fn = void(__thiscall*)(void*, PaintMode_t);
		return (*(original_fn**)this)[14](this, mode);
	};
};

using tPaint = void(__fastcall*)(void*, void*, PaintMode_t);
tPaint fPaint;

struct ClientModeShared
{
		void OverrideView(CViewSetup* pSetup) {
			using original_fn = void(__thiscall*)(void*, CViewSetup*);
			return (*(original_fn**)this)[18](this, pSetup);
		}
		bool CreateMove(float flInputSampleTime, CUserCmd* cmd) {
			using original_fn = bool(__thiscall*)(void*, float, CUserCmd*);
			return (*(original_fn**)this)[24](this, flInputSampleTime, cmd);
		}
};

#define FLOW_OUTGOING	0		
#define FLOW_INCOMING	1

class INetChannelInfo {
public:
	enum
	{
		GENERIC = 0, // must be first and is default group
		LOCALPLAYER, // bytes for local player entity update
		OTHERPLAYERS, // bytes for other players update
		ENTITIES, // all other entity bytes
		SOUNDS, // game sounds
		EVENTS, // event messages
		TEMPENTS, // temp entities
		USERMESSAGES, // user messages
		ENTMESSAGES, // entity messages
		VOICE, // voice data
		STRINGTABLE, // a stringtable update
		MOVE, // client move cmds
		STRINGCMD, // string command
		SIGNON, // various signondata
		TOTAL, // must be last and is not a real group
	};
	virtual const char* GetName() const = 0; // get channel name
	virtual const char* GetAddress() const = 0; // get channel IP address as string
	virtual float GetTime() const = 0; // current net time
	virtual float GetTimeConnected() const = 0; // get connection time in seconds
	virtual int GetBufferSize() const = 0; // netchannel packet history size
	virtual int GetDataRate() const = 0; // send data rate in byte/sec
	virtual bool IsLoopback() const = 0; // true if loopback channel
	virtual bool IsTimingOut() const = 0; // true if timing out
	virtual bool IsPlayback() const = 0; // true if demo playback
	virtual float GetLatency(int flow) const = 0; // current latency (RTT), more accurate but jittering
	virtual float GetAvgLatency(int flow) const = 0; // average packet latency in seconds
	virtual float GetAvgLoss(int flow) const = 0; // avg packet loss[0..1]
	virtual float GetAvgChoke(int flow) const = 0; // avg packet choke[0..1]
	virtual float GetAvgData(int flow) const = 0; // data flow in bytes/sec
	virtual float GetAvgPackets(int flow) const = 0; // avg packets/sec
	virtual int GetTotalData(int flow) const = 0; // total flow in/out in bytes
	virtual int GetTotalPackets(int flow) const = 0;
	virtual int GetSequenceNr(int flow) const = 0; // last send seq number
	virtual bool IsValidPacket(int flow, int frame_number) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float GetPacketTime(int flow, int frame_number) const = 0; // time when packet was send
	virtual int GetPacketBytes(int flow, int frame_number, int group) const = 0; // group size of this packet
	virtual bool GetStreamProgress(int flow, int* received, int* total) const = 0; // TCP progress if transmitting
	virtual float GetTimeSinceLastReceived() const = 0; // get time since last recieved packet in seconds
	virtual float GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
	virtual void GetPacketResponseLatency(int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke) const = 0;
	virtual void GetRemoteFramerate(float* pflFrameTime, float* pflFrameTimeStdDeviation) const = 0;
	virtual float GetTimeoutSeconds() const = 0;
};

class IEngineClient
{
public:
	void GetScreenSize(int& iWidth, int& iHeight)
	{
		using original_fn = void(__thiscall*)(void*, int&, int&);
		return (*(original_fn**)this)[5](this, iWidth, iHeight);
	}

	int GetLocalPlayer()
	{
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)this)[12](this);
	}
	float GetLastTimeStamp()
	{
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[14](this);
	}

	void GetViewAngles(QAngle& angView)
	{
		using original_fn = void(__thiscall*)(void*, QAngle&);
		return (*(original_fn**)this)[18](this, angView);
	}

	void SetViewAngles(QAngle& angView)
	{
		using original_fn = void(__thiscall*)(void*, QAngle&);
		return (*(original_fn**)this)[19](this, angView);
	}

	int GetMaxClients()
	{
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)this)[20](this);
	}

	// returns true if the player is fully connected and active in game (i.e, not still loading) and for check doesnt need isconnected
	bool IsInGame()
	{
		using original_fn = bool(__thiscall*)(void*);
		return (*(original_fn**)this)[26](this);
	}

	// returns true if the player is connected, but not necessarily active in game (could still be loading)
	bool IsConnected()
	{
		using original_fn = bool(__thiscall*)(void*);
		return (*(original_fn**)this)[27](this);
	}

	bool IsDrawingLoadingImage()
	{
		using original_fn = bool(__thiscall*)(void*);
		return (*(original_fn**)this)[28](this);
	}

	void* GetBSPTreeQuery()
	{
		using original_fn = void*(__thiscall*)(void*);
		return (*(original_fn**)this)[43](this);
	}
	INetChannelInfo* GetNetChannelInfo()
	{
		using original_fn = INetChannelInfo*(__thiscall*)(void*);
		return (*(original_fn**)this)[78](this);
	}
	void ExecuteClientCmd(const char* szCmdString)
	{
		using original_fn = void(__thiscall*)(void*,const char*);
		return (*(original_fn**)this)[108](this, szCmdString);
	}

	void ClientCmdUnrestricted(const char* szCmdString, bool bFromConsoleOrKeybind = false)
	{
		using original_fn = void(__thiscall*)(void*,const char*, bool);
		return (*(original_fn**)this)[114](this, szCmdString, 0);
	}
	bool IsVoiceRecording()
	{
		using original_fn = bool(__thiscall*)(void*);
		return (*(original_fn**)this)[224](this);
	}
};


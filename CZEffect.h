#pragma once
// get mask for int & BIT(0)
#define BIT(num) (1<<(num))
enum eChaosEffectsFilterMasks
{ // iFlags & mask
	UNSTABLE = (1 << 2),
	STREAM_UNSTABLE = (1 << 3),
	DISTRIBUTION = (1 << 11),
	MINIGAMES = (1 << 14),
	SCREEN_FILTER_EFFECTS = (1 << 22),
};
class ZEffect // 102d0750 ida
{ // 6.5 линий по 16byte CheatEngine
	// RE TODO: Twitch Flags/Vars/Behavior
public:
	// [block 16b CE 1RAW] (fp0)
	uint32_t uiTaskEffectTime; // From UI [+0]
	uint32_t uiUnk_Time; // [+4]
	void* pOnceFunc; // [+8]
	void* pOnDrawFunc; // [+12]

	// 16
	const char* pCheatName;  // [+16] // todo test pointers перемешай and ui name/desc
	const char* pCheatName2PNU;  // [+20] // cheat name [PROBABLY NOT USED]
	uint32_t uiUnkp6; // [+24] (a7) LOBYTE(1)
	uint32_t uiUnkp7; // [+28] (0)

	// 32
	//uint32_t unkp8;
	bool bIsActiveInUI;  // [+32] [1]
	bool bEnabledForCheatCodeVotingUI; // [+33] [1]
	bool bEnabledForSubEffectUI; // [+34] [1]
	bool bIsEnableByHaosLogic; // [+35] [1] // ::disabled by haos reason:pDisabledByHaosLogicReason Not Connected to twich  [Other disabled effects"ZChaosIII.asi"+2D0750]
	//uint32_t unkp9; // [+36]
	bool bUnkp9_1; // [+36] [1] // "Undocumented reason" code UNC CMP ENABLE FLAG
	bool bUnkp9_2; // [+37] [1] // some disabled unk flag [effect "30 Second Timer"]
	bool bUnkp9_3; // [+38] pad? [0]
	bool bUnkp9_4; // [+39] pad? [0]
	uint32_t uiFlags; // [+40] p10 <<2 Unstable, <<3 StreamUnstable, <<11? Distribution, <<14 Minigame, <<22 Screen
	const char* pChatCode; // [+44] p11 // "Taxibug"  QuakeMovement   if ( *((char *)effect + 40) < 0 )  "(No code)"

	// 48
	const char* pDisabledByHaosLogicReason; // [+48] p12
	uint32_t uiUnkp13; // [+52]
	uint32_t uiUnkp14; // [+56] // [1] !!! todo check mb bool
	uint32_t uiCountTriggered; // [+60] //  lo count activate 

	// 64
	double dTotalTime; // [+64] // dp16 8b? double timer processing effect? если перебрать за uiTaskEffectTime ничего не меняет
	//uint32_t unkp18; // [+72]
	bool bUnkp18_1; // [+72]
	bool bUnkp18_2; // [+73]
	bool bUnkp18_pad_3; // [+74]	// memset(pEffect, 0xAA, sizeof(ZEffect)); (exploit pad detector) (do not copy field 2 run obj) see start
	bool bUnkp18_pad_4; // [+75]	// memset(pEffect, 0xAA, sizeof(ZEffect)); (exploit pad detector) (do not copy field 2 run obj) see start
	uint32_t uiUnkp19_pad; // [+76]	// memset(pEffect, 0xAA, sizeof(ZEffect)); (exploit pad detector) (do not copy field 2 run obj) see start

	// 80
	uint32_t uiUnkp20; // [+80]
	uint32_t uiUnkp21; // [+84]
	ZEffect* m_pParentObject; // [+88] // OnRunning setting
	uint32_t uiUnkp23_pad; // [+92] // memset(pEffect, 0xAA, sizeof(ZEffect)); (exploit pad detector) (do not copy field 2 run obj) see start

	// 96
	double dRunningTotalTime; // [+96] // -1.0d  total timer in RunEffect [IfSet > uiTaskEffectTime running be ending]
	// or
	//int data[26];
};// 104b 0x68

class RunZEffect // 103bc090 ida
{
public:
	ZEffect* m_pParentObject;
	uint32_t m_Pad; // 0
	ZEffect m_oCopyRunningEffect;
}; // 112b

#define ZEFFECT_CHATCODE_EXPANDER_PAYLOAD_OFFSET 128
#define ZEFFECT_CHATCODE_EXPANDER_SIGN 0x13371337 // 37 13 37 13
class ZEffectChatCodeExpander // 4 pChatCode
{
public:
	//int fix1 = 'pme(';  // 0x706D6528
	//int fix2 = '\0)yt'; // 0x00297974
	// or
	char pChatCodeStr[ZEFFECT_CHATCODE_EXPANDER_PAYLOAD_OFFSET] = "(EMPTY)\0";
	int iSign = ZEFFECT_CHATCODE_EXPANDER_SIGN;
	void* pCustomCB_Once = NULL;
	void* pCustomCB_OnDraw = NULL;
	static bool Check_ZEffect_ChatCode_Expander(ZEffect* pEffect)
	{
		return (pEffect ? (pEffect->pChatCode) ? (((ZEffectChatCodeExpander*)(pEffect->pChatCode))->iSign == ZEFFECT_CHATCODE_EXPANDER_SIGN) : false : false);
	}

	// additional data
};

// void __fastcall OnDraw_Or_Once(RunZEffect* pEffect, bool bIsOnDraw) {}
#define ZEFFECT_HANDLER_DECLARE(function_name, arg_name) void __fastcall function_name(RunZEffect* arg_name, bool bIsOnDraw)
typedef void(__fastcall* ZEffect_EffectHandler_Function)(RunZEffect* pEffect, bool bIsOnDraw); // ZEffect_CB_Function* ptr = Effect_Do;
typedef void(__fastcall ZEffect_EffectHandler_pFunction)(RunZEffect* pEffect, bool bIsOnDraw); // ZEffect_CB_Function* ptr = Effect_Do;

// void __stdcall Func() {}
#define SIMPLE_CALL_DECLARE(function_name) void __stdcall function_name()
typedef void(__stdcall* SimpleCall_Function)(); // OnInit
typedef void(__stdcall SimpleCall_pFunction)(); // OnInit

// void __fastcall OnEnding(RunZEffect* pEndingEffect) {}
#define ZEFFECT_ONENDING_EFFECT_DECLARE(function_name, arg_name) void __fastcall function_name(RunZEffect* arg_name)
typedef void(__fastcall* ZEffect_EndingEffectCB_Function)(RunZEffect* pEndingEffect); // on ending
typedef void(__fastcall ZEffect_EndingEffectCB_pFunction)(RunZEffect* pEndingEffect); // on ending


// "ZChaosIII.asi"+2D0750 // effects list
// "ZChaosIII.asi"+3BC090 // running list
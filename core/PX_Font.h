#ifndef PIXELSES_FONT
#define PIXELSES_FONT

#include "PX_Surface.h"
#include "PX_Texture.h"
#include "PX_Base64.h"
#include "PX_Truetype.h"
#include "PX_Hashmap.h"

//#define __PX_FONT_GBKSIZE		13
#define __PX_FONT_ASCSIZE		8
#define __PX_FONT_HEIGHT		15
#define __PX_FONT_MODULE_XSPACE_SIZE 0
#define __PX_FONT_MODULE_YSPACE_SIZE 0
////////////////////////////////////////////////////////////////////////////////////////////
///////								Private function									////
//////																					////	
////////////////////////////////////////////////////////////////////////////////////////////

#define PX_FontGetCharactorHeight() (__PX_FONT_HEIGHT)
#define PX_FontGetAscCharactorWidth() (__PX_FONT_ASCSIZE)
//#define PX_FontGetGbkCharactorWidth() (__PX_FONT_GBKSIZE)

typedef struct
{
	union
	{
		px_char  c_magic[4];//PXFM
		px_dword magic;
	};
	px_dword codePage;
	px_dword charactor_code;
	px_dword BearingX;
	px_dword BearingY;
	px_dword Advance;
	px_dword Font_Width;
	px_dword Font_Height;
}PX_FontModule_Charactor_Header;

typedef struct
{
	PX_FontModule_Charactor_Header header;
	px_bool render_type;
	union 
	{
		px_shape shape;
		px_texture texture;
	};
	
}PX_FontModule_Charactor;

typedef enum  
{
	PX_FONTMODULE_CODEPAGE_GBK,
	PX_FONTMODULE_CODEPAGE_UTF8,
	PX_FONTMODULE_CODEPAGE_UTF16,
	PX_FONTMODULE_CODEPAGE_UNDEFINED,
	PX_FONTMODULE_CODEPAGE_UNICODE,
}PX_FONTMODULE_CODEPAGE;

typedef struct  
{
	px_memorypool *mp;
	px_byte* ttfbuffer;
	px_int ttfsize;
	px_map characters_map;
	px_int max_BearingY;
	px_int max_Height;
	px_int max_Width;
	PX_FONTMODULE_CODEPAGE codePage;
}PX_FontModule;

px_int PX_FontUnicodeToUTF8(px_dword unicode, px_byte utf8[4]);

px_int PX_FontTrimUncompletedUTF8String(px_char* pstr);
px_int PX_FontDrawChar(px_surface *psurface, px_int x,px_int y,px_uchar chr,px_color Color );
px_int PX_FontDrawText(px_surface *psurface,px_int x,px_int y,PX_ALIGN align,const px_char *Text,px_color Color);
px_void PX_FontTextGetRenderWidthHeight(const px_char *Text,px_int *width,px_int *height);

px_int PX_FontModuleGetCharacterRawCode(PX_FONTMODULE_CODEPAGE codePage,const px_char *Text,px_dword *code);
px_bool PX_FontModuleInitialize(px_memorypool *mp,PX_FontModule *module);
px_void PX_FontModuleSetSize(PX_FontModule* module,px_int size);
px_void PX_FontModuleSetCodepage(PX_FontModule* module, PX_FONTMODULE_CODEPAGE codePage);
px_bool PX_FontModuleInitializeTTF(px_memorypool* mp, PX_FontModule* module, PX_FONTMODULE_CODEPAGE codepage, px_int fontsize, const px_byte* ttf_buffer, px_int ttf_size);
px_bool PX_FontModuleLoad(PX_FontModule *module,px_byte *buffer,px_int size);
px_void PX_FontModuleFree(PX_FontModule *module);

px_int PX_FontModuleGetOneCharacterDesc(PX_FontModule *module,const px_char *Text,px_dword *code,px_int *width,px_int *height);
px_void PX_FontModuleTextGetRenderWidthHeight(PX_FontModule *module,const px_char *Text,px_int *advance,px_int *height);
px_int PX_FontModuleDrawCharacter(px_surface *psurface,PX_FontModule *mod,px_int x,px_int y,const px_dword code,px_color Color);
px_int PX_FontModuleDrawText(px_surface *psurface,PX_FontModule *mod,px_int x,px_int y,PX_ALIGN align,const px_char *Text,px_color Color);
px_bool PX_FontModuleAddNewTextureCharacterEx(PX_FontModule* mod, px_dword unicode, px_texture* ptexture, px_dword BearingX, px_dword BearingY);
px_bool PX_FontModuleAddNewTextureCharacter(PX_FontModule* mod, px_dword unicode, px_texture* pcopytexture);
#endif
 

#include "PX_Object_PushButton.h"
PX_OBJECT_EVENT_FUNCTION(PX_Object_PushButtonOnMouseMove)
{
	PX_Object_PushButton *pPushButton=PX_Object_GetPushButton(pObject);
	px_float x,y;

	x=PX_Object_Event_GetCursorX(e);
	y=PX_Object_Event_GetCursorY(e);

	if (pPushButton)
	{
		if(PX_ObjectIsPointInRegion(pObject,(px_float)x,(px_float)y))
		{
			if (pPushButton->state!=PX_OBJECT_PUSHBUTTON_STATE_ONPUSH)
			{
				if (pPushButton->state!=PX_OBJECT_PUSHBUTTON_STATE_ONCURSOR)
				{
					PX_Object_Event e = {0};
					e.Event=PX_OBJECT_EVENT_CURSOROVER;
					PX_ObjectExecuteEvent(pObject,e);
				}
				pPushButton->state=PX_OBJECT_PUSHBUTTON_STATE_ONCURSOR;
				pPushButton->tips_x = x;
				pPushButton->tips_y = y;
			}
		}
		else
		{
			if (pPushButton->state!=PX_OBJECT_PUSHBUTTON_STATE_NORMAL)
			{
				PX_Object_Event e = {0};
				e.Event=PX_OBJECT_EVENT_CURSOROUT;
				PX_ObjectExecuteEvent(pObject,e);
			}
			pPushButton->state=PX_OBJECT_PUSHBUTTON_STATE_NORMAL;
			pPushButton->bCursorShowTipsElapsed = 0;
		}
	}
}

PX_OBJECT_EVENT_FUNCTION(PX_Object_PushButtonOnMouseLButtonDown)
{
	PX_Object_PushButton *pPushButton=PX_Object_GetPushButton(pObject);
	px_float x,y;

	x=PX_Object_Event_GetCursorX(e);
	y=PX_Object_Event_GetCursorY(e);

	if (pPushButton)
	{
		if(PX_ObjectIsPointInRegion(pObject,(px_float)x,(px_float)y))
			pPushButton->state=PX_OBJECT_PUSHBUTTON_STATE_ONPUSH;
		else
		{
			if (pPushButton->state!=PX_OBJECT_PUSHBUTTON_STATE_NORMAL)
			{
				PX_Object_Event e;
				e.Event=PX_OBJECT_EVENT_CURSOROUT;
				e.Param_uint[0]=0;
				e.Param_uint[1]=0;
				e.Param_uint[2]=0;
				e.Param_uint[3]=0;
				PX_ObjectExecuteEvent(pObject,e);
			}
			pPushButton->state=PX_OBJECT_PUSHBUTTON_STATE_NORMAL;
		}
	}
}

PX_OBJECT_EVENT_FUNCTION(PX_Object_PushButtonOnMouseLButtonUp)
{
	PX_Object_PushButton *pPushButton=PX_Object_GetPushButton(pObject);
	px_float x,y;

	x=PX_Object_Event_GetCursorX(e);
	y=PX_Object_Event_GetCursorY(e);

	if (pPushButton)
	{
		if(PX_ObjectIsPointInRegion(pObject,(px_float)x,(px_float)y))
			if(pPushButton->state==PX_OBJECT_PUSHBUTTON_STATE_ONPUSH)
			{
				pPushButton->state=PX_OBJECT_PUSHBUTTON_STATE_ONCURSOR;
				e.Event=PX_OBJECT_EVENT_EXECUTE;
				PX_ObjectExecuteEvent(pObject,e);
			}
	}
}

PX_OBJECT_RENDER_FUNCTION(PX_Object_PushButtonRender)
{
	px_int fx, fy;
	PX_Object_PushButton* pPushButton = PX_ObjectGetDesc(PX_Object_PushButton, pObject);
	px_float objx, objy, objWidth, objHeight;
	px_rect rect=PX_ObjectGetRect(pObject);
	objx = rect.x;
	objy = rect.y;
	objWidth = rect.width;
	objHeight = rect.height;


	if (pPushButton == PX_NULL)
	{
		return;
	}

	if (!pObject->Visible)
	{
		return;
	}

	if (!pObject->Enabled)
	{
		pPushButton->state = PX_OBJECT_PUSHBUTTON_STATE_NORMAL;
	}
	if (pPushButton->state == PX_OBJECT_PUSHBUTTON_STATE_ONCURSOR)
	{
		pPushButton->bCursorShowTipsElapsed += elapsed;
	}

	if (pPushButton->gif)
	{
		
		switch (pPushButton->state)
		{
		case PX_OBJECT_PUSHBUTTON_STATE_NORMAL:
			if (pPushButton->gif->roffset != 0)
			{
				PX_GifReset(pPushButton->gif);
			}
			break;
		case PX_OBJECT_PUSHBUTTON_STATE_ONPUSH:
		case PX_OBJECT_PUSHBUTTON_STATE_ONCURSOR:
			PX_GifUpdate(pPushButton->gif, elapsed);
			break;
		}
		pPushButton->Texture = PX_GifGetTexture(pPushButton->gif);
	}

	if (pPushButton->Texture)
	{
		PX_TEXTURERENDER_BLEND blend;
		switch (pPushButton->state)
		{
		case PX_OBJECT_PUSHBUTTON_STATE_NORMAL:
			PX_TextureRender(psurface, pPushButton->Texture, (px_int)objx + (px_int)objWidth / 2, (px_int)objy + (px_int)objHeight / 2, PX_ALIGN_CENTER, PX_NULL);
			break;
		case PX_OBJECT_PUSHBUTTON_STATE_ONPUSH:
		{
			blend.alpha = 1;
			blend.hdr_B = 0.6f;
			blend.hdr_G = 0.6f;
			blend.hdr_R = 0.6f;
			PX_TextureRender(psurface, pPushButton->Texture, (px_int)objx + (px_int)objWidth / 2, (px_int)objy + (px_int)objHeight / 2, PX_ALIGN_CENTER, &blend);
		}
		break;
		case PX_OBJECT_PUSHBUTTON_STATE_ONCURSOR:
		{
			blend.alpha = 1;
			blend.hdr_B = 0.8f;
			blend.hdr_G = 0.8f;
			blend.hdr_R = 0.8f;
			PX_TextureRender(psurface, pPushButton->Texture, (px_int)objx + (px_int)objWidth / 2, (px_int)objy + (px_int)objHeight / 2, PX_ALIGN_CENTER, &blend);
		}
		break;
		}
	}
	else if (pPushButton->shape)
	{
		PX_ShapeRender(psurface, pPushButton->shape, (px_int)objx + (px_int)objWidth / 2, (px_int)objy + (px_int)objHeight / 2, PX_ALIGN_CENTER, pPushButton->TextColor);
	}
	else
	{
		switch (pPushButton->style)
		{
		case PX_OBJECT_PUSHBUTTON_STYLE_RECT:
			PX_GeoDrawRect(psurface, (px_int)objx, (px_int)objy, (px_int)objx + (px_int)objWidth - 1, (px_int)objy + (px_int)objHeight - 1, pPushButton->BackgroundColor);
			switch (pPushButton->state)
			{
			case PX_OBJECT_PUSHBUTTON_STATE_NORMAL:
				PX_GeoDrawRect(psurface, (px_int)objx, (px_int)objy, (px_int)objx + (px_int)objWidth - 1, (px_int)objy + (px_int)objHeight - 1, pPushButton->BackgroundColor);
				break;
			case PX_OBJECT_PUSHBUTTON_STATE_ONPUSH:
				PX_GeoDrawRect(psurface, (px_int)objx, (px_int)objy, (px_int)objx + (px_int)objWidth - 1, (px_int)objy + (px_int)objHeight - 1, pPushButton->PushColor);
				break;
			case PX_OBJECT_PUSHBUTTON_STATE_ONCURSOR:

				PX_GeoDrawRect(psurface, (px_int)objx, (px_int)objy, (px_int)objx + (px_int)objWidth - 1, (px_int)objy + (px_int)objHeight - 1, pPushButton->CursorColor);
				break;
			}
			break;
		case PX_OBJECT_PUSHBUTTON_STYLE_ROUNDRECT:
			PX_GeoDrawSolidRoundRect(psurface, (px_int)objx, (px_int)objy, (px_int)objx + (px_int)objWidth - 1, (px_int)objy + (px_int)objHeight - 1, pPushButton->roundradius, pPushButton->BackgroundColor);
			switch (pPushButton->state)
			{
			case PX_OBJECT_PUSHBUTTON_STATE_NORMAL:
				PX_GeoDrawSolidRoundRect(psurface, (px_int)objx, (px_int)objy, (px_int)objx + (px_int)objWidth - 1, (px_int)objy + (px_int)objHeight - 1, pPushButton->roundradius, pPushButton->BackgroundColor);
				break;
			case PX_OBJECT_PUSHBUTTON_STATE_ONPUSH:
				PX_GeoDrawSolidRoundRect(psurface, (px_int)objx, (px_int)objy, (px_int)objx + (px_int)objWidth - 1, (px_int)objy + (px_int)objHeight - 1, pPushButton->roundradius, pPushButton->PushColor);
				break;
			case PX_OBJECT_PUSHBUTTON_STATE_ONCURSOR:
				PX_GeoDrawSolidRoundRect(psurface, (px_int)objx, (px_int)objy, (px_int)objx + (px_int)objWidth - 1, (px_int)objy + (px_int)objHeight - 1, pPushButton->roundradius, pPushButton->CursorColor);
				break;
			}
			break;
		case PX_OBJECT_PUSHBUTTON_STYLE_CIRCLE:
		{
			PX_GeoDrawSolidCircle(psurface, (px_int)objx, (px_int)objy, (px_int)(pObject->diameter / 2), pPushButton->BackgroundColor);
			switch (pPushButton->state)
			{
			case PX_OBJECT_PUSHBUTTON_STATE_NORMAL:
				PX_GeoDrawBall(psurface, objx, objy, (pObject->diameter / 2), pPushButton->BackgroundColor);
				break;
			case PX_OBJECT_PUSHBUTTON_STATE_ONPUSH:
				PX_GeoDrawBall(psurface, objx, objy, (pObject->diameter / 2), pPushButton->PushColor);
				break;
			case PX_OBJECT_PUSHBUTTON_STATE_ONCURSOR:
				PX_GeoDrawBall(psurface, objx, objy, (pObject->diameter / 2), pPushButton->CursorColor);
				break;
			}
		}
		break;
		}

	}

	if (pPushButton->Border)
	{
		switch (pPushButton->style)
		{
		case PX_OBJECT_PUSHBUTTON_STYLE_RECT:
			PX_GeoDrawBorder(psurface, (px_int)objx, (px_int)objy, (px_int)objx + (px_int)objWidth - 1, (px_int)objy + (px_int)objHeight - 1, 1, pPushButton->BorderColor);
			break;
		case PX_OBJECT_PUSHBUTTON_STYLE_ROUNDRECT:
			PX_GeoDrawRoundRect(psurface, (px_int)objx, (px_int)objy, (px_int)objx + (px_int)objWidth - 1, (px_int)objy + (px_int)objHeight - 1, pPushButton->roundradius, 0.6f, pPushButton->BorderColor);
			break;
		case PX_OBJECT_PUSHBUTTON_STYLE_CIRCLE:
			PX_GeoDrawCircle(psurface, (px_int)objx, (px_int)objy, (px_int)(pObject->diameter / 2), 1, pPushButton->BorderColor);
			break;
		}
	}

	if (pPushButton->Tips && pPushButton->bCursorShowTipsElapsed > 1000)
	{
		px_int w, h;
		PX_FontModuleTextGetRenderWidthHeight(pPushButton->fontModule, pPushButton->Tips, &w, &h);
		PX_GeoDrawRect(psurface, (px_int)pPushButton->tips_x - w / 2 - 2, (px_int)pPushButton->tips_y - h - 2, (px_int)pPushButton->tips_x + w / 2 + 2, (px_int)pPushButton->tips_y + 2, PX_COLOR_BACKGROUNDCOLOR);
		PX_GeoDrawBorder(psurface, (px_int)pPushButton->tips_x - w / 2 - 2, (px_int)pPushButton->tips_y - h - 2, (px_int)pPushButton->tips_x + w / 2 + 2, (px_int)pPushButton->tips_y + 2, 1, PX_COLOR_BORDERCOLOR);
		PX_FontModuleDrawText(psurface, pPushButton->fontModule, (px_int)pPushButton->tips_x - 1, (px_int)pPushButton->tips_y - 1, PX_ALIGN_MIDBOTTOM, pPushButton->Tips, pPushButton->TextColor);
	}

	fx = (px_int)(objx + objWidth / 2);
	fy = (px_int)(objy + objHeight / 2);
	if(pPushButton->Text[0])
		PX_FontModuleDrawText(psurface, pPushButton->fontModule, fx, fy, PX_ALIGN_CENTER, pPushButton->Text, pPushButton->TextColor);

}

PX_OBJECT_FREE_FUNCTION(PX_Object_PushButtonFree)
{
	PX_Object_PushButton* PushButton = PX_Object_GetPushButton(pObject);
	MP_Free(pObject->mp, PushButton->Text);
	if (PushButton->Tips)
		MP_Free(pObject->mp, PushButton->Tips);
}


PX_Object* PX_Object_PushButtonAttachObject( PX_Object* pObject,px_int attachIndex, const px_char* Text, PX_FontModule* fontmodule)
{
	px_memorypool* mp=pObject->mp;
	px_int TextLen;

	PX_Object_PushButton* pPushButton;

	PX_ASSERTIF(pObject == PX_NULL);
	PX_ASSERTIF(attachIndex < 0 || attachIndex >= PX_COUNTOF(pObject->pObjectDesc));
	PX_ASSERTIF(pObject->pObjectDesc[attachIndex] != PX_NULL);
	pPushButton = (PX_Object_PushButton*)PX_ObjectCreateDesc(pObject, attachIndex, PX_OBJECT_TYPE_PUSHBUTTON, 0, PX_Object_PushButtonRender, PX_Object_PushButtonFree, 0, sizeof(PX_Object_PushButton));
	PX_ASSERTIF(pPushButton == PX_NULL);

	TextLen = PX_strlen(Text) + 1;
	pPushButton->Text = (px_char*)MP_Malloc(mp, TextLen);

	if (pPushButton->Text == PX_NULL)
	{
		MP_Free(pObject->mp, pObject);
		MP_Free(pObject->mp, pPushButton);
		return PX_NULL;
	}

	PX_strcpy(pPushButton->Text, Text, TextLen);

	pPushButton->TextColor = PX_OBJECT_UI_DEFAULT_FONTCOLOR;
	pPushButton->CursorColor = PX_OBJECT_UI_DEFAULT_CURSORCOLOR;
	pPushButton->PushColor = PX_OBJECT_UI_DEFAULT_PUSHCOLOR;
	pPushButton->BackgroundColor = PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR;
	pPushButton->BorderColor = PX_OBJECT_UI_DEFAULT_BORDERCOLOR;
	pPushButton->Border = PX_TRUE;
	pPushButton->state = PX_OBJECT_PUSHBUTTON_STATE_NORMAL;
	pPushButton->Border = PX_TRUE;
	pPushButton->style = PX_OBJECT_PUSHBUTTON_STYLE_RECT;
	pPushButton->roundradius = PX_OBJECT_PUSHBUTTON_ROUNDRADIUS;
	pPushButton->shape = PX_NULL;
	pPushButton->gif = PX_NULL;
	pPushButton->Texture = PX_NULL;
	pPushButton->fontModule = fontmodule;

	PX_ObjectRegisterEventEx(pObject, PX_OBJECT_EVENT_CURSORMOVE, PX_OBJECT_TYPE_PUSHBUTTON, PX_Object_PushButtonOnMouseMove, PX_NULL);
	PX_ObjectRegisterEventEx(pObject, PX_OBJECT_EVENT_CURSORDRAG, PX_OBJECT_TYPE_PUSHBUTTON, PX_Object_PushButtonOnMouseMove, PX_NULL);
	PX_ObjectRegisterEventEx(pObject, PX_OBJECT_EVENT_CURSORDOWN, PX_OBJECT_TYPE_PUSHBUTTON, PX_Object_PushButtonOnMouseLButtonDown, PX_NULL);
	PX_ObjectRegisterEventEx(pObject, PX_OBJECT_EVENT_CURSORUP, PX_OBJECT_TYPE_PUSHBUTTON, PX_Object_PushButtonOnMouseLButtonUp, PX_NULL);

	return pObject;
}

PX_Object * PX_Object_PushButtonCreate(px_memorypool *mp,PX_Object *Parent,px_int x,px_int y,px_int Width,px_int Height,const px_char *Text,PX_FontModule *fontmodule)
{
	PX_Object *pObject;

	pObject=PX_ObjectCreate(mp,Parent,(px_float)x,(px_float)y,0,(px_float)Width,(px_float)Height,0);
	if (pObject ==PX_NULL)
	{
		return PX_NULL;
	}
	if (!PX_Object_PushButtonAttachObject(pObject,0,Text,fontmodule))
	{
		PX_ObjectDelete(pObject);
		return PX_NULL;
	}
	
	return pObject;
}

PX_Object* PX_Object_PushButtonCreateAlign(px_memorypool* mp, PX_Object* Parent, px_int x, px_int y, px_int Width, px_int Height, const px_char* Text, PX_FontModule* fontmodule, PX_ALIGN align)
{
	PX_Object *pObject= PX_Object_PushButtonCreate(mp, Parent, x, y, Width, Height, Text, fontmodule);
	if (pObject)
	{
		PX_ObjectSetAlign(pObject, align);
	}
	return pObject;
}

PX_Object* PX_Object_CircleButtonCreate(px_memorypool* mp, PX_Object* Parent, px_int x, px_int y, px_int Radius, const px_char* Text, PX_FontModule* fontmodule)
{
	PX_Object *pObject= PX_Object_PushButtonCreate(mp, Parent, x, y, Radius*2, Radius*2, Text, fontmodule);
	if (pObject)
	{
		PX_Object_PushButton *pPushButton = PX_Object_GetPushButton(pObject);
		pPushButton->style = PX_OBJECT_PUSHBUTTON_STYLE_CIRCLE;
		pObject->diameter=Radius*2.f;
	}
	return pObject;
}

PX_Object_PushButton  * PX_Object_GetPushButton( PX_Object *pObject )
{
	PX_Object_PushButton*pdesc= (PX_Object_PushButton *)PX_ObjectGetDescByType(pObject,PX_OBJECT_TYPE_PUSHBUTTON);
	PX_ASSERTIF(pdesc==PX_NULL);
	return pdesc;
}

px_char	  * PX_Object_PushButtonGetText( PX_Object *PushButton )
{
	PX_Object_PushButton *pPushButton=PX_Object_GetPushButton(PushButton);
	if (pPushButton!=PX_NULL)
	{
		return pPushButton->Text;
	}
	return PX_NULL;
}

px_void PX_Object_PushButtonSetText( PX_Object *pObject,const px_char *Text )
{
	PX_Object_PushButton *pPushButton;
	px_int TextLen;
	if (pObject==PX_NULL||Text==PX_NULL)
	{
		return;
	}


	TextLen=PX_strlen(Text);
	pPushButton= PX_Object_GetPushButton(pObject);

	if (TextLen>PX_strlen(pPushButton->Text))
	{
		MP_Free(pObject->mp,pPushButton->Text);

		pPushButton->Text=(px_char *)MP_Malloc(pObject->mp,TextLen+1);
		if (!pPushButton->Text)
		{
			return;
		}
	}
	PX_strcpy(pPushButton->Text,Text,TextLen+1);

}

px_void PX_Object_PushButtonSetTips(PX_Object* pObject, const px_char* tips)
{
	PX_Object_PushButton* pPushButton;
	px_int TextLen;

	pPushButton = PX_Object_GetPushButton(pObject);
	TextLen = PX_strlen(tips);
	if (pPushButton->Tips)
	{
		if (TextLen > PX_strlen(pPushButton->Tips))
		{
			MP_Free(pObject->mp, pPushButton->Tips);

			pPushButton->Tips = (px_char*)MP_Malloc(pObject->mp, TextLen + 1);
			if (!pPushButton->Tips)
			{
				return;
			}
		}
	}
	else
	{
		pPushButton->Tips = (px_char*)MP_Malloc(pObject->mp, TextLen + 1);
		if (!pPushButton->Tips)
		{
			return;
		}
	}
	
	PX_strcpy(pPushButton->Tips, tips, TextLen + 1);
}


px_void PX_Object_PushButtonSetBackgroundColor( PX_Object *pObject,px_color Color )
{
	PX_Object_PushButton * pPushButton=PX_Object_GetPushButton(pObject);
	if (pPushButton)
	{
		pPushButton->BackgroundColor=Color;
	}
}

px_void PX_Object_PushButtonSetTextColor( PX_Object *pObject,px_color Color )
{
	PX_Object_PushButton * pPushButton=PX_Object_GetPushButton(pObject);
	if (pPushButton)
	{

		pPushButton->TextColor=Color;
	}
}

px_void PX_Object_PushButtonSetTexture(PX_Object *pObject,px_texture *texture)
{
	PX_Object_PushButton * pPushButton=PX_Object_GetPushButton(pObject);
	if (pPushButton)
	{
		pPushButton->Texture=texture;
	}
}

px_void PX_Object_PushButtonSetShape(PX_Object *pObject,px_shape *pshape)
{
	PX_Object_PushButton * pPushButton=PX_Object_GetPushButton(pObject);
	if (pPushButton)
	{
		pPushButton->shape=pshape;
	}
}

px_void PX_Object_PushButtonSetGif(PX_Object* pObject, px_gif* gif)
{
	PX_Object_PushButton* pPushButton = PX_Object_GetPushButton(pObject);
	if (pPushButton)
	{
		pPushButton->gif = gif;
	}
}

px_void PX_Object_PushButtonSetBorderColor( PX_Object *pObject,px_color Color )
{
	PX_Object_PushButton * pPushButton=PX_Object_GetPushButton(pObject);
	if (pPushButton)
	{

		pPushButton->BorderColor=Color;
	}
}

px_void PX_Object_PushButtonSetCursorColor(PX_Object *pObject,px_color Color)
{
	PX_Object_PushButton * pPushButton=PX_Object_GetPushButton(pObject);
	if (pPushButton)
	{

		pPushButton->CursorColor=Color;
	}
}

px_void PX_Object_PushButtonSetStyle(PX_Object *pObject,PX_OBJECT_PUSHBUTTON_STYLE style)
{
	PX_Object_PushButton * pPushButton=PX_Object_GetPushButton(pObject);
	if (pPushButton)
	{
		pPushButton->style=style;
	}
}

px_void PX_Object_PushButtonSetPushColor(PX_Object *pObject,px_color Color)
{
	PX_Object_PushButton * pPushButton=PX_Object_GetPushButton(pObject);
	if (pPushButton)
	{

		pPushButton->PushColor=Color;
	}
}

px_void PX_Object_PushButtonSetBorder( PX_Object *pObject,px_bool Border )
{
	PX_Object_PushButton *PushButton=PX_Object_GetPushButton(pObject);
	if (PushButton)
	{

		PushButton->Border=Border;
	}
}

px_void PX_Object_PushButtonSetRoundRadius(PX_Object* pObject, px_float RoundRadius)
{
	PX_Object_PushButton* PushButton = PX_Object_GetPushButton(pObject);
	if (PushButton)
	{

		PushButton->roundradius = RoundRadius;
	}
}


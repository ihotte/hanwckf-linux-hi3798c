/******************************************************************************
*	       Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*			    ALL RIGHTS RESERVED
* FileName: gfx2d_hal_hwc.c
* Description: Graphic 2D engine hwc interface declaration
*
* History:
* Version   Date	 Author	    DefectNum	 Description
* main\1    2013-08-02		    NULL	 Create this file.
*******************************************************************************/

/* SPDX-License-Identifier: GPL-2.0 */

#include <linux/fs.h>

#include "hi_type.h"

#ifndef _GFX2D_CONFIG_H_
#define _GFX2D_CONFIG_H_

typedef HI_VOID (*GFX2D_FUNC_CB)(HI_VOID *param);

HI_S32 GFX2D_CONFIG_SetMemSize(HI_U32 u32MemSize);

HI_U32 GFX2D_CONFIG_GetMemSize(HI_VOID);

HI_U32 GFX2D_CONFIG_GetNodeNum(HI_VOID);

HI_VOID GFX2D_CONFIG_StartTime(HI_VOID);

HI_U32 GFX2D_CONFIG_EndTime(HI_VOID);

HI_S32 GFX2D_CONFIG_WriteProc(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos);

#endif

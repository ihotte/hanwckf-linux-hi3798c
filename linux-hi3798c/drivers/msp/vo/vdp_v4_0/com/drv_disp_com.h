
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_com.h
Version	      : Initial Draft
Author	      : Hisilicon multimedia software group
Created	      : 2012/12/30
Last Modified :
Description   :
Function List :
History	      :
******************************************************************************/

/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __DRV_DISP_COM_H__
#define __DRV_DISP_COM_H__

#include "hi_type.h"
#include "hi_drv_video.h"
#include "hi_drv_disp.h"
#include "drv_disp_version.h"
#include "drv_disp_osal.h"
#include "hi_reg_common.h"

/*the mode of isogeny output*/
typedef enum tagISOGENY_MODE
{
	/*cv200/98m/98c, under which hd and sd 's video layer is  symmetrical.*/
	ISOGENY_SYMMETRICAL_MODE = 0,
	/*mv410,98cv200, the sd channel has a wbc input from hd channel.*/
	ISOGENY_WBC_MODE,
    ISOGENY_BUTT
}ISOGENY_MODE_E;

#endif /*  __DRV_DISP_COMMON_H__  */

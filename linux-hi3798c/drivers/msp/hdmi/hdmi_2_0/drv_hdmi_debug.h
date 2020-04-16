/******************************************************************************

  Copyright (C), 2014-2016, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name	: drv_hdmi_debug.h
  Version	: Initial Draft
  Author	: Hisilicon multimedia software group
  Created	: 2015/06/24
  Description	:
  History	:
  Date		: 2015/06/24
  Author	: t00273561
  Modification	:
*******************************************************************************/

/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __DRV_HDMI_DEBUG_H__
#define __DRV_HDMI_DEBUG_H__

#include "hi_type.h"
#include "drv_hdmi_platform.h"
#include "drv_hdmi_edid.h"

HI_S32 DRV_HDMI_DebugSourceWrite(struct file * file,
						    const char __user * buf,
						    size_t count,
						    loff_t *ppos);

HI_S32 DRV_HDMI_DebugEdidRawRead(HDMI_EDID_UPDATE_MODE_E enMode,
											HI_U8 *pau8Data,
											HI_U32 u32Len);

#endif	/* __DRV_HDMI_DEBUG_H__ */

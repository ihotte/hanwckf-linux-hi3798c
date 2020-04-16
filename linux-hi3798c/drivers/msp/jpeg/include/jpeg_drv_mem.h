/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
******************************************************************************
File Name	 : jpeg_drv_mem.h
Version		   : Initial Draft
Author		  :
Created		   : 2016/02/02
Description	   :
Function List	  :


History		  :
Date		    Author		  Modification
2016/02/02	      y00181162		     Created file
******************************************************************************/

/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __JPEG_DRV_MEM_H__
#define __JPEG_DRV_MEM_H__


/*********************************add include here******************************/
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <asm/atomic.h>
#include <asm/io.h>

#include "hi_type.h"
#include "hi_jpeg_config.h"


#ifdef CONFIG_JPEG_OMX_FUNCTION

/*****************************************************************************/


     /***************************** Macro Definition ******************************/
     /** \addtogroup	  JPEG */
     /** @{ */	  /** <!--[JPEG]*/

    #define JPGD_DRV_STREAM_BUFFER	(512 * 1024)

     /** @} */	/** <!-- ==== Macro Definition end ==== */

     /*************************** Enum Definition ****************************/
    /** \addtogroup	 JPEG */
    /** @{ */	 /** <!--[JPEG]*/

    /** @} */  /** <!-- ==== enum Definition end ==== */

    /*************************** Structure Definition ****************************/
    /** \addtogroup	 JPEG */
    /** @{ */	 /** <!--[JPEG]*/


    /** @} */  /** <!-- ==== Structure Definition End ==== */

    /********************** Global Variable declaration **************************/

    /******************************* API declaration *****************************/

    HI_VOID* DRV_JPEG_AllocMem(HI_U32 *pPhyBuf);

    HI_VOID DRV_JPEG_FremMem(HI_U32 PhyBuf, HI_CHAR* pVirBuf, HI_BOOL bSmmu);


    /** @} */  /** <!-- ==== API Declaration End ==== */

    /****************************************************************************/

#endif

#endif/**__JPEG_DRV_MEM_H__**/

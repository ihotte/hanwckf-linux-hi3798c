/*
 * Copyright (C) 2017, Hisilicon Tech. Co., Ltd.
 * SPDX-License-Identifier: GPL-2.0
 */
#ifndef __SYNC_INTF_H__
#define __SYNC_INTF_H__

typedef struct hiSYNC_Intf_Register_Param{
    DRV_PROC_READ_FN  rdproc;
    DRV_PROC_WRITE_FN	 wtproc;
}SYNC_REGISTER_PARAM_S;

SYNC_S *SYNC_getInfoPtr(HI_U32 SyncId);
HI_S32	SYNC_IntfRegister(SYNC_REGISTER_PARAM_S *param);
HI_VOID SYNC_IntfUnRegister(HI_VOID);
HI_S32	SYNC_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg);
#ifdef CONFIG_COMPAT
HI_S32	SYNC_COMPAT_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg);
#endif
HI_S32	SYNC_DRV_Open(struct inode *finode, struct file	 *ffile);
HI_S32	SYNC_DRV_Close(struct inode *finode, struct file  *ffile);
HI_S32	SYNC_Suspend(PM_BASEDEV_S *pdev, pm_message_t state);
HI_S32	SYNC_Resume(PM_BASEDEV_S *pdev);

#endif

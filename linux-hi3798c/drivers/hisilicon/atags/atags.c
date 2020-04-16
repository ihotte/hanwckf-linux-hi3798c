/******************************************************************************
 * Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 * Create By Cai Zhiyong 2015.6.10
******************************************************************************/

/* SPDX-License-Identifier: GPL-2.0 */

#define DRVNAME "atags"
#define pr_fmt(fmt) DRVNAME ": " fmt

#include <linux/kernel.h>
#include <linux/root_dev.h>
#include <linux/fs.h>

#include "setup.h"
#include "tagtables.h"

/******************************************************************************/

static int __init parse_tag_core(const struct tag *tag, void *fdt)
{
	if (tag->hdr.size > 2) {
		if ((tag->u.core.flags & 1) == 0)
			root_mountflags &= ~MS_RDONLY;
		ROOT_DEV = old_decode_dev(tag->u.core.rootdev);
	}
	return 0;
}
static struct tagtable tag_core __initdata = {ATAG_CORE, parse_tag_core};
/******************************************************************************/

static struct tagtable *tagtable[] __initdata = {
	&tag_param,
	&tag_sdkver,
	&tag_core,
	&tag_macaddr,
	&tag_phyaddr,
	&tag_phyintf,
	&tag_phygpio,
	&tag_chiptrim,
	&tag_emmc,
	NULL,
};
/******************************************************************************/

int __init parse_tags_hisi(const struct tag *tag, void *fdt)
{
	struct tagtable **tagtbl = tagtable;

	while (*tagtbl) {
		if ((*tagtbl)->tag == tag->hdr.tag)
			return (*tagtbl)->parse(tag, fdt);
		tagtbl++;
	}

	return 0;
}

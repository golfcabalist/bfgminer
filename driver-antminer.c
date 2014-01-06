/*
 * Copyright 2013 Luke Dashjr
 * Copyright 2013 Nate Woolls
 * Copyright 2013 Lingchao Xu <lingchao.xu@bitmaintech.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  See COPYING for more details.
 */

#include "config.h"

#include <stdbool.h>

#include "miner.h"
#include "icarus-common.h"
#include "lowlevel.h"
#include "lowl-vcom.h"

#define ANTMINER_IO_SPEED 115200
#define ANTMINER_HASH_TIME 0.0000000029761

BFG_REGISTER_DRIVER(antminer_drv)

//static
//bool antminer_lowl_match(const struct lowlevel_device_info * const info)
//{
//	return lowlevel_match_lowlproduct(info, &lowl_vcom, "AntMiner", "U1");
//}

static bool antminer_detect_one(const char *devpath)
{
	struct device_drv *drv = &antminer_drv;
	
	struct ICARUS_INFO *info = calloc(1, sizeof(struct ICARUS_INFO));
	if (unlikely(!info))
	quit(1, "Failed to malloc ICARUS_INFO");
	
	*info = (struct ICARUS_INFO){
		.baud = ANTMINER_IO_SPEED,
		.Hs = ANTMINER_HASH_TIME,
		.timing_mode = MODE_DEFAULT,
		.read_size = 5,
		.do_estimate_hashrate = false,
		.read_count = 15,
	};
	
	if (!icarus_detect_custom(devpath, drv, info)) {
		free(info);
		return false;
	}
	
	info->read_count = 15;
	
	return true;
}

static
bool antminer_lowl_probe(const struct lowlevel_device_info * const info)
{
	return vcom_lowl_probe_wrapper(info, antminer_detect_one);
}

//static bool antminer_identify(struct cgpu_info *erupter)
//{
//	struct thr_info *thr = erupter->thr[0];
//	struct icarus_state *state = thr->cgpu_data;
//	state->identify = true;
//	return true;
//}

static void antminer_drv_init()
{
	antminer_drv = icarus_drv;
	antminer_drv.dname = "antminer";
	antminer_drv.name = "ANT";
//	antminer_drv.lowl_match = antminer_lowl_match;
	antminer_drv.lowl_probe = antminer_lowl_probe;
//	antminer_drv.identify_device = antminer_identify;
	++antminer_drv.probe_priority;
}

struct device_drv antminer_drv = {
	.drv_init = antminer_drv_init,
};

struct device_drv antminer_drv_emerald = {
	.drv_init = antminer_drv_init,
};

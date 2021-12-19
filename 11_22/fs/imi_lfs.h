#ifndef __IMI_LFS_H__
#define __IMI_LFS_H__


#include "littlefs/lfs.h"

extern lfs_t g_system_lfs;

extern int imi_lfs_init(void);
extern void imi_lfs_deinit(void);

#endif

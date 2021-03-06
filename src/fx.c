#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "roccat-vulcan.h"


// Rows
#define RV_NUM_ROWS 6
#define RV_MAX_KEYS_PER_ROW 22
unsigned char rv_rows[RV_NUM_TOPO_MODELS][RV_NUM_ROWS][RV_MAX_KEYS_PER_ROW] = {

	// ISO model
	0x00, 0x0b, 0x11, 0x17, 0x1c, 0x30, 0x35, 0x3b, 0x41, 0x4e, 0x54, 0x55, 0x56, 0x63, 0x67, 0x6c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x01, 0x06, 0x0c, 0x12, 0x18, 0x1d, 0x21, 0x31, 0x36, 0x3c, 0x42, 0x48, 0x4f, 0x57, 0x64, 0x68, 0x6d, 0x71, 0x77, 0x7c, 0x81, 0xff,
	0x02, 0x07, 0x0d, 0x13, 0x19, 0x1e, 0x22, 0x32, 0x37, 0x3d, 0x43, 0x49, 0x50, 0x58, 0x65, 0x69, 0x6e, 0x72, 0x78, 0x7d, 0x82, 0xff,
	0x03, 0x08, 0x0e, 0x14, 0x1a, 0x1f, 0x23, 0x33, 0x38, 0x3e, 0x44, 0x4a, 0x60, 0x73, 0x79, 0x7e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x04, 0x09, 0x0f, 0x15, 0x1b, 0x20, 0x24, 0x34, 0x39, 0x3f, 0x45, 0x4b, 0x52, 0x6a, 0x74, 0x7a, 0x7f, 0x83, 0xff, 0xff, 0xff, 0xff,
	0x05, 0x0a, 0x10, 0x25, 0x46, 0x4c, 0x53, 0x59, 0x66, 0x6b, 0x6f, 0x75, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,

	// ANSI model
	0x00, 0x0b, 0x11, 0x17, 0x1c, 0x30, 0x35, 0x3b, 0x41, 0x4e, 0x54, 0x55, 0x56, 0x63, 0x67, 0x6c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x01, 0x06, 0x0c, 0x12, 0x18, 0x1d, 0x21, 0x31, 0x36, 0x3c, 0x42, 0x48, 0x4f, 0x57, 0x64, 0x68, 0x6d, 0x71, 0x77, 0x7c, 0x81, 0xff,
	0x02, 0x07, 0x0d, 0x13, 0x19, 0x1e, 0x22, 0x32, 0x37, 0x3d, 0x43, 0x49, 0x50, 0x51, 0x65, 0x69, 0x6e, 0x72, 0x78, 0x7d, 0x82, 0xff,
	0x03, 0x08, 0x0e, 0x14, 0x1a, 0x1f, 0x23, 0x33, 0x38, 0x3e, 0x44, 0x4a, 0x58, 0x73, 0x79, 0x7e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x04, 0x0f, 0x15, 0x1b, 0x20, 0x24, 0x34, 0x39, 0x3f, 0x45, 0x4b, 0x52, 0x6a, 0x74, 0x7a, 0x7f, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x05, 0x0a, 0x10, 0x25, 0x46, 0x4c, 0x53, 0x59, 0x66, 0x6b, 0x6f, 0x75, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// Cols
#define RV_NUM_COLS 21
#define RV_MAX_KEYS_PER_COL 6
unsigned char rv_cols[RV_NUM_TOPO_MODELS][RV_NUM_COLS][RV_MAX_KEYS_PER_COL] = {

	// ISO model
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
	0x06, 0x07, 0x08, 0x09, 0x0a, 0xff,
	0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
	0x11, 0x12, 0x13, 0x14, 0x15, 0xff,
	0x17, 0x18, 0x19, 0x1a, 0x1b, 0xff,
	0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0xff,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x30,
	0x31, 0x32, 0x33, 0x34, 0x35, 0xff,
	0x36, 0x37, 0x38, 0x39, 0x3b, 0xff,
	0x3c, 0x3d, 0x3e, 0x3f, 0x41, 0xff,
	0x42, 0x43, 0x44, 0x45, 0x46, 0xff,
	0x48, 0x49, 0x4a, 0x4b, 0x4e, 0x4c,
	0x4f, 0x50, 0x53, 0x54, 0x60, 0xff,
	0x52, 0x55, 0x57, 0x58, 0x59, 0xff,
	0x56, 0x63, 0x64, 0x65, 0x66, 0xff,
	0x67, 0x68, 0x69, 0x6a, 0x6b, 0xff,
	0x6c, 0x6d, 0x6e, 0x6f, 0xff, 0xff,
	0x71, 0x72, 0x73, 0x74, 0x75, 0xff,
	0x77, 0x78, 0x79, 0x7a, 0xff, 0xff,
	0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0xff,
	0x81, 0x82, 0x83, 0xff, 0xff, 0xff,

	// ANSI model
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
	0x06, 0x07, 0x08, 0x0a, 0xff, 0xff,
	0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
	0x11, 0x12, 0x13, 0x14, 0x15, 0xff,
	0x17, 0x18, 0x19, 0x1a, 0x1b, 0xff,
	0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0xff,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x30,
	0x31, 0x32, 0x33, 0x34, 0x35, 0xff,
	0x36, 0x37, 0x38, 0x39, 0x3b, 0xff,
	0x3c, 0x3d, 0x3e, 0x3f, 0x41, 0xff,
	0x42, 0x43, 0x44, 0x45, 0x46, 0xff,
	0x48, 0x49, 0x4a, 0x4b, 0x4e, 0x4c,
	0x4f, 0x50, 0x53, 0x54, 0xff, 0xff,
	0x51, 0x52, 0x55, 0x57, 0x58, 0x59,
	0x56, 0x63, 0x64, 0x65, 0x66, 0xff,
	0x67, 0x68, 0x69, 0x6a, 0x6b, 0xff,
	0x6c, 0x6d, 0x6e, 0x6f, 0xff, 0xff,
	0x71, 0x72, 0x73, 0x74, 0x75, 0xff,
	0x77, 0x78, 0x79, 0x7a, 0xff, 0xff,
	0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0xff,
	0x81, 0x82, 0x83, 0xff, 0xff, 0xff
};

// Neighbor tables
#define RV_MAX_NEIGH 10
unsigned char rv_neigh[RV_NUM_TOPO_MODELS][RV_NUM_KEYS][RV_MAX_NEIGH] = {

	// ISO model
	0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x00
	0x00, 0x02, 0x06, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x01
	0x01, 0x03, 0x06, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x02
	0x02, 0x04, 0x08, 0x09, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x03
	0x03, 0x05, 0x09, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x04
	0x04, 0x0a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x05
	0x01, 0x02, 0x07, 0x0c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x06
	0x02, 0x06, 0x08, 0x0c, 0x0d, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x07
	0x03, 0x07, 0x09, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x08
	0x03, 0x04, 0x08, 0x0a, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x09
	0x05, 0x09, 0x10, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x0a
	0x0c, 0x11, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x0b
	0x06, 0x07, 0x0b, 0x0d, 0x12, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x0c
	0x07, 0x0c, 0x0e, 0x12, 0x13, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x0d
	0x08, 0x0d, 0x0f, 0x14, 0x15, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x0e
	0x08, 0x09, 0x0e, 0x10, 0x15, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x0f
	0x0a, 0x0f, 0x15, 0x25, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x10
	0x0b, 0x12, 0x17, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x11
	0x0c, 0x0d, 0x11, 0x13, 0x18, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x12
	0x0d, 0x12, 0x14, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x13
	0x0e, 0x13, 0x15, 0x1a, 0x1b, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x14
	0x0e, 0x0f, 0x10, 0x14, 0x1b, 0x25, 0xff, 0xff, 0xff, 0xff, // 0x15
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x16
	0x11, 0x18, 0x1c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x17
	0x12, 0x13, 0x17, 0x19, 0x1d, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x18
	0x13, 0x18, 0x1a, 0x1d, 0x1e, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x19
	0x14, 0x19, 0x1b, 0x1f, 0x20, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x1a
	0x14, 0x15, 0x1a, 0x20, 0x25, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x1b
	0x17, 0x1d, 0x30, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x1c
	0x18, 0x19, 0x1c, 0x1e, 0x21, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x1d
	0x19, 0x1d, 0x1f, 0x21, 0x22, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x1e
	0x1a, 0x1e, 0x20, 0x23, 0x24, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x1f
	0x1a, 0x1b, 0x1f, 0x24, 0x25, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x20
	0x1d, 0x1e, 0x22, 0x30, 0x31, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x21
	0x1e, 0x21, 0x23, 0x31, 0x32, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x22
	0x1f, 0x22, 0x24, 0x33, 0x34, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x23
	0x1f, 0x20, 0x23, 0x25, 0x34, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x24
	0x10, 0x15, 0x1b, 0x20, 0x24, 0x34, 0x39, 0x3f, 0x46, 0xff, // 0x25
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x26
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x27
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x28
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x29
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x2a
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x2b
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x2c
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x2d
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x2e
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x2f
	0x1c, 0x21, 0x31, 0x35, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x30
	0x21, 0x22, 0x30, 0x32, 0x35, 0x36, 0xff, 0xff, 0xff, 0xff, // 0x31
	0x22, 0x31, 0x33, 0x36, 0x37, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x32
	0x23, 0x32, 0x34, 0x38, 0x39, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x33
	0x23, 0x24, 0x25, 0x33, 0x39, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x34
	0x30, 0x31, 0x36, 0x3b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x35
	0x31, 0x32, 0x35, 0x37, 0x3b, 0x3c, 0xff, 0xff, 0xff, 0xff, // 0x36
	0x32, 0x36, 0x38, 0x3c, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x37
	0x33, 0x37, 0x39, 0x3e, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x38
	0x25, 0x33, 0x34, 0x38, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x39
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x3a
	0x35, 0x36, 0x3c, 0x41, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x3b
	0x36, 0x37, 0x3b, 0x3d, 0x41, 0x42, 0xff, 0xff, 0xff, 0xff, // 0x3c
	0x37, 0x3c, 0x3e, 0x42, 0x43, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x3d
	0x38, 0x3d, 0x3f, 0x44, 0x45, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x3e
	0x25, 0x38, 0x39, 0x3e, 0x45, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x3f
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x40
	0x3b, 0x3c, 0x42, 0x4e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x41
	0x3c, 0x3d, 0x41, 0x43, 0x48, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x42
	0x3d, 0x42, 0x44, 0x48, 0x49, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x43
	0x3e, 0x43, 0x45, 0x4a, 0x4b, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x44
	0x3e, 0x3f, 0x44, 0x46, 0x4b, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x45
	0x25, 0x45, 0x4c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x46
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x47
	0x42, 0x43, 0x49, 0x4e, 0x4f, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x48
	0x43, 0x48, 0x4a, 0x4f, 0x50, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x49
	0x44, 0x49, 0x4b, 0x52, 0x60, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x4a
	0x44, 0x45, 0x4a, 0x4c, 0x52, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x4b
	0x46, 0x4b, 0x53, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x4c
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x4d
	0x41, 0x48, 0x54, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x4e
	0x48, 0x49, 0x50, 0x54, 0x57, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x4f
	0x49, 0x4f, 0x57, 0x58, 0x60, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x50
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x51
	0x4a, 0x4b, 0x53, 0x58, 0x59, 0x60, 0xff, 0xff, 0xff, 0xff, // 0x52
	0x52, 0x59, 0x4c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x53
	0x4e, 0x4f, 0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x54
	0x54, 0x56, 0x57, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x55
	0x55, 0x57, 0x63, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x56
	0x4f, 0x50, 0x55, 0x56, 0x58, 0x64, 0xff, 0xff, 0xff, 0xff, // 0x57
	0x50, 0x52, 0x57, 0x60, 0x65, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x58
	0x52, 0x53, 0x66, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x59
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x5a
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x5b
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x5c
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x5d
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x5e
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x5f
	0x4a, 0x50, 0x52, 0x58, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x60
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x61
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x62
	0x56, 0x64, 0x67, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x63
	0x57, 0x63, 0x65, 0x68, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x64
	0x58, 0x64, 0x69, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x65
	0x59, 0x6b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x66
	0x63, 0x68, 0x6c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x67
	0x64, 0x67, 0x69, 0x6d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x68
	0x65, 0x68, 0x6e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x69
	0x6b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x6a
	0x66, 0x6a, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x6b
	0x67, 0x6d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x6c
	0x68, 0x6c, 0x6e, 0x71, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x6d
	0x69, 0x6d, 0x72, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x6e
	0x6b, 0x75, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x6f
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x70
	0x6d, 0x72, 0x77, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x71
	0x6e, 0x71, 0x73, 0x78, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x72
	0x72, 0x74, 0x79, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x73
	0x73, 0x75, 0x7a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x74
	0x6f, 0x74, 0x7a, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x75
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x76
	0x71, 0x78, 0x7c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x77
	0x72, 0x77, 0x79, 0x7d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x78
	0x73, 0x78, 0x7a, 0x7e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x79
	0x74, 0x75, 0x79, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x7a
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x7b
	0x77, 0x7d, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x7c
	0x78, 0x7c, 0x7e, 0x82, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x7d
	0x79, 0x7d, 0x7f, 0x82, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x7e
	0x7a, 0x7e, 0x80, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x7f
	0x75, 0x7f, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x80
	0x7c, 0x82, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x81
	0x7d, 0x7e, 0x81, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x82
	0x7f, 0x80, 0x82, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x83
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x84
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x85
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x86
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x87
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x88
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x89
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x8a
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x8b
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x8c
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x8d
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x8e
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x8f

	// ANSI model
	0x01, 0x06, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x00
	0x00, 0x02, 0x06, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x01
	0x01, 0x03, 0x06, 0x07, 0x0c, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x02
	0x02, 0x04, 0x07, 0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x03
	0x03, 0x05, 0x08, 0x0a, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x04
	0x04, 0x0a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x05
	0x00, 0x01, 0x02, 0x07, 0x0b, 0x0c, 0xff, 0xff, 0xff, 0xff, // 0x06
	0x02, 0x03, 0x06, 0x08, 0x0c, 0x0d, 0xff, 0xff, 0xff, 0xff, // 0x07
	0x03, 0x04, 0x07, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, // 0x08
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x09
	0x04, 0x05, 0x0f, 0x10, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x0a
	0x06, 0x0c, 0x11, 0x12, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x0b
	0x06, 0x07, 0x0b, 0x0d, 0x11, 0x12, 0xff, 0xff, 0xff, 0xff, // 0x0c
	0x07, 0x08, 0x0c, 0x0e, 0x12, 0x13, 0xff, 0xff, 0xff, 0xff, // 0x0d
	0x08, 0x0d, 0x0f, 0x13, 0x14, 0x15, 0xff, 0xff, 0xff, 0xff, // 0x0e
	0x04, 0x08, 0x0a, 0x0e, 0x10, 0x15, 0xff, 0xff, 0xff, 0xff, // 0x0f
	0x0a, 0x0f, 0x15, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x10
	0x0b, 0x0c, 0x12, 0x17, 0x18, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x11
	0x0b, 0x0c, 0x0d, 0x11, 0x13, 0x17, 0x18, 0xff, 0xff, 0xff, // 0x12
	0x0d, 0x0e, 0x12, 0x14, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, // 0x13
	0x0e, 0x13, 0x15, 0x19, 0x1a, 0x1b, 0xff, 0xff, 0xff, 0xff, // 0x14
	0x0e, 0x0f, 0x10, 0x14, 0x1b, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x15
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x16
	0x11, 0x12, 0x18, 0x1c, 0x1d, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x17
	0x11, 0x12, 0x13, 0x17, 0x19, 0x1c, 0x1d, 0xff, 0xff, 0xff, // 0x18
	0x13, 0x14, 0x18, 0x1a, 0x1d, 0x1e, 0xff, 0xff, 0xff, 0xff, // 0x19
	0x14, 0x19, 0x1b, 0x1e, 0x1f, 0x20, 0xff, 0xff, 0xff, 0xff, // 0x1a
	0x14, 0x15, 0x1a, 0x20, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x1b
	0x17, 0x18, 0x1d, 0x21, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x1c
	0x17, 0x18, 0x19, 0x1c, 0x1e, 0x21, 0xff, 0xff, 0xff, 0xff, // 0x1d
	0x19, 0x1a, 0x1d, 0x1f, 0x21, 0x22, 0xff, 0xff, 0xff, 0xff, // 0x1e
	0x1a, 0x1e, 0x20, 0x22, 0x23, 0x24, 0xff, 0xff, 0xff, 0xff, // 0x1f
	0x1a, 0x1b, 0x1f, 0x24, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x20
	0x1d, 0x1e, 0x22, 0x30, 0x31, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x21
	0x1e, 0x1f, 0x21, 0x23, 0x31, 0x32, 0xff, 0xff, 0xff, 0xff, // 0x22
	0x1f, 0x22, 0x24, 0x32, 0x33, 0x34, 0xff, 0xff, 0xff, 0xff, // 0x23
	0x1f, 0x20, 0x23, 0x25, 0x34, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x24
	0x20, 0x24, 0x34, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x25
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x26
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x27
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x28
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x29
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x2a
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x2b
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x2c
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x2d
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x2e
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x2f
	0x21, 0x31, 0x35, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x30
	0x21, 0x22, 0x30, 0x32, 0x35, 0x36, 0xff, 0xff, 0xff, 0xff, // 0x31
	0x22, 0x23, 0x31, 0x33, 0x36, 0x37, 0xff, 0xff, 0xff, 0xff, // 0x32
	0x23, 0x32, 0x34, 0x37, 0x38, 0x39, 0xff, 0xff, 0xff, 0xff, // 0x33
	0x23, 0x24, 0x25, 0x33, 0x39, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x34
	0x30, 0x31, 0x36, 0x3b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x35
	0x31, 0x32, 0x35, 0x37, 0x3b, 0x3c, 0xff, 0xff, 0xff, 0xff, // 0x36
	0x32, 0x33, 0x36, 0x38, 0x3c, 0x3d, 0xff, 0xff, 0xff, 0xff, // 0x37
	0x33, 0x37, 0x39, 0x3d, 0x3e, 0x3f, 0xff, 0xff, 0xff, 0xff, // 0x38
	0x33, 0x34, 0x38, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x39
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x3a
	0x35, 0x36, 0x3c, 0x41, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x3b
	0x36, 0x37, 0x3b, 0x3d, 0x41, 0x42, 0xff, 0xff, 0xff, 0xff, // 0x3c
	0x37, 0x38, 0x3c, 0x3e, 0x42, 0x43, 0xff, 0xff, 0xff, 0xff, // 0x3d
	0x38, 0x3d, 0x3f, 0x43, 0x44, 0x45, 0xff, 0xff, 0xff, 0xff, // 0x3e
	0x38, 0x39, 0x3e, 0x45, 0x46, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x3f
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x40
	0x3b, 0x3c, 0x42, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x41
	0x3c, 0x3d, 0x41, 0x43, 0x48, 0x4e, 0xff, 0xff, 0xff, 0xff, // 0x42
	0x3d, 0x3e, 0x42, 0x44, 0x48, 0x49, 0xff, 0xff, 0xff, 0xff, // 0x43
	0x3e, 0x43, 0x45, 0x49, 0x4a, 0x4b, 0xff, 0xff, 0xff, 0xff, // 0x44
	0x3e, 0x3f, 0x44, 0x46, 0x4b, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x45
	0x3f, 0x45, 0x4b, 0x4c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x46
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x47
	0x42, 0x43, 0x49, 0x4e, 0x4f, 0x54, 0xff, 0xff, 0xff, 0xff, // 0x48
	0x43, 0x44, 0x48, 0x4a, 0x4f, 0x50, 0xff, 0xff, 0xff, 0xff, // 0x49
	0x44, 0x49, 0x4b, 0x50, 0x52, 0x58, 0xff, 0xff, 0xff, 0xff, // 0x4a
	0x44, 0x45, 0x4a, 0x4c, 0x52, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x4b
	0x45, 0x46, 0x4b, 0x52, 0x53, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x4c
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x4d
	0x42, 0x48, 0x4f, 0x54, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x4e
	0x48, 0x49, 0x4e, 0x50, 0x54, 0x55, 0x57, 0xff, 0xff, 0xff, // 0x4f
	0x49, 0x4a, 0x4f, 0x51, 0x57, 0x58, 0xff, 0xff, 0xff, 0xff, // 0x50
	0x50, 0x57, 0x58, 0x64, 0x65, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x51
	0x4a, 0x4b, 0x53, 0x58, 0x59, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x52
	0x52, 0x59, 0x4c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x53
	0x48, 0x4e, 0x4f, 0x55, 0x57, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x54
	0x4f, 0x54, 0x56, 0x57, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x55
	0x55, 0x57, 0x63, 0x64, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x56
	0x4f, 0x50, 0x51, 0x55, 0x56, 0x63, 0x64, 0x65, 0xff, 0xff, // 0x57
	0x4a, 0x50, 0x51, 0x52, 0x65, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x58
	0x52, 0x53, 0x66, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x59
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x5a
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x5b
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x5c
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x5d
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x5e
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x5f
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x60
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x61
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x62
	0x56, 0x57, 0x64, 0x67, 0x68, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x63
	0x51, 0x56, 0x57, 0x63, 0x65, 0x67, 0x68, 0x69, 0xff, 0xff, // 0x64
	0x51, 0x57, 0x58, 0x64, 0x68, 0x69, 0xff, 0xff, 0xff, 0xff, // 0x65
	0x52, 0x59, 0x6a, 0x6b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x66
	0x63, 0x64, 0x68, 0x6c, 0x6d, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x67
	0x63, 0x64, 0x65, 0x67, 0x69, 0x6c, 0x6d, 0x6e, 0xff, 0xff, // 0x68
	0x64, 0x65, 0x68, 0x6a, 0x6d, 0x6e, 0xff, 0xff, 0xff, 0xff, // 0x69
	0x65, 0x66, 0x69, 0x6b, 0x6e, 0x6f, 0xff, 0xff, 0xff, 0xff, // 0x6a
	0x66, 0x6a, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x6b
	0x67, 0x68, 0x6d, 0x71, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x6c
	0x67, 0x68, 0x69, 0x6c, 0x6e, 0x71, 0xff, 0xff, 0xff, 0xff, // 0x6d
	0x68, 0x69, 0x6d, 0x71, 0x72, 0x73, 0xff, 0xff, 0xff, 0xff, // 0x6e
	0x6a, 0x6b, 0x75, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x6f
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x70
	0x6c, 0x6d, 0x6e, 0x72, 0x77, 0x78, 0xff, 0xff, 0xff, 0xff, // 0x71
	0x6d, 0x6e, 0x71, 0x73, 0x77, 0x78, 0x79, 0xff, 0xff, 0xff, // 0x72
	0x6e, 0x72, 0x78, 0x79, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x73
	0x73, 0x75, 0x79, 0x7a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x74
	0x6f, 0x74, 0x7a, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x75
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x76
	0x71, 0x72, 0x78, 0x7c, 0x7d, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x77
	0x71, 0x72, 0x73, 0x77, 0x79, 0x7c, 0x7d, 0x7e, 0xff, 0xff, // 0x78
	0x72, 0x73, 0x74, 0x78, 0x7a, 0x7d, 0x7e, 0x7f, 0xff, 0xff, // 0x79
	0x73, 0x74, 0x75, 0x79, 0x7e, 0x7f, 0x80, 0xff, 0xff, 0xff, // 0x7a
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x7b
	0x77, 0x78, 0x7d, 0x81, 0x82, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x7c
	0x77, 0x78, 0x79, 0x7c, 0x7e, 0x81, 0x82, 0xff, 0xff, 0xff, // 0x7d
	0x78, 0x79, 0x7a, 0x7d, 0x7f, 0x82, 0x83, 0xff, 0xff, 0xff, // 0x7e
	0x75, 0x79, 0x7a, 0x7e, 0x80, 0x82, 0x83, 0xff, 0xff, 0xff, // 0x7f
	0x75, 0x7a, 0x7f, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x80
	0x7c, 0x7d, 0x82, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x81
	0x7c, 0x7d, 0x7e, 0x7f, 0x81, 0x83, 0xff, 0xff, 0xff, 0xff, // 0x82
	0x7e, 0x7f, 0x80, 0x82, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x83
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x84
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x85
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x86
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x87
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x88
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x89
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x8a
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x8b
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x8c
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x8d
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0x8e
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff  // 0x8f
};


void rv_blend_to(rv_rgb_map *src, rv_rgb_map *dst, rv_rgb tc, int amount) {
	int k, max;
	rv_rgb srgb, drgb;

	for (k = 0; k < RV_NUM_KEYS; k++) {
		srgb = src->key[k];
		drgb = dst->key[k];

		srgb.r = (srgb.r + drgb.r) / 2;
		srgb.g = (srgb.g + drgb.g) / 2;
		srgb.b = (srgb.b + drgb.b) / 2;

		if (abs(tc.r - srgb.r) < amount) srgb.r = tc.r;
		else if (tc.r > srgb.r) srgb.r += amount;
		     else srgb.r -= amount;
		if (abs(tc.g - srgb.g) < amount) srgb.g = tc.g;
		else if (tc.g > srgb.g) srgb.g += amount;
		     else srgb.g -= amount;
		if (abs(tc.b - srgb.b) < amount) srgb.b = tc.b;
		else if (tc.b > srgb.b) srgb.b += amount;
		     else srgb.b -= amount;

		dst->key[k] = srgb;
	}
}

int rv_fx_init() {
	return rv_send_led_map(NULL);
}

unsigned char rv_wheel_offset(unsigned char pos, char offset) {
	return pos + offset;
}

void rv_schedule_impact(unsigned char n0, rv_rgb_map *wheel[], unsigned char wheel_pos, int p1, int p2, rv_rgb c0, rv_rgb c1, rv_rgb c2) {
	int i,j;
	unsigned char seen[RV_NUM_KEYS];

	memset(seen, 0, sizeof(seen));
	wheel[wheel_pos]->key[n0] = c0;
	seen[n0] = 1;

	for (i = 0; i < RV_MAX_NEIGH; i++) {
		unsigned char n1 = rv_neigh[rv_topo_model][n0][i];
		if (n1 == 0xff) break;
		if (!seen[n1]) wheel[rv_wheel_offset(wheel_pos, p1)]->key[n1] = c1;
		seen[n1] = 1;
	}

	for (i = 0; i < RV_MAX_NEIGH; i++) {
		unsigned char n1 = rv_neigh[rv_topo_model][n0][i];
		if (n1 == 0xff) break;
		for (j = 0; j < RV_MAX_NEIGH; j++) {
			unsigned char n2 = rv_neigh[rv_topo_model][n1][j];
			if (n2 == 0xff) break;
			if (!seen[n2]) wheel[rv_wheel_offset(wheel_pos, p2)]->key[n2] = c2;
		}
	}
}

void rv_fx_impact() {
	int k;
	rv_rgb_map *wheel[256];
	unsigned char wheel_pos = 0; // Will overflow 255 => 0
	int ghost_type_pause = 0;

	memset(wheel, 0x00, sizeof(wheel));
	while (wheel[wheel_pos] == NULL) {
		wheel[wheel_pos] = malloc(sizeof(rv_rgb_map));
		if (!wheel[wheel_pos]) {
			rv_printf(RV_LOG_NORMAL, "Error: Unable to allocate memory for wheel\n");
			return;
		}
		memset(wheel[wheel_pos], 0, sizeof(rv_rgb_map));
		wheel_pos++;
	}

	if (rv_init_evdev(0) != RV_SUCCESS) {
		rv_printf(RV_LOG_NORMAL, "Error: No event input device found\n");
		return;
	}

	while (1) {
		int k, j, i;

		rv_update_evdev();

		k = 0; while (rv_pressed_keys[k] != 0xff) {
			rv_schedule_impact(rv_pressed_keys[k], wheel, wheel_pos, 2, 4, rv_colors[1], rv_colors[2], rv_colors[3]);
			ghost_type_pause = 150; // ~5secs
			k++;
		}

		k = 0; while (rv_repeated_keys[k] != 0xff) {
			rv_schedule_impact(rv_repeated_keys[k], wheel, wheel_pos, 2, 4, rv_colors[1], rv_colors[2], rv_colors[3]);
			ghost_type_pause = 150;
			k++;
		}

		// Ghost typing on random keys
		if (!ghost_type_pause && (rand() % 8 == 0)) {
			unsigned char rkey = rand() >> 23;
			if (rkey < RV_NUM_KEYS && rv_neigh[rv_topo_model][rkey][0] != 0xff) {
				rv_schedule_impact(rkey, wheel, wheel_pos, 2, 4, rv_colors[4], rv_colors[5], rv_colors[6]);
			}
		}

		rv_send_led_map(wheel[wheel_pos]);

		rv_blend_to(wheel[wheel_pos], wheel[rv_wheel_offset(wheel_pos, 1)], rv_colors[0], 16);

		for (k = 0; k < RV_NUM_KEYS; k++) { wheel[wheel_pos]->key[k] = rv_color_off; };

		NEXT:
		wheel_pos++;
		if (ghost_type_pause) ghost_type_pause--;

		// Runs at ~30fps
		usleep(30000);
	}

}

void rv_fx_topo_keys() {
	rv_rgb_map map;
	rv_rgb red = { .r = 0x00ff, .g = 0x0000, .b =  0x0000 };
	rv_rgb grn = { .r = 0x0000, .g = 0x00ff, .b =  0x0000 };
	rv_rgb blu = { .r = 0x0000, .g = 0x0000, .b =  0x00ff };
	rv_rgb off = { .r = 0x0000, .g = 0x0000, .b =  0x0000 };

	unsigned char ev2rv[RV_MAX_EV_CODE+1];
	memset(ev2rv, 0xff, RV_MAX_EV_CODE+1);

	if (rv_init_evdev(1) != RV_SUCCESS) {
		rv_printf(RV_LOG_NORMAL, "Error: No event input device found\n");
		return;
	}

	rv_printf(RV_LOG_NORMAL, "Recording available keys. Please press the key illuminated\nin RED. It should turn green (This will not work for the FN key, you will have to skip it).\n");

	memset(&map, 0x00, sizeof(map));

	for (int key = 0; key < RV_NUM_KEYS; key++) {
		int stop_key = (key < 20) ? 96 : 1;

		rv_printf(RV_LOG_NORMAL, "Lighting up key %u (%s to skip) ... ", key, (stop_key == 1) ? "ESC" : "KPENTER");
		map.key[key] = red;

		while (1) {
			rv_send_led_map(&map);
			int ev_code = rv_get_evdev_keypress();
			if (ev_code) {
				if (ev_code != stop_key) {
					ev2rv[ev_code] = key;
					map.key[key] = grn;
					rv_printf(RV_LOG_NORMAL, "code %u (%s)\n", ev_code, rv_get_ev_keyname(ev_code));
				}
				else {
					map.key[key] = blu;
					rv_printf(RV_LOG_NORMAL, "skipped\n");
				}
				break;
			}
			usleep(30000);
		}
	}

	rv_printf(RV_LOG_NORMAL, "\nKeymap code:\n");
	for (int code = 0; code < RV_MAX_EV_CODE+1; code++) {
		if (code % 16 == 0) rv_printf(RV_LOG_NORMAL, "\n");
		rv_printf(RV_LOG_NORMAL, "0x%02hhx, ", ev2rv[code]);
	}
	rv_printf(RV_LOG_NORMAL, "\n\n");
}

void rv_fx_topo_neigh() {
	rv_rgb_map map;
	rv_rgb red = { .r = 0x00ff, .g = 0x0000, .b =  0x0000 };
	rv_rgb grn = { .r = 0x0000, .g = 0x00ff, .b =  0x0000 };
	rv_rgb off = { .r = 0x0000, .g = 0x0000, .b =  0x0000 };

	if (rv_init_evdev(1) != RV_SUCCESS) {
		rv_printf(RV_LOG_NORMAL, "Error: No event input device found\n");
		return;
	}

	rv_printf(RV_LOG_NORMAL,
	"Recording neighbor topology. Neighbors are keys that share part of an edge of their surface with the\n"
	"candidate key. Sharing a corner is NOT a neighbor. What counts is the surface of the key - meaning the\n"
	"space bar has the most neighbors.\n"
	"The gap between ESC and F1 and any larger gap elsewhere is a separation. This means that ESC and F1\n"
	"are NOT neighbors. Smaller gaps (like between F4 and F5) do NOT separate, so F4 and F5 are neighbors.\n\n"
	);

	memset(&map, 0x00, sizeof(map));

	for (int key = 0; key < RV_NUM_KEYS; key++) {
		int stop_key = (key < 50) ? 131 : 0;

		rv_printf(RV_LOG_NORMAL, "Lighting up key %u. Mark all neighbor keys. Press %s when done ... ", key, stop_key ? "KPENTER" : "ESC");
		map.key[key] = red;

		while (1) {
			rv_update_evdev();
			int k = 0; while (rv_pressed_keys[k] != 0xff) {
				if (rv_pressed_keys[k] == stop_key) goto NEXT_NEIGH;
				if (map.key[rv_pressed_keys[k]].r == 0x0000)
					map.key[rv_pressed_keys[k]] = (map.key[rv_pressed_keys[k]].g == 0x0000) ? grn : off;
				k++;
			}
			rv_send_led_map(&map);
			usleep(30000);
		}
		NEXT_NEIGH:
		for (int k = 0; k < RV_NUM_KEYS; k++) {
			if (map.key[k].g != 0x0000) {
				rv_printf(RV_LOG_NORMAL, "0x%02hhx, ", k);
			}
			map.key[k] = off;
		}
		rv_printf(RV_LOG_NORMAL, "\n");
	}
}

void rv_fx_topo_cols() {
	rv_rgb_map map;
	rv_rgb red = { .r = 0x00ff, .g = 0x0000, .b =  0x0000 };
	rv_rgb grn = { .r = 0x0000, .g = 0x00ff, .b =  0x0000 };
	rv_rgb off = { .r = 0x0000, .g = 0x0000, .b =  0x0000 };

	if (rv_init_evdev(1) != RV_SUCCESS) {
		rv_printf(RV_LOG_NORMAL, "Error: No event input device found\n");
		return;
	}

	rv_printf(RV_LOG_NORMAL, "Recording cols topology. Keyboard has %u columns (#1-#%u, from left).\n", RV_NUM_COLS, RV_NUM_COLS);

	memset(&map, 0x00, sizeof(map));
	for (int colnum = 0; colnum < RV_NUM_COLS; colnum++) {
		int stop_key = (colnum < 10) ? 131 : 0;
		rv_printf(RV_LOG_NORMAL, "Activate all keys for column #%u. Press %s when done.\n", colnum+1, stop_key ? "KPENTER" : "ESC" );
		while (1) {
			rv_update_evdev();
			int k = 0; while (rv_pressed_keys[k] != 0xff) {
				if (rv_pressed_keys[k] == stop_key) goto NEXT_COL;
				if (map.key[rv_pressed_keys[k]].r == 0x0000)
					map.key[rv_pressed_keys[k]] = (map.key[rv_pressed_keys[k]].g == 0x0000) ? grn : off;
				k++;
			}
			rv_send_led_map(&map);
			usleep(30000);
		}
		NEXT_COL:
		for (int k = 0; k < RV_NUM_KEYS; k++) {
			if (map.key[k].g != 0x0000) {
				rv_printf(RV_LOG_NORMAL, "0x%02hhx, ", k);
				map.key[k] = red;
			}
		}
		rv_printf(RV_LOG_NORMAL, "\n");
	}
}


void rv_fx_topo_rows() {
	rv_rgb_map map;
	rv_rgb red = { .r = 0x00ff, .g = 0x0000, .b =  0x0000 };
	rv_rgb grn = { .r = 0x0000, .g = 0x00ff, .b =  0x0000 };
	rv_rgb off = { .r = 0x0000, .g = 0x0000, .b =  0x0000 };

	if (rv_init_evdev(1) != RV_SUCCESS) {
		rv_printf(RV_LOG_NORMAL, "Error: No event input device found\n");
		return;
	}

	rv_printf(RV_LOG_NORMAL, "Recording rows topology. Keyboard has %u rows (#1-#%u, from top).\n", RV_NUM_ROWS, RV_NUM_ROWS);

	memset(&map, 0x00, sizeof(map));
	for (int rownum = 0; rownum < RV_NUM_ROWS; rownum++) {
		int stop_key = (rownum < 3) ? 131 : 0;
		rv_printf(RV_LOG_NORMAL, "Activate all keys for row #%u. Press %s when done.\n", rownum+1, stop_key ? "KPENTER" : "ESC" );
		while (1) {
			rv_update_evdev();
			int k = 0; while (rv_pressed_keys[k] != 0xff) {
				if (rv_pressed_keys[k] == stop_key) goto NEXT_ROW;
				if (map.key[rv_pressed_keys[k]].r == 0x0000)
					map.key[rv_pressed_keys[k]] = (map.key[rv_pressed_keys[k]].g == 0x0000) ? grn : off;
				k++;
			}
			rv_send_led_map(&map);
			usleep(30000);
		}
		NEXT_ROW:
		for (int k = 0; k < RV_NUM_KEYS; k++) {
			if (map.key[k].g != 0x0000) {
				rv_printf(RV_LOG_NORMAL, "0x%02hhx, ", k);
				map.key[k] = red;
			}
		}
		rv_printf(RV_LOG_NORMAL, "\n");
	}
}
#pragma once

const int nMinOrder = 3;
const int nMaxOrder = 20;

enum eCOMMANDS {
	eNone = 0x0000,			/* no command specified by user */
	eSelect = 0x0001,		/* select command */
	eInsert = 0x0002,		/* insert command */
	eDelete = 0x0004,		/* delete command */
};
struct OPTIONS {
	uint32_t nOrder;		/* order of B+Tree */
	eCOMMANDS eCommand;		/* command (eCOMMANDS) */
	int nKEY;				/* primary key (for lookup and delete) */
};

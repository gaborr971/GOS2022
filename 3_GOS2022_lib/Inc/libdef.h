/*
 * libdef.h
 *
 *  Created on: Feb 6, 2024
 *      Author: Gabor
 */

#ifndef LIBDEF_H_
#define LIBDEF_H_

#include <gos.h>

#define LIB_NAME_LENGTH        ( 48 )
#define LIB_DESCRIPTION_LENGTH ( 64 )
#define LIB_AUTHOR_LENGTH      ( 32 )

typedef struct
{
	u16_t 	   major;
	u16_t	   minor;
	u16_t 	   build;
	gos_time_t date;
	char_t     name        [LIB_NAME_LENGTH];
	char_t     description [LIB_DESCRIPTION_LENGTH];
	char_t     author      [LIB_AUTHOR_LENGTH];
}lib_version_t;

#endif /* LIBDEF_H_ */

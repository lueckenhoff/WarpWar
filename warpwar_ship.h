/* FILEHEADER */

#ifndef _INC_WARPWAR_SHIP_H_
#define _INC_WARPWAR_SHIP_H_

#include <stdint.h>

typedef int warpwar_bool_t;  /* boolean */

typedef struct {
    /* use this terse terminology to stay aligned with the 1977 rulebook */
    unsigned int        pd; /* Power Drive */
    unsigned int        b;  /* Beams */
    unsigned int        s;  /* Shields */
    unsigned int        t;  /* Tubes */
    unsigned int        m;  /* Missiles */
    unsigned int        sr; /* Systemship Racks */
} warpwar_ship_stats_t;

enum {
    FLAG_WHITE     = 0x1,
    FLAG_BLUE      = 0x2,
    FLAG_WARPSHIP  = 0x4
};

typedef enum {
    ALLEGIANCE_WHITE = 1,
    ALLEGIANCE_BLUE = 2
}  warpwar_allegiance_t;

struct warpwar_ship_t {
    unsigned int            idnum;
    unsigned int            flags;
    unsigned int            tech_level;
    unsigned int            current_pd_setting;
    unsigned int            current_tactic;
    unsigned int            current_result;
    unsigned int            current_location_hex_id;
    warpwar_ship_stats_t    stats_full;
    warpwar_ship_stats_t    stats_current;
    struct warpware_ship_t *systemship_list;
    struct warpware_ship_t *next;
};


extern struct warpwar_ship_t * warpwar_ship_build
    (
    warpwar_bool_t          isa_warpship,
    warpwar_allegiance_t    allegiance   /* WHITE or BLUE */,
    unsigned int            pd /* Power Drive */,
    unsigned int            b  /* Beams */,
    unsigned int            s  /* Shields */,
    unsigned int            t  /* Tubes */,
    unsigned int            m  /* Missiles */,
    unsigned int            sr /* Systemship Racks */,
    unsigned int *          ptr_credits_in_bank,
    warpwar_bool_t          verbose
    );

extern void warpwar_print_one_ship(struct warpwar_ship_t * ship);
extern void warpwar_print_all_ships(void);
#endif /* not _INC_WARPWAR_SHIP_H_ */

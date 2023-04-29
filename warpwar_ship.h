/* FILEHEADER */

#ifndef _INC_WARPWAR_SHIP_H_
#define _INC_WARPWAR_SHIP_H_

#include <stdint.h>

typedef int warpwar_bool_t;  /* boolean */

typedef struct {
    /* terse terminology to align with the 1977 rulebook */
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
    char                    name[8];
    unsigned int            target_idnum;
    unsigned int            flags;
    unsigned int            tech_level;
    unsigned int            current_tactic;
    unsigned int            current_result;
    unsigned int            current_location_hex_id;
    warpwar_ship_stats_t    stats_full;
    warpwar_ship_stats_t    stats_effective;
    warpwar_ship_stats_t    current_orders;
    unsigned int *          missile_pd_settings;
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

extern char * warpwar_ship_name_get(unsigned int idnum);

extern int warpwar_ship_issue_orders
    (
    unsigned int            ship_idnum,
    unsigned int            target_idnum,
    int                     tactic,
    warpwar_ship_stats_t *  orders,
    warpwar_bool_t          verbose
    );

extern void warpwar_print_one_ship(struct warpwar_ship_t * ship);
extern void warpwar_print_all_ships(void);
#endif /* not _INC_WARPWAR_SHIP_H_ */

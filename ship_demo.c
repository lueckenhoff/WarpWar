
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>    /* for strcasecmp */

#include "warpwar_ship.h"

int main (int argc, char **argv)
{
    struct warpwar_ship_t *ptr_ship;
    warpwar_bool_t isa_warpship = 1;
    unsigned int idnum = 1;
    unsigned int pd, b, s, t, m, sr;
    unsigned int credits_in_bank = 10000;

    ptr_ship = warpwar_ship_build(isa_warpship = 1, ALLEGIANCE_BLUE,
        pd = 10, b = 5, s = 5, t = 1, m = 3, sr = 0, &credits_in_bank);
    ptr_ship = warpwar_ship_build(isa_warpship = 1, ALLEGIANCE_WHITE,
        pd = 8, b = 6, s = 6, t = 0, m = 0, sr = 0, &credits_in_bank);
    ptr_ship = warpwar_ship_build(isa_warpship = 0, ALLEGIANCE_BLUE,
        pd = 2, b = 0, s = 0, t = 1, m = 6, sr = 0, &credits_in_bank);
    warpwar_print_all_ships();
    return 0;
}


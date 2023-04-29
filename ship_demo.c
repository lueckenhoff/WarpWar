
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>    /* for strcasecmp */

#include "warpwar_crt.h"
#include "warpwar_ship.h"

int main (int argc, char **argv)
{
    struct warpwar_ship_t *ptr_ship;
    warpwar_bool_t isa_warpship = 1;
    warpwar_bool_t verbose = 0;
    unsigned int idnum = 1;
    unsigned int pd, b, s, t, m, sr;
    unsigned int credits_in_bank = 10000;
    warpwar_ship_stats_t   orders;
    int rval;

    ptr_ship = warpwar_ship_build(isa_warpship = 1, ALLEGIANCE_BLUE,
        pd = 10, b = 5, s = 5, t = 1, m = 3, sr = 0,
        &credits_in_bank, verbose);
    ptr_ship = warpwar_ship_build(isa_warpship = 1, ALLEGIANCE_WHITE,
        pd = 8, b = 6, s = 6, t = 0, m = 0, sr = 0,
        &credits_in_bank, verbose);
    ptr_ship = warpwar_ship_build(isa_warpship = 0, ALLEGIANCE_BLUE,
        pd = 2, b = 0, s = 0, t = 1, m = 6, sr = 0,
        &credits_in_bank, verbose);
    warpwar_print_all_ships();

    /* have ship id=1 attack ship id=2 with its beam */
    memset(&orders, 0, sizeof(orders));
    orders.pd = 2;
    orders.b  = 5;
    orders.s  = 3;
    rval = warpwar_ship_issue_orders(1, 2, TACTIC_ATTACK, &orders, 1);
    if (rval < 0)
    {
        printf("warpwar_ship_issue_orders returned %d\n", rval);
    }

    /* have ship id=2 attack ship id=1 with its beam */
    memset(&orders, 0, sizeof(orders));
    orders.pd = 2;
    orders.b  = 4;
    orders.s  = 2;
    rval = warpwar_ship_issue_orders(2, 1, TACTIC_ATTACK, &orders, 1);
    if (rval < 0)
    {
        printf("warpwar_ship_issue_orders returned %d\n", rval);
    }

    return 0;
}


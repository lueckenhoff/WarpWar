/* FILEHEADER */

#include <stdio.h>
#include <stdlib.h>     /* for malloc */
#include <strings.h>    /* for bzero */
#include <assert.h>     /* for assert */
#include "warpwar_ship.h"
#include "warpwar_crt.h"
#include "warpwar_combat.h"

static int warpwar_resolve_one_attack (struct warpwar_ship_t * ship)
{
    struct warpwar_ship_t *target_ship;
    int rval;
    int result;
    int be_verbose = 0;
    int damage;

    assert(ship);
    printf("warpwar_resolve_one_attack: %s\n", ship->name);
    if (NO_SHIP == ship->target_idnum)
    {
        printf("warpwar_resolve_one_attack: %s no ship targeted, returning\n", ship->name);
        return 0;
    }
    target_ship = warpwar_ship_lookup_by_idnum(ship->target_idnum);
    if (!target_ship)
    {
        printf("warpwar_resolve_one_attack: target ship %u does not exist!\n", ship->target_idnum);
        return -1;
    }
    rval = combat_result(warpwar_ship_tactic_get(ship->idnum),
                         warpwar_ship_tactic_get(target_ship->idnum),
                         warpwar_ship_pd_get(ship->idnum)
                                 - warpwar_ship_pd_get(target_ship->idnum),
                         &result, be_verbose = 0);
    printf("%s targeting %s result: %s\n", ship->name, target_ship->name,
           result_to_str(result));
    damage = result_to_damage(result, ship->current_orders.b, target_ship->current_orders.s);
    printf("%s targeting %s damage=%d\n", ship->name, target_ship->name, damage);
    target_ship->current_damage += damage;
    printf("%s current_damage := %u\n", target_ship->name,
           target_ship->current_damage);
    return 0;
}



void warpwar_resolve_all_attacks (void)
{
    struct warpwar_ship_t * ship;

    for (ship = gbl_ship_list; ship; ship = ship->next)
    {
        warpwar_resolve_one_attack(ship);
    }
}



warpwar_bool_t
warpwar_ship_isdead (struct warpwar_ship_t * ship)
{
    warpwar_ship_stats_t *stats;
    assert(ship);
    stats = &(ship->stats_effective);
    if ((0 == stats->pd)
        && (0 == stats->b)
        && (0 == stats->s)
        && (0 == stats->t)
        && (0 == stats->m)
        && (0 == stats->sr))
    {
        return 1;
    }
    return 0;
}



static void warpwar_resolve_one_ships_damage (struct warpwar_ship_t * ship)
{
    struct warpwar_ship_t *target_ship;
    int rval;
    int result;
    int be_verbose = 0;
    int damage;

    assert(ship);
    while ((ship->current_damage > 0) && (!warpwar_ship_isdead(ship)))
    {
        printf("warpwar_resolve_one_ships_damage: %s current_damage=%u\n", ship->name, ship->current_damage);
        warpwar_print_one_ship(ship);
        if ((ship->stats_effective.sr >= ship->stats_effective.pd)
            && (ship->stats_effective.sr >= ship->stats_effective.b)
            && (ship->stats_effective.sr >= ship->stats_effective.s)
            && (ship->stats_effective.sr >= ship->stats_effective.t)
            && (ship->stats_effective.sr >= ship->stats_effective.m/3))
        {
            ship->stats_effective.sr -= 1;
            ship->current_damage -= 1;
        }
        else if ((ship->stats_effective.m/3 >= ship->stats_effective.pd)
            && (ship->stats_effective.m/3 >= ship->stats_effective.b)
            && (ship->stats_effective.m/3 >= ship->stats_effective.s)
            && (ship->stats_effective.m/3 >= ship->stats_effective.t)
            && (ship->stats_effective.m/3 >= ship->stats_effective.sr))
        {
            if (ship->stats_effective.m > 3)
            {
                ship->stats_effective.m -= 3;
            }
            else
            {
                ship->stats_effective.m = 0;
            }
            ship->current_damage -= 1;
        }
        else if ((ship->stats_effective.t >= ship->stats_effective.pd)
            && (ship->stats_effective.t >= ship->stats_effective.b)
            && (ship->stats_effective.t >= ship->stats_effective.s)
            && (ship->stats_effective.t >= ship->stats_effective.m/3)
            && (ship->stats_effective.t >= ship->stats_effective.sr))
        {
            ship->stats_effective.s -= 1;
            ship->current_damage -= 1;
        }
        else if ((ship->stats_effective.b >= ship->stats_effective.pd)
            && (ship->stats_effective.b >= ship->stats_effective.s)
            && (ship->stats_effective.b >= ship->stats_effective.t)
            && (ship->stats_effective.b >= ship->stats_effective.m/3)
            && (ship->stats_effective.b >= ship->stats_effective.sr))
        {
            ship->stats_effective.b -= 1;
            ship->current_damage -= 1;
        }
        else if ((ship->stats_effective.s >= ship->stats_effective.pd)
            && (ship->stats_effective.s >= ship->stats_effective.b)
            && (ship->stats_effective.s >= ship->stats_effective.t)
            && (ship->stats_effective.s >= ship->stats_effective.m/3)
            && (ship->stats_effective.s >= ship->stats_effective.sr))
        {
            ship->stats_effective.s -= 1;
            ship->current_damage -= 1;
        }
        else if ((ship->stats_effective.pd > ship->stats_effective.b)
            && (ship->stats_effective.pd > ship->stats_effective.s)
            && (ship->stats_effective.pd > ship->stats_effective.t)
            && (ship->stats_effective.pd > ship->stats_effective.m/3)
            && (ship->stats_effective.pd > ship->stats_effective.sr))
        {
            ship->stats_effective.pd -= 1;
            ship->current_damage -= 1;
        }
    }
    warpwar_print_one_ship(ship);
    printf("warpwar_resolve_one_ships_damage: exit\n");
    assert(warpwar_ship_isdead(ship) || (0 == ship->current_damage));
}



void warpwar_resolve_all_ships_damages (void)
{
    struct warpwar_ship_t * ship;

    for (ship = gbl_ship_list; ship; ship = ship->next)
    {
        warpwar_resolve_one_ships_damage(ship);
    }
}




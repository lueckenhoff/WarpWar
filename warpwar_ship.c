/* FILEHEADER */

#include <stdio.h>
#include <stdlib.h>     /* for malloc */
#include <strings.h>    /* for bzero */
#include <assert.h>     /* for assert */
#include "warpwar_ship.h"

struct warpwar_ship_t * gbl_ship_list = NULL;
unsigned gbl_ship_idnum = 0;

struct warpwar_ship_t * warpwar_ship_build
    (
    warpwar_bool_t          isa_warpship,
    warpwar_allegiance_t    allegiance   /* WHITE or BLUE */,
    unsigned int            pd /* Power Drive */,
    unsigned int            b  /* Beams */,
    unsigned int            s  /* Shields */,
    unsigned int            t  /* Tubes */,
    unsigned int            m  /* Missiles */,
    unsigned int            sr /* Systemship Racks */,
    unsigned int *          ptr_credits_in_bank
    )
{
    struct warpwar_ship_t *newship;
    unsigned int    cost;

    printf("DBG: warpwar_ship_build: entry\n");
    fflush(stdout);
    if (!ptr_credits_in_bank)
    {
        printf("DBG: warpwar_ship_build: error: passed a NULL credits_in_bank ptr\n");
        fflush(stdout);
        return NULL;
    }
    printf("DBG: warpwar_ship_build: calculating cost\n");
    fflush(stdout);
    cost = pd + b + s + t + m/3 + sr;

    printf("DBG: warpwar_ship_build: finished calculating cost\n");
    fflush(stdout);
    printf("DBG: warpwar_ship_build: about to check cost (%u) <= ", cost);
    fflush(stdout);
    printf("credits_in_bank (%u)\n", *ptr_credits_in_bank);
    fflush(stdout);
    if (cost > *ptr_credits_in_bank)
    {
        return NULL;
    }

    printf("DBG: warpwar_ship_build: about to allocate mem\n");
    fflush(stdout);
    newship = malloc(sizeof(struct warpwar_ship_t));
    if (!newship)
    {
        return NULL;
    }
    printf("DBG: warpwar_ship_build: about zero newly allocated mem\n");
    fflush(stdout);
    bzero(newship, sizeof(struct warpwar_ship_t));
    printf("DBG: warpwar_ship_build: about assign ID num\n");
    fflush(stdout);
    newship->idnum = ++gbl_ship_idnum;
    switch (allegiance)
    {
    case ALLEGIANCE_WHITE:
        newship->flags |= FLAG_WHITE;
        break;
    case ALLEGIANCE_BLUE:
        newship->flags |= FLAG_BLUE;
        break;
    default:
        free(newship);
        return NULL;
    }
    printf("DBG: warpwar_ship_build: about to deduct cost\n");
    fflush(stdout);
    *ptr_credits_in_bank -= cost;
    if (isa_warpship)
    {
        newship->flags |= FLAG_WARPSHIP;
    }
    newship->full_stats.pd = pd;
    newship->full_stats.b = b;
    newship->full_stats.s = s;
    newship->full_stats.t = t;
    newship->full_stats.m = m;
    newship->full_stats.sr = sr;
    printf("DBG: warpwar_ship_build: about to copy full_stats to curr_stats\n");
    fflush(stdout);
    newship->curr_stats = newship->full_stats;
    printf("DBG: XXX about to add new ship to list\n");
    fflush(stdout);
    if (gbl_ship_list)
    {
        newship->next = gbl_ship_list;
    }
    gbl_ship_list = newship;
    return newship;
}

void warpwar_print_one_ship (struct warpwar_ship_t * ship)
{
    assert(ship);
    if (ship->flags & FLAG_WHITE)
    {
        printf("(white) ");
    }
    if (ship->flags & FLAG_BLUE)
    {
        printf("(blue) ");
    }
    if (ship->flags & FLAG_WARPSHIP)
    {
        printf("W");
    }
    else
    {
        printf("S");
    }
    printf("%u ", ship->idnum);
    printf(" pd %u, b %u, s %u, t %u, m %u, sr %u\n",
        ship->curr_stats.pd,
        ship->curr_stats.b,
        ship->curr_stats.s,
        ship->curr_stats.t,
        ship->curr_stats.m,
        ship->curr_stats.sr);
}

void warpwar_print_all_ships (void)
{
    struct warpwar_ship_t * ship;

    ship = gbl_ship_list;

    printf("All ships:\n");
    while (ship)
    {
        warpwar_print_one_ship(ship);
        ship = ship->next;
    }
}


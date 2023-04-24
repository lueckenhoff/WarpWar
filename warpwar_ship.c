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
    unsigned int *          ptr_credits_in_bank,
    warpwar_bool_t          verbose
    )
{
    struct warpwar_ship_t *newship;
    unsigned int    cost;

    if (verbose)
    {
        printf("DBG: warpwar_ship_build: entry\n");
        fflush(stdout);
    }
    if (!ptr_credits_in_bank)
    {
        printf("DBG: warpwar_ship_build: error: passed a NULL credits_in_bank ptr\n");
        fflush(stdout);
        return NULL;
    }
    if (verbose)
    {
        printf("DBG: warpwar_ship_build: calculating cost\n");
        fflush(stdout);
    }
    cost = pd + b + s + t + m/3 + sr;

    if (verbose)
    {
        printf("DBG: warpwar_ship_build: finished calculating cost\n");
        fflush(stdout);
        printf("DBG: warpwar_ship_build: about to check cost (%u) <= ", cost);
        fflush(stdout);
        printf("credits_in_bank (%u)\n", *ptr_credits_in_bank);
        fflush(stdout);
    }
    if (cost > *ptr_credits_in_bank)
    {
        return NULL;
    }

    if (verbose)
    {
        printf("DBG: warpwar_ship_build: about to allocate mem\n");
        fflush(stdout);
    }
    newship = malloc(sizeof(struct warpwar_ship_t));
    if (!newship)
    {
        return NULL;
    }
    if (verbose)
    {
        printf("DBG: warpwar_ship_build: about zero newly allocated mem\n");
        fflush(stdout);
    }
    bzero(newship, sizeof(struct warpwar_ship_t));
    if (verbose)
    {
        printf("DBG: warpwar_ship_build: about assign ID num\n");
        fflush(stdout);
    }
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
    if (verbose)
    {
        printf("DBG: warpwar_ship_build: about to deduct cost\n");
        fflush(stdout);
    }
    *ptr_credits_in_bank -= cost;
    if (isa_warpship)
    {
        newship->flags |= FLAG_WARPSHIP;
    }
    newship->stats_full.pd = pd;
    newship->stats_full.b = b;
    newship->stats_full.s = s;
    newship->stats_full.t = t;
    newship->stats_full.m = m;
    newship->stats_full.sr = sr;
    if (verbose)
    {
        printf("DBG: warpwar_ship_build: about to copy stats_full to stats_current\n");
        fflush(stdout);
    }
    newship->stats_current = newship->stats_full;
    if (verbose)
    {
        printf("DBG: warpwar_ship_build about to add new ship to list\n");
        fflush(stdout);
    }
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
        ship->stats_current.pd,
        ship->stats_current.b,
        ship->stats_current.s,
        ship->stats_current.t,
        ship->stats_current.m,
        ship->stats_current.sr);
}

void warpwar_print_all_ships (void)
{
    struct warpwar_ship_t * ship;

    printf("All ships:\n");
    for (ship = gbl_ship_list; ship; ship = ship->next)
    {
        warpwar_print_one_ship(ship);
    }
}


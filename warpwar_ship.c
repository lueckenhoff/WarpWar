/* FILEHEADER */

#include <stdio.h>
#include <stdlib.h>     /* for malloc */
#include <string.h>     /* for memcpy */
#include <strings.h>    /* for bzero */
#include <assert.h>     /* for assert */
#include "warpwar_ship.h"
#include "warpwar_crt.h"

struct warpwar_ship_t * gbl_ship_list = NULL;
unsigned gbl_ship_idnum = NO_SHIP;



struct warpwar_ship_t *
warpwar_ship_build
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
    if (m % 3)
    {
        printf("Error: warpwar_ship_build: must buy missiles in units of 3\n");
        return NULL;
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
        fprintf(stderr, "warpwar_ship_build: Error! cannot allocate memory!\n");
        return NULL;
    }
    if (verbose)
    {
        printf("DBG: warpwar_ship_build: about zero newly allocated mem\n");
        fflush(stdout);
    }
    bzero(newship, sizeof(struct warpwar_ship_t));
    if (t > 0)
    {
        newship->missile_pd_settings =
            (unsigned int *) malloc(sizeof(unsigned int) * t);
        if (!newship->missile_pd_settings)
        {
            fprintf(stderr, "warpwar_ship_build: Error! cannot allocate memory for missiles\n");
            free(newship);
            return NULL;
        }
    }
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
        snprintf(newship->name, sizeof(newship->name), "W%u", newship->idnum);
    }
    else
    {
        snprintf(newship->name, sizeof(newship->name), "S%u", newship->idnum);
    }
    newship->stats_full.pd = pd;
    newship->stats_full.b = b;
    newship->stats_full.s = s;
    newship->stats_full.t = t;
    newship->stats_full.m = m;
    newship->stats_full.sr = sr;
    if (verbose)
    {
        printf("DBG: warpwar_ship_build: about to copy stats_full to stats_effective\n");
        fflush(stdout);
    }
    newship->stats_effective = newship->stats_full;
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



int warpwar_ship_decommission (struct warpwar_ship_t *ship)
{
    struct warpwar_ship_t *pred;

    if (NULL == ship)
    {
	return -1;
    }
    if (gbl_ship_list == ship)
    {
	gbl_ship_list = ship->next;
    }
    else
    {
	for (pred = gbl_ship_list; pred; pred = ship->next)
	{
	    if (pred->next == ship)
	    {
		pred->next = ship->next;
		break;
	    }
	}
    }
    if (ship->missile_pd_settings)
    {
	free(ship->missile_pd_settings);
    }
    free(ship);
    return 0;
}



struct warpwar_ship_t *
warpwar_ship_lookup_by_idnum (unsigned int idnum)
{
    struct warpwar_ship_t * ship;

    for (ship = gbl_ship_list; ship; ship = ship->next)
    {
        if (ship->idnum == idnum)
        {
            return (ship);
        }
    }
    return NULL;
}



char * warpwar_ship_name_get (unsigned int idnum)
{
    struct warpwar_ship_t *ship = warpwar_ship_lookup_by_idnum(idnum);

    if (ship)
    {
        return ship->name;
    }
    return "NULL";
}



int warpwar_ship_tactic_get (unsigned int idnum)
{
    struct warpwar_ship_t *ship = warpwar_ship_lookup_by_idnum(idnum);

    if (ship)
    {
        return ship->current_tactic;
    }
    return TACTIC_BOGUS;
}



int warpwar_ship_pd_get (unsigned int idnum)
{
    struct warpwar_ship_t *ship = warpwar_ship_lookup_by_idnum(idnum);

    if (ship)
    {
        return ship->current_orders.pd;
    }
    return 0;
}



int
warpwar_ship_issue_orders
    (
    unsigned int            ship_idnum,
    unsigned int            target_idnum,
    int                     tactic,
    warpwar_ship_stats_t *  orders,
    warpwar_bool_t          verbose
    )
{
    struct warpwar_ship_t *ship;
    struct warpwar_ship_t *target_ship;
    unsigned int sum;

    /* Step 1: is given ship ID valid? Lookup subject ship by IDnum */
    ship = warpwar_ship_lookup_by_idnum(ship_idnum);
    if (!ship)
    {
        if (verbose)
        {
            printf("warpwar_ship_issue_orders: invalid ship ID %u\n", ship_idnum);
        }
        return -1;
    }

    /* Step 2: is given target ID valid? Lookup target ship by IDnum */
    target_ship = warpwar_ship_lookup_by_idnum(target_idnum);
    if (!target_ship)
    {
        if (verbose)
        {
            printf("warpwar_ship_issue_orders: invalid target ID %u\n", target_idnum);
        }
        return -1;
    }

    /* Step 3: is given tactic valid? Must be one of {Attack,Dodge,Retreat} */
    if (   (TACTIC_ATTACK  != tactic)
        && (TACTIC_DODGE   != tactic)
        && (TACTIC_RETREAT != tactic))
    {
        if (verbose)
        {
            printf("warpwar_ship_issue_orders: invalid tactic %d\n", tactic);
        }
        return -1;
    }
   
    /* Step 4: are given power settings valid?                  */
    /* a) Orders cannot exceed effective stats                  */
    if (   (orders->pd > ship->stats_effective.pd) 
        || (orders->b > ship->stats_effective.b) 
        || (orders->s > ship->stats_effective.s) 
        || (orders->t > ship->stats_effective.t))
    {
        if (verbose)
        {
            printf("warpwar_ship_issue_orders: orders for %s exceed effective stats! (4a)\n", ship->name);
        }
        return -1;
    }

    /* b) Sum of (beams,shields, tubes, SR and ordered drive,   */
    /*    must be less than or equal to effective PD            */
    sum = orders->pd + orders->b + orders->s + orders->t;
    if (sum > ship->stats_effective.pd)
    {
        if (verbose)
        {
            printf("warpwar_ship_issue_orders: orders for %s exceed effective PD! (4b)\n", ship->name);
        }
        return -1;
    }

    if (verbose)
    {
        printf("%s %s target=%s settings: pd=%u b=%u s=%u t=%u\n",
                ship->name, tactic_to_str(tactic),
                target_ship->name,
                orders->pd, orders->b, orders->s, orders->t);
    }

     /* Step 5: copy the orders into the ship (will resolve it separately) */
    ship->current_tactic = tactic;
    ship->target_idnum = target_idnum;
    memcpy(&ship->current_orders, orders, sizeof(warpwar_ship_stats_t));

    return 0;       /* signal success */
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
        ship->stats_effective.pd,
        ship->stats_effective.b,
        ship->stats_effective.s,
        ship->stats_effective.t,
        ship->stats_effective.m,
        ship->stats_effective.sr);
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




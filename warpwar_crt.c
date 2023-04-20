/* FILEHEADER */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>    /* for strcasecmp */

#include "warpwar_crt.h"

char * result_to_str (int result)
{
    switch (result)
    {
    case RESULT_MISS:
        return "miss   ";
    case RESULT_HIT:
        return "hit    ";
    case RESULT_HIT_PLUS1:
        return "hit+1  ";
    case RESULT_HIT_PLUS2:
        return "hit+2  ";
    case RESULT_ESCAPES:
        return "escapes";
    default:
        return " ????? ";
    }
}

int
combat_result (int firing_ships_tactic, int target_ships_tactic, int drive_diff, int *ptr_result, int verbose)
{
    int status = 0;
    int row, column;

    const int result_tbl [14][3] =
    {
/* Firing                 -------------- Target Ship Tactics -------------- */
/* Ship    Drive      */
/* Tactics Difference     Attack            Dodge             Retreat */
/* -------+--------------+-----------------+-----------------+--------*/
/* Attack  -3 or less */ {RESULT_MISS,      RESULT_MISS,      RESULT_ESCAPES},
/* Attack  -1, -2     */ {RESULT_HIT,       RESULT_MISS,      RESULT_ESCAPES},
/* Attack  0, +1      */ {RESULT_HIT_PLUS2, RESULT_MISS,      RESULT_MISS   },
/* Attack  +2         */ {RESULT_HIT_PLUS1, RESULT_HIT_PLUS1, RESULT_MISS   },
/* Attack  +3, +4     */ {RESULT_MISS,      RESULT_HIT,       RESULT_HIT    },
/* Attack  +5 or more */ {RESULT_MISS,      RESULT_MISS,      RESULT_MISS   },

/* Dodge   -4 or less */ {RESULT_MISS,      RESULT_MISS,      RESULT_ESCAPES},
/* Dodge   -2, -3     */ {RESULT_MISS,      RESULT_HIT,       RESULT_ESCAPES},
/* Dodge   0, -1      */ {RESULT_HIT,       RESULT_HIT,       RESULT_ESCAPES},
/* Dodge   +1, +2     */ {RESULT_HIT,       RESULT_MISS,      RESULT_ESCAPES},
/* Dodge   +3 or more */ {RESULT_MISS,      RESULT_MISS,      RESULT_ESCAPES},

/* Retreat -2 or less */ {RESULT_MISS,      RESULT_MISS,      RESULT_ESCAPES},
/* Retreat -1, 0      */ {RESULT_HIT,       RESULT_MISS,      RESULT_ESCAPES},
/* Retreat +1 or more */ {RESULT_MISS,      RESULT_MISS,      RESULT_ESCAPES},
    };

    if (verbose)
    {
        printf("DBG: combat_result (firing_ships_tactic=%d, target_ships_tactic=%d, drive_diff=%d, ptr_result=%p)\n", firing_ships_tactic, target_ships_tactic, drive_diff, ptr_result);
    }

    if (!ptr_result)
    {
        return -1;
    }

    /* figure out row and column to read from the table */
    switch (firing_ships_tactic)
    {
    case TACTIC_ATTACK:
        if (drive_diff <= -3)
        {
            row = 0;
        }
        else if ((-1 == drive_diff) || (-2 == drive_diff))
        {
            row = 1;
        }
        else if ((0 == drive_diff) || (1 == drive_diff))
        {
            row = 2;
        }
        else if (2 == drive_diff)
        {
            row = 3;
        }
        else if ((3 == drive_diff) || (4 == drive_diff))
        {
            row = 4;
        }
        else
        {
            row = 5;
        }
        break;

    case TACTIC_DODGE:
        if (drive_diff <= -4)
        {
            row = 6;
        }
        else if ((drive_diff == -2) || (drive_diff == -3))
        {
            row = 7;
        }
        else if ((drive_diff == 0) || (drive_diff == -1))
        {
            row = 8;
        }
        else if ((drive_diff == 1) || (drive_diff == 2))
        {
            row = 9;
        }
        else
        {
            row = 10;
        }
        break;

    case TACTIC_RETREAT:
        if (drive_diff <= -2)
        {
            row = 11;
        }
        else if ((drive_diff == 0) || (drive_diff == -1))
        {
            row = 12;
        }
        else
        {
            row = 13;
        }
        break;
    default:
        return -1;
    }

    switch (target_ships_tactic)
    {
    case TACTIC_ATTACK:
        column = 0;
        break;
    case TACTIC_DODGE:
        column = 1;
        break;
    case TACTIC_RETREAT:
        column = 2;
        break;
    default:
        return -1;
    }

    if (verbose)
    {
        int ix, jx;

        printf("DBG: row=%d,column=%d\n", row, column);
        printf("DBG: result_tbl[row][column]=%d\n", result_tbl[row][column]);
        for (ix = 0 ; ix < 14; ix++)
        {
            printf("DBG: result_tbl[%2d]: ", ix);
            for (jx = 0 ; jx < 3; jx++)
            {
                printf("%s ", result_to_str(result_tbl[ix][jx]));
            }
            printf("\n");
        }
    }

    *ptr_result = result_tbl[row][column];
    return 0;
}

int string_to_tactic (char *str)
{
    if (0 == strcasecmp(str, "attack"))
    {
        return TACTIC_ATTACK;
    }
    if (0 == strcasecmp(str, "dodge"))
    {
        return TACTIC_DODGE;
    }
    if (0 == strcasecmp(str, "retreat"))
    {
        return TACTIC_RETREAT;
    }
    return TACTIC_BOGUS;
}


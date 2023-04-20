
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>    /* for strcasecmp */

#include "warpwar_crt.h"

int main (int argc, char **argv)
{
    char *firing_ships_tactic_str;
    char *target_ships_tactic_str;
    int drive_diff;
    int firing_ships_tactic;
    int target_ships_tactic;
    int result;
    int status;
    int be_verbose = 0;

    printf("argc=%d\n", argc);
    if  (argc < 4)
    {
        printf("usage: %s firing_tactic target_tactic drive_diff\n", argv[0]);
        return -1;
    }
    firing_ships_tactic_str = argv[1];
    firing_ships_tactic = string_to_tactic(firing_ships_tactic_str);
    if (TACTIC_BOGUS == firing_ships_tactic)
    {
        printf("unknown tactic '%s'\n", firing_ships_tactic_str);
    }
    target_ships_tactic_str = argv[2];
    target_ships_tactic = string_to_tactic(target_ships_tactic_str);
    if (TACTIC_BOGUS == target_ships_tactic)
    {
        printf("unknown tactic '%s'\n", target_ships_tactic_str);
    }
    drive_diff = atoi(argv[3]);
    printf("firing_ships_tactic: %s (%d)\n",
           firing_ships_tactic_str, firing_ships_tactic);
    printf("target_ships_tactic: %s (%d)\n",
           target_ships_tactic_str, target_ships_tactic);
    printf("drive_diff: %d\n", drive_diff);
    status = combat_result(firing_ships_tactic, target_ships_tactic,
                           drive_diff, &result, be_verbose);
    if (be_verbose)
    {
        printf("DBG: combat_result returned %d\n", status);
    }
    printf("result: %s\n", result_to_str(result));
    return 0;
}


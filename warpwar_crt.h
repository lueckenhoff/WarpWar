/* FILEHEADER */

#ifndef _INC_WARPWAR_CRT_H_
#define _INC_WARPWAR_CRT_H_

enum {
    TACTIC_BOGUS = -1,
    TACTIC_ATTACK = 0,
    TACTIC_DODGE = 1,
    TACTIC_RETREAT = 2,

    RESULT_MISS = -1,
    RESULT_HIT = 0,
    RESULT_HIT_PLUS1 = 1,
    RESULT_HIT_PLUS2 = 2,
    RESULT_ESCAPES = 3
};

char * result_to_str (int result);

int combat_result
    (
    int firing_ships_tactic,
    int target_ships_tactic,
    int drive_diff,
    int *ptr_result,
    int verbose);

int string_to_tactic (char *str);


#endif /* not _INC_WARPWAR_CRT_H_ */

TODO LIST:

  - Update README
  - Write functions for moving wumpus, player movement, player shooting, parsing player commands.
  - Write a function 'get highest tunnel index' for use in the 'check for pits/bats/wumpus' functions.
  - Double-check per-file TODOs for critical tasks.
  - Double-check #includes for all subroutines.
  - Re-order subroutines in all files for readability.

================================================================================

# Subroutine Labels #

NSSVTTTTTTTTSSSSSSSSN | Mark: 11111111 00000000 (wump_kill)
NSSVTTTTTTTTSSSSSSSTN | Mark: 11111111 00000001 (kill_wump)
NSSVTTTTTTTTSSSSSSTSN | Mark: 11111111 00000010 (no_arrows)
NSSVTTTTTTTTSSSSSSTTN | Mark: 11111111 00000011 (shoot_self)
NSSVTTTTTTTTSSSSSTSSN | Mark: 11111111 00000100 (jump)
NSSVTTTTTTTTSSSSSTSTN | Mark: 11111111 00000101 (pit_kill)
NSSVTTTTTTTTSSSSSTTSN | Mark: 11111111 00000110 (pit_survive)
NSSVTTTTTTTTSSSSSTTTN | Mark: 11111111 00000111 (please_seed_rng)
NSSVTTTTTTTTSSSSTSSSN | Mark: 11111111 00001000 (problem_with_yes_no_answer)
NSSVTTTTTTTTSSSSTSSTN | Mark: 11111111 00001001 (instructions)
NSSVTTTTTTTTSSSSTSTSN | Mark: 11111111 00001010 (cave_description)

NSSVTSSSSSSSN         | MARK: 10000000 (wump)
NSSVTSSSSSSTN         | MARK: 10000001 (set_config_values)
NSSVTSSSSSTSN         | MARK: 10000010 (wump_init)
NSSVTSSSSSTTN         | MARK: 10000011 (seed_rng)
NSSVTSSSSTSSN         | MARK: 10000100 (build_cave)
NSSVTSSSSTSTN         | MARK: 10000101 (populate_cave)
NSSVTSSSSTTSN         | MARK: 10000110 (clear_cave_data)
NSSVTSSSSTTTN         | MARK: 10000111 (generate_cave_hop_length)
NSSVTSSSTSSSN         | MARK: 10001000 (get_tunnel_destination)
NSSVTSSSTSSTN         | MARK: 10001001 (set_tunnel_destination)
NSSVTSSSTSTSN         | MARK: 10001010 (get_random_room)
NSSVTSSSTSTTN         | MARK: 10001011 (get_room_struct_size)
NSSVTSSSTTSSN         | MARK: 10001100 (room_has_bats)
NSSVTSSSTTSTN         | MARK: 10001101 (room_has_pits)
NSSVTSSSTTTSN         | MARK: 10001110 (build_circular_tunnels)
NSSVTSSTSSSSN         | MARK: 10010000 (set_bats)
NSSVTSSTSSSTN         | MARK: 10010001 (set_pits)
NSSVTSSTSSTSN         | MARK: 10010010 (reset_cave_population)
NSSVTSSTSSTTN         | MARK: 10010011 (populate_bats)
NSSVTSSTSTSSN         | MARK: 10010100 (populate_pits)
NSSVTSSTSTSTN         | MARK: 10010101 (place_player)
NSSVTSSTSTTSN         | MARK: 10010110 (build_random_tunnels)
NSSVTSSTSTTTN         | MARK: 10010111 (get_next_tunnel_slot)
NSSVTSSTTSSSN         | MARK: 10011000 (get_answer)
NSSVTSSTTSSTN         | MARK: 10011001 (get_line)
NSSVTSSTTSTSN         | MARK: 10011010 (wump_loop)
NSSVTSSTTSTTN         | MARK: 10011011 (print_cave_description)
NSSVTSSTTTSSN         | MARK: 10011100 (are_bats_near)
NSSVTSSTTTSTN         | MARK: 10011101 (are_pits_near)
NSSVTSSTTTTSN         | MARK: 10011110 (is_wumpus_near)
NSSVTSSTTTTTN         | MARK: 10011111 (is_wumpus_very_near)
NSSVTSTSSSSSN         | MARK: 10100000 (room_has_wumpus)
NSSVTSTSSSSTN         | MARK: 10100001 (print_room_stats)

================================================================================

# Heap Assignments #

GAME_DATA_BASE = 0x1000

GAME_DATA_BASE     = Number of rooms in cave
GAME_DATA_BASE+001 = Number of pits
GAME_DATA_BASE+002 = Number of bats
GAME_DATA_BASE+003 = Number of links per room
GAME_DATA_BASE+004 = Maximum arrow flight distance
GAME_DATA_BASE+005 = Number of arrows
GAME_DATA_BASE+006 = Player location
GAME_DATA_BASE+007 = Wumpus location

ROOM_DATA_BASE = 0x2000
    Each room is structured as:
        BASE+0 = bool:contains_pit? (valid options are 0 or 1)
        BASE+1 = bool:contains_bat? (valid options are 0 or 1)
        BASE+2 = int:connected room (-1 indicates no tunnel, 0 or greater is the index of the destination room)
        ...
        BASE+n = int:connected room (last: see 'links per room')

USER_INPUT_BUFFER = 0x3000

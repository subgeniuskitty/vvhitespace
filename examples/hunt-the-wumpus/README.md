# Overview #

Hunt the Wumpus is a text game where the player navigates a cave, avoiding
bottomless pits and super bats, all while shooting arrows at a Wumpus. To play,
type `make run`.

For those unfamiliar with the history of Hunt the Wumpus, Wikipedia has a nice
article describing gameplay, development and history.

<https://en.wikipedia.org/wiki/Hunt_the_Wumpus>


# Configuration #

If you want to tweak game settings like the number of rooms, arrows or
obstacles, the file `wump_conf.pvvs` contains all the user-configurable
settings.  After editing the file, simply run `make clean run` to experience
the new settings.


# Instructions #

The Wumpus typically lives in a cave of twenty rooms, with each room having
three tunnels connecting it to other rooms in the cavern.  Caves may vary,
however, depending on options specified when starting the game.

The game has the following hazards for intrepid adventurers to wind their
way through:

  - **Pits:** If you fall into one of the bottomless pits, you find yourself
    slung back out on the far side of the Earth and in very poor shape to continue
    your quest since you're dead.

  - **Bats:** As with any other cave, the Wumpus cave has bats in residence.
    These are a bit more potent, however, and if you stumble into one of their
    rooms they will rush up and carry you elsewhere in the cave.

  - **Wumpus:** If you happen to walk into the room the Wumpus is in you'll
    find that he has quite an appetite for young adventurous humans!  Not
    recommended.

The Wumpus, by the way, is not bothered by the hazards since he has sucker
feet and is too big for a bat to lift.  If you try to shoot him and miss,
there's also a chance that he'll up and move himself into another cave,
though by nature the Wumpus is a sedentary creature.

Each turn you may either move or shoot a crooked arrow.  Moving is done
simply by specifying `m` for move and the number of the room that you'd
like to move down a tunnel towards.  Shooting is done similarly; indicate
that you'd like to shoot one of your magic arrows with an `s` for shoot,
then list a set of connected room numbers through which the deadly shaft
should fly!

If your path for the arrow is incorrect, however, it will flail about in
the room it can't understand and randomly pick a tunnel to continue
through.  You might just end up shooting yourself in the foot if you're
not careful!  On the other hand, if you shoot the Wumpus you've WON!


# Example #

    vvhitespace/examples/hunt-the-wumpus % make clean run
    
    Welcome to Hunt the Wumpus!
    
    Please press 16 keys to seed the RNG.
                   |
    ................
    
    Instructions? (y/n)
    n
    
    You're in a cave with 20 rooms and 3 tunnels leading from each room.
    There are 2 bats and 2 pits scattered throughout the cave, and your
    quiver holds 5 custom super anti-evil Wumpus arrows. Good luck.
    
    ----------
    
    You are in room 14 of the cave and have 5 arrows remaining.
    *sniff* (You smell the evil Wumpus nearby!)
    This room contains tunnels to the following rooms: 18 11 17
    Move or shoot? (m/s)
    m
    To which room do you wish to move?
    17
    
    ----------
    
    You are in room 17 of the cave and have 5 arrows remaining.
    *sniff* (You smell the evil Wumpus nearby!)
    This room contains tunnels to the following rooms: 4 14 0
    Move or shoot? (m/s)
    m
    To which room do you wish to move?
    4
    
    ----------
    
    You are in room 4 of the cave and have 5 arrows remaining.
    *sniff* (You smell the evil Wumpus nearby!)
    This room contains tunnels to the following rooms: 17 1 7
    Move or shoot? (m/s)
    m
    To which room do you wish to move?
    17
    
    ----------
    
    You are in room 17 of the cave and have 5 arrows remaining.
    *sniff* (You smell the evil Wumpus nearby!)
    This room contains tunnels to the following rooms: 4 14 0
    Move or shoot? (m/s)
    s
    Through which rooms do you wish to shoot your arrow?
    (type 5 room numbers separated by spaces)
    14 18 1
    The arrow sails out of room 17 and enters room 14.
    The arrow sails out of room 14 and enters room 18.
    The arrow sails out of room 18 and enters room 1.
    
    *thwock!* *groan* *crash*
    A horrible roar fills the cave, and you realize, with a smile, that you
    have slain the evil Wumpus and won the game!  You don't want to tarry for
    long, however, because not only is the Wumpus famous, but the stench of
    dead Wumpus is also quite well known, a stench plenty enough to slay the
    mightiest adventurer at a single whiff!!


# Heap Assignments #

Game data is stored starting at heap address `0x1000`. A variety of settings
are located here, one per word.

    GAME_DATA = 0x1000
    
    GAME_DATA+0 = Number of rooms in cave
    GAME_DATA+1 = Number of rooms containing pits
    GAME_DATA+2 = Number of rooms containing bats
    GAME_DATA+3 = Number of tunnels per room
    GAME_DATA+4 = Maximum arrow flight distance
    GAME_DATA+5 = Number of arrows
    GAME_DATA+6 = Player location
    GAME_DATA+7 = Wumpus location

Room data is stored as an array of structs starting at heap address `0x2000`.
Each room struct has the following layout.

    BASE+0 = bool:contains_pit? (valid options are 0 or 1)
    BASE+1 = bool:contains_bat? (valid options are 0 or 1)
    BASE+2 = int:connected room (-1 indicates no tunnel, >0 is index of destination room)
    ...
    BASE+n = int:connected room

Each struct is `number_of_tunnels_per_room + 2` words in size. Room numbers
begin at `0` and can be used as indices into this room data array.

The heap space from `0x3000` and up is used as a user input buffer.

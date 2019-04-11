
/*
	SAWD Small ASCII Walk-around Demo (C) Copyright 2008, CCPS Solutions
	
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#ifndef MAPS_H
#define MAPS_H

#define MAP_W 30
#define MAP_H 14
// world map
/*
	
	# wall
	W water
	T tree
	^ mountain
	H house
	. road

	"##############################",
	"#    ^          ^       ^    #",
	"#  ^   ^    ^    T  ^  H ^   #",
	"#WWWW          T          ^  #",
	"# WWWW  ^      .........     #",
	"#  T WWWW T    .       ..    #",
	"#     WWWWW    .        ..   #",
	"#     T WW......         ..  #",
	"#.....   W.WWWWW    T     ...#",
	"#    ......WWWWWWW WWWWWWWW  #",
	"# T       .  WWWWWWWW   WWWWW#",
	"#   H T   .  T  WWWW     WWWW#",
	"#         .   T   WWWWWWWWWWW#",
	"##############################"

	Warps:
	[22, 2] to Town of Kirar (South) [7, 13]

    [3, 11] to Town of Dushal (East) [29, 10]

*/
//char* world_map = "###############################    ^          ^       ^    ##  ^   ^    ^    T  ^  H ^   ##WWWW          T          ^  ## WWWW  ^      .........     ##  T WWWW T    .       ..    ##     WWWWW    .        ..   ##     T WW......         ..  ##.....   W.WWWWW    T     ...##    ......WWWWWWW WWWWWWWW  ## T       .  WWWWWWWW   WWWWW##   H T   .  T  WWWW     WWWW##         .   T   WWWWWWWWWWW###############################";

// North Kirar
/*

	# wall
	W water
	T tree
	^ mountain
	H house
	. road

	"##############################",
	"#WWWWWWWWWWWWWWWWWWWWWWWWWWWW#",
	"#WW########################WW#",
	"#WW#TT^TTTTTT   ^   ..H  ^#WW#",
	"#WW#TTH..TT^     ^  .  ^  #WW#",
	"#WW#TTT ...  ^     ^.     #WW#",
	"#WW#  TT^ ...    T  .^    #WW#",
	"#WW#      ^ ..    ...    ^#WW#",
	"#WW# ^     T . ^  .       #WW#",
	"#WW#    H..........  T ^  #WW#",
	"#WW# ^   ^     .          #WW#",
	"#WW############.###########WW#",
	"#WWWWWWWWWWWWW#.#WWWWWWWWWWWW#",
	"###############.##############"

	[6, 4] Thieves Hut (battle 10 Thieves in a row to get pub owner's beer)
		If first entry
			Hey you! What the hell do you think you're doing here!?
			You just walked into a whole lot of trouble!
			Get em boys!
			first of ten battles starts here
		else
			There is nothing of interest here..just a bunch of corpses.
			You should know that. You kicked their asses!

	[8, 9] Treasure room
		If have key
			If first entry
				You have found the treasure room!
				You fill your pockets with all you can carry.
				Gain 100,000,000 gold!
			else
				Damnit! I guess I didn't lock the door..
				Someone cleaned the place out!
		else
			The door is locked...there must be a key somewhere.

	[22, 3] Cave entrance (ends demo if you choose Yes)
		If you continue the demo will end. 
		If you exit, you will return to the world map.
		Continue?
		If Yes
			end demo
		If no 
			warp to World [22, 3]

	Warps:

	[14, 13] to Town of Kirar (South) [24, 0]
*/

// South Kirar
/*

	# wall
	W water
	T tree
	^ mountain
	H house
	. road

	"#########################.####",
	"#                ^   T  #.#  #",
	"#    ^  ^      T        ..   #",
	"#   ^      H ^      ^ ...    #",
	"#     H    ... ^      . ^    #",
	"#    ^.  ^  T...     ...H    #",
	"#     ....   ^ . ^....   ^   #",
	"#    ^   .     ....          #",
	"#      H...H    ^...H     T  #",
	"# ^    ^ . ^ .....   ^  ^    #",
	"#   T   ......  ^            #",
	"#  ^    .   ^       T    ^   #",
	"#      #.#    ^       ^^     #",
	"########.#####################"

	Inn [7, 8]
		Oh you look exhausted my dear!
		Would you care to rest?
		Rest $50
		Save

	Pub [11, 8]
		If not found beer
			Oi! The wanker who done stole me beer be north of here!
		else
			If first chat after finding beer
				Aye tis me beer! Thank ye me lad!
				Take this Key. (to treasure room) 
			else
				I be drinkin` now go away!

	Armor Shop [20, 8]
		Silver Vest			$250
		Silver Boots		$150
		Silver Gauntlets	$350

	Magic Shop [6, 4]
		Learn Inferno		$1000
		Learn Shatter		$2000
		Learn Drain			$3000

	Weapon Shop [11, 3]
		Silver Sword		$500 (warrior only)
		Silver Axe			$500 (dwarf only)
		Silver Bow			$500 (thief only)
		Silver Staff		$500 (mage only)

	Item Shop [24, 5]
		Healing Powder		$25
		Anti-Venom			$50
		Escape Roots		$1500
		Inferno Scroll		$250
		Shatter Scroll		$350
		Drain Scroll		$450

	Warps:
	[24, 0] to Town of Kirar (North) [14, 13]
	[7, 13] to World [22, 3]
*/
// West Dushal
/*

	# wall
	W water
	T tree
	^ mountain
	H house
	. road

	"##############################",
	"#                            #",
	"#         T     T     T      #",
	"#      T                     #",
	"#            T  H  T     T   #",
	"#   T         ...           ##",
	"#             .       ........",
	"#       H...  .........     ##",
	"# T         ....             #",
	"#   T     T          T   T   #",
	"#             T              #",
	"#      T                T    #",
	"#            T   T           #",
	"##############################"

	Your Home [8, 7]
		You walk to your room...
		Take a rest?
			If Yes
				Rest
				Save

	Magic Shop [16, 4]
		Learn Burn		$100
		Learn Break		$200

	Warps:
	[29, 6] to Town of Dushal (East) [0, 3]
	

*/
// East Dushal
/*

	# wall
	W water
	T tree
	^ mountain
	H house
	. road

	"##############################",
	"#                            #",
	"##    T   H   T          H   #", // pub - item shop
	"....      ...    ...  T  .   #",
	"## ...    T  ..... .......   #",
	"#    ...      ..  T   ..     #",
	"#      ..   ...        .     #",
	"#     .......          . T   #",
	"#  T ..  T       T     .     #",
	"#   ..            ........  ##",
	"#  .. T           H      .....", // inn
	"#  HH        T              ##", // weapon and armor shop
	"#                     T      #",
	"##############################"

	
	Inn [7, 8]
		Oh you look exhausted my dear!
		Would you care to rest?
		Rest $20
		Save

	Pub [11, 8]
		Closed for the day...
		How odd Beggar's Bar never closed...

	Armor Shop [20, 8]
		Leather Vest		$30
		Leather Boots		$40
		Leather Gauntlets	$40

	Weapon Shop [11, 3]
		Short Sword			$100 (warrior only)
		Heavy Axe			$100 (dwarf only)
		Wooden Bow			$100 (thief only)
		Wooden Staff		$100 (mage only)

	Item Shop [24, 5]
		Healing Powder		$15
		Escape Roots		$800
		Burn Scroll			$50
		Break Scroll		$80

	Warps:
	[0, 3] to Town of Dushal (West) [29, 6] 
	[29, 10] to World [3, 11]

*/


#endif

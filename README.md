# Video w/ Sound
Click the thumbnail to watch on youtube

[![Video of engine 4/14/2018](https://img.youtube.com/vi/zztrHvrZON4/0.jpg)](https://www.youtube.com/watch?v=zztrHvrZON4)
[![Video of engine 4/14/2018](https://img.youtube.com/vi/zztrHvrZON4/1.jpg)](https://www.youtube.com/watch?v=zztrHvrZON4)

# Features
In this demo, you can move Mega around, shoot, charge, and delete enemies on the grid. When the chip cust is full, you can bring up the chip select menu. 
The player can select chips and deselect them in the order they were added.  Return to battle and you can use the chips by pressing Right-Control. 
At this time only two chips are implemented: HP+10 and CrckPnl.
Mega can also be deleted. The demo now has sound.

# Controls
```
ARROWS -> Move
SPACE  -> Shoot (hold to charge)
P      -> Pause/Unpause 
Return -> Bring up chip select GUI / Hide 
R CTRL -> Use a chip
```

# Author TheMaverickProgrammer
## Update 4/17/2018
Chip icons are read from a file and correspond to their chip ID. Chips can only be selected if they're compatible with the currently selected chip codes. Compatible codes are code+1, code-1, code==code, and \*. All other codes will be greyed out. The currently selected chip renders the corresponding icon in-battle. This compatibility system works alongsdide the queueing system quite well. 

TODO: The chip icons and chip cards do not map equally. Some icon rows have bombs while the cards have a completely different item type in the image. Someone will have to manually type out each card and icon ID pair or rearrange the sprites to stream them mathematically like I have done in `TextureResourceManager`.

## Contributions to the project
Pheelbert wrote the base tile movement code, sprite resource loading, and the rendering pipeline. I've since then added many new features off the foundation. It's becoming something entirely new. 
Here are my changes and contributions in writing:

New: 
* Shader support in render pipeline
* Pixelated battle intro shader effect
* Pause & pause state shader effect
* Flash white when hit and shader
* Support for stun (yellow) shader
* Cust bar, cust bar progess logic, and cust bar shader
* MMBN identical Mettaur battle AI -> fight in order taking turns, metts do not attack open tiles ahead, metts toggle movement in restricted space
* Entity collision support -> Restrict movement if a tile is occupied
* HP dials in real-time when attacked
* Moving off cracked tiles become broken
* Broken tiles cooldown, flicker, and then restore
* Created audio resource manager with streaming and buffering. Has support for limited channels for an authentic retro experience.
* Provided all audio for the game
* Added Audio priorities 
* Added many new sprites
* Additional keyboard events for more buttons
* Chip library system
* Chip data class
* Chip selection GUI and basic state system 
* Chip select queue and deselect in order 
* Chip select GUI is complete with all the data from the chip
* Exact chip cards are rendered
* Greyscale shader applied on currently selected or invalid chip combos
* Chip UI component for player -> Renders chip name and attack power
* Boss AI ProgsMan (work in progress)
* Throwable Spells
* HP+10 chip works in-battle
* CrckPnl chip works in-battle
* Chip icons are rendered in battle
* Chip icons are rendered in select
* Chip combo select system is now working

Changes:

* Fixed bullet bug -> Bullets would never make it to back row (index error)
* Fixed wave bug -> Metts in back row could not spawn spell ^
* Improved animation times for smoother natural mmbn experience
* Fixed mega death bug -> Game no longer freezes on close
* Keyboard names and controls
* Moved origin logic for health UI components into its own function in Entity class Entity::getAnimOffset()

# Author Pheelbert
Wrote the foundation for the battle engine. He wrote the tile-based movement and update system emulating an authentic mmbn player experience.

## battlenetwork
## =============

https://www.youtube.com/watch?v=GQa0HsVPNE8&feature=youtu.be

A Megaman Battle Network project that I work on occasionally. Made from scratch with SFML and Thor in C++.

'extern/dlls/*(-d).dll' must be copied to the Release/ or Debug/

Be wary of the license, this project was created and I am still working on it for academic reasons. It would be a shame if someone were to steal my project and call it their own!

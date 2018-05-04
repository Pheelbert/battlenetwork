# Video
#### Branch: program-advances 
Click the thumbnail to watch on youtube. 

[![Video of engine 5/4/2018](https://img.youtube.com/vi/gGsZ7-6jY7c/0.jpg)](https://youtu.be/gGsZ7-6jY7c)
[![Video of engine 5/4/2018](https://img.youtube.com/vi/gGsZ7-6jY7c/1.jpg)](https://youtu.be/gGsZ7-6jY7c)

#### Video WITH SOUND outdated (since 4/17/2018)
Click the thumbnail to watch on youtube

[![Video of engine 4/14/2018](https://img.youtube.com/vi/zztrHvrZON4/0.jpg)](https://www.youtube.com/watch?v=zztrHvrZON4)
[![Video of engine 4/14/2018](https://img.youtube.com/vi/zztrHvrZON4/1.jpg)](https://www.youtube.com/watch?v=zztrHvrZON4)

# Features
In this demo, you can move Mega around, shoot, charge, and delete enemies on the grid. When the chip cust is full, you can bring up the chip select menu. 
The player can select chips and deselect them in the order they were added.  Return to battle and you can use the chips by pressing Right-Control. 
At this time only 3 chips are implemented: HP+10, CrckPnl, and Invsble.
Rename the file in `resources/database/library.txt - Copy`to `resources/database/library.txt` for a full library while playing the game.
There is 1 Program Advance: XtremeCannon. Can be activated by selecting `Cannon1 A` + `Cannon2 B` + `Cannon3 C` in order. It deals a whopping 600 points of damage to the first 3 enemies vertically.
Mega can also be deleted. The demo now has sound.

# Controls
```
ARROWS -> Move
SPACE  -> Shoot (hold to charge)
P      -> Pause/Unpause 
Return -> Bring up chip select GUI / Hide 
R CTRL -> Use a chip
```

# Wiki
Care to [contribute](https://github.com/TheMaverickProgrammer/battlenetwork/wiki)? 

# Author TheMaverickProgrammer
## Update 5/4/2018
Mobs can now be dynamically created by `MobFactory` classes. This allows for a whole set of fun spawning random enemy groups.
The `Mob` class has utility functions that are used in the intro to spawn enemies one at a time and with a pixel intro effect just like the games.
Chip menu opens up after intro just like the games.
Chip database script loads and maps to correct card and icon images.
PA (Program Advance) system is the latest feature. The `PA` class stores recipes called "steps" that are loaded from a script. After chip select, the chips are then filtered by the `PA` object and returns a matching PA combo chip.
Mob class now handles deletion of enemy objects. Field class has a new `OwnEntity(e,x,y)` method for objects that need to be cleaned up by field when deleted (like Spells and other effects). 

## Contributions to the project
Pheelbert wrote the base tile movement code, sprite resource loading, and the rendering pipeline. I've since then added many new features off the foundation. It's becoming something entirely new. 
Here are my changes and contributions in writing:

New: 
* Optimized Shader support in render pipeline
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
* Invsble chip works in-battle
* Chip icons are rendered in battle
* Chip icons are rendered in select
* Chip combo select system is now working
* Enemy AI and state system
* Player AI, state, and integrated keyboard control system
* Artifacts
* Loading screen + logs on loading screen effect
* PA (Program Advance) system, display, loading
* Chip library now loaded from a script
* Mob + MobFactory objects
* Refactored battle scene to accept Mob as input type and animate the battle intro
* Logger now spits out a file `log.txt` for debugging

Changes from original author:

* Fixed bullet bug -> Bullets would never make it to back row (index error)
* Fixed wave bug -> Metts in back row could not spawn spell ^
* Improved animation times for smoother natural mmbn experience
* Fixed mega death bug -> Game no longer freezes on close
* Keyboard names and controls
* Moved origin logic for health UI components into its own function in Entity class Entity::getAnimOffset()
* Upgraded Thor and SFML
* Rewrote AnimationComponent to use latest Thor API
* Rewrote player movement code to use latest AnimationComponent features
* Refactored Logger class to queueue and dequeeue logs for use in multithreaded loading screen

# Author Pheelbert
Wrote the foundation for the battle engine. He wrote the tile-based movement and update system emulating an authentic mmbn player experience.

## battlenetwork
## =============

https://www.youtube.com/watch?v=GQa0HsVPNE8&feature=youtu.be

A Megaman Battle Network project that I work on occasionally. Made from scratch with SFML and Thor in C++.

'extern/dlls/*(-d).dll' must be copied to the Release/ or Debug/

Be wary of the license, this project was created and I am still working on it for academic reasons. It would be a shame if someone were to steal my project and call it their own!


<img src="https://i.imgur.com/sVQhjLd.jpg" align="right" alt="Headerlogo" height="auto" width="800px">

# DayZ Vanilla++ Spawn Selection v1.0

**Join our Discord server:[Invite Link V++](https://discord.gg/pb27wYh)**

## 🚩 Table of Contents
- [What is Vanilla++](#what-is-vanilla)
- [Installation](#installation)
	- [Client](#client)
	- [Server](#server)
- [Spawn Selection](#spawn-selection)

## What is Vanilla++:

Vanilla++ is a DayZ workshop add-on which aims to add administration tools by implementing a collection of features, and tools which help communities manager their players.

## Installation:

### Client:

- If you are a player and want to play on Vanilla++ servers, please subscribe to [our steam workshop release](https://steamcommunity.com/sharedfiles/filedetails/?id=1578593068), as well as,  our dependency [Community Framework](https://steamcommunity.com/workshop/filedetails/?id=1559212036)

- Use the DayZ Launcher and load @VPPSpawnSelection, and @CommunityFramework, or by adding the following to your start-up commandline:
```c
-mod=@CommunityFramework;@VPPSpawnSelection
```

### Server:
- [Subscribe to workshop addon](https://steamcommunity.com/sharedfiles/filedetails/?id=1578593068) and [Community Framework](https://steamcommunity.com/workshop/filedetails/?id=1559212036)
- Launch DayZ launcher, Find @VPPSpawnSelection, right click, and click "Open Folder"
- Take the @VPPSpawnSelection, and @CommunityFramework folders, and copy them to the server's root directory.
- Take the contents of the Keys folder, which is located inside of both CommunityFramework, and VPPAdminTools, and move those files to the Keys folder, located in the server's root directory.
- Change your startup to include the following following start-up commandlines arguments:
``` -mod=@CommunityFramework;@VPPSpawnSelection; -profiles=serverLogs```

Note: The mod will generate a config named 'SpawnSelectorConfig.json` after startup. You can edit that on the go...changes will apply instantly no need for a restart.

Set: LOADOUT_SELECTION_TYPE to 1 to get customizable loadouts.
Set: LOADOUT_SELECTION_TYPE to 2 to get static loadouts.
Set: FORCE_RANDOM_SPAWNS to 1 to FORCE players to spawn randomly
use the following json formater to check your config is of valid syntax after editing. https://jsonformatter.curiousconcept.com/

#### Spawn Selection:
 - Give players option to choose startup gear
 - Change config with no restart needed.
 - Make your server look more bad-ass with our mods.

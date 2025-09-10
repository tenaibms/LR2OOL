# LR2OOL
[![MSBuild](https://github.com/tenaibms/LR2OOL/actions/workflows/msbuild.yml/badge.svg)](https://github.com/tenaibms/LR2OOL/actions/workflows/msbuild.yml)

## Outline
This tool adds basic some quality of life to LR2. Including:
- Mirroring courses
- Configurable hit-error bar
- Additional `#SRC_NUMBERS` for useful timing metrics, among other things

## Installation
Place `LR2OOL.dll` in your lr2 root directory and use your preferred dll injector ([A modified version of bokutachi launcher is provided here](https://github.com/tenaibms/BokutachiHook/releases/tag/launcher_only)) to inject the aforementioned dll.
The version of the game you inject it into must contain the F/S patch.

⚠️Do note that this software is still in development, and is currently very experimental. It is likely you will encounter bugs. Please report them!

## Preview
![image](https://github.com/user-attachments/assets/50e24f2a-52d6-4ded-a86e-e6222ede6df5)

## Usage

By default, press `<Insert>` to open the menu. This can be changed to whatever you wish.
To uninject the mod, press `<End>`. This currently cannot be changed.

### Additional SRC_NUMBER Indexes
| Index | Value                                  |
|-------|----------------------------------------|
| 295   | 1P random                              |
| 296   | Mean (whole part)                      |
| 297   | Mean (decimal part)                    |
| 298   | Stddev (whole part)                    |
| 299   | Stddev (decimal part)                  |
| 302   | Green number                           |
| 303   | White number                           |
| 304   | Green number (min)                     |
| 305   | Green number (max)                     |
| 400   | PGREAT ratio (whole part)              |
| 401   | PGREAT ratio (decimal part)            |
| 402   | GREAT ratio (whole part)               |
| 403   | GREAT ratio (decimal part)             |
| 404   | PGREAT percentage (whole)              |
| 405   | PGREAT percentage (decimal)            |
| 406   | GREAT percentage (whole)               |
| 407   | GREAT percentage (decimal)             |
| 408   | GOOD percentage (whole)                |
| 409   | GOOD percentage (decimal)              |
| 410   | BAD percentage (whole)                 |
| 411   | BAD percentage (decimal)               |
| 412   | POOR percentage (whole)                |
| 413   | POOR percentage (decimal)              |
| 414   | Green number (decimal part)            |
| 415   | White number (decimal part)            |
| 416   | Lift number (whole part)               |
| 417   | Lift number (decimal part)             |
| 418   | 2P random                              |
| 419   | Custom gauge (whole)                   |
| 420   | Custom gauge (1 decimal place)         |
| 421   | Custom gauge (2 decimal places)        |
| 422   | Total judgments                        |
| 423   | Session time (seconds)                 |
| 424   | Session time (minutes)                 |
| 425   | Session time (hours)                   |
| 426   | Total time (seconds)                   |
| 427   | Total time (minutes)                   |
| 428   | Total time (hours)                     |
| 429   | Self rate (decimal)                    |
| 430   | Rival rate (decimal)                   |


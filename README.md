

   _____                               _                                           __   _ _  __           _                 _       _             
  / ____|                             ( )                                         / _| | (_)/ _|         (_)               | |     | |            
 | |     ___  _ ____      ____ _ _   _|/ ___    __ _  __ _ _ __ ___   ___    ___ | |_  | |_| |_ ___   ___ _ _ __ ___  _   _| | __ _| |_ ___  _ __ 
 | |    / _ \| '_ \ \ /\ / / _` | | | | / __|  / _` |/ _` | '_ ` _ \ / _ \  / _ \|  _| | | |  _/ _ \ / __| | '_ ` _ \| | | | |/ _` | __/ _ \| '__|
 | |___| (_) | | | \ V  V / (_| | |_| | \__ \ | (_| | (_| | | | | | |  __/ | (_) | |   | | | ||  __/ \__ \ | | | | | | |_| | | (_| | || (_) | |   
  \_____\___/|_| |_|\_/\_/ \__,_|\__, | |___/  \__, |\__,_|_| |_| |_|\___|  \___/|_|   |_|_|_| \___| |___/_|_| |_| |_|\__,_|_|\__,_|\__\___/|_|   
                                  __/ |         __/ |                                                                                             
                                 |___/         |___/                                                                                              

Conway's game of life simulator
===============================

It was initially introduced as a math game. The feature is that countless rules can be created with a few simple rules. These characteristics give the following three meanings.

- One is that the game of life is a cellular automaton with computational power equivalent to that of a general-purpose Turing machine. Anything that can be calculated by any algorithm can be calculated using it.

- The other interesting thing is that it incorporated a general principle that later became a major factor in chaos theory, the idea of “deterministic chaos”: that is, the smallest number of rules which could generate an inherently unpredictable system.

- The last feature is that this is an artificial life simulation. It has the characteristic of self-replication, which is the most important characteristic of life. Cells in the game replicate cells like themselves when certain conditions are met.

<br>

## Rules
---

<img src="https://latex.codecogs.com/gif.latex?O_t=\text{Let the game board be } C_{m \times n \times t} \,\ \text{which is} \,\ m \times n \times t \,\ \text{tensor.}" />

'''math
\text{Let the game board be } C_{m \times n \times t} \,\ \text{which is} \,\ m \times n \times t \,\ \text{tensor.}
```

```math
\text{If} \,\ C(i, j, t) \,\ \text{which is dead cell is adjacent with 3 living cells when the time is} \,\ t \text{, then} \,\ 
C(i, j, t + 1) \,\ \text{is living cell.}
```

```math
\text{Else if} \,\ C(i, j, t) \,\ \text{which is living cell is adjacent with 2 or 3 living cells when the time is} \,\ t \text{, then} \,\ 
C(i, j, t + 1) \,\ \text{is living cell.}
```

```math
\text{Else} \,\ C(i, j, t + 1) \,\ \text{is dead cell.} \quad (\because \,\ \text{the surroundings become too complex or stay dead.})
```

<br>

## Installation
---
```
Fighting
```

<br>

## Usage
---
```
Pass
```
<br>

## In game animation
---
1. Building map
![buliding map](https://user-images.githubusercontent.com/38760913/145711525-d02a4be4-6697-47e7-8e3b-fc38c9772627.gif)

2. Loading map
![game load](https://user-images.githubusercontent.com/38760913/145711515-18e996cc-6471-4c8a-8115-2e16a26bdbd6.gif)

3. Simulation
![simulation](https://user-images.githubusercontent.com/38760913/145711529-296e5c92-fab5-4bf2-b430-c705c1b93de2.gif)

<br>

## Update log
---
1. 2021.12.09 - Version 1.0.0 Release
2. fwefawef

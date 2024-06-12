# NTNUCSIE-CPII-Final_Project
My final project of Computer Programming II.

## How to run
You can compile with `make` under [src/](./src/).
Then you can use `engine` to run the game.
    
```shell
./engine <related_path>
```

## Requirement files

<path>/novel.toml
<path>/font.ttf
<path>/sound/switch.mp3
<path>/sound/select.mp3
<path>/sound/change.mp3
<path>/sound/get_item.mp3
<path>/sound/dialogue.mp3
<path>/sound/scene.mp3
<path>/sound/esc.mp3
<path>/music/title.mp3
<path>/music/bgm.mp3
music and sound is **mp3** only

You can copy those file from [here](./res/)

## TOML Requirement

```toml
name = string

[player.<player>]
starter = string

[character.<character>]
name = string
avatar = path(string)
tachie = path(string)

[item.<item>]
name = string
description = path(string)
icon = path(string)

[event.<event>]
scene = path(string)
dialogue = string
# or
[event.<event>]
end = end_id(string)

[dialogue.<dialogue>]
character = character_id(string)
text = string
[[dialogue.<dialogue>.options]]
text = string
event = event_id(string)
# or 
[[dialogue.<dialogue>.options]]
text = string
next = dialogue_id(string)

[end.<end>]
title = string
background = path(string)
text = string
```

dialogue 要有 character 跟 text 且一定要有至少一個最多五個 option 且只能接 event 或 dialogue，且所有選項需一致


## TOML Optional
```toml
start_background

[player]
inventory =[item_id(string)]

[charater.<character>]
avatar_happy = path(string)
avatar_sad = path(string)
tachie_happy = path(string)
tachie_sad = path(string)

[event.<event>]
item = item_id(string) # in scene only

[dialogue.<dialogue>]
[[dialogue.<dialogue>.options]]
favorability_add = int

[end.<end>]
music = path(string)
```

## TOML Example
[novel.toml](./res/novel.toml)

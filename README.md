# Cistercian

## Overview

[video]

This is another attempt of mine at writing a panel with Qt. This time I am
hoping that QtQuick is going to feel like a good fit, what with it using
declarative UI and scene-graph, GPU-first rendering.

Be warned - this is really just some unambitious prototyping at this stage.
There is not configuration, nor any plugins other than a very simple taskbar.

Previous attempts:

- [unleash-panel] Written for X11 which I guess is not the modern way
- [tint-QPushButton] Curtailed as the style-sheets felt to difficult and
  inflexible for tint2 type theming
- [tint-QGraphicsItem] Struggles with transparency on wlroots scene-graph API
  based Wayland Compositors

[video]: https://youtu.be/dWrehzyv5zA 
[unleash-panel]: https://github.com/johanmalm/unleash-panel
[tint-QPushButton]: https://github.com/teallach-desktop/tint/tree/2f46cd7b24274b918392032f4e4bab80804009e9 
[tint-QGraphicsItem]: https://github.com/teallach-desktop/tint

## Build

```
cmake -B build
cmake --build build
```


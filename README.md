# Path Tracer

Realistic 3D scene renderer; it features HDR, reflections, refractions,
transparent and translucent environments, diffuse and matte surface.

This is one of my hobby projects coded live on my streams
on https://twitch.tv/honzamatousek

Playlist with recorded streams is here: https://www.youtube.com/watch?v=8IR0cPgdOuA&list=PLuOBL1HCzT4d02YPUrMsMoNE4ucTmP-26 (in czech language)

Project is currently on hold; sometime I will return and continue.

## Features

* Sphere, triangle and plane primitives support.
* Emission, diffuse, normal, roughness and metalness texture support.
* HDR, lights, reflections, refractions.
* Fresnel equations.
* Light dispersion (rainbows are possible).
* Nearest neighbor and bilinear texture filtering.
* Fractal materials (wood, marble).
* Custom language for scene definitions.
* Configurable FOV, resolution, depth-of-field focus.
* Utilities for generating spherical/flat lens (useful for simulating telescopes)
* Multi-threaded rendering and KD tree speed optimization.
* `.jpeg` texture support.
* `.obj`/`.mtl` support.

## Known issues

* File path in loaded file is not relative to including file. (Program must be run from examples directory.)
* Lack of documentation and working examples.
* It is slow as ~~hell~~ path tracer.

## State of code

Source code contains many bugs and the are many unfinished things. To compile it, use `cmake`. Linux is preferred.

## License

Code is licensed under GPLv3 license. Feel free to fork / suggest changes.

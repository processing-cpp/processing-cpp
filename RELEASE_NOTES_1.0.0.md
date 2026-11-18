# C++ Mode 1.0.0

Changes since 0.6.1. This release is about stability. Sketches written the Processing way now behave the way they do in Java, both renderers got a long list of fixes, and there's a test suite that runs before every release.

## Sketches that now just work

- `selectInput()`, `selectOutput()` and `selectFolder()` work like in Java: pass a callback name, keep drawing while the dialog is open, and get a `File` in `fileSelected(File selection)`. `selection == null` means the user cancelled. Linux (zenity or kdialog), macOS and Windows are supported.
- `null` is a keyword now, so `if (img == null)` compiles as written.
- `fullScreen()` sizes the sketch to the display, also when called in `settings()`, and opens straight into fullscreen (no more resize flash on tiling window managers). `fullScreen(P2D, 2)` picks a monitor and `fullScreen(SPAN)` covers all of them.
- `#FF8800` color literals.
- Java static calls: `Math.abs()`, `Integer.MAX_VALUE`, `Float.isNaN()`, `String.format()`, `System.out.println()`, `System.currentTimeMillis()`.
- `int("42")`, `float("2.5")` and `boolean("true")`, plus `int(s)` and `float(s)` on a `String`.
- `thread("work")` runs your `work()` in the background.
- `size(w, h)` works again when called from plain C++ outside a sketch class.
- Sketches with a `static` global (like `static demoMain_t demo;`) build again; 0.6.1 failed to link them and the IDE could run out of memory (#15).

## Java Processing behavior

- `parseInt()` and `parseFloat()` follow Processing's rules: `parseInt("3.7")` is 3, bad input gives 0 for ints and NaN for floats.
- `IntDict`, `FloatDict` and `StringDict` keep insertion order and have the full Java API (`increment`, `add`, `sortValues`, `keyArray`, `maxKey` and the rest).
- `IntList` and `FloatList`: `add(index, amount)` adds to a value like in Java, `insert()` inserts. `StringList` gained `upper()`, `lower()` and `sortReverse()`.
- `split(s, "::")` with a string delimiter, `String.hashCode()`, `min()`/`max()` of arrays, `applyMatrix()` with 6 values, `isLooping()`, and the `surface` object (`surface.setTitle()`, `setSize()`, `setLocation()`, `setResizable()`).
- `saveJSONObject(json, file)` and `saveTable(table, file)` take Java's argument order (the old order still works).
- `stroke(color, alpha)` and `fill(color, alpha)` work like Java (alpha in `colorMode` units). A colored stroke with alpha used to come out gray.
- PShape: `setFill(color)`, `setFill(#FF8800)`, `setFill(false)` and the same for stroke.
- `PImage` gained `save()` and `blend()`, and `get()`, `copy()` and `resize()` match Java.
- `launch()` opens files and links with the default app, `savePath()` creates folders.
- `print`/`println` format numbers, colors, booleans and vectors like Java.

## Rendering

- Shapes, curves, blend modes, `strokeCap`, `ellipseMode(CORNER)`, arcs, contours and textures fixed in the OpenGL renderer.
- PGraphics: pixels, `get()`/`set()`, lights and `tint()` behave like the main canvas.
- libprocessing renderer: curves, rounded rects, 3D points and lines, `textureWrap`, `smooth()`, depth test, PShape and much faster `loadPixels()`/`updatePixels()`.
- `saveFrame()` writes real TIFF files, and the default name matches Java (`screen-####.tif`).

## Under the hood

- The Java-syntax rewrites now go through a real tokenizer (`CppLexer`), so strings, comments and directives are never touched and line numbers in errors stay exact.
- `scripts/test_all.py` runs four test suites in under a minute: translator rewrites, free functions, 39 API areas, and 87 runtime values checked against Java.

## Windows and Linux

- Windows: builds no longer fail silently when another MinGW (Chocolatey and similar) is on PATH. `g++` now runs with its own `bin` folder first. (#6)
- Windows: the install wizard no longer wipes your user PATH when it adds MSYS2. (#7)
- Linux: fixed GLEW failing to start on Wayland. (#9, #14)
- Fixed link errors for the `image(PImage&, ...)` overloads that were declared but never implemented. (#13)
- The mode shows its real version in the Contribution Manager. (#12)
- `scripts/rebuild-jar.py` works on Windows. (#8)

## Web editor

The web editor at processing-cpp.github.io/editor runs this release. New sketches compile about twice as fast, and its compile server now updates itself from GitHub releases, tests every build before going live, and rolls back by itself if something breaks. Compile errors and runtime errors show in the error panel with a Copy button, and a sketch that fails to start no longer leaves the loading spinner up.

## Thanks

Thanks to @bogado for tracking down the Wayland GLEW bug, the missing `image()` overloads and the broken version field, and for testing the Wayland fix. Thanks to @ffunatsu for Windows support in the jar build script, to TomArrow for the detailed report in #15, and to everyone who reported bugs.

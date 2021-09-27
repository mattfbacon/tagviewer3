# TagViewer 3

TagViewer in C++ using Qt5. Uses SQLite for storage.

## Building

```sh
qmake -makefile -Wall # optional but recommended: `-spec linux-clang`
# for release
make release # or just `make`
# for debug
./run_debug.sh # sets up ASAN, LSAN, etc
```

Binary will be in `dist/`.

## License

GPL 3 or later. Commercial licensing available upon request.

## Thanks/Credit

- [PhantomStyle](https://github.com/JulienMaille/phantomstyle): used for the application’s theme. Modified to remove deprecations, match code style, and remove unused colors that were used in Ripcord. Code in `lib/phantom/`, as well as `src/themes.cpp` and `include/themes.hpp` for the palettes.
- [Vecta.io Nano](https://vecta.io/nano): used to compress SVGs.
- [Timmmm’s Answer on Stack Overflow](https://stackoverflow.com/a/43936590/4945014): a QLabel subclass that retains the QPixmap’s aspect ratio. Code in `src/AspectRatioLabel.cpp` and `include/AspectRatioLabel.hpp`. Modified heavily to use my code style, and to control where the margins are placed, including RTL support, but why not give credit?
- [Chris’s Answer on Stack Overflow](https://stackoverflow.com/a/9265972/4945014): a simple function to convert from `QList<T>` to `QVariantList`. `list_as_variant` in `include/templates.hpp`. Modified to use my code style. I probably could have written it myself but I didn’t so credit to the person who did!

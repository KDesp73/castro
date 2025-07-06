# Castro

Independent Move Generation Library

## Get Started

```bash
git clone https://github.com/KDesp73/castro
cd castro
make all -j3
```

`libcastro.a`, `libcastro.so` and `check` should be created

## Example

```c
#include <castro.h>

int main() {
    Board board = {0};
    BoardInitFen(&board, NULL); // Starting Position

    Moves moves = GenerateMoves(&board, MOVE_LEGAL);
    Move move = moves.list[0];
    MakeMove(&board, move);

    BoardPrintMove(&board, move);

    BoardFree(&board);

    return 0;
}
```
### Running Tests

To run tests for each module:

```bash
make test
```
> [!NOTE]
> Data-driven testing handled by
> [IncludeOnly/test.h](https://github.com/KDesp73/IncludeOnly/blob/main/libs/test.h).

## Documentation

Documentation for this library can be found in the header file [castro.h](./src/castro.h)

## License

[MIT](./LICENSE)

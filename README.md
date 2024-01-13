# $$\micro\text{cell}$$
Project inspired by [Tsodings minicel project](https://github.com/tsoding/minicel) ([video](https://youtu.be/HCAgvKQDJng?si=Q_-pfFew8hnSm1ir)) - I saw only first 10 minutes but it looks cool

## how to use
$\micro\text{cell}$ procesess input file in this format:
```
1       |  2              |  =A0*3
=B1+42  |  =sin(C0*pi/2)  |  15.6
```
Columns are A-Z and rows 0-99. Pipe (`|`) symbol is used to separate cells and white spaces are ignored. To create expression you use `=` followed by mathematical expression which can use basic math operations (`+`, `-`, `*`, `/`, `^`), values of other cells, functions (`sin`, `cos`, `sqrt`, `ceil`, `floor`, `log`) or contants (`pi`, `e`).

Use `make` to build the program and `./ucel <input-file>` to evaluate given file.
Result of example above:
```
   |       A       |       B       |       C       |
----------------------------------------------------
 0 |     1.000     |     2.000     |     3.000     |
 1 |    41.000     |    -1.000     |    15.600     |
```
## files
- `main.c` - $\micro\text{cell}$ <3
- `/adt` - data structures
  - `list` - simple implementation of list
  - `stack` - simple implementation of stack
- `/eval` - all files needed to evaluate math expression
  - `tokenizer` - splits string into tokens
  - `substitute` - replaces constants with actual values
  - `analyzer` - fixes negative numbers, operators before paretheses etc.
  - `eval` - evaluates expression by converting it to postfix notation and uses Shunting yard algorithm to evaluate it
- `/ucel` - $\micro\text{cell}$ stuff
  - `ucel` - parsing table and evaluating cells in right order
  - `utils` - printing utils

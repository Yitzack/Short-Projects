# Sudoku Solver

Basic sudoku solver.

## Compile directions

```cargo build```

## Details of operation

Find the naked singles, the hidden singles, and then recersively make a guess. Repeat until puzzle is broken. Backtrack and try again until solved.

Program comes with an unsolved puzzle and its solution.

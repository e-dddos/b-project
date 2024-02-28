class GridExplorer:
    def __init__(self, grid):
        self.grid = grid
        self.rows = len(grid)
        self.cols = len(grid[0])
        self.visited = set()
        self.directions = [(0, 1), (1, 0), (0, -1), (-1, 0)]  # right, down, left, up
        self.current_position = (0, 0)  # starting position

    def move_forward(self):
        new_position = (
            self.current_position[0] + self.directions[0][0],
            self.current_position[1] + self.directions[0][1],
        )
        if self.is_valid_move(new_position):
            self.current_position = new_position
            self.visited.add(new_position)

    def turn_right(self):
        self.directions.append(self.directions.pop(0))

    def explore_grid(self):
        while len(self.visited) < self.rows * self.cols:
            self.move_forward()
            if len(self.visited) < self.rows * self.cols:
                self.turn_right()

    def is_valid_move(self, position):
        return (
            0 <= position[0] < self.rows
            and 0 <= position[1] < self.cols
            and self.grid[position[0]][position[1]] != "X"
            and position not in self.visited
        )

    def print_grid(self):
        for i in range(self.rows):
            for j in range(self.cols):
                if (i, j) == self.current_position:
                    print("C", end=" ")  # Current position
                elif self.grid[i][j] == "X":
                    print("X", end=" ")  # Obstacle
                else:
                    print(".", end=" ")  # Free square
            print()


# Example usage:
grid = [
    [".", ".", ".", ".", ".", "."],
    [".", ".", "X", ".", ".", "."],
    [".", ".", ".", ".", ".", "."],
    [".", ".", ".", ".", ".", "."],
]

explorer = GridExplorer(grid)
explorer.explore_grid()
explorer.print_grid()

import tkinter as tk
from tkinter import messagebox
import random


class GameMenu(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Game Selection")

        # Label
        self.label = tk.Label(self, text="Choose a Game:", font=("Arial", 18))
        self.label.pack(pady=20)

        # Buttons
        self.sus_button = tk.Button(self, text="SUS Game", font=("Arial", 14), command=self.start_sus_game)
        self.sus_button.pack(pady=10)

        self.ultimate_button = tk.Button(self, text="Ultimate Tic Tac Toe", font=("Arial", 14), command=self.start_ultimate_game)
        self.ultimate_button.pack(pady=10)

        self.quit_button = tk.Button(self, text="Exit", font=("Arial", 14), command=self.quit)
        self.quit_button.pack(pady=10)

    def start_sus_game(self):
        self.destroy()  # Close the menu window
        root = tk.Tk()
        game = SUSGame(root)
        root.mainloop()

    def start_ultimate_game(self):
        self.destroy()
        root = tk.Tk()
        game = UltimateTicTacToe(root)
        root.mainloop()


import tkinter as tk
from tkinter import messagebox
import random

class SUSGame:
    def __init__(self, root):
        self.root = root
        self.root.title("SUS Game")

        # Game variables
        self.grid = [["" for _ in range(3)] for _ in range(3)]
        self.taken = [[False for _ in range(3)] for _ in range(3)]
        self.player_symbols = ["S", "U"]
        self.players = []
        self.current_player_index = 0
        self.p1_score = 0
        self.p2_score = 0
        self.moves = 0
        self.checked_sequences = set()
        self.play_against_computer = False

        # Start menu
        self.setup_menu()

    def setup_menu(self):
        """Sets up the main menu."""
        for widget in self.root.winfo_children():
            widget.destroy()

        tk.Label(self.root, text="Welcome to the SUS Game!", font=("Arial", 18)).pack(pady=10)

        # Player 1 selection
        tk.Button(self.root, text="Play vs Human", font=("Arial", 14), command=self.start_human_game).pack(pady=5)
        tk.Button(self.root, text="Play vs Computer", font=("Arial", 14), command=self.start_computer_game).pack(pady=5)
        tk.Button(self.root, text="Exit", font=("Arial", 14), command=self.root.quit).pack(pady=5)

    def start_human_game(self):
        self.play_against_computer = False
        self.choose_symbols()

    def start_computer_game(self):
        self.play_against_computer = True
        self.choose_symbols()

    def choose_symbols(self):
        """Allows Player 1 to choose their symbol and assigns the other symbol to Player 2."""
        def submit_symbol():
            p1_symbol = symbol_var.get().upper()
            if p1_symbol not in ["S", "U"]:
                messagebox.showwarning("Invalid Choice", "Please choose 'S' or 'U'.")
                return
            p2_symbol = "U" if p1_symbol == "S" else "S"
            self.players = [("Player 1", p1_symbol), ("Computer" if self.play_against_computer else "Player 2", p2_symbol)]
            self.initialize_game()

        for widget in self.root.winfo_children():
            widget.destroy()

        tk.Label(self.root, text="Player 1: Choose your symbol (S or U)", font=("Arial", 14)).pack(pady=10)
        symbol_var = tk.StringVar(value="S")
        tk.Entry(self.root, textvariable=symbol_var, font=("Arial", 14), width=5).pack(pady=5)
        tk.Button(self.root, text="Submit", font=("Arial", 14), command=submit_symbol).pack(pady=10)

    def initialize_game(self):
        """Initializes the game board."""
        for widget in self.root.winfo_children():
            widget.destroy()

        # Reset variables
        self.grid = [["" for _ in range(3)] for _ in range(3)]
        self.taken = [[False for _ in range(3)] for _ in range(3)]
        self.checked_sequences = set()
        self.p1_score = 0
        self.p2_score = 0
        self.moves = 0
        self.current_player_index = 0

        # Player labels
        self.score_label = tk.Label(self.root, text=self.get_score_text(), font=("Arial", 14))
        self.score_label.grid(row=0, column=0, columnspan=3, pady=10)
        self.player_label = tk.Label(self.root, text=f"Current Turn: {self.players[self.current_player_index][0]}", font=("Arial", 14))
        self.player_label.grid(row=1, column=0, columnspan=3)

        # Game buttons
        self.buttons = [[None for _ in range(3)] for _ in range(3)]
        for r in range(3):
            for c in range(3):
                btn = tk.Button(self.root, text="", font=("Arial", 24), width=5, height=2,
                                command=lambda x=r, y=c: self.make_move(x, y))
                btn.grid(row=r+2, column=c)
                self.buttons[r][c] = btn

    def make_move(self, x, y):
        """Processes a player's move."""
        if self.taken[x][y]:
            messagebox.showwarning("Invalid Move", "This cell is already occupied!")
            return

        symbol = self.players[self.current_player_index][1]
        self.grid[x][y] = symbol
        self.taken[x][y] = True
        self.buttons[x][y].config(text=symbol)
        self.moves += 1

        if self.check_sus_sequence():
            self.update_scores()
            messagebox.showinfo("SUS Found", f"{self.players[self.current_player_index][0]} formed 'S-U-S'!")

        # Switch players
        self.current_player_index = 1 - self.current_player_index
        self.player_label.config(text=f"Current Turn: {self.players[self.current_player_index][0]}")

        if self.moves == 9:
            self.end_game()
        elif self.play_against_computer and self.current_player_index == 1:
            self.computer_move()

    def computer_move(self):
        """Generates a random move for the computer."""
        empty_cells = [(r, c) for r in range(3) for c in range(3) if not self.taken[r][c]]
        if empty_cells:
            x, y = random.choice(empty_cells)
            self.make_move(x, y)

    def check_sus_sequence(self):
        """Checks for 'S-U-S' sequences."""
        sus = ["S", "U", "S"]
        lines = [
            [self.grid[r][c] for c in range(3)] for r in range(3)  # Rows
        ] + [
            [self.grid[r][c] for r in range(3)] for c in range(3)  # Columns
        ] + [
            [self.grid[i][i] for i in range(3)],  # Diagonal
            [self.grid[i][2-i] for i in range(3)]  # Anti-diagonal
        ]

        for idx, line in enumerate(lines):
            if line == sus and idx not in self.checked_sequences:
                self.checked_sequences.add(idx)
                return True
        return False

    def update_scores(self):
        """Updates scores after forming 'S-U-S'."""
        if self.current_player_index == 0:
            self.p1_score += 1
        else:
            self.p2_score += 1
        self.score_label.config(text=self.get_score_text())

    def get_score_text(self):
        """Returns the current score as a string."""
        return f"Scores - {self.players[0][0]}: {self.p1_score} | {self.players[1][0]}: {self.p2_score}"

    def end_game(self):
        """Ends the game and shows the final result."""
        if self.p1_score > self.p2_score:
            winner = self.players[0][0]
        elif self.p2_score > self.p1_score:
            winner = self.players[1][0]
        else:
            winner = "No one! It's a draw."

        messagebox.showinfo("Game Over", f"Game Over!\nWinner: {winner}")
        self.setup_menu()



class UltimateTicTacToe:
    def __init__(self, root):
        self.root = root
        self.root.title("Ultimate Tic Tac Toe")

        # Game State Variables
        self.current_player = "X"
        self.subgrid_winners = [[""] * 3 for _ in range(3)]  # Main board status
        self.boards = [[[[""] * 3 for _ in range(3)] for _ in range(3)] for _ in range(3)]  # 9 small boards
        self.buttons = [[None] * 9 for _ in range(9)]  # Buttons for all cells
        self.play_with_computer = False

        self.ask_game_mode()  # Ask user for game mode
        self.create_ui()

    def ask_game_mode(self):
        """Asks the user to choose the game mode (Player 2 or Computer)."""
        answer = messagebox.askquestion("Game Mode", "Do you want to play against the computer?")
        if answer == "yes":
            self.play_with_computer = True
        else:
            self.play_with_computer = False

    def create_ui(self):
        """Initializes the UI components of the game."""
        # Label for Current Player
        self.label = tk.Label(self.root, text=f"Current Player: {self.current_player}", font=("Arial", 16))
        self.label.grid(row=0, column=0, columnspan=3, pady=10)

        # Main Frame for the 9 Small Boards
        self.main_frame = tk.Frame(self.root)
        self.main_frame.grid(row=1, column=0, padx=10, pady=10)

        # Create the 9 Small Boards
        for i in range(3):
            for j in range(3):
                frame = tk.Frame(self.main_frame, relief="solid", borderwidth=2)
                frame.grid(row=i, column=j, padx=5, pady=5)
                self.create_small_board(frame, i, j)

    def create_small_board(self, parent_frame, main_row, main_col):
        """Creates a 3x3 small board in a given frame."""
        for i in range(3):
            for j in range(3):
                button = tk.Button(parent_frame, text="", font=("Arial", 12), width=5, height=2,
                                   command=lambda r=main_row, c=main_col, x=i, y=j: self.make_move(r, c, x, y))
                button.grid(row=i, column=j)
                self.buttons[main_row * 3 + i][main_col * 3 + j] = button

    def make_move(self, main_row, main_col, small_row, small_col):
        """Handles a player's move."""
        if self.boards[main_row][main_col][small_row][small_col] != "":
            return  # Cell is already occupied

        # Update Cell
        self.boards[main_row][main_col][small_row][small_col] = self.current_player
        self.buttons[main_row * 3 + small_row][main_col * 3 + small_col].config(text=self.current_player, state="disabled")

        # Check if this small board is won
        if self.check_small_board(main_row, main_col):
            self.subgrid_winners[main_row][main_col] = self.current_player
            self.disable_small_board(main_row, main_col)

            # Check for overall winner
            if self.check_main_board():
                messagebox.showinfo("Game Over", f"Player {self.current_player} wins the Ultimate Tic Tac Toe!")
                self.reset_game()
                return

        # Check for Draw on the Main Board
        if self.check_main_board_draw():
            messagebox.showinfo("Game Over", "The game ends in a Draw!")
            self.reset_game()
            return

        # Switch player
        self.current_player = "O" if self.current_player == "X" else "X"
        self.label.config(text=f"Current Player: {self.current_player}")

        # Trigger computer move if enabled
        if self.play_with_computer and self.current_player == "O":
            self.computer_move()

    def computer_move(self):
        """Handles the computer's random move."""
        available_moves = []
        for main_row in range(3):
            for main_col in range(3):
                if self.subgrid_winners[main_row][main_col] == "":  # Only playable boards
                    for small_row in range(3):
                        for small_col in range(3):
                            if self.boards[main_row][main_col][small_row][small_col] == "":
                                available_moves.append((main_row, main_col, small_row, small_col))

        if available_moves:
            move = random.choice(available_moves)
            self.make_move(*move)

    def check_small_board(self, main_row, main_col):
        """Checks if a small board has a winner."""
        board = self.boards[main_row][main_col]
        for i in range(3):
            if board[i][0] == board[i][1] == board[i][2] != "":
                return True
            if board[0][i] == board[1][i] == board[2][i] != "":
                return True
        if board[0][0] == board[1][1] == board[2][2] != "" or board[0][2] == board[1][1] == board[2][0] != "":
            return True
        return False

    def check_main_board(self):
        """Checks if the overall main board has a winner."""
        for i in range(3):
            if self.subgrid_winners[i][0] == self.subgrid_winners[i][1] == self.subgrid_winners[i][2] != "":
                return True
            if self.subgrid_winners[0][i] == self.subgrid_winners[1][i] == self.subgrid_winners[2][i] != "":
                return True
        if self.subgrid_winners[0][0] == self.subgrid_winners[1][1] == self.subgrid_winners[2][2] != "" or \
           self.subgrid_winners[0][2] == self.subgrid_winners[1][1] == self.subgrid_winners[2][0] != "":
            return True
        return False

    def check_main_board_draw(self):
        """Checks if the main board is completely full or won, resulting in a Draw."""
        for i in range(3):
            for j in range(3):
                if self.subgrid_winners[i][j] == "" and not self.is_small_board_full(i, j):
                    return False  # If any playable board exists, no draw yet
        return True  # No playable boards left

    def is_small_board_full(self, main_row, main_col):
        """Checks if a small board is full."""
        for i in range(3):
            for j in range(3):
                if self.boards[main_row][main_col][i][j] == "":
                    return False
        return True

    def disable_small_board(self, main_row, main_col):
        """Disables all buttons in a won small board."""
        for i in range(3):
            for j in range(3):
                button = self.buttons[main_row * 3 + i][main_col * 3 + j]
                button.config(state="disabled")

    def reset_game(self):
        """Resets the game state for a new game."""
        self.current_player = "X"
        self.subgrid_winners = [[""] * 3 for _ in range(3)]
        self.boards = [[[[""] * 3 for _ in range(3)] for _ in range(3)] for _ in range(3)]

        for row in self.buttons:
            for button in row:
                button.config(text="", state="normal")

        self.label.config(text=f"Current Player: {self.current_player}")
        self.ask_game_mode()


if __name__ == "__main__":
    menu = GameMenu()
    menu.mainloop()
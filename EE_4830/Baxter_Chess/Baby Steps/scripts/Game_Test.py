from Game import Game
from Pawn import Pawn


g = Game()
g.initial_board.initialize_board()
g.create_piece_dict()


#g.place_piece(Pawn(),[2,2],"Black")


g.move_piece("P6","c5",False)
g.move_piece("P6","c4",False)
g.move_piece("P6","c3",False)
g.move_piece("P4","c3",True)
g.move_piece("P6","b2",False)
g.print_piece_dict()
g.print_board()

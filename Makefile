NAME = rede_social
CXX = cc
CXXFLAGS = -lpthread -g #-Wall -Werror -Wextra 
SRC = main.c usuarios.c modo_chat.c Grupo.c AVL.c Heap.c BST.c
OBJ = $(SRC:.c=.o)
HDR = usuarios.h

all:	$(NAME)
$(NAME):	$(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
%.o:	%.c $(HDR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:	
	rm -f $(OBJ)
fclean: clean	
	rm -f $(NAME)
re: fclean all
.PHONY: all re clean fclean
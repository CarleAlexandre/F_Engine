NAME		=	noHeaven

SERVER_NAME	=	server

BUILDDIR	=	build/

CC			=	gcc

CFLAGS		=	-std=c++20 -O3 -g

OBJ			=	$(SRC:%.cpp=%.o)

DEPS		+=	$(wildcard include/*.hpp)

SRC			+=	$(wildcard source/*.cpp)

INCLUDE		= -I C:/mingw64/include -I include

ifeq ($(OS), Windows_NT)
LIBS		=	-lraylib -lopengl32 -lgdi32 -lwinmm -lstdc++ -latomic
endif
ifeq ($(shell uname -s), Linux)
LIBS		=	-lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lstdc++
endif
 
$(BUILDDIR)$(NAME)		:	$(OBJ) $(DEPS)
		mkdir -p $(BUILDDIR)
		$(CC) $(OBJ) ${LIBS} -g -o $@

#add this above after lib to have no console: <-mwindows -Wl,--subsystem,windows>

$(OBJ)		:	%.o	:	%.cpp $(DEPS)
		$(CC) $(CFLAGS) ${INCLUDE} -g -c $< -o $@

all		:	$(BUILDDIR)$(NAME)

clean		:
		rm -rf $(OBJ)

fclean		:	clean
		rm -rf $(BUILDDIR)

re		:	fclean all


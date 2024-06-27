# author: Hadryan Sales

source=source/main.c \
       source/core.c \
	   source/cglm.c \
	   deps/glad/glad.c \
	   source/app_circle.c \
	   source/app_clipping.c \
	   source/app_t2.c \
include=-Ideps -Ideps/glad

all :
	gcc $(source) $(include) -lGL -lm -lglfw -Wall -Wno-missing-braces -fms-extensions -o mesh
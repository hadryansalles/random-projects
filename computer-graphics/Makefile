# author: Hadryan Sales

# source=source/core.c
source=source/main.c source/core.c source/cglm.c deps/glad/glad.c
include=-Ideps -Ideps/glad

all :
	gcc $(source) $(include) -lGL -lm -lglfw -Wall -Wno-missing-braces -fms-extensions -o mesh
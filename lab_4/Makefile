CC = cc
CFLAGS = -Wall -Wextra -lm -ldl
TARGET = main flist.so 2ndegree.so

build: ${TARGET}

main: main.c
	${CC} ${CFLAGS} -o $@ $<

2ndegree.so: 2n_degree_blocks.c
	${CC} ${CFLAGS} -shared -fPIC -o $@ $<

flist.so: free_list_blocks.c
	${CC} ${CFLAGS} -shared -fPIC -o $@ $<


clean:
	@rm -f ${TARGET}
	@rm -rf ./*.dSYM
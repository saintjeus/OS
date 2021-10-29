all: merge shared thread

merge: merge.c helper.c
	gcc helper.c merge.c -o merge

shared: shared.c helper.c
	gcc shared.c helper.c -o shared

thread: thread.c helper.c
	gcc thread.c helper.c -lpthread -o thread


clean: 
	rm merge
	rm shared
	rm thread

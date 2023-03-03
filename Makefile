%.o: %.c %.h
	gcc -g -c $< -o $@

minheap_tester: minheap.o minheap_tester.o
	gcc minheap.o minheap_tester.o -o minheap_tester

.PHONY:clear
clear:
	rm minheap_tester

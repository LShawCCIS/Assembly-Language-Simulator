make:   main.cpp
	g++ -c *.cpp
	g++ *.o -o a.out
	./a.out

rm:	main.cpp
	rm *.o
	rm a.out

open:	main.cpp
	emacs main.cpp &
	emacs Register.h &
	emacs Instructions.h &
	emacs Memory.h &

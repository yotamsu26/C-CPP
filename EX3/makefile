linked_list.o: linked_list.h linked_list.c
	gcc -Wall -Wextra -Wvla -c linked_list.h linked_list.c

markov_chain.o: markov_chain.h markov_chain.c linked_list.o
	gcc -Wall -Wextra -Wvla -c markov_chain.h markov_chain.c

tweets_generator.o: markov_chain.o
	gcc -Wall -Wextra -Wvla -c tweets_generator.c -o tweets_generator.o

snakes_and_ladders.o: markov_chain.o
	gcc -Wall -Wextra -Wvla -c snakes_and_ladders.c -o snakes_and_ladders.o

tweets: tweets_generator.o
	gcc -Wall -Wextra -Wvla tweets_generator.o linked_list.o markov_chain.o -o tweets_generator

snakes: snakes_and_ladders.o
	gcc -Wall -Wextra -Wvla snakes_and_ladders.o linked_list.o markov_chain.o -o snakes_and_ladders
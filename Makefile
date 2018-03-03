dungeon: dungeon.o priorityqueue.o dijkstra.o hero.o monster.o character.o
	gcc dungeon.o priorityqueue.o dijkstra.o hero.o monster.o character.o -o dungeon -lncurses

dungeon.o: dungeon.c
	gcc -Wall dungeon.c -c

prorityqueue.o: priorityqueue.c
	gcc -Wall priorityqueue.c -c

dijkstra.o: dijkstra.c
	gcc -Wall dijkstra.c -c

hero.o: hero.c
	gcc -Wall hero.c -c

monster.o: monster.c
	gcc -Wall monster.c -c

character.o: character.c
	gcc -Wall character.c -c

clean:
	rm -f dungeon *.o

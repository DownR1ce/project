cc = g++
prom = game
deps = *.h
obj = main.o Element.o game.o Home.o MazeMap.o movement.o player.o SmallGame.o utils.o
flag = -pthread

$(prom): $(obj)
	$(cc) -o $(prom) $(flag) $^
 
%.o: %.cpp %.h
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)

.PHONY: clean

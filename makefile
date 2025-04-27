cc = g++
prom = game
deps = *.h
obj = main.o Element.o game.o Home.o MazeMap.o movement.o SmallGame.o player.o utils.o
flag = -pthread


# 跨平台库设置
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
    LIBS := -lncurses
else ifeq ($(UNAME), Darwin)
    LIBS := -lncurses
else ifneq (,$(findstring MINGW,$(UNAME)))
    LIBS := -lpdcurses
else
    LIBS :=
endif

$(prom): $(obj)
	$(cc) -o $(prom) $(flag) $^ $(LIBS)

%.o: %.cpp %.h
	$(cc) -c $< -o $@

.PHONY: all clean check_deps

all: check_deps $(TARGET)


check_deps:
ifeq ($(UNAME), Darwin)
	@if ! command -v brew >/dev/null 2>&1; then \
		echo "Installing Homebrew..."; \
		echo | /bin/bash -c "$$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"; \
		echo "Homebrew installation may require manual confirmation. If it fails, install manually from https://brew.sh/"; \
	fi
	@if ! brew list ncurses >/dev/null 2>&1; then \
		echo "Installing ncurses..."; \
		brew install ncurses; \
	fi
else ifeq ($(UNAME), Linux)
	@if ! dpkg -s libncurses5-dev >/dev/null 2>&1; then \
		echo "Installing libncurses5-dev..."; \
		sudo apt-get update && sudo apt-get install -y libncurses5-dev; \
	fi
else ifneq (,$(findstring MINGW,$(UNAME)))
	@if ! pacman -Qi mingw-w64-x86_64-pdcurses >/dev/null 2>&1; then \
		echo "Installing pdcurses..."; \
		pacman -S --noconfirm mingw-w64-x86_64-pdcurses; \
	fi
else
	@echo "Unsupported OS. Please install dependencies manually."
	@exit 1
endif

clean:
	rm -rf $(obj) $(prom)

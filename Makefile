all: deps

.PHONY: deps
deps: deps/polyvox deps/selene
# deps/astar

#vl:
#	git clone https://github.com/MicBosi/visualizationlibrary.git vl
deps/polyvox:
	git clone https://bitbucket.org/volumesoffun/polyvox.git deps/polyvox

deps/selene:
	git clone https://github.com/jeremyong/Selene.git deps/selene

#deps/astar:
#	git clone https://github.com/justinhj/astar-algorithm-cpp.git deps/astar

.PHONY: vg
vg:
	valgrind --leak-check=yes --log-file="valgrind.txt" _build/BeardedMen

.PHONY: debug
debug:
	gdb _build/BeardedMen

.PHONY: localization-update
localization-update:
	lupdate src -ts lang/localization_ru.ts

.PHONY: localization-release
localization-release:
	lrelease lang/*.ts

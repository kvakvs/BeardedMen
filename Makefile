run:
	_build/Debug/BeardedMen

all: deps

.PHONY: deps
deps: deps/selene

deps/selene:
	git clone https://github.com/jeremyong/Selene.git deps/selene

.PHONY: vg
vg:
	valgrind --leak-check=yes --log-file="valgrind.txt" _build/BeardedMen

.PHONY: debug
debug:
	gdb _build/BeardedMen

.PHONY: localization
localization: localization-update localization-release

.PHONY: localization-update
localization-update:
	lupdate src -ts lang/localization_ru.ts

.PHONY: localization-release
localization-release:
	lrelease lang/*.ts

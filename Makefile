compile:
	ninja -C build
run:
	./build/main
test:
	./build/dsa_tests
multitest:
	./build/dsa_tests --gtest_repeat=100
filtered_tests:
	./build/dsa_tests --gtest_filter=swap*
valgrind:
	valgrind --leak-check=yes -s ./build/main
build_again:
	rm -r build/*
	cmake -G "Ninja" -B build -S .
to_build_release:
	cmake -S . -B release/ -D CMAKE_BUILD_TYPE=Release
	cmake --build release/	
to_run_release:
	./release/main
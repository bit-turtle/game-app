if [ ! -d "build" ]; then
	mkdir build
	cd build
	cmake ..
	cd ..
fi
cmake --build build --config Release -j 4

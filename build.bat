mkdir build
if not errorlevel 1 (
	cd build
	cmake ..
	cd ..
)
cmake --build build --config Release -j 4
.\bundle.bat

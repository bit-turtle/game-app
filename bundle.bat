rd /s/q bundle
md bundle
xcopy /s/e Release\game-app.exe bundle
xcopy /s/e Release\openal32.dll bundle
xcopy /s/e Release\highscore.txt bundle
xcopy /s/e textures bundle
xcopy /s/e sounds bundle
xcopy /s/e assets bundle
echo "Created Bundle!"

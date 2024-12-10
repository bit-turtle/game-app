rd /s/q bundle
md bundle
xcopy /s/e Release\game-app.exe bundle
xcopy /s/e Release\openal32.dll bundle
xcopy /s/e Release\highscore.txt bundle
md bundle\textures
xcopy /s/e textures bundle\textures
md bundle\sounds
xcopy /s/e sounds bundle\sounds
md bundle\assets
xcopy /s/e assets bundle\assets
echo "Created Bundle! ('\bundle')"

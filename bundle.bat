rd /s/q bundle
md bundle
xcopy /s/e build\Release\Asteroidal.exe bundle
xcopy /s/e build\Release\openal32.dll bundle
xcopy /s/e build\Release\highscore.txt bundle
md bundle\textures
xcopy /s/e textures bundle\textures
md bundle\sounds
xcopy /s/e sounds bundle\sounds
md bundle\assets
xcopy /s/e assets bundle\assets
echo "Created Bundle! ('\bundle')"

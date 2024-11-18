#!/bin/bash
FULLNAME="Amazing Game!"
GAMENAME=game-app
ROOT=GameApp.AppDir
rm -rf $ROOT
mkdir $ROOT $ROOT/usr $ROOT/usr/bin $ROOT/usr/bin/$GAMENAME
GAME=$ROOT/usr/bin/$GAMENAME
# Copy
cp game-app $GAME
cp -r textures $GAME
cp -r sounds $GAME
cp -r assets $GAME
cp icon.png $ROOT/$GAMENAME.png
# .DirIcon
ln -s $ROOT/$GAMENAME.png $ROOT/.DirIcon
# AppRun
echo "#!/bin/sh" >> $ROOT/AppRun
echo "SELF=\$(readlink -f \"\$0\")" >> $ROOT/AppRun
echo "HERE=\${SELF%/*}" >> $ROOT/AppRun
echo "export PATH=\"\${HERE}/usr/bin/:\${HERE}/usr/sbin/:\${HERE}/usr/games/:\${HERE}/bin/:\${HERE}/sbin/\${PATH:+:\$PATH}\"" >> $ROOT/AppRun
echo "export LD_LIBRARY_PATH=\"\${HERE}/usr/lib/:\${HERE}/usr/lib/i386-linux-gnu/:\${HERE}/usr/lib/x86_64-linux-gnu/:\${HERE}/usr/lib32/:\${HERE}/usr/lib64/:\${HERE}/lib/:\${HERE}/lib/i386-linux-gnu/:\${HERE}/lib/x86_64-linux-gnu/:\${HERE}/lib32/:\${HERE}/lib64/\${LD_LIBRARY_PATH:+:\$LD_LIBRARY_PATH}\"" >> $ROOT/AppRun
echo "export PYTHONPATH=\"\${HERE}/usr/share/pyshared/\${PYTHONPATH:+:\$PYTHONPATH}\"" >> $ROOT/AppRun
echo "export XDG_DATA_DIRS=\"\${HERE}/usr/share/\${XDG_DATA_DIRS:+:\$XDG_DATA_DIRS}\"" >> $ROOT/AppRun
echo "export PERLLIB=\"\${HERE}/usr/share/perl5/:\${HERE}/usr/lib/perl5/\${PERLLIB:+:\$PERLLIB}\"" >> $ROOT/AppRun
echo "export GSETTINGS_SCHEMA_DIR=\"\${HERE}/usr/share/glib-2.0/schemas/\${GSETTINGS_SCHEMA_DIR:+:\$GSETTINGS_SCHEMA_DIR}\"" >> $ROOT/AppRun
echo "export QT_PLUGIN_PATH=\"\${HERE}/usr/lib/qt4/plugins/:\${HERE}/usr/lib/i386-linux-gnu/qt4/plugins/:\${HERE}/usr/lib/x86_64-linux-gnu/qt4/plugins/:\${HERE}/usr/lib32/qt4/plugins/:\${HERE}/usr/lib64/qt4/plugins/:\${HERE}/usr/lib/qt5/plugins/:\${HERE}/usr/lib/i386-linux-gnu/qt5/plugins/:\${HERE}/usr/lib/x86_64-linux-gnu/qt5/plugins/:\${HERE}/usr/lib32/qt5/plugins/:\${HERE}/usr/lib64/qt5/plugins/\${QT_PLUGIN_PATH:+:\$QT_PLUGIN_PATH}\"" >> $ROOT/AppRun
echo "EXEC=\${HERE}/usr/bin/game-app" >> $ROOT/AppRun
echo "cd \$EXEC" >> $ROOT/AppRun
echo "exec \$EXEC/game-app" >> $ROOT/AppRun
chmod +x $ROOT/AppRun
# Desktop
echo "[Desktop Entry]" >> $ROOT/$GAMENAME.desktop
echo "Name=$FULLNAME" >> $ROOT/$GAMENAME.desktop
echo "Exec=AppRun" >> $ROOT/$GAMENAME.desktop
echo "Icon=$GAMENAME" >> $ROOT/$GAMENAME.desktop
echo "Type=Application" >> $ROOT/$GAMENAME.desktop
echo "Categories=Game" >> $ROOT/$GAMENAME.desktop
chmod +x $ROOT/$GAMENAME.desktop
# Done
echo "Created AppImage Directory ('/$ROOT')"
echo "Use AppImageKit to make AppImage (https://github.com/AppImage/AppImageKit/releases)"

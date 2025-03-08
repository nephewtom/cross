rm -rf web
mkdir web
emcc.bat easing.c -o web/easing.html -Iraylib/src -Lraylib/src -lraylib.web -s USE_GLFW=3 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 --shell-file .\template.html 

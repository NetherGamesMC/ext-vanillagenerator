echo "[SETUP] Running phpize."
if phpize >/dev/null ; then
     echo "[SETUP] Configuring workspace."
     ./configure >/dev/null
     if [ "$1" == "--compile" ] ; then
          echo "[BUILD] Compiling with \"make\"."
          make >/dev/null
          echo "Done!"
     else
          echo "Done! Please run \"make\" to build extension."
     fi
else
     echo "Error, \"phpize\" not found! Please install phpize."
fi